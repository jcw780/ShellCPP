cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(test)

# add the executable
add_executable(test test.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
  target_compile_options(test PRIVATE -march=native PRIVATE -Wall PRIVATE -Wextra)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  target_compile_options(test PRIVATE -march=native PRIVATE -Wall PRIVATE -Wextra)
# elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  # change architecture settings depending target
  target_compile_options(test PRIVATE /arch:AVX2 PRIVATE /W4)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    target_compile_options(test PRIVATE -Ofast)
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    target_compile_options(test PRIVATE -Ofast)
  # elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    # using Intel C++ 
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
    # change architecture settings depending target
    target_compile_options(test PRIVATE /Ot)
  endif()
endif()