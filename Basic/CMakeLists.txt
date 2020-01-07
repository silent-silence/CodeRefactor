cmake_minimum_required(VERSION 3.5)

project(Basic LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Basic_SRC SourceLocation.cpp IdentifierTable.cpp)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    add_library(Basic STATIC ${Basic_SRC})
else()
    add_library(Basic SHARED ${Basic_SRC})
    set_target_properties(Basic PROPERTIES VERSION 1.0 SOVERSION 1)
endif()

target_compile_definitions(Basic PRIVATE Basic_LIBRARY)
