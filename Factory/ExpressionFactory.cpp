//
// Created by 17271 on 2019/4/2.
//

#include <cmake-build-debug/parser.hpp>
#include "ExpressionFactory.h"
#include "parser.hpp"
#include "ExpressionFactory.h"
#include "Expression/Expression.h"
#include "Expression/Identity.h"
#include "Expression/Literal.hpp"
#include "Expression/Operation/BinaryOperation.h"
#include "Expression/Operation/PrefixUnaryOperation.h"
#include "Expression/Operation/RandomAccess.h"
#include "Expression/Operation/FunctionCall.h"
#include "Expression/Operation/MemberAccess.h"
#include "Expression/Operation/PostfixUnaryOperation.h"
#include "Expression/Operation/CastExpression.h"
#include "Expression/Operation/TernaryOperation.h"
#include "Expression/Operation/CommaExpression.h"
#include "Expression/ArgumentList.h"

using std::make_shared;					using std::string;
using std::shared_ptr;					using std::dynamic_pointer_cast;

typedef yy::Parser::token::yytokentype token;

std::shared_ptr<Expression> ExpressionFactory::makeIdentity(std::string id)
{
	return make_shared<Identity>(id);
}

std::shared_ptr<Expression> ExpressionFactory::makeLiteral(int num)
{
	return make_shared<Literal<int>>(num);
}

std::shared_ptr<Expression> ExpressionFactory::makeLiteral(double num)
{
	return make_shared<Literal<double>>(num);
}

std::shared_ptr<Expression> ExpressionFactory::makeBinaryOperation(std::shared_ptr<Expression> firstExpression, int assignmentOpType, std::shared_ptr<Expression> secondExpression)
{
	BinaryOperation::BinaryOperator op;
	switch(assignmentOpType)
	{
		case '=':							op = BinaryOperation::BinaryOperator::BasicAssignment;				break;
		case token::TOK_MUL_ASSIGN:			op = BinaryOperation::BinaryOperator::MultiplicationAssignment;		break;
		case token::TOK_DIV_ASSIGN:			op = BinaryOperation::BinaryOperator::DivisionAssignment;			break;
		case token::TOK_MOD_ASSIGN:			op = BinaryOperation::BinaryOperator::ModuloAssignment;				break;
		case token::TOK_ADD_ASSIGN:			op = BinaryOperation::BinaryOperator::AdditionAssignment;			break;
		case token::TOK_SUB_ASSIGN:			op = BinaryOperation::BinaryOperator::SubtractionAssignment;		break;
		case token::TOK_LEFT_SHIFT_ASSIGN:	op = BinaryOperation::BinaryOperator::BitwiseLeftShiftAssignment;	break;
		case token::TOK_RIGHT_SHIFT_ASSIGN:	op = BinaryOperation::BinaryOperator::BitwiseRightShifAssignment; 	break;
		case token::TOK_AND_ASSIGN:			op = BinaryOperation::BinaryOperator::BitwiseAndAssignment;			break;
		case token::TOK_XOR_ASSIGN:			op = BinaryOperation::BinaryOperator::BitwiseXorAssignment;			break;
		case token::TOK_OR_ASSIGN:			op = BinaryOperation::BinaryOperator::BitwiseOrAssignment;			break;
		case token::TOK_OR_OP:				op = BinaryOperation::BinaryOperator::LogicalOr;					break;
		case token::TOK_AND_OP:				op = BinaryOperation::BinaryOperator::LogicalAnd;					break;
		case '|':							op = BinaryOperation::BinaryOperator::BitwiseOr;					break;
		case '^':							op = BinaryOperation::BinaryOperator::BitwiseXor;					break;
		case '&':							op = BinaryOperation::BinaryOperator::BitwiseAnd;					break;
		case token::TOK_EQ_OP:				op = BinaryOperation::BinaryOperator::EqualTo;						break;
		case token::TOK_NE_OP:				op = BinaryOperation::BinaryOperator::NotEqualTo;					break;
		case '<':							op = BinaryOperation::BinaryOperator::LessThan;						break;
		case '>':							op = BinaryOperation::BinaryOperator::GreaterThan;					break;
		case token::TOK_LE_OP:				op = BinaryOperation::BinaryOperator::LessThanOrEqualTo;			break;
		case token::TOK_GE_OP:				op = BinaryOperation::BinaryOperator::GreaterThanOrEqualTo;			break;
		case token::TOK_LEFT_SHIFT_OP:		op = BinaryOperation::BinaryOperator::BitwiseLeftShift;				break;
		case token::TOK_RIGHT_SHIFT_OP:		op = BinaryOperation::BinaryOperator::BitwiseRightShift;			break;
		case '+':							op = BinaryOperation::BinaryOperator::Addition;						break;
		case '-':							op = BinaryOperation::BinaryOperator::Subtraction;					break;
		case '*':							op = BinaryOperation::BinaryOperator::Multiplication;				break;
		case '/':							op = BinaryOperation::BinaryOperator::Division;						break;
		case '%':							op = BinaryOperation::BinaryOperator::Modulo;						break;
	}

	return make_shared<BinaryOperation>(firstExpression, op, secondExpression);
}

