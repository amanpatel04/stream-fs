cmake_minimum_required(VERSION 3.10) 

project(stream-fs VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} src/main.cpp
                                src/hard_disk.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include)


target_compile_definitions(${PROJECT_NAME} PRIVATE 
    PROJECT_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\"
)



