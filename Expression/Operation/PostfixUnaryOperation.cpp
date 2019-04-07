//
// Created by 17271 on 2019/4/2.
//

#include <iostream>
#include "PostfixUnaryOperation.h"

PostfixUnaryOperation::PostfixUnaryOperation(std::shared_ptr <Expression> expression, PostfixUnary operatorType)
	: m_expression{expression}, m_operatorType{operatorType}
{}

std::string PostfixUnaryOperation::print()
{
	std::string ret("(Postfix unary operation)");
	switch(m_operatorType)
	{
		case Increment:	ret += m_expression->print() + "++";	break;
		case Decrement:	ret += m_expression->print() + "++";	break;
	}
	return ret;
}