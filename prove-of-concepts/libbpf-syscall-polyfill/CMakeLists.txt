cmake_minimum_required(VERSION 3.15)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

project(
    "libbpf-syscall-userspace-polyfill"
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

set(BPFTIME_LLVM_JIT NO)
add_subdirectory("../../core" ${CMAKE_CURRENT_BINARY_DIR}/libcore)

set(LOCAL_INCLUDES helpers_impl.hpp wrappers.hpp)

function(polyfill_setup_target target out_name)
    set_target_properties(${target} PROPERTIES CXX_STANDARD 20 PREFIX "" OUTPUT_NAME ${out_name})
    target_include_directories(${target} PUBLIC ${LOCAL_INCLUDES})

    # target_include_directories(${target} PUBLIC ${FRIDA_CORE_INSTALL_DIR} ${FRIDA_GUM_INSTALL_DIR})
endfunction()

# agent.so
add_library(agent SHARED agent.cpp helpers_impl.cpp)
polyfill_setup_target(agent "agent")
target_link_libraries(agent core-bpf ${libbpf_LIBRARIES})
target_include_directories(agent PUBLIC "../../core" ${libbpf_INCLUDE_DIRS})

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
# add_ebpf_program_target(opensnoop_bpf ${CMAKE_CURRENT_SOURCE_DIR}/opensnoop.bpf.c ${CMAKE_CURRENT_BINARY_DIR}/opensnoop.bpf.o)
