cmake_minimum_required(VERSION 3.16)
project(main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_COMPILER /usr/bin/clang++-13)

add_compile_options(-I/usr/include/llvm-13)
add_compile_options(-I/usr/include/llvm-c-13)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

add_executable(main main.cpp)
target_link_libraries(main LLVM-13)
set_target_properties(main PROPERTIES LINKER_LANGUAGE CXX)