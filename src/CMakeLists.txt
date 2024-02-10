cmake_minimum_required(VERSION 3.10)
project(library-manager)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(CREATE_LINK
    "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_SOURCE_DIR}/compile_commands.json"
    SYMBOLIC
)

add_executable(Main main.cpp)

target_include_directories(Main PUBLIC
    "${CMAKE_SOURCE_DIR}/../include"
)

add_library(ObjectSource book.cpp person.cpp)
target_include_directories(ObjectSource PUBLIC
    "${CMAKE_SOURCE_DIR}/../include"
)

target_link_libraries(Main PUBLIC ObjectSource)
