cmake_minimum_required(VERSION 3.20)
project(shm)

add_executable(sysv_shm_writer sysv_shm_writer.cpp)
add_executable(sysv_shm_reader sysv_shm_reader.cpp)

add_executable(posix_shm_writer posix_shm_writer.cpp)
add_executable(posix_shm_reader posix_shm_reader.cpp)