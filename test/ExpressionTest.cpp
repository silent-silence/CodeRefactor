//
// Created by gaojian on 19-4-3.
//

#ifdef ENV_TEST

#include <memory>
#include <fstream>
#include <string>
#include <gtest/gtest.h>
#include <Expression/Identity.h>
#include <Expression/Constant.h>
#include <Expression/ArgumentList.h>
#include <Expression/Operation/RandomAccess.h>
#include "Expression/Literal.hpp"
#include "Expression/Operation/BinaryOperation.h"
#include "Expression/Operation/CommaExpression.h"
#include "Expression/Operation/PostfixUnaryOperation.h"
#include "Expression/Operation/PrefixUnaryOperation.h"
#include "Expression/Operation/TernaryOperation.h"
#include "Factory/ExpressionFactory.h"
#include "parser.hpp"

using std::make_shared;				using std::shared_ptr;
using std::fstream;					using std::string;

TEST(ExpressionTest, simpleExpressionMake)
{
	auto intLiteral = make_shared<Literal<int>>(1);
	EXPECT_EQ("(Literal)1", intLiteral->print());
	auto doubleLiteral = make_shared<Literal<double>>(1.2);
	EXPECT_EQ("(Literal)1.2", doubleLiteral->print());
	auto identity = make_shared<Identity>("id");
	EXPECT_EQ("(Identity)id", identity->print());
	auto constant = make_shared<Constant>("cons");
	EXPECT_EQ("(Constant)cons", constant->print());
}

TEST(ExpressionTest, complexExpressionMake)
{
	auto intLiteral = make_shared<Literal<int>>(1);
	auto doubleLiteral = make_shared<Literal<double>>(1.2);
	auto identity = make_shared<Identity>("id");
	auto constant = make_shared<Constant>("cons");

	auto argument1 = make_shared<ArgumentList>(nullptr, intLiteral);
	auto argument2 = make_shared<ArgumentList>(argument1, doubleLiteral);
	auto argument3 = make_shared<ArgumentList>(argument2, constant);
	EXPECT_EQ("(Argument list)(Argument list)(Argument list)(Literal)1(Literal)1.2(Constant)cons", argument3->print());

	auto ternary = make_shared<TernaryOperation>(intLiteral, doubleLiteral, identity);
	EXPECT_EQ("(Ternary)(Literal)1 ? (Literal)1.2 : (Identity)id", ternary->print());

	auto random = make_shared<RandomAccess>(identity, intLiteral);
	EXPECT_EQ("(Random access)(Identity)id[(Literal)1]", random->print());

}

#endif