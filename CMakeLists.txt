cmake_minimum_required(VERSION 3.26.4)

project(trackme)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TRACKME_LOG_LEVEL 2)

set(SOURCES
  src/timer.c
  src/mongoose.c
  src/db.c
  src/log.c)

set(TEST_SOURCES
  test/test_timer.c
  test/test_db.c)

set(
  CMAKE_C_FLAGS
  "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wno-unused-parameter -fsanitize=undefined -fsanitize=address"
)

add_library(mongoose STATIC src/mongoose.c)

add_executable(main
  src/main.c
  ${SOURCES})
set_property(TARGET main PROPERTY CXX_STANDARD 23)

add_executable(test_main
  test/test_main.c
  ${TEST_SOURCES}
  ${SOURCES})
set_property(TARGET test_main PROPERTY CXX_STANDARD 23)

find_package(mongoc-1.0 1.27 REQUIRED)

target_link_libraries(main
  mongoose
  mongo::mongoc_shared)

target_link_libraries(test_main
  mongo::mongoc_shared
  cmocka)

target_link_options(test_main PRIVATE "LINKER:--wrap=time" "LINKER:--wrap=mongoc_collection_insert_one"
  -fsanitize=undefined -fsanitize=address
  )
