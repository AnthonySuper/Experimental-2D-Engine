cmake_minimum_required(VERSION 2.8.0)

project(nmtt)

enable_testing()

set(PROJECT_core_SRCS
  src/util.cpp
  src/window.cpp
  src/actor.cpp
  src/script_engine.cpp
  src/vector.cpp
  src/polygon.cpp
  src/scene.cpp
  src/collision_result.cpp
  src/sym.cpp
  src/message.cpp
  src/physics_system.cpp
  src/physics_body.cpp
  src/world.cpp
  src/entity.cpp
  src/entity_ref.cpp
  src/component_ref.cpp
  src/component.cpp)

set(PROJECT_core_HEADERS
  include/actor.hpp
  include/window.hpp
  include/scene.hpp
  include/script_engine.hpp
  include/util.hpp
  include/vector.hpp
  include/polygon.hpp
  include/errors.hpp
  include/collision_result.hpp
  include/mrb_wrapper.hpp
  include/sym.hpp
  include/message.hpp
  include/physics_system.hpp
  include/physics_body.hpp
  include/world.hpp
  include/entity.hpp
  include/entity_ref.hpp
  include/component.hpp
  include/component_ref.hpp)

set(PROJECT_render_SRCS
  src/render/gl_program.cpp
  src/render/gl_texture.cpp
  src/render/sprite_sheet.cpp
  src/render/renderer.cpp)

set(PROJECT_render_HEADERS
  include/render/gl_program.hpp
  include/render/gl_texture.hpp
  include/render/gl_include.hpp
  include/render/sprite_sheet.hpp
  include/render/renderer.hpp)

source_group("core\\Source Files" FILES ${PROJECT_core_SRCS})
source_group("core\\Header Files" FILES ${PROJECT_core_HEADERS})
source_group("render\\Source Files" FILES ${PROJECT_render_SRCS})
source_group("render\\Header Files" FILES ${PROJECT_render_HEADERS})

set(PROJECT_SRCS
  ${PROJECT_core_SRCS}
  ${PROJECT_render_SRCS})

set(PROJECT_HEADERS
  ${PROJECT_core_HEADERS}
  ${PROJECT_render_HEADERS})

add_executable(app app.cpp 
  ${PROJECT_SRCS} 
  ${PROJECT_HEADERS})

target_link_libraries(app
  ${CMAKE_SOURCE_DIR}/mruby/build/host/lib/libmruby.a)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(SDL2_image REQUIRED >= 2.0.0)
find_package(OpenGL REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
  ${SDL2TTF_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIRS}
  "lib/"
  "include/"
  "mruby/include/")

target_link_libraries(app
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARY}
  ${SDL2TTF_LIBRARY}a
  ${OPENGL_gl_LIBRARY})

if(UNIX AND NOT APPLE) 
  find_package(GLEW REQUIRED)
  include_directories(${GLEW_INCLUDE_DIRS})
  link_libraries(${GLEW_LIBRARIES})
endif()

link_libraries(${OPENGL_gl_LIBRARY})

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE "lib/")

set(PROJECT_core_TEST_SRCS
  test/test_util.cpp
  test/vector_test.cpp
  test/polygon_test.cpp
  test/mrb_wrapper.cpp)


set(PROJECT_render_TEST_SRCS
  test/render/sprite_sheet_test.cpp)

source_group("core\\Test Files" FILES ${PROJECT_core_TEST_SRCS})
source_group("render\\Test Files" FILES ${PROJECT_render_TEST_SRCS})


set(PROJECT_TEST_SRCS
  ${PROJECT_core_TEST_SRCS}
  ${PROJECT_render_TEST_SRCS})

add_executable(catch_test test/main.cpp
  ${PROJECT_SRCS}
  ${PROJECT_TEST_SRCS})

file(COPY test_files/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_files/)

target_link_libraries(catch_test Catch)
target_link_libraries(catch_test
  ${CMAKE_SOURCE_DIR}/mruby/build/host/lib/libmruby.a)

target_link_libraries(catch_test
  ${SDL2_IMAGE_LIBRARY}
  ${SDL2_LIBRARY}
  ${SDL2TTF_LIBRARY})

add_custom_command(TARGET app POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/assets
  $<TARGET_FILE_DIR:app>/assets)

add_custom_command(TARGET catch_test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files
  $<TARGET_FILE_DIR:catch_test>/test_files)

add_test(NAME CatchTests COMMAND catch_test)
