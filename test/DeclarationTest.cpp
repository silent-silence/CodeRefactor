#ifdef ENV_TEST

#include <gtest/gtest.h>
#include <memory>
#include "Errors/SymbolError.hpp"
#include "OpenHelper/StringStreamOpenHelper.h"
#include "Parser/Driver.h"
#include "Parser/YaccAdapter.h"
#include "AST/ASTContext.h"
#include "Decl/DeclContextHolder.h"
#include "Decl/Decl.h"
#include "Decl/DeclBase.h"

using std::shared_ptr;				using std::make_shared;
using std::dynamic_pointer_cast;

TEST(DelcarationTest, Decls)
{
	auto context = make_shared<DeclContext>();
	SourceLocation location;

	DeclName name("variable");
	auto variable = make_shared<VariableDecl>(context, location, QualType(), name);
	context->addDecl(variable);

	auto look = context->lookup(name);
	EXPECT_EQ(look.lock(), variable);

	auto variableRename = make_shared<VariableDecl>(context, location, QualType(), name);
	context->addDecl(variableRename);
	auto lookRename = context->lookup(name);
	EXPECT_NE(lookRename.lock(), variable);
	EXPECT_EQ(lookRename.lock(), variableRename);
	/*auto enumConstant = make_shared<EnumConstantDecl>();
	auto block = make_shared<BlockDecl>();
	auto function = make_shared<FunctionDecl>();
	auto record = make_shared<RecordDecl>();
	auto enumDecl = make_shared<EnumDecl>();*/
}

class SymbolTableTest : public testing::Test {
protected:
	void SetUp() override {}
	void TearDown() override {}

	StringStreamOpenHelper openHelper;
	ASTContext astContext;
	DeclContextHolder declContext;
	YaccAdapter adapter{astContext, declContext, openHelper};
	Driver driver{openHelper, adapter};
};

TEST_F(SymbolTableTest, DeclFindTest)
{
	openHelper << "int a; a;";
	EXPECT_NO_THROW(driver.parse());
	EXPECT_NO_THROW(declContext.getContext()->lookup("a"));
	EXPECT_THROW(declContext.getContext()->lookup("b"), SymbolNotFound);
	adapter.clean();
	declContext.clean();

	openHelper << "int b; a;";
	EXPECT_THROW(driver.parse(), SymbolNotFound);
	EXPECT_THROW(declContext.getContext()->lookup("a"), SymbolNotFound);
	EXPECT_NO_THROW(declContext.getContext()->lookup("b"));
	adapter.clean();
	declContext.clean();
}

#endif