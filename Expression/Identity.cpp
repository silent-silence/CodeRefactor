#include <iostream>
#include "Identity.h"

Identity::Identity(std::string id)
	: m_id{id}
{}

void Identity::print()
{
	std::cout << "(Identity)" << m_id;
}