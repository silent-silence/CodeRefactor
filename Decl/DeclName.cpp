//
// Created by 17271 on 2019/5/31.
//

#include "IdentifierTable.h"
#include "DeclName.h"

std::string DeclName::getAsString() const
{
	return m_storedIdentifier->getName();
}

std::weak_ptr<IdentifierInfo> DeclName::getAsIdentifierInfo() const
{
	return m_storedIdentifier;
}
