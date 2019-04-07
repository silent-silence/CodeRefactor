#ifndef BINARY_OPERATOR_H
#define BINARY_OPERATOR_H

#include "Operation.h"
#include <string>
#include <memory>

class BinaryOperation : public Operation {
public:

	enum BinaryOperator {
		BasicAssignment,			// =
		MultiplicationAssignment,	// *=
		DivisionAssignment,			// /=
		ModuloAssignment,			// %=
		AdditionAssignment,			// +=
		SubtractionAssignment,		// -=
		BitwiseLeftShiftAssignment,	// <<=
		BitwiseRightShifAssignment,	// >>=
		BitwiseAndAssignment,		// &=
		BitwiseXorAssignment,		// ^=
		BitwiseOrAssignment,		// |=
		LogicalOr,					// ||
		LogicalAnd,					// &&
		BitwiseOr,					// |
		BitwiseXor,					// ^
		BitwiseAnd,					// &
		EqualTo,					// ==
		NotEqualTo,					// !=
		LessThan,					// <
		GreaterThan,				// >
		LessThanOrEqualTo,			// <=
		GreaterThanOrEqualTo,		// >=
		BitwiseLeftShift,			// <<
		BitwiseRightShift,			// >>
		Addition,					// +
		Subtraction,				// -
		Multiplication,				// *
		Division,					// /
		Modulo						// %
	};
	BinaryOperation(std::shared_ptr<Expression> firstExpression, BinaryOperator operatorType, std::shared_ptr<Expression> secondExpression);
	~BinaryOperation() override = default;

	std::string print() override;

private:
	std::shared_ptr<Expression> m_firstExpression;
	BinaryOperator m_operatorType;
	std::shared_ptr<Expression> m_secondExpression;
};

#endif