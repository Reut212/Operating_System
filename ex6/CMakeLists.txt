project(ex6)

cmake_minimum_required(VERSION 3.13)

cmake_policy(SET CMP0015 NEW)

add_executable( main main1.cpp)
add_executable( client client.cpp)
add_executable( singleton singleton.cpp)
add_executable(pollclient pollclient.cpp reactor.cpp)
add_executable(pollserver pollserver.cpp reactor.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(main Threads::Threads )
target_link_libraries(singleton Threads::Threads )
target_link_libraries(pollclient Threads::Threads )
target_link_libraries(pollserver Threads::Threads )
