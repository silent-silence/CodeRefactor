#include <iostream>
#include "BinaryOperation.h"

BinaryOperation::BinaryOperation(std::shared_ptr<Expression> firstExpression, BinaryOperator operatorType, std::shared_ptr<Expression> secondExpression)
	: m_firstExpression{firstExpression}, m_operatorType{operatorType}, m_secondExpression{secondExpression}
{}

std::string BinaryOperation::print()
{
	std::string ret("(Binary operaion)");
	ret += m_firstExpression->print();
	switch(m_operatorType)
	{
		case BasicAssignment:				ret += "=";		break;
		case MultiplicationAssignment:		ret += "*=";	break;
		case DivisionAssignment:			ret += "-=";	break;
		case ModuloAssignment:				ret += "%=";	break;
		case AdditionAssignment:			ret += "+=";	break;
		case SubtractionAssignment:			ret += "-=";	break;
		case BitwiseLeftShiftAssignment:	ret += "<<=";	break;
		case BitwiseRightShifAssignment:	ret += ">>=";	break;
		case BitwiseAndAssignment:			ret += "&=";	break;
		case BitwiseXorAssignment:			ret += "^=";	break;
		case BitwiseOrAssignment:			ret += "|=";	break;
		case LogicalOr:						ret += "||";	break;
		case LogicalAnd:					ret += "&&";	break;
		case BitwiseOr:						ret += "|";		break;
		case BitwiseXor:					ret += "^";		break;
		case BitwiseAnd:					ret += "&";		break;
		case EqualTo:						ret += "==";	break;
		case NotEqualTo:					ret += "!=";	break;
		case LessThan:						ret += "<";		break;
		case GreaterThan:					ret += ">";		break;
		case LessThanOrEqualTo:				ret += "<=";	break;
		case GreaterThanOrEqualTo:			ret += ">=";	break;
		case BitwiseLeftShift:				ret += "<<";	break;
		case BitwiseRightShift:				ret += ">>";	break;
		case Addition:						ret += "+";		break;
		case Subtraction:					ret += "-";		break;
		case Multiplication:				ret += "*";		break;
		case Division:						ret += "/";		break;
		case Modulo:						ret += "%";		break;
	}
	ret += m_secondExpression->print();
	return ret;
}