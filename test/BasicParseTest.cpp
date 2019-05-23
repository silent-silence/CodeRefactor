//
// Created by gaojian on 19-4-3.
//

#ifdef ENV_TEST

#include <memory>
#include <fstream>
#include <string>
#include <gtest/gtest.h>
#include <OpenHelper/FileOpenHelper.h>
#include "OpenHelper/StringStreamOpenHelper.h"
#include "Parser/Driver.h"
#include "Parser/YaccAdapter.h"
#include "AST/ASTContext.h"
#include "Decl/DeclContextHolder.h"

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
	// TODO: do this after identities is recognised
	/*openHelper << "a.a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<MemberExpr>(astContext.getTop().lock()));
	adapter.clean();

	openHelper << "a->a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<MemberExpr>(astContext.getRoot().lock()));
	adapter.clean();*/
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
	// TODO: do this after CallExpr::SubExpr initialized
	/*openHelper << "34();";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CStyleCastExpr>(astContext.getTop().lock()));
	adapter.clean();

	openHelper << "34(12, 43);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CStyleCastExpr>(astContext.getRoot().lock()));
	adapter.clean();*/
}

TEST_F(BasicParseTest, IfTest)
{}

TEST_F(BasicParseTest, ForTest)
{}

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
	EXPECT_THROW(driver.parse(), std::runtime_error);

	// char
	openHelper << "char;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned char;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long char;";
	EXPECT_THROW(driver.parse(), std::runtime_error);

	// short
	openHelper << "short;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "short int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long short;";
	EXPECT_THROW(driver.parse(), std::runtime_error);

	// int
	openHelper << "int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "signed int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "int double;";
	EXPECT_THROW(driver.parse(), std::runtime_error);

	// float
	openHelper << "float;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned float;";
	EXPECT_THROW(driver.parse(), std::runtime_error);

	openHelper << "long float;";
	EXPECT_THROW(driver.parse(), std::runtime_error);

	// double
	openHelper << "double;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long double;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long long double;";
	EXPECT_THROW(driver.parse(), std::runtime_error);

	openHelper << "unsigned double;";
	EXPECT_THROW(driver.parse(), std::runtime_error);

	// signed & unsigned
	openHelper << "signed;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned signed;";
	EXPECT_THROW(driver.parse(), std::runtime_error);

	openHelper << "unsigned unsigned;";
	EXPECT_THROW(driver.parse(), std::runtime_error);

	// long & long long
	openHelper << "long;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long long;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long unsigned long int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long long long;";
	EXPECT_THROW(driver.parse(), std::runtime_error);

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
	EXPECT_THROW(driver.parse(), std::runtime_error);
}

TEST_F(BasicParseTest, VariableTest)
{
	openHelper << "long int a;";
	driver.parse();
	auto var = dynamic_pointer_cast<VariableDecl>(declContext.getContext()->lookup("a").lock());
	EXPECT_EQ(var->getIdentifier(), string("a"));
}

#endif