cmake_minimum_required(VERSION 3.10)

project(CodeRefactor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

option(_TEST_ENV "Switch to test enviroment" OFF)
option(_QTGUI "Build GUI" ON)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/AST
    ${CMAKE_SOURCE_DIR}/Decl
    ${CMAKE_SOURCE_DIR}/Basic
    ${CMAKE_SOURCE_DIR}/Errors
    ${CMAKE_SOURCE_DIR}/Parser
    ${CMAKE_SOURCE_DIR}/Iterator
    ${CMAKE_SOURCE_DIR}/Interface
    ${CMAKE_SOURCE_DIR}/OpenHelper
    ${CMAKE_SOURCE_DIR}/ASTOperation
    ${CMAKE_CURRENT_BINARY_DIR}/Parser
)

add_subdirectory(${CMAKE_SOURCE_DIR}/AST)
add_subdirectory(${CMAKE_SOURCE_DIR}/Decl)
add_subdirectory(${CMAKE_SOURCE_DIR}/Basic)
add_subdirectory(${CMAKE_SOURCE_DIR}/Errors)
add_subdirectory(${CMAKE_SOURCE_DIR}/Parser)
add_subdirectory(${CMAKE_SOURCE_DIR}/Iterator)
add_subdirectory(${CMAKE_SOURCE_DIR}/OpenHelper)
add_subdirectory(${CMAKE_SOURCE_DIR}/ASTOperation)

aux_source_directory(${CMAKE_SOURCE_DIR} DIR_MAIN)

add_executable(${PROJECT_NAME} ${DIR_test}
    ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS}
    ${DIR_MAIN}
)

if(_TEST_ENV STREQUAL ON)
    message(STATUS "In test enviroment")
    add_definitions(-DENV_TEST)
    link_libraries(gtest pthread)
    aux_source_directory(${CMAKE_SOURCE_DIR}/test DIR_test)
endif()

if(CMAKE_BUILD_TYPE STREQUAL Release)
    message(STATUS "Build static version")
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    SET(BUILD_SHARED_LIBS OFF)
    SET(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

if(_QTGUI STREQUAL ON)
    message(STATUS "Build GUI")
    add_definitions(-DENV_GUI)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    find_package(Qt5 COMPONENTS Core Quick REQUIRED)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Interface)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick AST
        Decl
        Basic
        Errors
        Parser
        Iterator
        Interface
        OpenHelper
        ASTOperation)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        AST
        Decl
        Basic
        Errors
        Parser
        Iterator
        Interface
        OpenHelper)
endif()


