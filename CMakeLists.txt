cmake_minimum_required(VERSION 3.20)

project("Compile commands keil converter"
         VERSION 0.0.1)

set(PROJECT_NAME keil_project_analyzer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(fmt)
add_subdirectory(json)
add_subdirectory(tinyxml2)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -g)
target_link_libraries(${PROJECT_NAME} PRIVATE tinyxml2)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt)
