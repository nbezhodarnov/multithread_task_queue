cmake_minimum_required(VERSION 3.18)

set(PROJECT "thread_lab9")

project(${PROJECT})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (UNIX)
    find_package(Threads REQUIRED)
endif (UNIX)

add_executable(${PROJECT} Lab_9_Bezkhodarnov.cpp)

if (UNIX)
    target_link_libraries (${PROJECT} ${CMAKE_THREAD_LIBS_INIT})
endif (UNIX)
