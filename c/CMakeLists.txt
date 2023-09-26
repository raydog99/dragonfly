cmake_minimum_required(VERSION 3.20)

project(Tcpip-C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

file(GLOB LIB_SOURCES "lib/*.c")

add_library(tcpip SHARED ${LIB_SOURCES})

file(GLOB TEST_SOURCES "test/*.c")

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    target_link_libraries(${TEST_NAME} tcpip)
endforeach()