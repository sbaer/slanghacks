cmake_minimum_required(VERSION 3.22)

project(slanghacks
    VERSION 1.0
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(slanghacks "main.cpp")

add_custom_command(TARGET slanghacks POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/slang/bin"
        "${CMAKE_BINARY_DIR}/$<CONFIG>/"
)



