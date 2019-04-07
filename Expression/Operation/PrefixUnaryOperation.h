#ifndef PREFIX_UNARY_OPERATOR_H
#define PREFIX_UNARY_OPERATOR_H

#include "Operation.h"
#include <string>
#include <memory>

class PrefixUnaryOperation : public Operation {
public:

	enum PrefixUnary {
		Increment, 		// ++
		Decrement, 		// --
		Sizeof, 		// sizeof
		AddressOf,		// &
		Indirection,	// *
		UnaryPlus,		// +
		UnaryMinus,		// -
		BitwiseNot,		// ~
		Not				// !
	};

	PrefixUnaryOperation(PrefixUnary operatorType, std::shared_ptr<Expression> expression);
	~PrefixUnaryOperation() override = default;

	std::string print() override;

private:
	PrefixUnary m_operatorType;
	std::shared_ptr<Expression> m_expression;
};

#endif
