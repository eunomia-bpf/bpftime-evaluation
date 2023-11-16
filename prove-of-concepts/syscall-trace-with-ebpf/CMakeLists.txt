cmake_minimum_required(VERSION 3.15)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

project(
    "syscall-trace-with-ebpf"
    VERSION 0.1.0
    LANGUAGES C CXX
)

include(../../cmake/frida.cmake)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

set(BPFTIME_LLVM_JIT YES)
add_subdirectory("../../core" ${CMAKE_CURRENT_BINARY_DIR}/libcore)

set(LOCAL_INCLUDES "def.h" "vmlinux_mini.h")

function(tracer_setup_target target out_name)
    set_target_properties(${target} PROPERTIES CXX_STANDARD 20 PREFIX "" OUTPUT_NAME ${out_name})
    target_include_directories(${target} PUBLIC ${LOCAL_INCLUDES})
    target_include_directories(${target} PUBLIC ${FRIDA_CORE_INSTALL_DIR} ${FRIDA_GUM_INSTALL_DIR})
endfunction()

# agent.so
add_library(agent SHARED agent.cpp)
tracer_setup_target(agent "agent")
target_link_libraries(agent ${FRIDA_GUM_INSTALL_DIR}/libfrida-gum.a opcodes)
add_dependencies(agent FridaGum)

# hooker.so
add_library(hooker SHARED hooker.cpp)
tracer_setup_target(hooker "hooker")
add_dependencies(hooker core-bpf)
find_package(PkgConfig)
pkg_check_modules(libbpf REQUIRED libbpf>=1.1.0)
target_link_libraries(hooker core-bpf ${libbpf_LIBRARIES})
target_include_directories(hooker PUBLIC "../../core" ${libbpf_INCLUDE_DIRS})

add_custom_target(
    syscall_table
    COMMAND /bin/bash ${CMAKE_CURRENT_SOURCE_DIR}/generate_syscall_table.sh "${CMAKE_CURRENT_BINARY_DIR}/syscall_list.h"
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/syscall_list.h
    USES_TERMINAL
)

add_dependencies(hooker syscall_table)
target_include_directories(hooker PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# injector
add_executable(injector injector.cpp)
tracer_setup_target(injector "injector")
target_link_libraries(injector ${FRIDA_CORE_INSTALL_DIR}/libfrida-core.a)
add_dependencies(injector FridaCore)

# victim
add_executable(victim victim.cpp)

function(add_ebpf_program_target target_name source_file output_file)
    # opensnoop.bpf.o
    execute_process(COMMAND bash -c "uname -m | sed 's/x86_64/x86/' \
| sed 's/arm.*/arm/' \
| sed 's/aarch64/arm64/' \
| sed 's/ppc64le/powerpc/' \
| sed 's/mips.*/mips/' \
| sed 's/riscv64/riscv/' \
| sed 's/loongarch64/loongarch/'"
        OUTPUT_VARIABLE UNAME_ARCH
        COMMAND_ERROR_IS_FATAL ANY
    )
    string(STRIP ${UNAME_ARCH} UNAME_ARCH_STRIPPED)
    add_custom_target(${target_name} ALL
        COMMAND clang -O2 -target bpf -c -g -D__TARGET_ARCH_${UNAME_ARCH_STRIPPED} -I${CMAKE_CURRENT_SOURCE_DIR}/../../runtime/sdk/vmlinux/${ARCH} -I${CMAKE_CURRENT_SOURCE_DIR}/../../runtime/sdk/include/ ${source_file} -o ${output_file}
        BYPRODUCTS ${output_file}
        SOURCES ${source_file}
    )
endfunction()

# The bpf program used for testing
add_ebpf_program_target(opensnoop_bpf ${CMAKE_CURRENT_SOURCE_DIR}/opensnoop.bpf.c ${CMAKE_CURRENT_BINARY_DIR}/opensnoop.bpf.o)

# The test driver
add_executable(test_driver test_driver.cpp)
add_dependencies(test_driver agent hooker injector victim opensnoop_bpf)
target_compile_definitions(test_driver PRIVATE AGENT_PATH=$<TARGET_FILE:agent>
    HOOKER_PATH=$<TARGET_FILE:hooker>
    INJECTOR_PATH=$<TARGET_FILE:injector>
    VICTIM_PATH=$<TARGET_FILE:victim>
    OPENSNOOP_BPF_PATH=${CMAKE_CURRENT_BINARY_DIR}/opensnoop.bpf.o)
set_target_properties(test_driver PROPERTIES CXX_STANDARD 20)
add_test(NAME test-syscall-trace COMMAND test_driver)
