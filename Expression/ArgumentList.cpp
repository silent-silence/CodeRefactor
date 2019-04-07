//
// Created by 17271 on 2019/4/2.
//

#include <iostream>
#include "ArgumentList.h"

ArgumentList::ArgumentList(std::shared_ptr<ArgumentList> argumentList, std::shared_ptr<Expression> expression)
	: m_argumentList{argumentList}, m_expression{expression}
{}

std::string ArgumentList::print()
{
	return "(Argument list)" + (m_argumentList ? m_argumentList->print() : "") + m_expression->print();
}
