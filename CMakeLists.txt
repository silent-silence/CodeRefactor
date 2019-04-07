cmake_minimum_required(VERSION 3.13)
project(CodeRefactor)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

find_package(FLEX)
find_package(BISON)
BISON_TARGET(Parser Parser/grammar.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp COMPILE_FLAGS "-L c++")
FLEX_TARGET(Scanner Parser/lex.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp COMPILE_FLAGS "-+")
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

option(_TEST_ENV "Switch to test enviroment" OFF)
if(_TEST_ENV STREQUAL ON)
    message("In test enviroment")
    add_definitions(-DENV_TEST)
    link_libraries(gtest pthread)
endif()

add_executable(CodeRefactor
        main.cpp
        ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS}
        Factory/ExpressionFactory.h Factory/ExpressionFactory.cpp Expression/ArgumentList.h Expression/ArgumentList.cpp Expression/Constant.h Expression/Constant.cpp Expression/Expression.h Expression/Expression.cpp Expression/Identity.h Expression/Identity.cpp Expression/Literal.hpp Expression/Operation/BinaryOperation.h Expression/Operation/BinaryOperation.cpp Expression/Operation/CastExpression.h Expression/Operation/CastExpression.cpp Expression/Operation/CommaExpression.h Expression/Operation/CommaExpression.cpp Expression/Operation/FunctionCall.h Expression/Operation/FunctionCall.cpp Expression/Operation/MemberAccess.h Expression/Operation/MemberAccess.cpp Expression/Operation/Operation.h Expression/Operation/Operation.cpp Expression/Operation/PostfixUnaryOperation.h Expression/Operation/PostfixUnaryOperation.cpp Expression/Operation/PrefixUnaryOperation.cpp Expression/Operation/PrefixUnaryOperation.h Expression/Operation/RandomAccess.h Expression/Operation/RandomAccess.cpp Expression/Operation/TernaryOperation.h Expression/Operation/TernaryOperation.cpp test/ExpressionTest.cpp Parser/Scanner.h Parser/Driver.cpp Parser/Driver.h Opener/Opener.cpp Opener/Opener.h Opener/StringOpener.cpp Opener/StringOpener.h Opener/StdioOpener.cpp Opener/StdioOpener.h Opener/FileOpener.cpp Opener/FileOpener.h test/OpenerTest.cpp)
