project(ex6)

cmake_minimum_required(VERSION 3.13)

cmake_policy(SET CMP0015 NEW)

set(SOURCE_FILES Queue.c)

add_executable( Queue ${SOURCE_FILES} )

