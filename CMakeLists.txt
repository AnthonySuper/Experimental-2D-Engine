cmake_minimum_required(VERSION 3.3.0)

project(nmtt)

add_executable(app app.cpp src/test.cpp include/test.hpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} "include/")

target_link_libraries(app ${SDL2_LIBRARY})
