#ifdef ENV_TEST

#include <gtest/gtest.h>
#include <memory>
#include "Decl/Decl.h"
#include "Decl/DeclBase.h"

using std::shared_ptr;				using std::make_shared;

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
#endif