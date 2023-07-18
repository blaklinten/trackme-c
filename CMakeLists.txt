cmake_minimum_required(VERSION 3.26.4)

project(trackme)

add_library(mongoose STATIC src/mongoose.c)
add_executable(main src/main.c)

target_link_libraries(main mongoose /home/blaklinten/Git/trackMe-c/src/libgit2/lib/libgit2.so)
