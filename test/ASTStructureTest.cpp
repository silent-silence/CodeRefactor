//
// Created by 17271 on 2019/5/27.
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
#include "ASTOperation/Printer.h"

using std::make_shared;				using std::shared_ptr;
using std::fstream;					using std::string;
using std::dynamic_pointer_cast;	using std::weak_ptr;

class ASTStructureTest : public testing::Test {
protected:
	void SetUp() override {}
	void TearDown() override {
		printer.resetPrinter();
		output.clear();
		adapter.clean();
	}

	void reset() {
		printer.resetPrinter();
		output.clear();
		adapter.clean();
	}

	StringStreamOpenHelper openHelper;
	ASTContext astContext;
	DeclContextHolder declContext;
	YaccAdapter adapter{astContext, declContext, openHelper};
	Driver driver{openHelper, adapter};
	StringStreamOpenHelper printerOutput;
	Printer printer{printerOutput};
	string output;
};

TEST_F(ASTStructureTest, LiteralExpression)
{
	openHelper << "12;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("12"));
	reset();

	openHelper << "12.3;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<FloatingLiteral>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("12.3"));
	reset();
}

TEST_F(ASTStructureTest, UnaryExpression)
{
	openHelper << "1++;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("1++"));
	reset();

	openHelper << "--1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("--1"));
	reset();

	openHelper << "+1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("+1"));
	reset();
}

TEST_F(ASTStructureTest, AssignExpression)
{
	openHelper << "1=1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundAssignOperator>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("1=1"));
	reset();
}

TEST_F(ASTStructureTest, BinaryExpression)
{
	openHelper << "1*1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BinaryOperator>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("1*1"));
	reset();

	openHelper << "1,1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BinaryOperator>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("1,1"));
	reset();
}

TEST_F(ASTStructureTest, ConditionalExpression)
{
	openHelper << "1?1:1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ConditionalOperator>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("1?1:1"));
	reset();
}

TEST_F(ASTStructureTest, StringLiteralExpression)
{
	std::string input = "\"string\";";
	openHelper << input;
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<StringLiteral>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("\"string\""));
	reset();

	input = "'s';";
	openHelper << input;
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CharacterLiteral>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("'s'"));
	reset();
}

TEST_F(ASTStructureTest, CompoundStmt)
{
	openHelper << "{ 123; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  123;\n"
"}\n"
));
	reset();

	openHelper << "{ 123; 1234; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  123;\n"
"  1234;\n"
"}\n"
));
	reset();

	openHelper << "1; { 123; 1234; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "{ 123; 1234; } 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(astContext.getRoot().lock()));
	reset();

	openHelper << "{ 123; 1234; 123; { 123; 123; } { 13; } { } } 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(astContext.getRoot().lock()));
	reset();
}

TEST_F(ASTStructureTest, SimpleStmts)
{
	openHelper << ";";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<NullStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(";\n"));
	reset();

	openHelper << "continue;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ContinueStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("continue;\n"));
	reset();

	openHelper << "break;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BreakStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("break;\n"));
	reset();

	openHelper << "return;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ReturnStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("return ;\n"));
	reset();

	openHelper << "return 132;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ReturnStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("return 132;\n"));
	reset();
}

TEST_F(ASTStructureTest, ParenExprTest)
{
	openHelper << "(123);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ParenExpr>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("(123)"));
	reset();
}

TEST_F(ASTStructureTest, SizeofTest)
{
	openHelper << "sizeof(int);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(astContext.getRoot().lock()));
	reset();

	openHelper << "sizeof(1);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("sizeof (1)"));
	reset();

	openHelper << "sizeof 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("sizeof 1"));
	reset();
}

TEST_F(ASTStructureTest, ArraySubscriptTest)
{
	openHelper << "12[43];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ArraySubscriptExpr>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("12[43]"));
	reset();

	openHelper << "12[43][12];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ArraySubscriptExpr>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("12[43][12]"));
	reset();

	// TODO function return value
	/*openHelper << "foo()[43][12];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ArraySubscriptExpr>(astContext.getRoot().lock()));*/
}

TEST_F(ASTStructureTest, IfTest)
{
	openHelper << "if(1) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"if(1)\n"
";\n"
));
	reset();

	openHelper << "if(1) { 123; 23452; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"if(1)\n"
"{\n"
"  123;\n"
"  23452;\n"
"}\n"
));
	reset();

	// TODO Matching if else
	openHelper << "if(1) if(2) ; else {123; } else {345;}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"if(1)\n"
"if(2)\n"
";\n"
"else {\n"
"  123;\n"
"}\n"
"else {\n"
"  345;\n"
"}\n"
));
	reset();

	openHelper << "if(1) { } else if(2) { }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"if(1)\n"
"{\n"
"}\n"
"else if(2)\n"
"{\n"
"}\n"
));
	reset();
}

TEST_F(ASTStructureTest, ForTest)
{
	/*openHelper << "for(;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"for(; ; )\n"
";"
));
	reset();

	openHelper << "for(int a;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "for(1;2;3) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "for(int b;2;3) { 12; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	reset();*/

	// TODO: do this after struct is recognised
	/*openHelper << "for(struct { int a; int b; } c;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	adapter.clean();*/
}

TEST_F(ASTStructureTest, WhileTest)
{
	openHelper << "while(1) {}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<WhileStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("while(1)\n{\n}\n"));
	reset();
}

TEST_F(ASTStructureTest, DoWhileTest)
{
	openHelper << "do 12; while(1)";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DoStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"do\n"
"12;\n"
"while(1)\n"
));
	reset();

	openHelper << "do { 12; } while(1)";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DoStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"do\n"
"{\n"
"  12;\n"
"}\n"
"while(1)\n"
));
	reset();
}

TEST_F(ASTStructureTest, CaseTest)
{
	openHelper << "case 1: 12;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CaseStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"case 1:\n"
));
	reset();
}


#endif