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
"float foo();"
"double foo2(int a, int b);"
""
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
""
"float foo()"
"{"
"	return 1;"
"}"
""
"double foo2(int a, int b)"
"{"
"	return a + b;"
"}";
	openHelper << input;
	EXPECT_NO_THROW(driver.parse());
}

#endif