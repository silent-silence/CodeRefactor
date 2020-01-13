cmake_minimum_required(VERSION 3.5)

project(Decl LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Decl_SRC Decl.cpp DeclBase.cpp DeclContextHolder.cpp DeclGroup.cpp DeclName.cpp)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    add_library(Decl STATIC ${Decl_SRC})
else()
    add_library(Decl SHARED ${Decl_SRC})
    set_target_properties(Decl PROPERTIES VERSION 1.0 SOVERSION 1)
endif()

target_compile_definitions(Decl INTERFACE Decl_LIBRARY)
