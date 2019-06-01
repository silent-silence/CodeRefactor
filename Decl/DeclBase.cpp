//
// Created by gaojian on 19-5-6.
//

#include "Errors/SymbolError.hpp"
#include "DeclBase.h"
#include "Decl.h"
#include <functional>

using std::hash;				using std::string;
using std::to_string;

Decl::~Decl()
{}

unsigned StoredDecl::declId;

StoredDecl::StoredDecl(StoredDecl::StoredKind kind, std::shared_ptr<NamedDecl> ptr)
	: declKind{kind}, declPtr{ptr}
{
	++declId;
}

std::string StoredDecl::getDeclAsString() const
{
	string declName;
	switch(declKind)
	{
		case StoredKind::DeclPtr:
			declName = declPtr->getNameAsString();
			break;
		case StoredKind::DeclId:
			declName = to_string(declId);
			break;
	}
	return declName;
}

size_t StoredDecl::operator()(const StoredDecl &name) const
{
	return hash<string>()(name.getDeclAsString());
}

LookupStoredDecl::LookupStoredDecl(std::string name)
	: lookupName{name}
{}

std::string LookupStoredDecl::getDeclAsString() const
{
	return lookupName;
}

bool operator ==(const StoredDecl &s1, const StoredDecl &s2)
{
	return s1.getDeclAsString() == s2.getDeclAsString();
}

Decl::Kind DeclContext::getDeclKind() const
{
	return m_declKind;
}

void DeclContext::addDecl(std::shared_ptr<Decl> d)
{
	if(std::dynamic_pointer_cast<NamedDecl>(d))
		m_decls[StoredDecl(StoredDecl::StoredKind::DeclPtr, std::dynamic_pointer_cast<NamedDecl>(d))] = d;
	else
		m_decls[StoredDecl(StoredDecl::StoredKind::DeclId)] = d;
}

std::weak_ptr<Decl> DeclContext::lookup(StoredDecl &name)
{
	std::weak_ptr<Decl> decl = m_decls.find(name) != m_decls.end() ? m_decls.find(name)->second : nullptr;
	// if not find in current context, lookup in parent
	if(!decl.lock() && !getParent().expired())
		decl = getParent().lock()->lookup(name);
	// chekc again
	if(!decl.lock())
		throw SymbolNotFound("Symbol '" + name.getDeclAsString() + "' not found!");
	return decl;
}

std::weak_ptr<Decl> DeclContext::lookup(StoredDecl &&name)
{
	std::weak_ptr<Decl> decl = m_decls.find(name) != m_decls.end() ? m_decls.find(name)->second : nullptr;
	// if not find in current context, lookup in parent
	if(!decl.lock() && !getParent().expired())
		decl = getParent().lock()->lookup(name);
	// chekc again
	if(!decl.lock())
		throw SymbolNotFound("Symbol '" + name.getDeclAsString() + "' not found!");
	return decl;
}

std::weak_ptr<Decl> DeclContext::lookup(std::string &name)
{
	return lookup(LookupStoredDecl(name));
}

std::weak_ptr<Decl> DeclContext::lookup(std::string &&name)
{
	return lookup(LookupStoredDecl(name));
}

std::weak_ptr<Decl> DeclContext::lookupCurrentContext(StoredDecl &name)
{
	std::weak_ptr<Decl> decl = m_decls.find(name) != m_decls.end() ? m_decls.find(name)->second : nullptr;
	if(!decl.lock())
		throw SymbolNotFound("Symbol '" + name.getDeclAsString() + "' not found!");
	return decl;
}

std::weak_ptr<Decl> DeclContext::lookupCurrentContext(StoredDecl &&name)
{
	std::weak_ptr<Decl> decl = m_decls.find(name) != m_decls.end() ? m_decls.find(name)->second : nullptr;
	if(!decl.lock())
		throw SymbolNotFound("Symbol '" + name.getDeclAsString() + "' not found!");
	return decl;
}

std::weak_ptr<Decl> DeclContext::lookupCurrentContext(std::string &name)
{
	return lookupCurrentContext(LookupStoredDecl(name));
}

std::weak_ptr<Decl> DeclContext::lookupCurrentContext(std::string &&name)
{
	return lookupCurrentContext(LookupStoredDecl(name));
}
