cmake_minimum_required(VERSION 3.20)

project("Compile commands keil converter"
         VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(tinyxml2)
add_subdirectory(json)

add_executable(converter main.cpp)
target_compile_options(converter PRIVATE -g)
target_link_libraries(converter PRIVATE tinyxml2)
target_link_libraries(converter PRIVATE nlohmann_json)
