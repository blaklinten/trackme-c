cmake_minimum_required(VERSION 3.26.4)

project(trackme)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TRACKME_LOG_LEVEL 2)

set(MAIN_SOURCES
  src/track_me.c
  src/web.c
  src/ctemplate.c
  src/timer.c
  src/mongoose.c
  src/db.c
  src/log.c)

set(UNIT_TEST_SOURCES
  test/unit_test.c
  test/test_timer.c
  src/timer.c
  src/mongoose.c
  src/log.c)

set(INTEGRATION_TEST_SOURCES
  test/integration_test.c
  test/test_db.c
  ${MAIN_SOURCES})

set(
  CMAKE_C_FLAGS
  "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wno-unused-parameter -fsanitize=undefined -fsanitize=address"
)

add_library(mongoose STATIC src/mongoose.c)
add_library(ctemplate STATIC src/ctemplate.c)

add_executable(main
  src/main.c
  ${MAIN_SOURCES})
set_property(TARGET main PROPERTY CXX_STANDARD 23)

add_executable(unit_test
  ${UNIT_TEST_SOURCES})
set_property(TARGET unit_test PROPERTY CXX_STANDARD 23)

add_executable(integration_test
  ${INTEGRATION_TEST_SOURCES})
set_property(TARGET integration_test PROPERTY CXX_STANDARD 23)

find_package(mongoc-1.0 1.27 REQUIRED)

target_link_libraries(main
  mongoose
  ctemplate
  mongo::mongoc_shared)

target_link_libraries(unit_test
  cmocka)

target_link_libraries(integration_test
  mongo::mongoc_shared
  cmocka)

target_link_options(unit_test PRIVATE "LINKER:--wrap=time"
  -fsanitize=undefined -fsanitize=address
  )
