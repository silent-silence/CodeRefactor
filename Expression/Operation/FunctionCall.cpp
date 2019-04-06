//
// Created by 17271 on 2019/4/2.
//

#include <iostream>
#include "FunctionCall.h"
#include "../ArgumentList.h"

FunctionCall::FunctionCall(std::shared_ptr <Expression> expression, std::shared_ptr<ArgumentList> argumentList)
	: m_expression{expression}, m_argumentList{argumentList}
{}

void FunctionCall::print()
{
	std::cout << "(Function call)";
	m_expression->print();
	std::cout << "(";
	m_argumentList->print();
	std::cout << ")";
}