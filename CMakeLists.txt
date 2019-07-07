cmake_minimum_required(VERSION 3.10)

project(CodeRefactor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLEX)
find_package(BISON)

option(_TEST_ENV "Switch to test enviroment" OFF)
option(_QTGUI "Build GUI" ON)

if(_QTGUI STREQUAL ON)
    message(STATUS "Build GUI")
    add_definitions(-DENV_GUI)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    find_package(Qt5 COMPONENTS Core Quick REQUIRED)
endif()

BISON_TARGET(Parser Parser/grammar.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp COMPILE_FLAGS "-L c++")
FLEX_TARGET(Scanner Parser/lex.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp COMPILE_FLAGS "-+")
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

aux_source_directory(${CMAKE_SOURCE_DIR} DIR_MAIN)
aux_source_directory(${CMAKE_SOURCE_DIR}/AST DIR_AST)
aux_source_directory(${CMAKE_SOURCE_DIR}/Decl DIR_Decl)
aux_source_directory(${CMAKE_SOURCE_DIR}/Basic DIR_Basic)
aux_source_directory(${CMAKE_SOURCE_DIR}/Errors DIR_Errors)
aux_source_directory(${CMAKE_SOURCE_DIR}/Parser DIR_Parser)
aux_source_directory(${CMAKE_SOURCE_DIR}/Iterator DIR_Iterator)
aux_source_directory(${CMAKE_SOURCE_DIR}/OpenHelper DIR_OpenHelper)
aux_source_directory(${CMAKE_SOURCE_DIR}/ASTOperation DIR_Operation)
aux_source_directory(${CMAKE_SOURCE_DIR}/test DIR_test)
if(_QTGUI STREQUAL ON)
    aux_source_directory(${CMAKE_SOURCE_DIR}/Interface DIR_Interface)
endif()

if(_TEST_ENV STREQUAL ON)
    message(STATUS "In test enviroment")
    add_definitions(-DENV_TEST)
    link_libraries(gtest pthread)
endif()

if(CMAKE_BUILD_TYPE STREQUAL Release)
        message(STATUS "Build static version")
        SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
        SET(BUILD_SHARED_LIBS OFF)
        SET(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

if(_QTGUI STREQUAL ON)
add_executable(${PROJECT_NAME} ${DIR_test} "qml.qrc"
    ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS} ${DIR_Interface}
    ${DIR_MAIN}
    ${DIR_AST}
    ${DIR_Decl}
    ${DIR_Basic}
    ${DIR_Errors}
    ${DIR_Parser}
    ${DIR_Iterator}
    ${DIR_Operation}
    ${DIR_OpenHelper} Errors/SyntaxError.hpp)

#add_executable(${PROJECT_NAME} "main.cpp" ${DIR_Interface} "qml.qrc")
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick)

else()
    add_executable(${PROJECT_NAME} ${DIR_test} "qml.qrc"
            ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS}
            ${DIR_MAIN}
            ${DIR_AST}
            ${DIR_Decl}
            ${DIR_Basic}
            ${DIR_Errors}
            ${DIR_Parser}
            ${DIR_Iterator}
            ${DIR_Operation}
            ${DIR_OpenHelper} Errors/SyntaxError.hpp Errors/NullPointerError.h)
endif()