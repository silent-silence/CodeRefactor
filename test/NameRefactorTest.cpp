//
// Created by 17271 on 2019/6/13.
//

#ifdef ENV_TEST

#include <memory>
#include <string>
#include <gtest/gtest.h>
#include "ASTOperation/Corrector.h"
#include "OpenHelper/StringStreamOpenHelper.h"
#include "OpenHelper/StdioOpenHelper.h"
#include "Parser/Driver.h"
#include "Parser/YaccAdapter.h"
#include "AST/ASTContext.h"
#include "Decl/DeclContextHolder.h"
#include "Errors/TypeError.hpp"
#include "ASTOperation/Printer.h"

using std::string;

class NameRefactorTest : public testing::Test {
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
	StringStreamOpenHelper printHelper;
	NameRefactor nameRefactor;
	Printer printer{printHelper};
	std::string output;
};

TEST_F(NameRefactorTest, TypedefRenameTest)
{
	openHelper <<
"typedef int abc;";
	driver.parse();
	nameRefactor.rename(declContext.getContextRoot());
	printer.print(declContext.getContextRoot());
	printHelper >> output;
	EXPECT_EQ(output, string(
"typedef int Abc;\n"
	));
	reset();
}

TEST_F(NameRefactorTest, TypedefUserDefTypeRenameTest)
{
	openHelper <<
"typedef struct { int a; } abc;";
	driver.parse();
	nameRefactor.rename(declContext.getContextRoot());
	printer.print(declContext.getContextRoot());
	printHelper >> output;
	EXPECT_EQ(output, string(
"typedef struct {\n"
"  int a;\n"
"} Abc;\n"
));
	reset();
}

TEST_F(NameRefactorTest, EnumRenameTest)
{
	openHelper <<
"enum {"
"	abc, def, ijk"
"} a;";
	driver.parse();
	nameRefactor.rename(declContext.getContextRoot());
	printer.print(declContext.getContextRoot());
	printHelper >> output;
	EXPECT_EQ(output, string(
"enum {\n"
"  ABC, DEF, IJK\n"
"} a;\n"
));
	reset();
}

TEST_F(NameRefactorTest, VarRenameTest)
{
	openHelper <<
"int AbC;";
	driver.parse();
	nameRefactor.rename(declContext.getContextRoot());
	printer.print(declContext.getContextRoot());
	printHelper >> output;
	EXPECT_EQ(output, string(
"int abc;\n"
));
	reset();
}

TEST_F(NameRefactorTest, ConstantVarRenameTest)
{
	openHelper <<
"const int DEF;";
	driver.parse();
	nameRefactor.rename(declContext.getContextRoot());
	printer.print(declContext.getContextRoot());
	printHelper >> output;
	EXPECT_EQ(output, string(
"const int k_def;\n"
));
	reset();
}

TEST_F(NameRefactorTest, FunctionRenameTest)
{
	openHelper <<
"int abc();";
	driver.parse();
	nameRefactor.rename(declContext.getContextRoot());
	printer.print(declContext.getContextRoot());
	printHelper >> output;
	EXPECT_EQ(output, string(
"int Abc();\n"
"\n"
"int Abc();\n"
	));
	reset();
}

#endif