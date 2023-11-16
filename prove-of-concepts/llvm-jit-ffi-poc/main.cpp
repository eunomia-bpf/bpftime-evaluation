//===------- HowToUseLLJIT.cpp - An example use of ORC-based LLJIT --------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This small program provides an example of how to quickly build a small
//  module with a 'add1' function and use of IRBuilder to create add & return
//  instructions.

#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdint>
#include <llvm-15/llvm/ExecutionEngine/JITSymbol.h>
#include <llvm-15/llvm/ExecutionEngine/Orc/Core.h>
#include <llvm-15/llvm/ExecutionEngine/Orc/SymbolStringPool.h>
#include <llvm-15/llvm/IR/Intrinsics.h>
#include <utility>
using namespace llvm;
using namespace llvm::orc;

ExitOnError ExitOnErr;

extern "C" int32_t ffi_func(int32_t a, int32_t b)
{
	return a * b;
}
/*
It will generate a module like：

; ModuleID = 'jit-ffi-test'
source_filename = "jit-ffi-test"

define i32 @mul2add1(i32 %AnArg) {
EntryBlock:
  %0 = call i32 @mul2(i32 %AnArg, i32 2)
  %1 = add i32 1, %0
  ret i32 %1
}

declare i32 @mul2(i32 %0, i32 %1)
*/
ThreadSafeModule createDemoModule(const std::string& ffiFuncName)
{
	auto Context = std::make_unique<LLVMContext>();
	auto M = std::make_unique<Module>("jit-ffi-test", *Context);

	// Create the add1 function entry and insert this entry into module M.
	// The function will have a return type of "int" and take an argument of
	// "int".
	Function *Mul2Add1F = Function::Create(
		FunctionType::get(Type::getInt32Ty(*Context),
				  { Type::getInt32Ty(*Context) }, false),
		Function::ExternalLinkage, "mul2add1", M.get());

	Function *Mul2 = Function::Create(
		FunctionType::get(Type::getInt32Ty(*Context),
				  { Type::getInt32Ty(*Context),
				    Type::getInt32Ty(*Context) },
				  false),
		Function::ExternalLinkage, ffiFuncName, M.get());

	// Add a basic block to the function. As before, it automatically
	// inserts because of the last argument.
	BasicBlock *BB = BasicBlock::Create(*Context, "EntryBlock", Mul2Add1F);

	// Create a basic block builder with default parameters.  The builder
	// will automatically append instructions to the basic block `BB'.
	IRBuilder<> builder(BB);

	// Get pointers to the integer argument of the add1 function...
	assert(Mul2Add1F->arg_begin() != Mul2Add1F->arg_end()); // Make sure
								// there's an
								// arg
	Argument *ArgX = &*Mul2Add1F->arg_begin(); // Get the arg
	ArgX->setName("AnArg"); // Give it a nice symbolic name for fun.

	auto callInst = builder.CreateCall(
		FunctionType::get(Type::getInt32Ty(*Context),
				  { Type::getInt32Ty(*Context),
				    Type::getInt32Ty(*Context) },
				  false),
		Mul2, { ArgX, builder.getInt32(2) });

	// Get pointers to the constant `1'.
	Value *One = builder.getInt32(1);

	// Create the add instruction, inserting it into the end of BB.
	Value *Add = builder.CreateAdd(One, callInst);

	// Create the return instruction and add it to the basic block
	builder.CreateRet(Add);

	return ThreadSafeModule(std::move(M), std::move(Context));
}

int main(int argc, char *argv[])
{
	// Initialize LLVM.
	InitLLVM X(argc, argv);

	InitializeNativeTarget();
	InitializeNativeTargetAsmPrinter();

	cl::ParseCommandLineOptions(argc, argv, "HowToUseLLJIT");
	ExitOnErr.setBanner(std::string(argv[0]) + ": ");

	// Create an LLJIT instance.
	auto J = ExitOnErr(LLJITBuilder().create());

	// Define the FFI function
	auto &dylib = J->getMainJITDylib();
	
	auto sym = JITEvaluatedSymbol::fromPointer(&ffi_func);
	sym.setFlags(JITSymbolFlags::Callable | JITSymbolFlags::Exported);
	ExitOnErr(dylib.define(
		absoluteSymbols({ { J->mangleAndIntern("mul2"), sym } })));

	// Create the module add add it into LLJIT
	auto M = createDemoModule(J->mangle("mul2"));
	M.withModuleDo([](auto &m) { m.dump(); });
	ExitOnErr(J->addIRModule(std::move(M)));

	dylib.dump(outs());

	// Look up the JIT'd function, cast it to a function pointer, then
	// it.
	auto Add1Addr = ExitOnErr(J->lookup("mul2add1"));
	auto Func = Add1Addr.toPtr<int(int, int)>();

	int Result = Func(42, 10);
	outs() << "mul2add1(42) = " << Result << "\n";

	return 0;
}
