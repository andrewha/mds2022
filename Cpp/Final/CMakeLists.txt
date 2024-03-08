cmake_minimum_required(VERSION 3.5)

project(
    final_project_sga
    VERSION 0.1
    DESCRIPTION "Final Project SGA"
    LANGUAGES CXX)

#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)

# -O3 for max speed optimization

add_compile_options(-Wall 
                    -O3)

add_executable(main 
                ${PROJECT_SOURCE_DIR}/src/main.cpp 
                ${PROJECT_SOURCE_DIR}/src/record.cpp 
                ${PROJECT_SOURCE_DIR}/src/register.cpp 
                ${PROJECT_SOURCE_DIR}/src/printers.cpp
            )

target_include_directories(main 
                            PUBLIC 
                            ${PROJECT_SOURCE_DIR}/include
                        )
