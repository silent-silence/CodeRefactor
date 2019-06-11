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
using Printer::ASTPrinter;

class ASTStructurePrintTest : public testing::Test {
protected:
	void SetUp() override {}
	void TearDown() override {
		Printer::resetPrinter();
		output.clear();
		adapter.clean();
	}

	void reset() {
		Printer::resetPrinter();
		output.clear();
		adapter.clean();
	}

	StringStreamOpenHelper openHelper;
	ASTContext astContext;
	DeclContextHolder declContext;
	YaccAdapter adapter{astContext, declContext, openHelper};
	Driver driver{openHelper, adapter};
	StringStreamOpenHelper printerOutput;
	ASTPrinter printer{printerOutput};
	string output;
};

TEST_F(ASTStructurePrintTest, LiteralExpression)
{
	openHelper << "12;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("12"));
	reset();

	openHelper << "12.3;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<FloatingLiteral>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("12.3"));
	reset();
}

TEST_F(ASTStructurePrintTest, UnaryExpression)
{
	openHelper << "1++;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("1++"));
	reset();

	openHelper << "--1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("--1"));
	reset();

	openHelper << "+1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("+1"));
	reset();
}

TEST_F(ASTStructurePrintTest, AssignExpression)
{
	openHelper << "1=1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundAssignOperator>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("1=1"));
	reset();
}

TEST_F(ASTStructurePrintTest, BinaryExpression)
{
	openHelper << "1*1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BinaryOperator>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("1*1"));
	reset();

	openHelper << "1,1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BinaryOperator>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("1,1"));
	reset();
}

TEST_F(ASTStructurePrintTest, ConditionalExpression)
{
	openHelper << "1?1:1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ConditionalOperator>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("1?1:1"));
	reset();
}

TEST_F(ASTStructurePrintTest, StringLiteralExpression)
{
	std::string input = "\"string\";";
	openHelper << input;
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<StringLiteral>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("\"string\""));
	reset();

	input = "'s';";
	openHelper << input;
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CharacterLiteral>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("'s'"));
	reset();
}

TEST_F(ASTStructurePrintTest, CompoundStmt)
{
	openHelper << "{ 123; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
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
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  123;\n"
"  1234;\n"
"}\n"
));
	reset();

	openHelper << "{ 123; 1234; 123; { 123; 123; } { 13; } { } }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  123;\n"
"  1234;\n"
"  123;\n"
"  {\n"
"    123;\n"
"    123;\n"
"  }\n"
"  {\n"
"    13;\n"
"  }\n"
"  {\n"
"  }\n"
"}\n"
));
	reset();
}

TEST_F(ASTStructurePrintTest, SimpleStmts)
{
	openHelper << ";";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<NullStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(";\n"));
	reset();

	openHelper << "continue;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ContinueStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("continue;\n"));
	reset();

	openHelper << "break;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BreakStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("break;\n"));
	reset();

	openHelper << "return;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ReturnStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("return ;\n"));
	reset();

	openHelper << "return 132;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ReturnStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("return 132;\n"));
	reset();
}

TEST_F(ASTStructurePrintTest, ParenExprTest)
{
	openHelper << "(123);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ParenExpr>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("(123)"));
	reset();
}

TEST_F(ASTStructurePrintTest, SizeofTest)
{
	openHelper << "sizeof(int);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(astContext.getRoot().lock()));
	reset();

	openHelper << "sizeof(1);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("sizeof (1)"));
	reset();

	openHelper << "sizeof 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("sizeof 1"));
	reset();
}

TEST_F(ASTStructurePrintTest, CastTest)
{
	openHelper << "(int)43;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CStyleCastExpr>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("(int)43"));
	reset();
}

TEST_F(ASTStructurePrintTest, ArraySubscriptTest)
{
	openHelper << "12[43];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ArraySubscriptExpr>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("12[43]"));
	reset();

	openHelper << "12[43][12];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ArraySubscriptExpr>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("12[43][12]"));
	reset();

	// TODO function return value
	/*openHelper << "foo()[43][12];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ArraySubscriptExpr>(astContext.getRoot().lock()));*/
}

TEST_F(ASTStructurePrintTest, IfTest)
{
	openHelper <<
"if(1)"
"	;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"if(1)\n"
"  ;\n"
));
	reset();

	openHelper <<
"if(1)"
"{"
"	123;"
"	23452;"
"}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
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
	openHelper <<
