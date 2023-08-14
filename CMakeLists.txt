cmake_minimum_required(VERSION 3.25)
project(env_server)
#include_directories("/home/lml/.local/include" "/usr/local/include")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(store)
add_subdirectory(service)

add_executable(env_server main.cpp)

target_link_libraries(env_server service store)
