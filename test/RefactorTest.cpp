//
// Created by 17271 on 2019/6/1.
//
#ifdef ENV_TEST

#include <memory>
#include <string>
#include <gtest/gtest.h>
#include "ASTOperation/Refactor.h"
#include "OpenHelper/StringStreamOpenHelper.h"
#include "OpenHelper/StdioOpenHelper.h"
#include "Parser/Driver.h"
#include "Parser/YaccAdapter.h"
#include "AST/ASTContext.h"
#include "Decl/DeclContextHolder.h"
#include "Errors/TypeError.hpp"
#include "ASTOperation/Printer.h"

using std::string;			using Printer::ASTPrinter;

class RefactorTest : public testing::Test {
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
	Refactor refactor{openHelper};
	StdioOpenHelper printerOutput;
	ASTPrinter printer{printerOutput};
	string output;
};

TEST_F(RefactorTest, IfRefactorTest)
{
/*	openHelper <<
"{"
"	if(1 && (2 || 3))"
"	{"
"		13;"
"	}"
"}";
	driver.parse();
	refactor.refactor_MCIf(astContext.getRoot().lock());
	printer.printAST(astContext.getRoot().lock());*/
}

#endif