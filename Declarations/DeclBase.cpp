//
// Created by gaojian on 19-5-6.
//

#include "DeclBase.h"
#include "Decl.h"

Decl::~Decl()
{}

void DeclContext::addDecl(std::shared_ptr<Decl> d) {
	if(std::dynamic_pointer_cast<NamedDecl>(d))
		m_decls[std::dynamic_pointer_cast<NamedDecl>(d)->getDeclarationName()] = d;
	else
		m_decls[DeclarationName()] = d;
}
