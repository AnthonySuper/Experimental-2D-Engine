cmake_minimum_required(VERSION 3.3.0)

project(nmtt)

enable_testing()

add_executable(app app.cpp 
  src/util.cpp
  include/util.hpp
  src/position.cpp
  include/position.hpp)

target_link_libraries(app
  ${CMAKE_SOURCE_DIR}/mruby/build/host/lib/libmruby.a)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} 
  "include/"
  "mruby/include/")

target_link_libraries(app ${SDL2_LIBRARY})

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE "lib/")
add_executable(catch_test test/main.cpp 
  src/util.cpp
  test/test_util.cpp
  src/position.cpp
  test/position_test.cpp)

target_link_libraries(catch_test Catch)
target_linK_libraries(catch_test 
  ${CMAKE_SOURCE_DIR}/mruby/build/host/lib/libmruby.a)
add_test(NAME CatchTests COMMAND catch_test)
