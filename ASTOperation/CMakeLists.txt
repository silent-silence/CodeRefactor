cmake_minimum_required(VERSION 3.5)

project(ASTOperation LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ASTOperation_SRC Corrector.cpp Printer.cpp Refactor.cpp)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    add_library(ASTOperation STATIC ${ASTOperation_SRC})
else()
    add_library(ASTOperation SHARED ${ASTOperation_SRC})
    set_target_properties(ASTOperation PROPERTIES VERSION 1.0 SOVERSION 1)
endif()

target_compile_definitions(ASTOperation PRIVATE ASTOperation_LIBRARY)
