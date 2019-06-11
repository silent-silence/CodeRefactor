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
#include "AST/Expr.h"

using std::make_shared;					using std::weak_ptr;
using std::shared_ptr;

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

std::shared_ptr<DeclContext> DeclContextHolder::getContextRoot() const
{
	return m_contextRoot;
}

std::shared_ptr<Decl> DeclContextHolder::createVariable(std::shared_ptr<DeclContext> &context, std::string &name, SourceLocation &location, std::shared_ptr<QualType> type)
{
	weak_ptr<Decl> foundDecl;
	try {
		foundDecl = context->lookupCurrentContext(name);
	} catch (SymbolNotFound &e) {
		// The name is not found
		auto decl = make_shared<VarDecl>(
				Decl::Kind::Var, context, location, make_shared<IdentifierInfo>(name), type, VarDecl::StorageClass::None
		);
		context->addDecl(decl);
		return decl;
	}
	// The name is token, check if it's a variable declaration
	Decl::Kind foundDeclKind = foundDecl.lock()->getKind();
	if(foundDeclKind == Decl::Kind::Var || foundDeclKind == Decl::Kind::Typedef)
		throw SymbolAlreadyExist("Symbol '" + name + "' already exist");
	auto decl = make_shared<VarDecl>(
			Decl::Kind::Var, context, location, make_shared<IdentifierInfo>(name), type, VarDecl::StorageClass::None
	);
	context->addDecl(decl);
	return decl;
}

std::shared_ptr<Decl> DeclContextHolder::createTypedefDecl(std::shared_ptr<DeclContext> &context, std::shared_ptr<QualType> type, std::string &name, SourceLocation &location)
{
	weak_ptr<Decl> foundDecl;
	try {
		foundDecl = context->lookupCurrentContext(name);
	} catch (SymbolNotFound &e) {
		// The name is not found
		auto typedefDecl = make_shared<TypedefDecl>(
				context, location, make_shared<IdentifierInfo>(name), type
		);
		context->addDecl(typedefDecl);
		return typedefDecl;
	}
	// The name is token, check if it's a variable declaration
	Decl::Kind foundDeclKind = foundDecl.lock()->getKind();
	if(foundDeclKind == Decl::Kind::Var || foundDeclKind == Decl::Kind::Typedef)
		throw SymbolAlreadyExist("Symbol '" + name + "' already exist");
	auto typedefDecl = make_shared<TypedefDecl>(
			context, location, make_shared<IdentifierInfo>(name), type
	);
	context->addDecl(typedefDecl);
	return typedefDecl;
}

std::shared_ptr<DeclContext> DeclContextHolder::createBlock(std::shared_ptr<DeclContext> &context, SourceLocation &&location)
{
	auto block = make_shared<BlockDecl>(context, location);
	context->addDecl(block);
	return block;
}

std::shared_ptr<DeclContext> DeclContextHolder::createStruct(std::shared_ptr<DeclContext> &context, SourceLocation &&location, std::string &name)
{
	auto structBlock = make_shared<RecordDecl>(Decl::Kind::Record, TagDecl::TagKind::KindStruct, context, location, make_shared<IdentifierInfo>());
	context->addDecl(structBlock);
	return structBlock;
}

std::shared_ptr<DeclContext> DeclContextHolder::createStruct(std::shared_ptr<DeclContext> &context, SourceLocation &&location)
{
	auto structBlock = make_shared<RecordDecl>(Decl::Kind::Record, TagDecl::TagKind::KindStruct, context, location, make_shared<IdentifierInfo>());
	context->addDecl(structBlock);
	return structBlock;
}

std::shared_ptr<Decl> DeclContextHolder::createEnum(std::shared_ptr<DeclContext> &context, SourceLocation &&location, std::string &name)
{
	auto enumBlock = make_shared<EnumDecl>(context, location, make_shared<IdentifierInfo>(name));
	context->addDecl(enumBlock);
	return enumBlock;
}

std::shared_ptr<Decl> DeclContextHolder::createEnum(std::shared_ptr<DeclContext> &context, SourceLocation &&location)
{
	auto enumBlock = make_shared<EnumDecl>(context, location, make_shared<IdentifierInfo>());
	context->addDecl(enumBlock);
	return enumBlock;
}

std::shared_ptr<Decl> DeclContextHolder::createEnumConstant(std::shared_ptr<DeclContext> &context, SourceLocation &&location, std::string name, std::shared_ptr<Expr> init)
{
	shared_ptr<QualType> type;
	if(init)
		type = init->getType().lock();
	else
		type = BuiltinType::creator(BuiltinType::UInt, QualType::Const);
	auto enumConstant = make_shared<EnumConstantDecl>(
			context, location, make_shared<IdentifierInfo>(name), type, init
			);
	context->addDecl(enumConstant);
	return enumConstant;
}

std::shared_ptr<Decl> DeclContextHolder::createFunction(std::shared_ptr<DeclContext> &context, std::string &name,
									   SourceLocation &location, std::shared_ptr<QualType> type)
{
	auto func = make_shared<FunctionDecl>(
			Decl::Kind::Function, context, location, make_shared<IdentifierInfo>(name), type, FunctionDecl::StorageClass::None, false
			);
	context->addDecl(func);
	return func;
}

std::shared_ptr<Decl> DeclContextHolder::createParmVar(std::shared_ptr<DeclContext> &context, std::string &name,
													   SourceLocation &location, std::shared_ptr<QualType> type)
{
	auto parm = make_shared<ParmVarDecl>(
			Decl::Kind::ParmVar, context, location, make_shared<IdentifierInfo>(name), type, VarDecl::StorageClass::None
	);
	context->addDecl(parm);
	return parm;
}