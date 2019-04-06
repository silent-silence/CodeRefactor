//
// Created by 17271 on 2019/4/2.
//

#include <iostream>
#include "ArgumentList.h"

ArgumentList::ArgumentList(std::shared_ptr<ArgumentList> argumentList, std::shared_ptr<Expression> expression)
	: m_argumentList{argumentList}, m_expression{expression}
{}

void ArgumentList::print()
{
	std::cout << "(Argument list)";
	m_expression->print();
}