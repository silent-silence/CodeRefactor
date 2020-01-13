cmake_minimum_required(VERSION 3.5)

project(Iterator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Iterator_SRC StmtIterator.cpp DeclIterator.cpp)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    add_library(Iterator STATIC ${Iterator_SRC})
else()
    add_library(Iterator SHARED ${Iterator_SRC})
    set_target_properties(Iterator PROPERTIES VERSION 1.0 SOVERSION 1)
endif()

target_compile_definitions(Iterator INTERFACE Iterator_LIBRARY)
