//
// Created by gaojian on 19-5-6.
//

#include <Errors/SymbolError.hpp>
#include "DeclBase.h"
#include "Decl.h"

Decl::~Decl()
{}

void DeclContext::addDecl(std::shared_ptr<Decl> d) {
	if(std::dynamic_pointer_cast<NamedDecl>(d))
		m_decls[std::dynamic_pointer_cast<NamedDecl>(d)->getDeclarationName()] = d;
	else
		m_decls[DeclName()] = d;
}

std::weak_ptr<Decl> DeclContext::lookup(DeclName &name)
{
	auto decl = m_decls.find(name) != m_decls.cend() ? m_decls.find(name)->second : nullptr;
	if(!decl)
		throw SymbolNotFound("Symbol '" + name.getIdentifier() + "' not found!");
	return decl;
}

std::weak_ptr<Decl> DeclContext::lookup(DeclName &&name)
{
	auto decl = m_decls.find(name) != m_decls.cend() ? m_decls.find(name)->second : nullptr;
	if(!decl)
		throw SymbolNotFound("Symbol '" + name.getIdentifier() + "' not found!");
	return decl;
}

std::weak_ptr<Decl> DeclContext::lookup(std::string &name)
{
	return lookup(DeclName(name));
}

std::weak_ptr<Decl> DeclContext::lookup(std::string &&name)
{
	return lookup(DeclName(name));
}
