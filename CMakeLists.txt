cmake_minimum_required(VERSION 3.13)
project(CodeRefactor)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

find_package(FLEX)
find_package(BISON)
BISON_TARGET(Parser Parser/grammar.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp COMPILE_FLAGS "-L c++")
FLEX_TARGET(Scanner Parser/lex.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp COMPILE_FLAGS "-+")
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

option(_TEST_ENV "Switch to test enviroment" ON)
if(_TEST_ENV STREQUAL ON)
    message(STATUS "In test enviroment")
    add_definitions(-DENV_TEST)
    link_libraries(gtest pthread)
endif()

add_executable(CodeRefactor
        main.cpp
        ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS}
		Parser/Scanner.h Parser/Driver.cpp Parser/Driver.h
		OpenHelper/OpenHelper.cpp OpenHelper/OpenHelper.h OpenHelper/StringStreamOpenHelper.cpp OpenHelper/StringStreamOpenHelper.h OpenHelper/StdioOpenHelper.cpp OpenHelper/StdioOpenHelper.h OpenHelper/FileOpenHelper.cpp OpenHelper/FileOpenHelper.h
        Decl/Decl.h Decl/DeclName.h Decl/DeclBase.h Decl/DeclBase.cpp
		AST/AST.hpp AST/Expr.cpp AST/Expr.h AST/Stmt.cpp AST/Stmt.h AST/StmtNodes.def AST/Type.cpp AST/Type.h AST/TypeNodes.def AST/ASTContext.h AST/ASTContext.cpp
		Basic/SourceLocation.h Basic/SourceLocation.cpp Basic/IdentifierTable.h Basic/IdentifierTable.cpp
		test/BasicParseTest.cpp test/OpenHelperTest.cpp test/DeclarationTest.cpp Decl/DeclGroup.cpp Decl/DeclGroup.h Parser/YaccAdapter.cpp Parser/YaccAdapter.h Decl/DeclContextHolder.cpp Decl/DeclContextHolder.h Errors/SymbolError.hpp test/HybridParseTest.cpp Errors/TypeError.hpp  test/ASTStructureTest.cpp
		ASTOperation/Printer.cpp ASTOperation/Printer.h ASTOperation/Corrector.cpp ASTOperation/Corrector.h ASTOperation/Refactor.h ASTOperation/Refactor.cpp
        Decl/DeclName.cpp Decl/Decl.cpp test/RefactorTest.cpp)
