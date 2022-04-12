cmake_minimum_required(VERSION 3.13)  # CMake version check
project(ex4)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES client.cpp multi_threaded_server.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(server multi_threaded_server.cpp)
add_executable(client.cpp)