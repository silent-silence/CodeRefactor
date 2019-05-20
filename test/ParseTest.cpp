//
// Created by gaojian on 19-4-3.
//

#ifdef ENV_TEST

#include <memory>
#include <fstream>
#include <string>
#include <gtest/gtest.h>
#include "Opener/StringStreamOpenHelper.h"
#include "Parser/Driver.h"
#include "AST/ASTContext.h"

using std::make_shared;				using std::shared_ptr;
using std::fstream;					using std::string;
using std::dynamic_pointer_cast;	using std::weak_ptr;

class ParseTest : public testing::Test {
protected:
	void SetUp() override {}
	void TearDown() override {}

	StringStreamOpenHelper opener;
	ASTContext context;
	Driver driver{opener, context};
};

TEST_F(ParseTest, LiteralExpression)
{
	opener << "12;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(context.getTop().lock()));
	context.cleanQueue();

	opener << "12.3;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<FloatingLiteral>(context.getTop().lock()));
}

TEST_F(ParseTest, UnaryExpression)
{
	opener << "1++;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(context.getTop().lock()));
	context.cleanQueue();

	opener << "--1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(context.getTop().lock()));
	context.cleanQueue();

	opener << "+1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(context.getTop().lock()));
}

TEST_F(ParseTest, AssignExpression)
{
	opener << "1=1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundAssignOperator>(context.getTop().lock()));
}

TEST_F(ParseTest, BinaryExpression)
{
	opener << "1*1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BinaryOperator>(context.getTop().lock()));
}

TEST_F(ParseTest, ConditionalExpression)
{
	opener << "1?1:1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ConditionalOperator>(context.getTop().lock()));
}

TEST_F(ParseTest, StringLiteralExpression)
{
	std::string input = "\"string\";";
	opener << input;
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<StringLiteral>(context.getTop().lock()));
}

TEST_F(ParseTest, CompoundStmt)
{
	opener << "{ 123; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(context.getTop().lock()));
	context.cleanQueue();

	opener << "{ 123; 1234; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(context.getTop().lock()));
	context.cleanQueue();

	opener << "1; { 123; 1234; }";
	driver.parse();
	// Compound stmt is the top?
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(context.getTop().lock()));
	context.cleanQueue();

	opener << "{ 123; 1234; } 1;";
	driver.parse();
	// Integer literal is the top?
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(context.getTop().lock()));
	context.cleanQueue();
}
#endif