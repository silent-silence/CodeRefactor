#include <iostream>
#include "Constant.h"

Constant::Constant(std::string name)
	: m_name{name}
{}

std::string Constant::print()
{
	return "(Constant)" + m_name;
}