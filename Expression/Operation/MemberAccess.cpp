//
// Created by 17271 on 2019/4/2.
//

#include <iostream>
#include "MemberAccess.h"
#include "../Identity.h"

MemberAccess::MemberAccess(std::shared_ptr<Expression> expression, MemberOperator accessOperator,
						   std::shared_ptr<Identity> identity)
	: m_expression{expression}, m_accessOperator{accessOperator}, m_identity{identity}
{}

void MemberAccess::print()
{
	std::cout << "(Member access)";
	switch(m_accessOperator)
	{
		case StructureDereference:	m_expression->print(); std::cout << "->"; m_identity->print();	break;
		case StructureReference:	m_expression->print(); std::cout << "."; m_identity->print();	break;
	}
}