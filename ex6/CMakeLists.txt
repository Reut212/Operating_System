project(ex6)

cmake_minimum_required(VERSION 3.13)

cmake_policy(SET CMP0015 NEW)

add_executable( main main.cpp)
add_executable( client client.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(main Threads::Threads )
