cmake_minimum_required(VERSION 3.5)

project(Errors LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Errors_SRC NullPointerError.cpp)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    add_library(Errors STATIC ${Errors_SRC})
else()
    add_library(Errors SHARED ${Errors_SRC})
    set_target_properties(Errors PROPERTIES VERSION 1.0 SOVERSION 1)
endif()

target_compile_definitions(Errors INTERFACE Errors_LIBRARY)
