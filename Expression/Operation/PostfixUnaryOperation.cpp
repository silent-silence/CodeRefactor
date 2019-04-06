//
// Created by 17271 on 2019/4/2.
//

#include <iostream>
#include "PostfixUnaryOperation.h"

PostfixUnaryOperation::PostfixUnaryOperation(std::shared_ptr <Expression> expression, PostfixUnary operatorType)
	: m_expression{expression}, m_operatorType{operatorType}
{}

void PostfixUnaryOperation::print()
{
	std::cout << "(Postfix unary operation)";
	switch(m_operatorType)
	{
		case Increment:	m_expression->print(); std::cout << "++";	break;
		case Decrement:	m_expression->print(); std::cout << "++";	break;
	}
}