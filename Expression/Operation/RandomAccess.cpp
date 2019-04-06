//
// Created by 17271 on 2019/4/2.
//

#include <iostream>
#include "RandomAccess.h"

RandomAccess::RandomAccess(std::shared_ptr <Expression> expression, std::shared_ptr <Expression> accessPoint)
	: m_expression{expression}, m_accessPoint{accessPoint}
{}

void RandomAccess::print()
{
	std::cout << "(Random access)";
	m_expression->print();
	std::cout << "[";
	m_accessPoint->print();
	std::cout << "]";
}