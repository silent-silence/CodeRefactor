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
#include "Errors/NullPointerError.h"
#include "ASTOperation/Printer.h"

using std::make_shared;				using std::shared_ptr;
using std::fstream;					using std::string;
using std::dynamic_pointer_cast;	using std::weak_ptr;

class PrintTest : public testing::Test {
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

TEST_F(PrintTest, LiteralExpression)
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

TEST_F(PrintTest, UnaryExpression)
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

TEST_F(PrintTest, AssignExpression)
{
	openHelper << "1=1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundAssignOperator>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("1=1"));
	reset();
}

TEST_F(PrintTest, BinaryExpression)
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

TEST_F(PrintTest, ConditionalExpression)
{
	openHelper << "1?1:1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ConditionalOperator>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("1?1:1"));
	reset();
}

TEST_F(PrintTest, StringLiteralExpression)
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

TEST_F(PrintTest, MemberAccess)
{
	openHelper <<
"{"
"	struct {"
"		int a;"
"		int b;"
"	} c, *d;"
"	c.a;"
"	c.b;"
"	d->a;"
"	d->b;"
"}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  struct {\n"
"    int a;\n"
"    int b;\n"
"  } c, *d;\n"
"  c.a;\n"
"  c.b;\n"
"  d->a;\n"
"  d->b;\n"
"}"
));
	reset();
}

TEST_F(PrintTest, CompoundStmt)
{
	openHelper << "{ 123; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  123;\n"
"}"
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
"}"
));
	reset();

	openHelper << "{ 123; 1234; 123; { 123; 123; } { 13; } { } }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
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
"}"
));
	reset();

	openHelper << ""
"{"
"	1;"
"	{"
"		2;"
"		{"
"			3;"
"			{"
"				4;"
"				{}"
"			}"
"		}"
"	}"
"}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  1;\n"
"  {\n"
"    2;\n"
"    {\n"
"      3;\n"
"      {\n"
"        4;\n"
"        {\n"
"        }\n"
"      }\n"
"    }\n"
"  }\n"
"}"
));
	reset();
}

TEST_F(PrintTest, SimpleStmts)
{
	openHelper << ";";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<NullStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(";"));
	reset();

	openHelper << "continue;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ContinueStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("continue;"));
	reset();

	openHelper << "break;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BreakStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("break;"));
	reset();

	openHelper << "return;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ReturnStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("return ;"));
	reset();

	openHelper << "return 132;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ReturnStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("return 132;"));
	reset();
}

TEST_F(PrintTest, ParenExprTest)
{
	openHelper << "(123);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ParenExpr>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("(123)"));
	reset();
}

TEST_F(PrintTest, SizeofTest)
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

TEST_F(PrintTest, CastTest)
{
	openHelper << "(int)43;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CStyleCastExpr>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("(int)43"));
	reset();
}

TEST_F(PrintTest, ArraySubscriptTest)
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

TEST_F(PrintTest, IfTest)
{
	openHelper <<
"if(1)"
"	;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"if (1)\n"
"  ;"
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
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"if (1)\n"
"{\n"
"  123;\n"
"  23452;\n"
"}"
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
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"if (1)\n"
"  if (2)\n"
"    ;\n"
"  else \n"
"  {\n"
"    123;\n"
"  }\n"
"else \n"
"{\n"
"  345;\n"
"}"
));
	reset();

	openHelper << "if(1) { } else if(2) { }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"if (1)\n"
"{\n"
"}\n"
"else if (2)\n"
"{\n"
"}"
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
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  if (1)\n"
"  {\n"
"  }\n"
"  else if (2)\n"
"  {\n"
"  }\n"
"}"
));
	reset();

	openHelper <<
"{"
"	if(1)"
"		;"
"	else if(2)"
"		;"
"}";
	driver.parse();
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  if (1)\n"
"    ;\n"
"  else if (2)\n"
"    ;\n"
"}"
));
	reset();

	openHelper <<
"{"
"	if(1)"
"	{"
"		12;"
"	}"
"	else if(2)"
"	{"
"		13;"
"	}"
"}";
	driver.parse();
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  if (1)\n"
"  {\n"
"    12;\n"
"  }\n"
"  else if (2)\n"
"  {\n"
"    13;\n"
"  }\n"
"}"
));
	reset();
}

TEST_F(PrintTest, ForTest)
{
	openHelper << "for(;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"for (;;)\n"
"  ;"
));
	reset();

	openHelper << "for(int a;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"for (int a;;)\n"
"  ;"
));
	reset();

	openHelper << "for(1;2;3) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"for (1;2;3)\n"
"  ;"
));
	reset();

	openHelper << "for(int b;2;3) { 12; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"for (int b;2;3)\n"
"{\n"
"  12;\n"
"}"
));
	reset();

	openHelper << "{ for(int b;2;3) ; }";
	driver.parse();
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  for (int b;2;3)\n"
"    ;\n"
"}"
));
	reset();

	openHelper << "{ for(int b;2;3) { 12; } }";
	driver.parse();
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  for (int b;2;3)\n"
"  {\n"
"    12;\n"
"  }\n"
"}"
));
	reset();

	// TODO: do this after struct is recognised
	/*openHelper << "for(struct { int a; int b; } c;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	adapter.clean();*/
}

