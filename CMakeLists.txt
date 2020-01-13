cmake_minimum_required(VERSION 3.16)

project(CodeRefactor LANGUAGES CXX)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(QT_QMAKE_EXECUTABLE qmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../)
set(CMAKE_DEBUG_POSTFIX ".so")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

option(_TEST_ENV "Switch to test enviroment" OFF)
option(_QTGUI "Build GUI" ON)

set(LIBRARIES AST Decl Basic Parser Errors Iterator Interface OpenHelper ASTOperation)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(Qt5 COMPONENTS Core Quick REQUIRED)

BISON_TARGET(parser ${PROJECT_SOURCE_DIR}/Parser/grammar.y ${PROJECT_BINARY_DIR}/parser.cpp COMPILE_FLAGS "-L c++")
FLEX_TARGET(lexer ${PROJECT_SOURCE_DIR}/Parser/lex.l ${PROJECT_BINARY_DIR}/scanner.cpp COMPILE_FLAGS "-+")
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

foreach(LIB ${LIBRARIES})
    file(GLOB ${LIB}_SRC ${CMAKE_SOURCE_DIR}/${LIB}/*.cpp ${CMAKE_SOURCE_DIR}/${LIB}/*.qrc)
    add_library(${LIB} SHARED ${${LIB}_SRC} ${BISON_parser_OUTPUTS} ${FLEX_lexer_OUTPUTS})
    target_link_libraries(${LIB} PRIVATE Qt5::Core Qt5::Quick)
    message("Target ${LIB} have loaded.")
    target_compile_definitions(${LIB} PRIVATE LIB_LIBRARY)
endforeach(LIB)

aux_source_directory(${CMAKE_SOURCE_DIR} DIR_main)
add_executable(${PROJECT_NAME}
    ${DIR_test}
    ${DIR_main}
    )

if(CMAKE_BUILD_TYPE STREQUAL Release)
    message(STATUS "Build static version")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

if(_TEST_ENV STREQUAL ON)
    message(STATUS "In test enviroment")
    add_definitions(-DENV_TEST)
    link_libraries(gtest pthread)

    file(GLOB Test_SRC ${CMAKE_SOURCE_DIR}/Test/*.cpp)
    add_library(Test SHARED Test_SRC)
    message("Target Test have loaded.")
    target_compile_definitions(Test PRIVATE LIB_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PRIVATE Test)
endif()

if(_QTGUI STREQUAL ON)
    message(STATUS "Build GUI")
    add_definitions(-DENV_GUI)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick ${LIBRARIES})
