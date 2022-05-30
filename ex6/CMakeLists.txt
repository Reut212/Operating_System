project(ex6)

cmake_minimum_required(VERSION 3.13)

cmake_policy(SET CMP0015 NEW)

add_executable( Queue Queue.cpp )
add_executable( ActiveObject active_object.cpp )
add_executable( pipeline pipeline.cpp )

