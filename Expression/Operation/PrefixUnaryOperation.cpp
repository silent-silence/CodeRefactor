#include <iostream>
#include "PrefixUnaryOperation.h"

PrefixUnaryOperation::PrefixUnaryOperation(PrefixUnary operatorType, std::shared_ptr<Expression> expression)
	: m_operatorType{operatorType}, m_expression{expression}
{}

void PrefixUnaryOperation::print()
{
	std::cout << "(Prefix unary operation)";
	switch(m_operatorType)
	{
		case Increment:		std::cout << "++";		m_expression->print();	break;
		case Decrement:		std::cout << "--";		m_expression->print();	break;
		case Sizeof:		std::cout << "sizeof";	m_expression->print();	break;
		case AddressOf:		std::cout << "&";		m_expression->print();	break;
		case Indirection:	std::cout << "*";		m_expression->print();	break;
		case UnaryPlus:		std::cout << "+";		m_expression->print();	break;
		case UnaryMinus:	std::cout << "-";		m_expression->print();	break;
		case BitwiseNot:	std::cout << "~";		m_expression->print();	break;
		case Not:			std::cout << "!";		m_expression->print();	break;
	}
}