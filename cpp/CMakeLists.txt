cmake_minimum_required(VERSION 3.10)
project(cpp-code-snippets)

set(CMAKE_CXX_STANDARD 17)

option(USE_SANITIZER "Turn AddressSanitizer on" OFF)

if(USE_SANITIZER)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize-recover=address")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
  set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize-recover=address")
  set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize-recover=address")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
endif()

add_executable(test_static test_static.cpp)
add_executable(test_ref_null test_ref_null.cpp)
add_executable(test_constexpr test_constexpr.cpp)
add_executable(parse_sps parse_sps.cpp)
add_executable(leak_sample leak_sample.cpp)

