cmake_minimum_required(VERSION 3.23)
project(OS2_Semaphore C)

set(CMAKE_C_STANDARD 23)

add_executable(OS2_Semaphore main.c)
add_executable(MyVersion MyVersion.c)
add_executable(MyVersion2 MyVersion2Type.c)
add_executable(SystemV SystemV.c)