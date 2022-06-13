project(ex7)

cmake_minimum_required(VERSION 3.13)

cmake_policy(SET CMP0015 NEW)

add_executable(myfs myfs.c)
add_executable(test test.c myfs.c)
add_executable(Mylibc Mylibc.c)