"if(1)"
"	if(2) "
"		;"
"	else "
"	{"
"		123;"
"	}"
"else"
"{"
"	345;"
"}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"if(1)\n"
"  if(2)\n"
"    ;\n"
"  else \n"
"  {\n"
"    123;\n"
"  }\n"
"else \n"
"{\n"
"  345;\n"
"}\n"
));
	reset();

	openHelper << "if(1) { } else if(2) { }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
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

	openHelper <<
"{"
"	if(1)"
"	{"
"	}"
"	else if(2)"
"	{"
"	}"
"}";
	driver.parse();
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  if(1)\n"
"  {\n"
"  }\n"
"  else if(2)\n"
"  {\n"
"  }\n"
"}\n"
));
	reset();
}

TEST_F(ASTStructurePrintTest, ForTest)
{
	openHelper << "for(;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"for(;;)\n"
";\n"
));
	reset();

	openHelper << "for(int a;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"for(int a;;)\n"
";\n"
));
	reset();

	openHelper << "for(1;2;3) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"for(1;2;3)\n"
";\n"
));
	reset();

	openHelper << "for(int b;2;3) { 12; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"for(int b;2;3)\n"
"{\n"
"  12;\n"
"}\n"
));
	reset();

	// TODO: do this after struct is recognised
	/*openHelper << "for(struct { int a; int b; } c;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	adapter.clean();*/
}

TEST_F(ASTStructurePrintTest, WhileTest)
{
	openHelper << "while(1) {}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<WhileStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("while(1)\n{\n}\n"));
	reset();
}

TEST_F(ASTStructurePrintTest, DoWhileTest)
{
	openHelper << "do 12; while(1)";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DoStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
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
	printer.printAST(astContext.getRoot().lock());
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

TEST_F(ASTStructurePrintTest, CaseTest)
{
	openHelper << "case 1:";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CaseStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"case 1:\n"
));
	reset();
}

TEST_F(ASTStructurePrintTest, DefaultTest)
{
	openHelper << "default:";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DefaultStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"default:\n"
));
	reset();
}

TEST_F(ASTStructurePrintTest, SwitchTest)
{
	openHelper << "switch(1) {}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SwitchStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"switch(1)\n"
"{\n"
"}\n"
));
	reset();

	openHelper << ""
"switch(1)"
"{"
"	case 0:"
"	case 1:"
"		break;"
"	default:"
"		break;"
"}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SwitchStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"switch(1)\n"
"{\n"
"  case 0:\n"
"  case 1:\n"
"  break;\n"
"  default:\n"
"  break;\n"
"}\n"
));
	reset();
}

TEST_F(ASTStructurePrintTest, SimpleDeclStmtTest)
{
	openHelper << "int a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int a;\n"
));
	reset();

	openHelper << "int b, c, d;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int b, c, d;\n"
));
	reset();
}

TEST_F(ASTStructurePrintTest, ArrayDeclStmtTest)
{
	openHelper << "int a[];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("int a[];\n"));
	reset();

	openHelper << "int b[], c, d[];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("int b[], c, d[];\n"));
	reset();

	openHelper << "int e[][][];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("int e[][][];\n"));
	reset();
}

TEST_F(ASTStructurePrintTest, PointerDeclStmtTest)
{
	openHelper << "int *a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("int* a;\n"));
	reset();

	openHelper << "int *b, c, *d;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("int* b, c, *d;\n"));
	reset();

	openHelper << "int ***e;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("int*** e;\n"));
	reset();
}

TEST_F(ASTStructurePrintTest, StructDeclTest)
{
	openHelper <<
"struct {"
"	int a;"
"};";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(""));
	reset();

	openHelper <<
"struct {"
"	int a;"
"} a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"struct {\n"
"  int a;\n"
"} a;\n"));
	reset();

	openHelper <<
"struct {"
"	int a;"
"	int b;"
"} b, c, d;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"struct {\n"
"  int a;\n"
"  int b;\n"
"} b, c, d;\n"));
	reset();
}

TEST_F(ASTStructurePrintTest, FunctionDeclTest)
{
	openHelper <<
"int main();";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int main();\n"
));
	reset();

	openHelper <<
"int a(void);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int a(void);\n"
));
	reset();

	openHelper <<
"int b(int c);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int b(int c);\n"
));
	reset();

	openHelper <<
"int d(int e, double f, void, float g);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int d(int e, double f, void, float g);\n"
	));
	reset();
}

TEST_F(ASTStructurePrintTest, TypedefDeclTest)
{
	openHelper <<
"typedef int a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"typedef int a;\n"
	));
	reset();

	openHelper <<
"typedef int b, c, d;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.printAST(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"typedef int b, c, d;\n"
	));
	reset();
}

#endif