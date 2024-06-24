cmake_minimum_required(VERSION 3.10)
project(http_server_cmake VERSION 0.1.0)
set(CMAKE_C_STANDARD 23)
add_executable(http_server_cmake src/main.c)
target_link_libraries(http_server_cmake ws2_32)


