//
// Created by 17271 on 2019/4/2.
//

#include <iostream>
#include "TernaryOperation.h"

TernaryOperation::TernaryOperation(std::shared_ptr <Expression> firstExpression,
								 std::shared_ptr <Expression> secondExpression,
								 std::shared_ptr <Expression> thirdExpression)
	: m_firstExpression{firstExpression}, m_secondExpression{secondExpression}, m_thirdExpression{thirdExpression}
{}

void TernaryOperation::print()
{
	std::cout << "(Ternary)";
	m_firstExpression->print();
	std::cout << " ? ";
	m_secondExpression->print();
	std::cout << " : ";
	m_thirdExpression->print();
}