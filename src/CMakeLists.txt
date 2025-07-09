set(XML_SOURCES xml_analysis.cpp)
add_library(xml STATIC ${XML_SOURCES})
target_include_directories(xml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(xml PUBLIC tinyxml2 fmt)

set(CMD_LINE_SOURCES cmd_line.cpp)
add_library(cmd_line STATIC ${CMD_LINE_SOURCES})
target_include_directories(cmd_line PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES main.cpp )
add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(${PROJECT_NAME} PRIVATE -g)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json xml cmd_line)
