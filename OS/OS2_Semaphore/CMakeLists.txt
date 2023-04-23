cmake_minimum_required(VERSION 3.23)
project(OS2_Semaphore C)

set(CMAKE_C_STANDARD 23)

add_executable(Named Named.c)
add_executable(NotNamed NotNamed.c)
add_executable(SystemV SystemV.c)