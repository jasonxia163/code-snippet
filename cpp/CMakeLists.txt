cmake_minimum_required(VERSION 3.10)
project(cpp-code-snippets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_SANITIZER "Turn AddressSanitizer on" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=auto -fno-omit-frame-pointer")

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
add_executable(test_atoi test_atoi.cpp)
add_executable(test_weak_ptr test_weak_ptr.cpp)
add_executable(test_map test_map.cpp)
add_executable(test_rref test_rref.cpp)
add_executable(gen_version gen_version.cpp)

