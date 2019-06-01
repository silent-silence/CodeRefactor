//
// Created by 17271 on 2019/5/31.
//

#include "Decl.h"

TranslationUnitDecl::TranslationUnitDecl()
	: Decl{Decl::Kind::TranslationUnit, SourceLocation()}, DeclContext{Decl::Kind::TranslationUnit}
{}

std::weak_ptr<DeclContext> TranslationUnitDecl::getParent()
{
	return getDeclContext();
}

const std::weak_ptr<DeclContext> TranslationUnitDecl::getParent() const
{
	return getDeclContext();
}

NamedDecl::NamedDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
		DeclName name)
	: Decl(declKind, context, location), m_name{name}
{}

ValueDecl::ValueDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
DeclName name,
		std::shared_ptr<QualType> type)
: NamedDecl{declKind, context, location, name}, m_declType{type}
{}

ValueDecl::~ValueDecl()
{}

DeclaratorDecl::DeclaratorDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
		DeclName name,
		std::shared_ptr<QualType> type/*,
		std::shared_ptr<DeclaratorInfo> info*/)
	: ValueDecl{declKind, context, location, name, type}/*, m_declInfo{info}*/
{}

DeclaratorDecl::~DeclaratorDecl()
{}

VarDecl::VarDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
		std::shared_ptr<IdentifierInfo> id,
		std::shared_ptr<QualType> type,
		/*std::shared_ptr<DeclaratorInfo> info,*/
		StorageClass sc)
	: DeclaratorDecl{declKind, context, location, DeclName(id), type/*, info*/}, SClass{sc}, DeclaredInCondition{false}
{}

VarDecl::VarDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
		DeclName name,
		std::shared_ptr<QualType> type,
		/*std::shared_ptr<DeclaratorInfo> info,*/
		StorageClass sc)
	: DeclaratorDecl{declKind, context, location, name, type/*, info*/}, SClass{sc}, DeclaredInCondition{false}
{}

BlockDecl::BlockDecl(std::weak_ptr<DeclContext> context, SourceLocation location)
	: Decl(Decl::Kind::Block, context, location), DeclContext{Decl::Kind::Block}
{}

std::weak_ptr<DeclContext> BlockDecl::getParent()
{
	return getDeclContext();
}

const std::weak_ptr<DeclContext> BlockDecl::getParent() const
{
	return getDeclContext();
}

std::weak_ptr<CompoundStmt> BlockDecl::getBody() const
{
	return m_body;
}

void BlockDecl::setBody(std::weak_ptr<CompoundStmt> body)
{
	m_body = body;
}
