cmake_minimum_required(VERSION 3.26.4)

project(trackme)

set(SOURCES src/timer.c)
add_library(mongoose STATIC src/mongoose.c)
add_executable(main src/main.c ${SOURCES})
add_executable(test_main test/test_main.c ${SOURCES})

target_link_libraries(main mongoose /home/blaklinten/Git/trackMe-c/src/libgit2/lib/libgit2.so)
target_link_libraries(test_main cmocka)
