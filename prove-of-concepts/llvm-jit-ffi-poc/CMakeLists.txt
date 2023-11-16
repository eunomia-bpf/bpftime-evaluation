cmake_minimum_required(VERSION 3.0.0)
project(llvm-jit-ffi-poc VERSION 0.1.0 LANGUAGES C CXX)

add_executable(llvm-jit-ffi-poc main.cpp)

set_property(TARGET llvm-jit-ffi-poc PROPERTY CXX_STANDARD 20)

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
    OrcJIT
    Support
    nativecodegen
)

message(STATUS "LLVM_LIBS=${LLVM_LIBS}")

target_link_libraries(llvm-jit-ffi-poc ${LLVM_LIBS})
target_include_directories(llvm-jit-ffi-poc PUBLIC ${LLVM_INCLUDE_DIRS})
