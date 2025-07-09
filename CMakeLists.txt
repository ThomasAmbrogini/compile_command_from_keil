cmake_minimum_required(VERSION 3.20)

project("Compile commands keil converter"
         VERSION 0.0.1)

set(PROJECT_NAME keil_project_analyzer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(fmt)
add_subdirectory(json)
add_subdirectory(tinyxml2)
add_subdirectory(src)
