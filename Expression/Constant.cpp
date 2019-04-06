#include <iostream>
#include "Constant.h"

Constant::Constant(std::string name)
	: m_name{name}
{}

void Constant::print()
{
	std::cout << "(Constant:)" << m_name;
}