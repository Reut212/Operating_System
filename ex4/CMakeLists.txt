project(ex4)

cmake_minimum_required(VERSION 3.13)

cmake_policy(SET CMP0015 NEW)

set(SOURCE_FILES multi_threaded_server.c)

include_directories( stack )

#here
link_directories(stack)

add_subdirectory(stack)

add_executable( server ${SOURCE_FILES} )
add_executable( client client.c)
add_executable( allocation allocation.c)



#and here
target_link_libraries( server stack )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(server Threads::Threads )
