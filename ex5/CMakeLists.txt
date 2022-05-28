project(ex5)

cmake_minimum_required(VERSION 3.13)

cmake_policy(SET CMP0015 NEW)

set(SOURCE_FILES multi_threaded_server.c)

include_directories( funcs )

#here
link_directories(funcs)

add_subdirectory(funcs)

add_executable( server ${SOURCE_FILES} )
add_executable( client client.c)
add_executable( tests tests.c)



#and here
target_link_libraries( server funcs )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(server Threads::Threads )
