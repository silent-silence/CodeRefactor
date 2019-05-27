//
// Created by 17271 on 2019/5/22.
//

#include "DeclContextHolder.h"
#include "DeclBase.h"
#include "Decl.h"
#include "DeclName.h"

using std::make_shared;

DeclContextHolder::DeclContextHolder()
{
	m_contextRoot = make_shared<DeclContext>();
}

std::shared_ptr<DeclContext> DeclContextHolder::getContext() const
{
	return m_contextRoot;
}

void DeclContextHolder::createVariable(std::string name, SourceLocation &location, std::shared_ptr<QualType> type)
{
	DeclName varName{name};
	auto decl = make_shared<VariableDecl>(m_contextRoot, location, type, varName);
	m_contextRoot->addDecl(decl);
}

#ifdef ENV_TEST
void DeclContextHolder::clean()
{
	m_contextRoot = make_shared<DeclContext>();
}
#endif