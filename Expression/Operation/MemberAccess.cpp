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

std::string MemberAccess::print()
{
	std::string ret("(Member access)");
	switch(m_accessOperator)
	{
		case StructureDereference:	ret += m_expression->print() + "->" + m_identity->print();	break;
		case StructureReference:	ret += m_expression->print() + "." + m_identity->print();	break;
	}
	return ret;
}