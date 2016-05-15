cmake_minimum_required(VERSION 3.3.0)

project(nmtt)

enable_testing()

set(PROJECT_SRCS
  src/util.cpp
  src/polygon.cpp
  src/collision_result.cpp)

set(PROJECT_INCLUDES
  include/util.hpp
  include/vector.hpp
  include/polygon.hpp
  include/collision_result.hpp)

add_executable(app app.cpp ${PROJECT_SRCS} ${PROJECT_INCLUDES})

target_link_libraries(app
  ${CMAKE_SOURCE_DIR}/mruby/build/host/lib/libmruby.a)

set(Boost_USE_STATIC_LIBS OFF)

set(Boost_USE_MULTITHREADED ON)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall")

find_package(Boost 1.60 REQUIRED COMPONENTS system regex)

include_directories(app ${Boost_INCLUDE_DIRS})

target_link_libraries(app ${Boost_LIBRARIES})



list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR}
  "include/"
  "mruby/include/")

target_link_libraries(app ${SDL2_LIBRARY})

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE "lib/")

add_executable(catch_test test/main.cpp 
  ${PROJECT_SRCS}
  test/test_util.cpp
  test/vector_test.cpp
  test/polygon_test.cpp)


target_link_libraries(catch_test Catch)
target_linK_libraries(catch_test 
  ${CMAKE_SOURCE_DIR}/mruby/build/host/lib/libmruby.a)

add_test(NAME CatchTests COMMAND catch_test)
