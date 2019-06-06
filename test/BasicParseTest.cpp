//
// Created by gaojian on 19-4-3.
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

class BasicParseTest : public testing::Test {
protected:
	void SetUp() override {}
	void TearDown() override {
		printer.resetPrinter();
		adapter.clean();
	}

	void reset() {
		printer.resetPrinter();
		adapter.clean();
	}

	StringStreamOpenHelper openHelper;
	ASTContext astContext;
	DeclContextHolder declContext;
	YaccAdapter adapter{astContext, declContext, openHelper};
	Driver driver{openHelper, adapter};
	StringStreamOpenHelper printerOutput;
	Printer printer{printerOutput};
};

TEST_F(BasicParseTest, LiteralExpression)
{
	openHelper << "12;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(astContext.getRoot().lock()));
	reset();

	openHelper << "12.3;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<FloatingLiteral>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, UnaryExpression)
{
	openHelper << "1++;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(astContext.getRoot().lock()));
	reset();

	openHelper << "--1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(astContext.getRoot().lock()));
	reset();

	openHelper << "+1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<UnaryOperator>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, AssignExpression)
{
	openHelper << "1=1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundAssignOperator>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, BinaryExpression)
{
	openHelper << "1*1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BinaryOperator>(astContext.getRoot().lock()));
	reset();

	openHelper << "1,1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BinaryOperator>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, ConditionalExpression)
{
	openHelper << "1?1:1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ConditionalOperator>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, StringLiteralExpression)
{
	std::string input = "\"string\";";
	openHelper << input;
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<StringLiteral>(astContext.getRoot().lock()));
	reset();

	input = "'s';";
	openHelper << input;
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CharacterLiteral>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, CompoundStmt)
{
	openHelper << "{ 123; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "{ 123; 1234; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "1; { 123; 1234; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CompoundStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "{ 123; 1234; } 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(astContext.getRoot().lock()));
	reset();

	openHelper << "{ 123; 1234; 123; { 123; 123; } { 13; } { } } 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IntegerLiteral>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, SimpleStmts)
{
	openHelper << ";";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<NullStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "continue;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ContinueStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "break;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<BreakStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "return;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ReturnStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "return 132;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ReturnStmt>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, ParenExprTest)
{
	openHelper << "(123);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ParenExpr>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, SizeofTest)
{
	openHelper << "sizeof(int);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(astContext.getRoot().lock()));
	reset();

	openHelper << "sizeof(1);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(astContext.getRoot().lock()));
	reset();

	openHelper << "sizeof 1;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SizeOfAlignOfExpr>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, ArraySubscriptTest)
{
	openHelper << "12[43];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ArraySubscriptExpr>(astContext.getRoot().lock()));
	reset();

	openHelper << "12[43][12];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ArraySubscriptExpr>(astContext.getRoot().lock()));
	reset();

	// TODO function return value
	/*openHelper << "foo()[43][12];";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ArraySubscriptExpr>(astContext.getRoot().lock()));*/
}

TEST_F(BasicParseTest, MemberExprTest)
{
	openHelper <<
"struct {"
"	int a;"
"} b;"
"b.a;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<MemberExpr>(astContext.getRoot().lock()));
	reset();

	openHelper <<
"struct {"
"	int c;"
"} d;"
"d->c;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<MemberExpr>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, CastTest)
{
	openHelper << "(int)43;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CStyleCastExpr>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, CallTest)
{
	openHelper << "34();";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CallExpr>(astContext.getRoot().lock()));
	reset();

	openHelper << "34(12, 43);";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CallExpr>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, IfTest)
{
	openHelper << "if(1) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "if(1) { 123; 23452; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	reset();

	// TODO Matching if else
	openHelper << "if(1) if(2) ; else {123; } else {345;}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "if(1) { } else if(2) { }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<IfStmt>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, ForTest)
{
	openHelper << "for(;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "for(int a;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "for(1;2;3) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "for(int b;2;3) { 12; }";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	reset();

	// TODO: do this after struct is recognised
	/*openHelper << "for(struct { int a; int b; } c;;) ;";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<ForStmt>(astContext.getRoot().lock()));
	reset();*/
}

TEST_F(BasicParseTest, WhileTest)
{
	openHelper << "while(1) {}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<WhileStmt>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, DoWhileTest)
{
	openHelper << "do 12; while(1)";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DoStmt>(astContext.getRoot().lock()));
	reset();

	openHelper << "do { 12; } while(1)";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DoStmt>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, SwitchTest)
{
	openHelper << "switch(1) {}";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<SwitchStmt>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, CaseTest)
{
	openHelper << "case 1:";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<CaseStmt>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, DefaultTest)
{
	openHelper << "default:";
	driver.parse();
	EXPECT_TRUE(dynamic_pointer_cast<DefaultStmt>(astContext.getRoot().lock()));
	reset();
}

TEST_F(BasicParseTest, QualifiedBasicTypeTest)
{
	// void
	openHelper << "void;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned void;";
	EXPECT_THROW(driver.parse(), TypeError);

	// char
	openHelper << "char;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned char;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long char;";
	EXPECT_THROW(driver.parse(), TypeError);

	// short
	openHelper << "short;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "short int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long short;";
	EXPECT_THROW(driver.parse(), TypeError);

	// int
	openHelper << "int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "signed int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "int double;";
	EXPECT_THROW(driver.parse(), TypeError);

	// float
	openHelper << "float;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned float;";
	EXPECT_THROW(driver.parse(), TypeError);

	openHelper << "long float;";
	EXPECT_THROW(driver.parse(), TypeError);

	// double
	openHelper << "double;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long double;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long long double;";
	EXPECT_THROW(driver.parse(), TypeError);

	openHelper << "unsigned double;";
	EXPECT_THROW(driver.parse(), TypeError);

	// signed & unsigned
	openHelper << "signed;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "unsigned signed;";
	EXPECT_THROW(driver.parse(), TypeError);

	openHelper << "unsigned unsigned;";
	EXPECT_THROW(driver.parse(), TypeError);

	// long & long long
	openHelper << "long;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long long;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long unsigned long int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "long long long;";
	EXPECT_THROW(driver.parse(), TypeError);

	// storage
	openHelper << "typedef int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "extern int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "static int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "register int;";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "extern static int;";
	EXPECT_THROW(driver.parse(), TypeError);

	openHelper << "struct { int a; };";
	EXPECT_NO_THROW(driver.parse());

	openHelper << "struct { int a; int b; int c; };";
	EXPECT_NO_THROW(driver.parse());
}

TEST_F(BasicParseTest, PointerTypeTest)
{
	openHelper << "int * a;";
	driver.parse();
	EXPECT_EQ(
			dynamic_pointer_cast<VarDecl>(
					declContext.getContextRoot()->lookup("a").lock()
			)->getType().lock()->getTypePtr()->getTypeClass(),
			Type::Pointer
	);
}

TEST_F(BasicParseTest, VariableTest)
{
	openHelper << "long int a;";
	driver.parse();
	EXPECT_EQ(
			dynamic_pointer_cast<VarDecl>(declContext.getContextRoot()->lookup("a").lock())->getNameAsString(),
			string("a")
	);
	reset();

	openHelper << "long int b[][];";
	driver.parse();
	EXPECT_EQ(
			dynamic_pointer_cast<VarDecl>(declContext.getContextRoot()->lookup("b").lock())->getNameAsString(),
			string("b")
	);
	reset();

	openHelper << "long int c, d, e;";
	driver.parse();
	EXPECT_EQ(
			dynamic_pointer_cast<VarDecl>(declContext.getContextRoot()->lookup("c").lock())->getNameAsString(),
			string("c")
	);
	EXPECT_EQ(
			dynamic_pointer_cast<VarDecl>(declContext.getContextRoot()->lookup("d").lock())->getNameAsString(),
			string("d")
	);
	EXPECT_EQ(
			dynamic_pointer_cast<VarDecl>(declContext.getContextRoot()->lookup("e").lock())->getNameAsString(),
			string("e")
	);
	reset();
}

TEST_F(BasicParseTest, FunctionWithParmNoProtoTypeDeclTest)
{
	openHelper << "int a();";
	driver.parse();
	EXPECT_EQ(
			dynamic_pointer_cast<FunctionDecl>(declContext.getContextRoot()->lookup("a").lock())->getNameAsString(),
			string("a")
	);
	shared_ptr<FunctionDecl> func = dynamic_pointer_cast<FunctionDecl>(declContext.getContextRoot()->lookup("a").lock());
	EXPECT_EQ(func->getKind(), Decl::Function);
	reset();
}

TEST_F(BasicParseTest, FunctionWithParmNoProtoTypeDefinitionTest)
{
	openHelper <<
"int a()"
"{"
"	123;"
"}";
	driver.parse();
	EXPECT_EQ(
			dynamic_pointer_cast<FunctionDecl>(declContext.getContextRoot()->lookup("a").lock())->getNameAsString(),
			string("a")
	);
	shared_ptr<FunctionDecl> func = dynamic_pointer_cast<FunctionDecl>(declContext.getContextRoot()->lookup("a").lock());
	EXPECT_EQ(func->getKind(), Decl::Function);
	reset();
}

TEST_F(BasicParseTest, FunctionWithParmTest)
{
	openHelper << "void a(int b, double c) {}";
	driver.parse();
	EXPECT_EQ(
			dynamic_pointer_cast<FunctionDecl>(declContext.getContextRoot()->lookup("a").lock())->getNameAsString(),
			string("a")
	);
	reset();
}

#endif