std::shared_ptr<Expression> ExpressionFactory::makePrefixUnaryOperation(int unaryOpType, std::shared_ptr<Expression> expression)
{
	PrefixUnaryOperation::PrefixUnary op;
	switch(unaryOpType)
	{
		case token::TOK_INC_OP:	op = PrefixUnaryOperation::PrefixUnary::Increment;	break;
		case token::TOK_DEC_OP:	op = PrefixUnaryOperation::PrefixUnary::Decrement;	break;
		case token::TOK_SIZEOF:	op = PrefixUnaryOperation::PrefixUnary::Sizeof;		break;
		case '&':				op = PrefixUnaryOperation::PrefixUnary::AddressOf;	break;
		case '*':				op = PrefixUnaryOperation::PrefixUnary::Indirection;break;
		case '+':				op = PrefixUnaryOperation::PrefixUnary::UnaryPlus;	break;
		case '-':				op = PrefixUnaryOperation::PrefixUnary::UnaryMinus;	break;
		case '~':				op = PrefixUnaryOperation::PrefixUnary::BitwiseNot;	break;
		case '!':				op = PrefixUnaryOperation::PrefixUnary::Not;		break;
	}

	return make_shared<PrefixUnaryOperation>(op, expression);
}

std::shared_ptr<Expression> ExpressionFactory::makeSizeofTypeOperation(std::string typeName)
{
	//sizeof(int);
}

std::shared_ptr<Expression> ExpressionFactory::makeRandomAccessExpression(std::shared_ptr<Expression> expression, std::shared_ptr<Expression> accessPoint)
{
	return make_shared<RandomAccess>(expression, accessPoint);
}

std::shared_ptr<Expression> ExpressionFactory::makeFunctionCallExpression(std::shared_ptr<Expression> callable, std::shared_ptr<Expression> argumentList)
{
	shared_ptr<ArgumentList> arguments;
	if(argumentList != nullptr)
		arguments = dynamic_pointer_cast<ArgumentList>(argumentList);
	else
		arguments = nullptr;
	return make_shared<FunctionCall>(callable, arguments);
}

std::shared_ptr<Expression> ExpressionFactory::makeMemberAccessExpression(std::shared_ptr<Expression> object, int accessMethod, std::shared_ptr<Expression> member)
{
	MemberAccess::MemberOperator op;

	switch(accessMethod)
	{
		case '.':					op = MemberAccess::MemberOperator::StructureReference;		break;
		case token::TOK_POINT_OP:	op = MemberAccess::MemberOperator::StructureDereference;	break;
	}

	shared_ptr<Identity> id = dynamic_pointer_cast<Identity>(member);
	return make_shared<MemberAccess>(object, op, id);
}

std::shared_ptr<Expression> ExpressionFactory::makePostfixUnaryOperation(std::shared_ptr<Expression> expression, int unaryOp)
{
	PostfixUnaryOperation::PostfixUnary op;
	switch(unaryOp)
	{
		case token::TOK_INC_OP: op = PostfixUnaryOperation::PostfixUnary::Increment; break;
		case token::TOK_DEC_OP: op = PostfixUnaryOperation::PostfixUnary::Decrement; break;
	}
	return make_shared<PostfixUnaryOperation>(expression, op);
}

std::shared_ptr<Expression> ExpressionFactory::makeCastExpression(std::string caseToType, std::shared_ptr<Expression> expression)
{
	return make_shared<CastExpression>(caseToType, expression);
}

std::shared_ptr<Expression> ExpressionFactory::makeTernaryOperation(std::shared_ptr<Expression> firstExpression, std::shared_ptr<Expression> secondExpression, std::shared_ptr<Expression> thirdExpression)
{
	return make_shared<TernaryOperation>(firstExpression, secondExpression, thirdExpression);
}

std::shared_ptr<Expression> ExpressionFactory::makeCommaExpression(std::shared_ptr<Expression> firstExpression, std::shared_ptr<Expression> secondExpression)
{
	return make_shared<CommaExpression>(firstExpression, secondExpression);
}
