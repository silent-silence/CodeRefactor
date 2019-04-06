//
// Created by 17271 on 2019/4/2.
//

#include <iostream>
#include "CommaExpression.h"

CommaExpression::CommaExpression(std::shared_ptr <Expression> firstExpression,
								 std::shared_ptr <Expression> secondExpression)
	: m_preExpression{firstExpression}, m_lastExpression{secondExpression}
{}

void CommaExpression::print()
{
	std::cout << "(Comma expression)";
	m_preExpression->print();
	std::cout << ", ";
	m_lastExpression->print();
}