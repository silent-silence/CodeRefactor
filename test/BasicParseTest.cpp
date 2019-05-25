//
// Created by gaojian on 19-4-3.
//

#ifdef ENV_TEST

#include <memory>
#include <string>
#include <gtest/gtest.h>
#include "OpenHelper/StringStreamOpenHelper.h"
#include "Parser/Driver.h"
#include "Parser/YaccAdapter.h"
#include "AST/ASTContext.h"
#include "Decl/DeclContextHolder.h"
#include "Errors/TypeError.hpp"

using std::make_shared;				using std::shared_ptr;
using std::fstream;					using std::string;
using std::dynamic_pointer_cast;	using std::weak_ptr;

class BasicParseTest : public testing::Test {
protected:
	void SetUp() override {}
	void TearDown() override {}

	StringStreamOpenHelper openHelper;
	ASTContext astContext;
	DeclContextHolder declContext;
	YaccAdapter adapter{astContext, declContext, openHelper};
	Driver driver{openHelper, adapter};
};

TEST_F(BasicParseTest, LiteralExpression)
{
	openHelper << "12;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "12.3;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<FloatingLiteral>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, UnaryExpression)
{
	openHelper << "1++;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "--1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "+1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(astContext.getRoot().lock()));
}

TEST_F(BasicParseTest, AssignExpression)
{
	openHelper << "1=1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundAssignOperator>(astContext.getRoot().lock()));
}

TEST_F(BasicParseTest, BinaryExpression)
{
	openHelper << "1*1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BinaryOperator>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "1,1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BinaryOperator>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, ConditionalExpression)
{
	openHelper << "1?1:1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ConditionalOperator>(astContext.getRoot().lock()));
}

TEST_F(BasicParseTest, StringLiteralExpression)
{
	std::string input = "\"string\";";
	openHelper << input;
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<StringLiteral>(astContext.getRoot().lock()));
	adapter.clean();

	input = "'s';";
	openHelper << input;
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CharacterLiteral>(astContext.getRoot().lock()));
}

TEST_F(BasicParseTest, CompoundStmt)
{
	openHelper << "{ 123; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "{ 123; 1234; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "1; { 123; 1234; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "{ 123; 1234; } 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "{ 123; 1234; 123; { 123; 123; } { 13; } { } } 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, SimpleStmts)
{
	openHelper << ";";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<NullStmt>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "continue;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ContinueStmt>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "break;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BreakStmt>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "return;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ReturnStmt>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "return 132;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ReturnStmt>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, ParenExprTest)
{
	openHelper << "(123);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ParenExpr>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, SizeofTest)
{
	openHelper << "sizeof(int);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "sizeof(1);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "sizeof 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, ArraySubscriptTest)
{
	openHelper << "12[43];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ArraySubscriptExpr>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "12[43][12];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ArraySubscriptExpr>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, MemberExprTest)
{
	openHelper << "int a; a.a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<MemberExpr>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "a->a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<MemberExpr>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, CastTest)
{
	openHelper << "(int)43;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CStyleCastExpr>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, CallTest)
{
	openHelper << "34();";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CallExpr>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "34(12, 43);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CallExpr>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, IfTest)
{
	openHelper << "if(1) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "if(1) { 123; 23452; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	adapter.clean();

	// TODO Matching if else
	openHelper << "if(1) if(2) ; else {123; } else {345;}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "if(1) { } else if(2) { }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, ForTest)
{
	openHelper << "for(;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "for(int a;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "for(1;2;3) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "for(int b;2;3) { 12; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	adapter.clean();

	// TODO: do this after struct is recognised
	/*openHelper << "for(struct { int a; int b; } c;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	adapter.clean();*/
}

TEST_F(BasicParseTest, WhileTest)
{
	openHelper << "while(1) {}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<WhileStmt>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, DoWhileTest)
{
	openHelper << "do 12; while(1)";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DoStmt>(astContext.getRoot().lock()));
	adapter.clean();

	openHelper << "do { 12; } while(1)";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DoStmt>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, SwitchTest)
{
	openHelper << "switch(1) {}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SwitchStmt>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, CaseTest)
{
	openHelper << "case 1: 12;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CaseStmt>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, DefaultTest)
{
	openHelper << "default : 12;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DefaultStmt>(astContext.getRoot().lock()));
	adapter.clean();
}

TEST_F(BasicParseTest, QualifiedBasicTypeTest)
{
	// void
	openHelper << "void;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned void;";
	EXPECT_THROW(driver.parse(), TypeError);

	// char
	openHelper << "char;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned char;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long char;";
	EXPECT_THROW(driver.parse(), TypeError);

	// short
	openHelper << "short;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "short int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long short;";
	EXPECT_THROW(driver.parse(), TypeError);

	// int
	openHelper << "int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "signed int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "int double;";
	EXPECT_THROW(driver.parse(), TypeError);

	// float
	openHelper << "float;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned float;";
	EXPECT_THROW(driver.parse(), TypeError);

	openHelper << "long float;";
	EXPECT_THROW(driver.parse(), TypeError);

	// double
	openHelper << "double;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long double;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long long double;";
	EXPECT_THROW(driver.parse(), TypeError);

	openHelper << "unsigned double;";
	EXPECT_THROW(driver.parse(), TypeError);

	// signed & unsigned
	openHelper << "signed;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned signed;";
	EXPECT_THROW(driver.parse(), TypeError);

	openHelper << "unsigned unsigned;";
	EXPECT_THROW(driver.parse(), TypeError);

	// long & long long
	openHelper << "long;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long long;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long unsigned long int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long long long;";
	EXPECT_THROW(driver.parse(), TypeError);

	// storage
	openHelper << "typedef;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "extern;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "static;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "register;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "extern static;";
	EXPECT_THROW(driver.parse(), TypeError);
}

TEST_F(BasicParseTest, VariableTest)
{
	openHelper << "long int a;";
	driver.parse();
	EXPECT_EQ(
			dynamic_pointer_cast<VariableDecl>(declContext.getContext()->lookup("a").lock())->getIdentifier(),
			string("a")
	);
	astContext.cleanAST();
	declContext.clean();

	openHelper << "long int b[][];";
	driver.parse();
	EXPECT_EQ(
			dynamic_pointer_cast<VariableDecl>(declContext.getContext()->lookup("b").lock())->getIdentifier(),
			string("b")
	);
	astContext.cleanAST();
	declContext.clean();

	openHelper << "long int c, d, e;";
	driver.parse();
	EXPECT_EQ(
			dynamic_pointer_cast<VariableDecl>(declContext.getContext()->lookup("c").lock())->getIdentifier(),
			string("c")
	);
	EXPECT_EQ(
			dynamic_pointer_cast<VariableDecl>(declContext.getContext()->lookup("d").lock())->getIdentifier(),
			string("d")
	);
	EXPECT_EQ(
			dynamic_pointer_cast<VariableDecl>(declContext.getContext()->lookup("e").lock())->getIdentifier(),
			string("e")
	);
	astContext.cleanAST();
	declContext.clean();
}


TEST_F(BasicParseTest, FunctionTest)
{
	openHelper << "int a();";
	driver.parse();
	EXPECT_EQ(
			dynamic_pointer_cast<VariableDecl>(declContext.getContext()->lookup("a").lock())->getIdentifier(),
			string("a")
	);
	astContext.cleanAST();
	declContext.clean();
}
#endif