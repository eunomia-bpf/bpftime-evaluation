cmake_minimum_required(VERSION 3.0.0)
project(llvm-ir-jit-poc VERSION 0.1.0 LANGUAGES C CXX)

add_executable(llvm-ir-jit-poc main.cpp)


set(LLVM_SEARCH_PATHS
  /usr/lib/llvm-15/lib/cmake/llvm
  /usr/lib/llvm-14/lib/cmake/llvm
  /usr/lib/llvm-13/lib/cmake/llvm
)

find_package(LLVM REQUIRED CONFIG
  PATHS ${LLVM_SEARCH_PATHS}
  NO_DEFAULT_PATH)

llvm_map_components_to_libnames(LLVM_LIBS
  Core
  Interpreter
  ExecutionEngine
  MC
  MCJIT
  Support
  nativecodegen
  orcjit 
)

message(STATUS "LLVM_LIBS=${LLVM_LIBS}")

target_link_libraries(llvm-ir-jit-poc ${LLVM_LIBS})
target_include_directories(llvm-ir-jit-poc PUBLIC ${LLVM_INCLUDE_DIRS})
