//
// Created by 17271 on 2019/6/11.
//

#include "DeclIterator.h"
#include "DeclBase.h"

DeclIterator::DeclIterator(std::list<std::shared_ptr<Decl>>::iterator it)
	: m_it{it}
{}
DeclIterator& DeclIterator::operator++()
{
	++m_it;
	return *this;
}

DeclIterator& DeclIterator::operator--()
{
	--m_it;
	return *this;
}

std::shared_ptr<Decl> DeclIterator::operator*()
{
	return *m_it;
}

bool DeclIterator::operator==(DeclIterator it) const
{
	return m_it == it.m_it;
}

bool DeclIterator::operator!=(DeclIterator it) const
{
	return m_it != it.m_it;
}