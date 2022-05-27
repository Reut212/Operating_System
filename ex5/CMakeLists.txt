project(ex5)

cmake_minimum_required(VERSION 3.13)

cmake_policy(SET CMP0015 NEW)

set(SOURCE_FILES multi_threaded_server.cpp)

add_executable( server ${SOURCE_FILES} )
add_executable( client client.cpp)



