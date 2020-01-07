cmake_minimum_required(VERSION 3.5)

project(AST LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(AST_SRC ASTContext.cpp Expr.cpp Stmt.cpp Type.cpp)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    add_library(AST STATIC ${AST_SRC})
else()
    add_library(AST SHARED ${AST_SRC})
    set_target_properties(AST PROPERTIES VERSION 1.0 SOVERSION 1)
endif()

target_compile_definitions(AST PRIVATE AST_LIBRARY)