TEST_F(PrintTest, WhileTest)
{
	openHelper << "while(1) {}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<WhileStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("while (1)\n{\n}"));
	reset();
}

TEST_F(PrintTest, DoWhileTest)
{
	openHelper << "do 12; while(1)";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DoStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"do\n"
"  12;\n"
"while (1)"
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
"} while (1)"
));
	reset();
}

TEST_F(PrintTest, CaseTest)
{
	openHelper << "case 1:";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CaseStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"case 1:"
));
	reset();
}

TEST_F(PrintTest, DefaultTest)
{
	openHelper << "default:";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DefaultStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"default:"
));
	reset();
}

TEST_F(PrintTest, SwitchTest)
{
	openHelper << "switch(1) {}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SwitchStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"switch(1)\n"
"{\n"
"}"
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
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"switch(1)\n"
"{\n"
"  case 0:\n"
"  case 1:\n"
"    break;\n"
"  default:\n"
"    break;\n"
"}"
));
	reset();
}

TEST_F(PrintTest, SimpleDeclStmtTest)
{
	openHelper << "int a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int a;"
));
	reset();

	openHelper << "int b, c, d;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int b, c, d;"
));
	reset();
}

TEST_F(PrintTest, ArrayDeclStmtTest)
{
	openHelper << "int a[];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("int a[];"));
	reset();

	openHelper << "int b[], c, d[];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("int b[], c, d[];"));
	reset();

	openHelper << "int e[][][];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("int e[][][];"));
	reset();
}

TEST_F(PrintTest, PointerDeclStmtTest)
{
	openHelper << "int *a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("int* a;"));
	reset();

	openHelper << "int *b, c, *d;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("int* b, c, *d;"));
	reset();

	openHelper << "int ***e;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string("int*** e;"));
	reset();

	openHelper <<
"int *f;"
"int *g = f;";
	driver.parse();
	printer.detectNullPointer(true);
	EXPECT_THROW(printer.print(astContext.getRoot().lock()), NullPointerError);
	reset();
}

TEST_F(PrintTest, StructDeclTest)
{
	openHelper <<
"struct {"
"	int a;"
"};";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(";"));
	reset();

	openHelper <<
"struct {"
"	int a;"
"} a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"struct {\n"
"  int a;\n"
"} a;"));
	reset();

	openHelper <<
"struct {"
"	int a;"
"	int b;"
"} b, c, d;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"struct {\n"
"  int a;\n"
"  int b;\n"
"} b, c, d;"));
	reset();
}

TEST_F(PrintTest, FunctionDeclTest)
{
	openHelper <<
"int main();";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int main();"
));
	reset();

	openHelper <<
"int a(void);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int a(void);"
));
	reset();

	openHelper <<
"int b(int c);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int b(int c);"
));
	reset();

	openHelper <<
"int d(int *e, double f, void, float g[]);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int d(int* e, double f, void, float g[]);"
	));
	reset();
}

TEST_F(PrintTest, TypedefDeclTest)
{
	openHelper <<
"typedef int a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"typedef int a;"
	));
	reset();

	openHelper <<
"typedef int b, c, d;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"typedef int b, c, d;"
	));
	reset();
}

TEST_F(PrintTest, VarDeclWithInitTest)
{
	openHelper <<
"int a = 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int a = 1;"
	));
	reset();

	openHelper <<
"int b = 1, c = 2;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int b = 1, c = 2;"
	));
	reset();

	openHelper <<
"int d[] = \"12344\";";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"int d[] = \"12344\";"
	));
	reset();
}

TEST_F(PrintTest, EnumDeclWithInitTest)
{
	openHelper <<
"enum {"
"  a, b, c"
"} d;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DeclStmt>(astContext.getRoot().lock()));
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"enum {\n"
"  a, b, c\n"
"} d;"
	));
	reset();
}

TEST_F(PrintTest, CommentTest)
{
	openHelper <<
"//comment 1\n"
"int a;"
"/* comment 2 */";
	driver.parse();
	printer.print(declContext.getContextRoot());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"//comment 1\n"
"int a;\n"
"/* comment 2 */\n"
	));
	reset();
}

TEST_F(PrintTest, LabelStmtTest)
{
	openHelper <<
"stmt_label:"
"	1 + 2;";
	EXPECT_NO_THROW(driver.parse());
	EXPECT_EQ(astContext.getRoot().lock()->getStmtClass(), Stmt::StmtClass::LabelStmtClass);
	EXPECT_EQ(declContext.getContextRoot()->lookup("stmt_label").lock()->getKind(), Decl::Kind::Goto);
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"stmt_label:\n"
"  1+2;"
	));
	reset();
}

TEST_F(PrintTest, GotoStmtTest)
{
	openHelper <<
"{"
"	stmt_label:"
"		1 + 2;"
"	goto stmt_label;"
"}";
	EXPECT_NO_THROW(driver.parse());
	printer.print(astContext.getRoot().lock());
	printerOutput >> output;
	EXPECT_EQ(output, string(
"{\n"
"  stmt_label:\n"
"    1+2;\n"
"  goto stmt_label;\n"
"}"
	));
	reset();
}

#endif