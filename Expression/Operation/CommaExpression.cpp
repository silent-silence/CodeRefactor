//
// Created by 17271 on 2019/4/2.
//

#include <iostream>
#include "CommaExpression.h"

CommaExpression::CommaExpression(std::shared_ptr <Expression> firstExpression,
								 std::shared_ptr <Expression> secondExpression)
	: m_preExpression{firstExpression}, m_lastExpression{secondExpression}
{}

std::string CommaExpression::print()
{
	return "(Comma expression)" + m_preExpression->print() + ", " + m_lastExpression->print();
}