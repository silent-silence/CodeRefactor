//
// Created by gaojian on 19-4-3.
//

#ifdef ENV_TEST

#include <memory>
#include <fstream>
#include <string>
#include <gtest/gtest.h>
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

TEST(ExpressionTest, operatorLogical)
{
}

TEST(ExpressionTest, expressionParser)
{
}

#endif