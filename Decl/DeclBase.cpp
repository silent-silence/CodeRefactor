//
// Created by gaojian on 19-5-6.
//

#include "Errors/SymbolError.hpp"
#include "DeclBase.h"
#include "Decl.h"
#include "Basic/IdentifierTable.h"
#include <functional>

using std::hash;				using std::string;
using std::to_string;			using std::dynamic_pointer_cast;

/// @Decl
Decl::Decl(Kind declKind, SourceLocation location)
	: m_kind{declKind}, m_loc{location}, m_isUsed{false}
{}

Decl::Decl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location)
	: m_kind{declKind}, m_belongToContext{context}, m_loc{location}, m_isUsed{false}
{}

/// @destructor pure-virtual
Decl::~Decl()
{}

SourceLocation Decl::getSourceLocation() const
{
	return m_loc;
}

Decl::Kind Decl::getKind() const
{
	return m_kind;
}

std::weak_ptr<Decl> Decl::getNextDeclInContext()
{
	return m_nextDeclInContext;
}

const std::weak_ptr<Decl> Decl::getNextDeclInContext() const
{
	return m_nextDeclInContext;
}

std::weak_ptr<DeclContext> Decl::getDeclContext()
{
	return m_belongToContext;
}

const std::weak_ptr<DeclContext> Decl::getDeclContext() const
{
	return m_belongToContext;
}

/// @DeclContext
DeclContext::DeclContext(Decl::Kind K)
	: m_declKind{K}
{}

/// @destructor pure-virtual
DeclContext::~DeclContext()
{}

Decl::Kind DeclContext::getDeclKind() const
{
	return m_declKind;
}

void DeclContext::addDecl(std::shared_ptr<Decl> decl)
{
	if(std::dynamic_pointer_cast<NamedDecl>(decl))
	{
		auto namedDecl = std::dynamic_pointer_cast<NamedDecl>(decl);
		StoredDecl storedDecl(StoredDecl::StoredKind::DeclPtr, namedDecl);
		try {
			lookupForExistence(storedDecl, false);
		} catch (SymbolNotFound &s) {		// normal flow
			m_decls[storedDecl] = decl;
			m_declsList.push_back(decl);
		} catch(SymbolAlreadyExist &s) {	// throw
			throw s;
		}
	}
	else	// unnamed decls(compound stmt in funtion)
	{
		m_decls[StoredDecl(StoredDecl::StoredKind::DeclId)] = decl;
		m_declsList.push_back(decl);
	}
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

void DeclContext::lookupForExistence(StoredDecl &name, bool lookIntoParent)
{
	bool findInCurrentContext = m_decls.find(name) != m_decls.end();

	if(!findInCurrentContext && lookIntoParent && !getParent().expired())
		getParent().lock()->lookupForExistence(name, lookIntoParent);
	else if(!findInCurrentContext)
		throw SymbolNotFound("Symbol '" + name.getDeclAsString() + "' not found!");
	else if(findInCurrentContext)
		throw SymbolAlreadyExist("Symbol '" + name.getDeclAsString() + "' already exist");
}

DeclIterator DeclContext::decl_begin()
{
	return DeclIterator(m_declsList.begin());
}

DeclIterator DeclContext::decl_end()
{
	return DeclIterator(m_declsList.end());
}

void DeclContext::renameDecl(std::shared_ptr<Decl> decl, std::string newName)
{
	auto identifier = dynamic_pointer_cast<NamedDecl>(decl)->getIdentifier().lock();
	auto it = m_decls.find(LookupStoredDecl(identifier->getName()));
	StoredDecl storedDecl = it->first;
	identifier->setName(newName);
	m_decls.erase(it);
	m_decls[storedDecl] = decl;
}

void DeclContext::moveToTail(std::shared_ptr<Decl> decl)
{
	auto it = std::find(m_declsList.begin(), m_declsList.end(), decl);
	if(it != m_declsList.end())
		m_declsList.erase(it);
	m_declsList.emplace_back(decl);
}

/// @StoredDecl
unsigned StoredDecl::declId;

StoredDecl::StoredDecl()
	: declKind{StoredDecl::StoredKind::DeclId}
{}

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

/// @LookupStoredDecl
LookupStoredDecl::LookupStoredDecl(std::string name)
		: StoredDecl(), lookupName{name}
{}

std::string LookupStoredDecl::getDeclAsString() const
{
	return lookupName;
}

/// @StoredDeclHash
std::size_t StoredDeclHash::operator()(const StoredDecl &name) const
{
	return hash<string>()(name.getDeclAsString());
}

bool operator ==(const StoredDecl &s1, const StoredDecl &s2)
{
	return s1.getDeclAsString() == s2.getDeclAsString();
}
