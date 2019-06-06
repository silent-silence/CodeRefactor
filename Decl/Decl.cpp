//
// Created by 17271 on 2019/5/31.
//

#include "Decl.h"
#include "AST/Type.h"

/// @TranslationUnitDecl
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

/// @NamedDecl
NamedDecl::NamedDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
		DeclName name)
	: Decl(declKind, context, location), m_name{name}
{}

DeclName NamedDecl::getDeclarationName() const
{
	return m_name;
}

std::string NamedDecl::getNameAsString() const
{
	return m_name.getAsString();
}

std::weak_ptr<IdentifierInfo> NamedDecl::getIdentifier() const
{
	return m_name.getAsIdentifierInfo();
}

/// @ValueDecl
ValueDecl::ValueDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
		DeclName name,
		std::shared_ptr<QualType> type)
	: NamedDecl{declKind, context, location, name}, m_declType{type}
{}

std::weak_ptr<QualType> ValueDecl::getType() const
{
	return m_declType;
}

void ValueDecl::setType(std::shared_ptr<QualType> newType)
{
	m_declType = newType;
}

/// @destructor pure-virtual
ValueDecl::~ValueDecl()
{}

/// @DeclaratorDecl
DeclaratorDecl::DeclaratorDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
		DeclName name,
		std::shared_ptr<QualType> type/*,
		std::shared_ptr<DeclaratorInfo> info*/)
	: ValueDecl{declKind, context, location, name, type}/*, m_declInfo{info}*/
{}

/// @destructor pure-virtual
DeclaratorDecl::~DeclaratorDecl()
{}

/// @VarDecl
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

/// @BlockDecl
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

/// @TypeDecl
TypeDecl::TypeDecl(Kind kind, std::weak_ptr<DeclContext> context, SourceLocation l, std::shared_ptr<IdentifierInfo> info)
	: NamedDecl{kind, context, l, info}, m_typeForDecl{nullptr}
{}

/// @destructor pure-virtual
TypeDecl::~TypeDecl()
{}

std::weak_ptr<Type> TypeDecl::getTypeForDecl() const
{
	return m_typeForDecl;
}

void TypeDecl::setTypeForDecl(std::shared_ptr<Type> type)
{
	m_typeForDecl = type;
}

/// @TypedefDecl
TypedefDecl::TypedefDecl(std::weak_ptr<DeclContext> context, SourceLocation l, std::shared_ptr<IdentifierInfo> info,
		std::shared_ptr<QualType> type)
	: TypeDecl{Typedef, context, l, info}, underlyingType{type}
{
	setTypeForDecl(type->getTypePtr());
}

/// @TagDecl
TagDecl::TagDecl(Decl::Kind declKind, TagDecl::TagKind tagKind, std::weak_ptr<DeclContext> context,
				 SourceLocation location, std::shared_ptr<IdentifierInfo> info,
				 SourceLocation tkl)
	: TypeDecl{declKind, context, location, info}, DeclContext{declKind}, TagKeyWordLoc{tkl}, m_kind{tagKind}
{}

std::weak_ptr<DeclContext> TagDecl::getParent()
{
	return getDeclContext();
}

const std::weak_ptr<DeclContext> TagDecl::getParent() const
{
	return getDeclContext();
}

bool TagDecl::isDefinition() const
{
	return m_isDefinition;
}

std::string TagDecl::getKindName() const
{
	switch (getTagKind())
	{
		case TagKind::KindStruct:	return "struct";
		case TagKind::KindEnum:		return "enum";
		case TagKind::KindUnion:	return "union";
	}
}

TagDecl::TagKind TagDecl::getTagKind() const
{
	return m_kind;
}

void TagDecl::setTagKind(TagDecl::TagKind kind)
{
	m_kind = kind;
}

bool TagDecl::isStruct() const
{
	return m_kind == TagKind::KindStruct;
}

bool TagDecl::isUnion() const
{
	return m_kind == TagKind::KindUnion;
}

bool TagDecl::isEnum() const
{
	return m_kind == TagKind::KindEnum;
}

std::weak_ptr<TypedefDecl> TagDecl::getTypedefForAnonDecl() const
{
	return m_typedefForAnonDecl;
}

void TagDecl::setTypedefForAnonDecl(std::shared_ptr<TypedefDecl> TDD)
{
	m_typedefForAnonDecl = TDD;
}

/// @RecordDecl
RecordDecl::RecordDecl(Decl::Kind declKind, TagDecl::TagKind tagKind, std::weak_ptr<DeclContext> context,
					   SourceLocation l, std::shared_ptr<IdentifierInfo> info, SourceLocation tkl)
	: TagDecl(declKind, tagKind, context, l, info, tkl)
{}

bool RecordDecl::isAnonymous() const
{
	return m_isAnonymous;
}

void RecordDecl::setAnonymous(bool isAnonymous)
{
	m_isAnonymous = isAnonymous;
}

bool RecordDecl::hasObjectMember() const
{
	return m_hasObjectMember;
}

void RecordDecl::setHasObjectMember(bool has)
{
	m_hasObjectMember = has;
}

/// @FunctionDecl
FunctionDecl::FunctionDecl(Decl::Kind declKind, std::shared_ptr<DeclContext> context, SourceLocation location,
		DeclName name, std::shared_ptr<QualType> type,
		StorageClass S, bool isInline)
	: DeclaratorDecl(declKind, context, location, name, type), DeclContext(declKind), SClass(S), C99InlineDefinition(isInline), EndRangeLoc(location)
{}

std::weak_ptr<DeclContext> FunctionDecl::getParent()
{
	return getDeclContext();
}

const std::weak_ptr<DeclContext> FunctionDecl::getParent() const
{
	return getDeclContext();
}

void FunctionDecl::setLocEnd(SourceLocation E)
{
	EndRangeLoc = E;
}

std::weak_ptr<Stmt> FunctionDecl::getBody() const
{
	return Body;
}

void FunctionDecl::setBody(std::shared_ptr<Stmt> B)
{
	Body = B;
}

unsigned FunctionDecl::getNumParams() const
{
	return getNumParams();
}

const std::weak_ptr<ParmVarDecl> FunctionDecl::getParamDecl(unsigned i) const
{
	return ParamInfo[i];
}

std::weak_ptr<ParmVarDecl> FunctionDecl::getParamDecl(unsigned i)
{
	return ParamInfo[i];
}

void FunctionDecl::setParams(std::vector<std::shared_ptr<ParmVarDecl>> &NewParamInfo)
{
	ParamInfo = NewParamInfo;
}

FunctionDecl::StorageClass FunctionDecl::getStorageClass() const
{
	return SClass;
}

void FunctionDecl::setStorageClass(FunctionDecl::StorageClass SC)
{
	SClass = SC;
}

bool FunctionDecl::isC99InlineDefinition() const
{
	return C99InlineDefinition;
}

void FunctionDecl::setC99InlineDefinition(bool I)
{
	C99InlineDefinition = I;
}

/// @ParmVarDecl
ParmVarDecl::ParmVarDecl(Decl::Kind declKind, std::shared_ptr<DeclContext> context, SourceLocation location, std::shared_ptr<IdentifierInfo> id, std::shared_ptr<QualType> type, StorageClass S)
	: VarDecl(declKind, context, location, id, type, S)
{}

std::weak_ptr<QualType> ParmVarDecl::getOriginalType() const
{
	return getType();
}