//
// Created by 17271 on 2019/5/22.
//

#include "Errors/SymbolError.hpp"
#include "DeclContextHolder.h"
#include "DeclBase.h"
#include "Decl.h"
#include "DeclName.h"
#include "Basic/IdentifierTable.h"
#include "AST/Stmt.h"

using std::make_shared;					using std::weak_ptr;

DeclContextHolder::DeclContextHolder()
{
	m_contextRoot = make_shared<TranslationUnitDecl>();
}

#ifdef ENV_TEST
void DeclContextHolder::clean()
{
	m_contextRoot = make_shared<TranslationUnitDecl>();
}
#endif

std::shared_ptr<DeclContext> DeclContextHolder::getContext() const
{
	return m_contextRoot;
}

std::shared_ptr<Decl> DeclContextHolder::createVariable(std::shared_ptr<DeclContext> &context, std::string name, SourceLocation &location, std::shared_ptr<QualType> type)
{
	weak_ptr<Decl> foundDecl;
	try
	{
		foundDecl = context->lookupCurrentContext(name);
	} catch (SymbolNotFound &e) {
		// The name is not found
		auto decl = make_shared<VarDecl>(Decl::Kind::Var, context, location, make_shared<IdentifierInfo>(name), type, VarDecl::StorageClass::None);
		context->addDecl(decl);
		return decl;
	}
	// The name is token, check if it's a variable declaration
	if(foundDecl.lock()->getKind() == Decl::Var)
		throw SymbolAlreadyExist("Symbol '" + name + "' already exist");
	auto decl = make_shared<VarDecl>(Decl::Kind::Var, context, location, make_shared<IdentifierInfo>(name), type, VarDecl::StorageClass::None);
	context->addDecl(decl);
	return decl;
}

std::shared_ptr<DeclContext> DeclContextHolder::createBlock(std::shared_ptr<DeclContext> &context, SourceLocation &&location)
{
	auto block = make_shared<BlockDecl>(context, location);
	context->addDecl(block);
	return block;
}
