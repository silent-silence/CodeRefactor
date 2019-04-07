#include <iostream>
#include "PrefixUnaryOperation.h"

PrefixUnaryOperation::PrefixUnaryOperation(PrefixUnary operatorType, std::shared_ptr<Expression> expression)
	: m_operatorType{operatorType}, m_expression{expression}
{}

std::string PrefixUnaryOperation::print()
{
	std::string ret("(Prefix unary operation)");
	switch(m_operatorType)
	{
		case Increment:		ret += "++" + m_expression->print();		break;
		case Decrement:		ret += "--" + m_expression->print();		break;
		case Sizeof:		ret += "sizeof" + m_expression->print();	break;
		case AddressOf:		ret += "&" + m_expression->print();			break;
		case Indirection:	ret += "*" + m_expression->print();			break;
		case UnaryPlus:		ret += "+" + m_expression->print();			break;
		case UnaryMinus:	ret += "-" + m_expression->print();			break;
		case BitwiseNot:	ret += "~" + m_expression->print();			break;
		case Not:			ret += "!" + m_expression->print();			break;
	}
	return ret;
}