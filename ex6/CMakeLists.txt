project(ex6)

cmake_minimum_required(VERSION 3.13)

cmake_policy(SET CMP0015 NEW)

add_executable( Queue Queue.cpp )
add_executable( ActiveObject active_object.cpp)
add_executable( pipeline main.cpp client.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Queue Threads::Threads )
target_link_libraries(ActiveObject Threads::Threads )
target_link_libraries(pipeline Threads::Threads )