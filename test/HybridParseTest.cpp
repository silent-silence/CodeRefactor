//
// Created by gaojian on 19-5-24.
//

#ifdef ENV_TEST

#include <gtest/gtest.h>
#include <string>
#include "OpenHelper/StringStreamOpenHelper.h"
#include "Parser/Driver.h"
#include "Parser/YaccAdapter.h"
#include "AST/ASTContext.h"
#include "Decl/DeclContextHolder.h"

using std::string;

class HybridParseTest : public testing::Test {
protected:
	void SetUp() override {}
	void TearDown() override {}

	StringStreamOpenHelper openHelper;
	ASTContext astContext;
	DeclContextHolder declContext;
	YaccAdapter adapter{astContext, declContext, openHelper};
	Driver driver{openHelper, adapter};
};

TEST_F(HybridParseTest, Sequenced)
{
	string input =
"{"
"	int a = 1;"
"	int b = 3;"
"	{"
"		int c = a + b;"
"	}"
"}";
	openHelper << input;
	EXPECT_NO_THROW(driver.parse());
}

#endif