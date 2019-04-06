#include <iostream>
#include "BinaryOperation.h"

BinaryOperation::BinaryOperation(std::shared_ptr<Expression> firstExpression, BinaryOperator operatorType, std::shared_ptr<Expression> secondExpression)
	: m_firstExpression{firstExpression}, m_operatorType{operatorType}, m_secondExpression{secondExpression}
{}

void BinaryOperation::print()
{
	std::cout << "(Binary operaion)";
	m_firstExpression->print();
	switch(m_operatorType)
	{
		case BasicAssignment:				std::cout << "=";	break;
		case MultiplicationAssignment:		std::cout << "*=";	break;
		case DivisionAssignment:			std::cout << "-=";	break;
		case ModuloAssignment:				std::cout << "%=";	break;
		case AdditionAssignment:			std::cout << "+=";	break;
		case SubtractionAssignment:			std::cout << "-=";	break;
		case BitwiseLeftShiftAssignment:	std::cout << "<<=";	break;
		case BitwiseRightShifAssignment:	std::cout << ">>=";	break;
		case BitwiseAndAssignment:			std::cout << "&=";	break;
		case BitwiseXorAssignment:			std::cout << "^=";	break;
		case BitwiseOrAssignment:			std::cout << "|=";	break;
		case LogicalOr:						std::cout << "||";	break;
		case LogicalAnd:					std::cout << "&&";	break;
		case BitwiseOr:						std::cout << "|";	break;
		case BitwiseXor:					std::cout << "^";	break;
		case BitwiseAnd:					std::cout << "&";	break;
		case EqualTo:						std::cout << "==";	break;
		case NotEqualTo:					std::cout << "!=";	break;
		case LessThan:						std::cout << "<";	break;
		case GreaterThan:					std::cout << ">";	break;
		case LessThanOrEqualTo:				std::cout << "<=";	break;
		case GreaterThanOrEqualTo:			std::cout << ">=";	break;
		case BitwiseLeftShift:				std::cout << "<<";	break;
		case BitwiseRightShift:				std::cout << ">>";	break;
		case Addition:						std::cout << "+";	break;
		case Subtraction:					std::cout << "-";	break;
		case Multiplication:				std::cout << "*";	break;
		case Division:						std::cout << "/";	break;
		case Modulo:						std::cout << "%";	break;
	}
	m_secondExpression->print();
}