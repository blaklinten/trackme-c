cmake_minimum_required(VERSION 3.26.4)

project(trackme)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TRACKME_LOG_LEVEL 2)

set(MAIN_SOURCES
  lib/ctemplate.c
  lib/mongoose.c
  lib/sqlite3.c
  src/db.c
  src/timer.c
  src/track_me.c
  src/util/log.c
  src/web.c
)

set(UNIT_TEST_SOURCES
  # test/test_list.c
  lib/mongoose.c
  lib/sqlite3.c
  src/db.c
  src/timer.c
  src/track_me.c
  src/util/log.c
  test/test_timer.c
  test/test_trackme.c
  test/unit_test.c
)

set(INTEGRATION_TEST_SOURCES
  lib/sqlite3.c
  src/db.c
  src/timer.c
  src/util/log
  test/integration_test.c
  test/test_db.c
)

set(
  CMAKE_C_FLAGS
  "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wno-unused-parameter -fsanitize=undefined -fsanitize=address"
)

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

target_link_libraries(unit_test
  cmocka)

target_link_libraries(integration_test
  cmocka)

target_link_options(unit_test PRIVATE "LINKER:--wrap=time"
  -fsanitize=undefined -fsanitize=address
  )
