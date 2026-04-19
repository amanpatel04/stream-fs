cmake_minimum_required(VERSION 3.10) 

project(stream-fs VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES src/hard_disk.cpp
            src/indexing.cpp
            src/streamfs.cpp
)

add_library(stream_fs_lib ${SOURCES})

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE stream_fs_lib)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(stream_fs_lib PUBLIC include)


target_compile_definitions(${PROJECT_NAME} PRIVATE 
    PROJECT_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\"
)

target_compile_definitions(stream_fs_lib PRIVATE 
    PROJECT_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\"
)

enable_testing()
add_subdirectory(tests)
