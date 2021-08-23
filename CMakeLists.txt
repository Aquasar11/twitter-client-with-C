cmake_minimum_required(VERSION 3.17)
project(FOP_Project__Twitter_ C)

set(CMAKE_C_STANDARD 99)

add_executable(Project main.c cJSON.c)

target_link_libraries(Project ws2_32)