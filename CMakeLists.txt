cmake_minimum_required(VERSION 3.20)

project("Compile commands keil converter"
         VERSION 0.0.1)

add_subdirectory(tinyxml2)

add_executable(converter main.cpp)
target_compile_options(converter PRIVATE -g)
target_link_libraries(converter PRIVATE tinyxml2)
