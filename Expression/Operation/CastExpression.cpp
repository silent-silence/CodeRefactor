//
// Created by 17271 on 2019/4/2.
//

#include <iostream>
#include "CastExpression.h"

CastExpression::CastExpression(std::string castType, std::shared_ptr <Expression> expression)
	: m_castType{castType}, m_expression{expression}
{}

void CastExpression::print()
{
	std::cout << "(Cast expression)(" << m_castType << ")";
	m_expression->print();
}
