#include <iostream>
#include "Identity.h"

Identity::Identity(std::string id)
	: m_id{id}
{}

std::string Identity::print()
{
	return "(Identity)" + m_id;
}
