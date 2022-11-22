cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS)

project(A_Star_Search)

# add source files to a variable
file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

target_include_directories( ${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

configure_file(${CMAKE_SOURCE_DIR}/1.board 1.board COPYONLY)