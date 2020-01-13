cmake_minimum_required(VERSION 3.5)

project(Interface LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

set(Interface_SRC Interface.cpp qml.qrc)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    add_library(Interface STATIC ${Interface_SRC})
else()
    add_library(Interface SHARED ${Interface_SRC})
    set_target_properties(Interface PROPERTIES VERSION 1.0 SOVERSION 1)
endif()

#target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG> Interface_LIBRARY)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick)
