cmake_minimum_required(VERSION 3.5)

project(
    ringbuffer_sga_stage7
    VERSION 0.1
    DESCRIPTION "Ring Buffer SGA Stage 7"
    LANGUAGES CXX)

#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)

add_executable(main ${PROJECT_SOURCE_DIR}/src/main.cpp)

target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR}/include)
