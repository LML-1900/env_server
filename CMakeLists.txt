cmake_minimum_required(VERSION 3.25)
project(env_server)
set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# OSRM config
find_package(LibOSRM REQUIRED)
include_directories(SYSTEM ${LibOSRM_INCLUDE_DIRS})
link_directories(${LibOSRM_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LibOSRM_CXXFLAGS}")

add_subdirectory(store)
add_subdirectory(service)

add_executable(env_server main.cpp)

target_link_libraries(env_server service store)
