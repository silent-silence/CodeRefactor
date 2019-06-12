#ifdef ENV_TEST

#include <gtest/gtest.h>
#include <memory>
#include "Errors/SymbolError.hpp"
#include "Errors/TypeError.hpp"
#include "OpenHelper/StringStreamOpenHelper.h"
#include "Parser/Driver.h"
#include "Parser/YaccAdapter.h"
#include "AST/ASTContext.h"
#include "Decl/DeclContextHolder.h"
#include "Decl/Decl.h"
#include "Decl/DeclBase.h"
#include "Basic/IdentifierTable.h"

using std::shared_ptr;				using std::make_shared;
using std::dynamic_pointer_cast;

TEST(DelcarationTest, Decls)
{
	auto context = make_shared<TranslationUnitDecl>();
	SourceLocation location;

	DeclName name(make_shared<IdentifierInfo>("variable"));
	auto type = BuiltinType::creator(BuiltinType::Int, QualType::None);
	auto variable = make_shared<VarDecl>(Decl::Kind::Var, context, location, name, type, VarDecl::StorageClass::None);
	context->addDecl(variable);

	auto look = context->lookup("variable");
	EXPECT_EQ(look.lock(), variable);

	auto variableRename = make_shared<VarDecl>(Decl::Kind::Var, context, location, name, type, VarDecl::StorageClass::None);
	EXPECT_THROW(context->addDecl(variableRename), SymbolAlreadyExist);
	auto lookRename = context->lookup("variable");
	EXPECT_EQ(lookRename.lock(), variable);
	EXPECT_NE(lookRename.lock(), variableRename);
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
	DeclContextHolder declContextHolder;
	YaccAdapter adapter{astContext, declContextHolder, openHelper};
	Driver driver{openHelper, adapter};
};

TEST_F(SymbolTableTest, SimpleDeclTest)
{
	openHelper << "int a; a;";
	EXPECT_NO_THROW(driver.parse());
	EXPECT_NO_THROW(declContextHolder.getContextRoot()->lookup("a"));
	EXPECT_THROW(declContextHolder.getContextRoot()->lookup("b"), SymbolNotFound);
}

TEST_F(SymbolTableTest, SimpleDeclNotFindTest)
{
	openHelper << "int b; a;";
	EXPECT_THROW(driver.parse(), SymbolNotFound);
	EXPECT_THROW(declContextHolder.getContextRoot()->lookup("a"), SymbolNotFound);
	EXPECT_NO_THROW(declContextHolder.getContextRoot()->lookup("b"));
}

TEST_F(SymbolTableTest, MultipleTest)
{
	openHelper << "int a, b, c;";
	EXPECT_NO_THROW(driver.parse());
	EXPECT_NO_THROW(declContextHolder.getContextRoot()->lookup("a"));
	EXPECT_NO_THROW(declContextHolder.getContextRoot()->lookup("b"));
	EXPECT_NO_THROW(declContextHolder.getContextRoot()->lookup("c"));
	EXPECT_THROW(declContextHolder.getContextRoot()->lookup("d"), SymbolNotFound);

	auto varDeclA = dynamic_pointer_cast<VarDecl>(declContextHolder.getContextRoot()->lookup("a").lock());
	EXPECT_EQ(dynamic_pointer_cast<BuiltinType>(varDeclA->getType().lock()->getTypePtr())->getKind(), BuiltinType::Int);
	auto varDeclB = dynamic_pointer_cast<VarDecl>(declContextHolder.getContextRoot()->lookup("b").lock());
	EXPECT_EQ(dynamic_pointer_cast<BuiltinType>(varDeclB->getType().lock()->getTypePtr())->getKind(), BuiltinType::Int);
	auto varDeclC = dynamic_pointer_cast<VarDecl>(declContextHolder.getContextRoot()->lookup("c").lock());
	EXPECT_EQ(dynamic_pointer_cast<BuiltinType>(varDeclC->getType().lock()->getTypePtr())->getKind(), BuiltinType::Int);
}

TEST_F(SymbolTableTest, InDifferentBlocks)
{
	openHelper <<
"{"
"	int a = 1;"
"	int b = 3;"
"	{"
"		int c = a + b;"
"	}"
"	a = 11;"
"}";
	EXPECT_NO_THROW(driver.parse());
}

TEST_F(SymbolTableTest, InDifferentBlocksNotFoundSymbol)
{
	openHelper <<
"{"
"	int a = 1;"
"	int b = 3;"
"	{"
"		int c = a + b;"
"	}"
"	c = 2;"
"}";
	EXPECT_THROW(driver.parse(), SymbolNotFound);
}

TEST_F(SymbolTableTest, DeclsInDifferentBlocks)
{
	openHelper <<
"{"
"	int a = 1;"
"	int b = 3;"
"	{"
"		int c = a + b;"
"		{"
"			int d = c;"
"		}"
"		c = 1;"
"	}"
"	{"
"		a + b;"
"	}"
"	b = a;"
"}";
	EXPECT_NO_THROW(driver.parse());
}

TEST_F(SymbolTableTest, VarRedeclInDifferentBlockTest)
{
	openHelper <<
"{"
"	int a = 1;"
"	int b = 3;"
"	{"
"		int a = 1 + b;"
"		{"
"			int b = 3;"
"		}"
"		a = 1;"
"	}"
"}";
	EXPECT_NO_THROW(driver.parse());
}

TEST_F(SymbolTableTest, VarRedeclInSameBlockTest)
{
	openHelper <<
"{"
 "	int a = 1;"
 "	int a = 3;"
 "}";
	EXPECT_THROW(driver.parse(), SymbolAlreadyExist);
}

TEST_F(SymbolTableTest, SimpleTypedefTest)
{
	openHelper <<
"typedef int a;";
	driver.parse();
	EXPECT_EQ(declContextHolder.getContextRoot()->lookup("a").lock()->getKind(), Decl::Kind::Typedef);
	shared_ptr<TypedefDecl> typedefType = dynamic_pointer_cast<TypedefDecl>(declContextHolder.getContextRoot()->lookup("a").lock());
	EXPECT_EQ(dynamic_pointer_cast<TypedefType>(typedefType->getTypeForDecl().lock()->getTypePtr())->getTypeClass(), Type::TypeClass::Typedef);
	EXPECT_EQ(
			dynamic_pointer_cast<BuiltinType>(
					dynamic_pointer_cast<TypedefType>(
							typedefType->getTypeForDecl().lock()->getTypePtr()
					)->getDeclForType().lock()->getTypePtr()
			)->getKind(), BuiltinType::Int);
}

TEST_F(SymbolTableTest, TypedefRedfineTest)
{
	openHelper <<
"typedef int a;"
"double a b;";
	EXPECT_THROW(driver.parse(), TypeError);
}

TEST_F(SymbolTableTest, StructDeclTest)
{
	openHelper <<
"struct {"
"	int a;"
"	double b;"
"} c;";
	EXPECT_NO_THROW(driver.parse());
	EXPECT_THROW(declContextHolder.getContextRoot()->lookup("a"), SymbolNotFound);
	EXPECT_NO_THROW(declContextHolder.getContextRoot()->lookup("c"));
	shared_ptr<VarDecl> decl = dynamic_pointer_cast<VarDecl>(declContextHolder.getContextRoot()->lookup("c").lock());
	EXPECT_EQ(decl->getType().lock()->getTypePtr()->getTypeClass(), Type::TypeClass::Record);
}

#endif
