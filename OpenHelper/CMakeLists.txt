cmake_minimum_required(VERSION 3.5)

project(OpenHelper LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenHelper_SRC OpenHelper.cpp FileOpenHelper.cpp StdioOpenHelper.cpp StringStreamOpenHelper.cpp)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    add_library(OpenHelper STATIC ${OpenHelper_SRC})
else()
    add_library(OpenHelper SHARED ${OpenHelper_SRC})
    set_target_properties(OpenHelper PROPERTIES VERSION 1.0 SOVERSION 1)
endif()

target_compile_definitions(OpenHelper INTERFACE OpenHelper_LIBRARY)
