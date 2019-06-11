//
// Created by gaojian on 19-5-24.
//

#ifdef ENV_TEST

#include <gtest/gtest.h>
#include <string>
#include "ASTOperation/Printer.h"
#include "OpenHelper/StringStreamOpenHelper.h"
#include "Parser/Driver.h"
#include "Parser/YaccAdapter.h"
#include "AST/ASTContext.h"
#include "Decl/DeclContextHolder.h"

using std::string;				using Printer::ContextPrinter;

class HybridParseTest : public testing::Test {
protected:
	void SetUp() override {}
	void TearDown() override {}

	StringStreamOpenHelper openHelper;
	ASTContext astContext;
	DeclContextHolder declContext;
	YaccAdapter adapter{astContext, declContext, openHelper};
	Driver driver{openHelper, adapter};
	StringStreamOpenHelper printHelper;
	ContextPrinter printer{printHelper};
	std::string output;
};

TEST_F(HybridParseTest, VarDeclInContext)
{
	openHelper <<
"int a;"
"int b;"
"double c;"
"char *d;";
	driver.parse();
	printer.printContext(declContext.getContextRoot());
	printHelper >> output;
	EXPECT_EQ(output, string(
"int a;\n"
"int b;\n"
"double c;\n"
"signed char* d;\n"
));
}

TEST_F(HybridParseTest, StructDeclInContext)
{
	openHelper <<
"struct {"
"  int a;"
"} a;"
"struct {"
"  int b;"
"} b;";
	driver.parse();
	printer.printContext(declContext.getContextRoot());
	printHelper >> output;
	EXPECT_EQ(output, string(
"struct {\n"
"  int a;\n"
"} a;\n"
"struct {\n"
"  int b;\n"
"} b;\n"
));
}

TEST_F(HybridParseTest, FunctionWithNoParamDeclInContext)
{
	openHelper <<
"int a()"
"{"
"	int b;"
"	return 1;"
"}"
"int b()"
"{"
"	int a;"
"	return 1;"
"}";
	driver.parse();
	printer.printContext(declContext.getContextRoot());
	printHelper >> output;
	EXPECT_EQ(output, string(
"int a()\n"
"{\n"
"  int b;\n"
"  return 1;\n"
"}\n"
"int b()\n"
"{\n"
"  int a;\n"
"  return 1;\n"
"}\n"
));
}

TEST_F(HybridParseTest, FunctionDeclInContext)
{
	openHelper <<
"int a(int b, int c)"
"{"
"	int d;"
"	return 1;"
"}"
"int b(int c, int d)"
"{"
"	int a;"
"	return 1;"
"}";
	driver.parse();
	printer.printContext(declContext.getContextRoot());
	printHelper >> output;
	EXPECT_EQ(output, string(
"int a(int b, int c)\n"
"{\n"
"  int d;\n"
"  return 1;\n"
"}\n"
"int b(int c, int d)\n"
"{\n"
"  int a;\n"
"  return 1;\n"
"}\n"
));
}

TEST_F(HybridParseTest, AllSupportedFutures)
{
	string input =
"float foo()"
"{"
"	return 1;"
"}"
""
"double foo2(int a, int b)"
"{"
"	return a + b;"
"}"
"int main(void)"
"{"
"	signed a = 1;"
"	char b = 3;"
"	{"
"		double c = a + b;"
"	}"
"	for(int i; i < 100; i++)"
"		foo();"
"	foo2();"
"	return 0;"
"}"
;
	openHelper << input;
	EXPECT_NO_THROW(driver.parse());
}

#endif