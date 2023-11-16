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


function(tracer_setup_target target out_name)
    set_target_properties(${target} PROPERTIES CXX_STANDARD 20 PREFIX "" OUTPUT_NAME ${out_name})
    target_include_directories(${target} PUBLIC ${FRIDA_CORE_INSTALL_DIR} ${FRIDA_GUM_INSTALL_DIR})
endfunction()

# agent.so
add_library(agent SHARED agent.cpp)
tracer_setup_target(agent "agent")
target_link_libraries(agent ${FRIDA_GUM_INSTALL_DIR}/libfrida-gum.a)
add_dependencies(agent FridaGum)

# injector
add_executable(injector injector.cpp)
tracer_setup_target(injector "injector")
target_link_libraries(injector ${FRIDA_CORE_INSTALL_DIR}/libfrida-core.a)
add_dependencies(injector FridaCore)

# victim
add_executable(victim victim.cpp)
