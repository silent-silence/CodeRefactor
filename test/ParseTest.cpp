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
#include "AST/ASTContext.h"

using std::make_shared;				using std::shared_ptr;
using std::fstream;					using std::string;
using std::dynamic_pointer_cast;	using std::weak_ptr;

class ParseTest : public testing::Test {
protected:
	void SetUp() override {}
	void TearDown() override {}

	StringStreamOpenHelper openHelper;
	ASTContext context;
	Driver driver{openHelper, context};
};

TEST_F(ParseTest, LiteralExpression)
{
	openHelper << "12;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "12.3;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<FloatingLiteral>(context.getTop().lock()));
	context.cleanAST();
}

TEST_F(ParseTest, UnaryExpression)
{
	openHelper << "1++;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "--1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "+1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(context.getTop().lock()));
}

TEST_F(ParseTest, AssignExpression)
{
	openHelper << "1=1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundAssignOperator>(context.getTop().lock()));
}

TEST_F(ParseTest, BinaryExpression)
{
	openHelper << "1*1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BinaryOperator>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "1,1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BinaryOperator>(context.getTop().lock()));
	context.cleanAST();
}

TEST_F(ParseTest, ConditionalExpression)
{
	openHelper << "1?1:1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ConditionalOperator>(context.getTop().lock()));
}

TEST_F(ParseTest, StringLiteralExpression)
{
	std::string input = "\"string\";";
	openHelper << input;
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<StringLiteral>(context.getTop().lock()));
	context.cleanAST();

	input = "'s';";
	openHelper << input;
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CharacterLiteral>(context.getTop().lock()));
}

TEST_F(ParseTest, CompoundStmt)
{
	openHelper << "{ 123; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "{ 123; 1234; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "1; { 123; 1234; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "{ 123; 1234; } 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "{ 123; 1234; 123; { 123; 123; } { 13; } { } } 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(context.getTop().lock()));
	context.cleanAST();
}

TEST_F(ParseTest, SimpleStmts)
{
	openHelper << ";";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<NullStmt>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "continue;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ContinueStmt>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "break;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BreakStmt>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "return;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ReturnStmt>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "return 132;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ReturnStmt>(context.getTop().lock()));
	context.cleanAST();
}

TEST_F(ParseTest, ParenExprTest)
{
	openHelper << "(123);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ParenExpr>(context.getTop().lock()));
	context.cleanAST();
}

TEST_F(ParseTest, SizeofTest)
{
	openHelper << "sizeof(int);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "sizeof(1);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "sizeof 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(context.getTop().lock()));
	context.cleanAST();
}

TEST_F(ParseTest, ArraySubscriptTest)
{
	openHelper << "12[43];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ArraySubscriptExpr>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "12[43][12];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ArraySubscriptExpr>(context.getTop().lock()));
	context.cleanAST();
}

TEST_F(ParseTest, MemberExprTest)
{
	// TODO: do this after identities is recognised
	/*openHelper << "a.a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<MemberExpr>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "a->a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<MemberExpr>(context.getTop().lock()));
	context.cleanAST();*/
}

TEST_F(ParseTest, CastTest)
{
	openHelper << "(int)43;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CStyleCastExpr>(context.getTop().lock()));
	context.cleanAST();
}

TEST_F(ParseTest, CallTest)
{
	// TODO: do this after CallExpr::SubExpr initialized
	/*openHelper << "34();";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CStyleCastExpr>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "34(12, 43);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CStyleCastExpr>(context.getTop().lock()));
	context.cleanAST();*/
}

TEST_F(ParseTest, IfTest)
{}

TEST_F(ParseTest, ForTest)
{}

TEST_F(ParseTest, WhileTest)
{
	openHelper << "while(1) {}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<WhileStmt>(context.getTop().lock()));
	context.cleanAST();
}

TEST_F(ParseTest, DoWhileTest)
{
	openHelper << "do 12; while(1)";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DoStmt>(context.getTop().lock()));
	context.cleanAST();

	openHelper << "do { 12; } while(1)";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DoStmt>(context.getTop().lock()));
	context.cleanAST();
}

TEST_F(ParseTest, SwitchTest)
{
	openHelper << "switch(1) {}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SwitchStmt>(context.getTop().lock()));
	context.cleanAST();
}

TEST_F(ParseTest, CaseTest)
{
	openHelper << "case 1: 12;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CaseStmt>(context.getTop().lock()));
	context.cleanAST();
}

TEST_F(ParseTest, DefaultTest)
{
	openHelper << "default : 12;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DefaultStmt>(context.getTop().lock()));
	context.cleanAST();
}

TEST_F(ParseTest, BasicTypeTest)
{
	openHelper << "int a;";
	driver.parse();
}

TEST_F(ParseTest, QualifiedBasicTypeTest)
{
	openHelper << "extern const int a;";
	driver.parse();
}
#endif