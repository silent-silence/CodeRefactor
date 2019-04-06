cmake_minimum_required(VERSION 3.13)
project(CodeRefactor)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

find_package(FLEX)
find_package(BISON)
BISON_TARGET(Parser Parser/grammar.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp COMPILE_FLAGS "-L c++")
FLEX_TARGET(Scanner Parser/lex.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp COMPILE_FLAGS "-+")
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

option(SET_TEST_ENV 0)
if(SET_TEST_ENV EQUAL 1)
    add_definitions(-DENV_TEST)
    link_directories(/usr/local/lib64)
    link_libraries(gtest pthread)
endif()

add_executable(CodeRefactor
        main.cpp
        Parser/yylval.h ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS}
        Factory/ExpressionFactory.h Factory/ExpressionFactory.cpp Expression/ArgumentList.h Expression/ArgumentList.cpp Expression/Constant.h Expression/Constant.cpp Expression/Expression.h Expression/Expression.cpp Expression/Identity.h Expression/Identity.cpp Expression/Literal.hpp Expression/Operation/BinaryOperation.h Expression/Operation/BinaryOperation.cpp Expression/Operation/CastExpression.h Expression/Operation/CastExpression.cpp Expression/Operation/CommaExpression.h Expression/Operation/CommaExpression.cpp Expression/Operation/FunctionCall.h Expression/Operation/FunctionCall.cpp Expression/Operation/MemberAccess.h Expression/Operation/MemberAccess.cpp Expression/Operation/Operation.h Expression/Operation/Operation.cpp Expression/Operation/PostfixUnaryOperation.h Expression/Operation/PostfixUnaryOperation.cpp Expression/Operation/PrefixUnaryOperation.cpp Expression/Operation/PrefixUnaryOperation.h Expression/Operation/RandomAccess.h Expression/Operation/RandomAccess.cpp Expression/Operation/TernaryOperation.h Expression/Operation/TernaryOperation.cpp test/ExpressionTest.cpp Parser/Scanner.h Parser/Driver.cpp Parser/Driver.h)
