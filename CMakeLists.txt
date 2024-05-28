cmake_minimum_required(VERSION 3.26.4)

project(trackme)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES src/timer.c)
set(TEST_SOURCES
  test/test_main.c
  test/test_timer.c
  test/test_db.c)

add_library(mongoose STATIC src/mongoose.c)

add_executable(main
  src/main.c
  ${SOURCES})
add_executable(test_main
  ${TEST_SOURCES}
  ${SOURCES})

find_package(mongoc-1.0 1.7 REQUIRED)

target_link_libraries(main
  mongoose
  mongo::mongoc_shared)

target_link_libraries(test_main
  mongo::mongoc_shared
  cmocka)

target_link_options(test_main PRIVATE "LINKER:--wrap=time")
