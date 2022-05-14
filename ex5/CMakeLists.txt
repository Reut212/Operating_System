project(ex5)

cmake_minimum_required(VERSION 3.13)

cmake_policy(SET CMP0015 NEW)

set(SOURCE_FILES multi_threaded_server.cpp)

include_directories( stack )

#here
link_directories(stack)

add_subdirectory(stack)

add_executable( server ${SOURCE_FILES} )
add_executable( client client.cpp)
add_executable( tests tests.cpp)



#and here
target_link_libraries( server stack )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(server Threads::Threads )
target_link_libraries(tests Threads::Threads )

