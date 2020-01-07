cmake_minimum_required(VERSION 3.5)

project(Parser LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLEX)
find_package(BISON)

BISON_TARGET(GRAMMARY grammar.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp COMPILE_FLAGS "-L c++")
FLEX_TARGET(LEX lex.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp COMPILE_FLAGS "-+")
ADD_FLEX_BISON_DEPENDENCY(LEX GRAMMARY)

set(Parser_SRC Driver.cpp YaccAdapter.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    add_library(Parser STATIC ${Parser_SRC})
else()
    add_library(Parser SHARED ${Parser_SRC})
    set_target_properties(Parser PROPERTIES VERSION 1.0 SOVERSION 1)
endif()

target_compile_definitions(Parser PRIVATE Parser_LIBRARY)
