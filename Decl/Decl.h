#ifndef DECL_H
#define DECL_H

#include "DeclBase.h"
#include "DeclName.h"
#include <vector>
#include <memory>

class QualType;
class Expr;
class Type;
class CompoundStmt;
class SourceRange;
class SourceLocation;
class Parameter;

/*class DeclaratorInfo {

};*/

class TranslationUnitDecl : public Decl, public DeclContext {
public:
	TranslationUnitDecl();
	~TranslationUnitDecl() override = default;

	std::weak_ptr<DeclContext> getParent() override;
	const std::weak_ptr<DeclContext> getParent() const override;
};

/// @brief Represent a declaration with a name.
class NamedDecl : public Decl {
public:
	NamedDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
			DeclName name);
	~NamedDecl() override = default;

	DeclName getDeclarationName() const { return m_name; }
	std::string getNameAsString() const { return m_name.getAsString(); }
	std::weak_ptr<IdentifierInfo> getIdentifier() const { return m_name.getAsIdentifierInfo(); }

private:
	DeclName m_name;
};

class ValueDecl : public NamedDecl {
public:
	ValueDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
			DeclName name,
			std::shared_ptr<QualType> type);
	~ValueDecl() override = 0;

	std::weak_ptr<QualType> getType() { return m_declType; }
	void setType(std::shared_ptr<QualType> newType) { m_declType = newType; }

private:
	std::shared_ptr<QualType> m_declType;
};

class DeclaratorDecl : public ValueDecl {
public:
	DeclaratorDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
			DeclName name,
			std::shared_ptr<QualType> type/*,
			std::shared_ptr<DeclaratorInfo> info*/);
	~DeclaratorDecl() override = 0;

private:
	/*std::shared_ptr<DeclaratorInfo> m_declInfo;*/
};

/// @brief Represent a declaration of variable.
class VarDecl : public DeclaratorDecl {
public:
	enum class StorageClass {
		None, Auto, Register, Extern, Static, PrivateExtern
	};

	VarDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
			std::shared_ptr<IdentifierInfo> id,
			std::shared_ptr<QualType> type,
			/*std::shared_ptr<DeclaratorInfo> info,*/
			StorageClass sc);
	VarDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
			DeclName name,
			std::shared_ptr<QualType> type,
			/*std::shared_ptr<DeclaratorInfo> info,*/
			StorageClass sc);

	~VarDecl() override = default;

private:
	//TODO: save initializer
	StorageClass SClass;
	bool DeclaredInCondition ;
};

/// @brief Represent a declaration of enum constant, for example, enum X {a, b}, a/b is an EnumConstantDecl.
/*class EnumConstantDecl : public VarDecl {
public:
	~EnumConstantDecl() override = default;

	std::weak_ptr<Expr> getInitExpr() { return m_init; }
	const std::weak_ptr<Expr> getInitExpr() const { return m_init; }

	const int getInitValue() const { return m_value; }

	void setInitExpr(std::weak_ptr<Expr> e) { m_init = e; }
	void setInitValue(const int value) { m_value = value; }

private:
	std::weak_ptr<Expr> m_init;
	int m_value;
};*/

/// @brief Represent a block of declarations.
class BlockDecl : public Decl, public DeclContext {
public:
	BlockDecl(std::weak_ptr<DeclContext> context, SourceLocation location);
	~BlockDecl() override = default;


	std::weak_ptr<DeclContext> getParent() override;
	const std::weak_ptr<DeclContext> getParent() const override;

	std::weak_ptr<CompoundStmt> getBody() const;
	void setBody(std::weak_ptr<CompoundStmt> body);

private:
	std::weak_ptr<CompoundStmt> m_body;
};

/// @brief Represent a function declaration.
/*class FunctionDecl : public DeclContext, public NamedDecl {
public:
	~FunctionDecl() override = default;

	SourceRange getSourceRange() const;
	void setLocEnd(SourceLocation E);

	std::weak_ptr<CompoundStmt> getBody() const { return m_body; }
	void setBody(std::weak_ptr<CompoundStmt> body) { m_body = body; }

	unsigned getNumParams() const { return m_parameters.size(); }
	const std::weak_ptr<Parameter> getParamDecl(unsigned i) const { return m_parameters[i]; }
	std::weak_ptr<Parameter> getParamDecl(unsigned i) { return m_parameters[i]; }
	void setParams(std::vector<std::weak_ptr<Parameter>> parameter) { m_parameters = parameter; }

	std::shared_ptr<QualType> getResultType() const { return m_returnType; };
private:
	std::shared_ptr<QualType> m_returnType;
	std::vector<std::weak_ptr<Parameter>> m_parameters;
	std::weak_ptr<CompoundStmt> m_body;
	SourceLocation endRangeLoc;
};*/

/// @brief Represent a declaration of type.
class TypeDecl : public NamedDecl {
public:
	TypeDecl(Kind kind, std::weak_ptr<DeclContext> context, SourceLocation l, std::shared_ptr<IdentifierInfo> info)
		: NamedDecl{kind, context, l, info}, m_typeForDecl{nullptr}
	{}

	std::weak_ptr<Type> getTypeForDecl() const { return m_typeForDecl; }
	void setTypeForDecl(std::shared_ptr<Type> type) { m_typeForDecl = type; }

private:
	std::shared_ptr<Type> m_typeForDecl;
};

class TypedefDecl : public TypeDecl {
public:
	TypedefDecl(std::weak_ptr<DeclContext> context, SourceLocation l, std::shared_ptr<IdentifierInfo> info, std::shared_ptr<QualType> type)
		: TypeDecl{Typedef, context, l, info}, underlyingType{type}
	{}

private:
	std::shared_ptr<QualType> underlyingType;
};

/// @brief Represent a declaration of a struct/enum/union
class TagDecl : public NamedDecl, public DeclContext {
public:
	~TagDecl() override = default;

	enum class TagKind {
		KindStruct,
		KindEnum,
		KindUnion
	};

	bool isDefinition() const { return m_isDefinition; }

	std::string getKindName() const {
		switch (getTagKind()) {
			case TagKind::KindStruct:	return "struct";
			case TagKind::KindEnum:		return "enum";
			case TagKind::KindUnion:	return "union";
		}
	}
	TagKind getTagKind() const { return m_kind; }
	void setTagKind(TagKind kind) { m_kind = kind; }
	bool isStruct() const { return m_kind == TagKind::KindStruct; }
	bool isUnion() const { return m_kind == TagKind::KindUnion; }
	bool isEnum() const { return m_kind == TagKind::KindEnum; }

private:
	TagKind m_kind;
	bool m_isDefinition;
	std::shared_ptr<QualType> m_type;
	SourceLocation TagKeyWordLoc;
	SourceLocation RBraceLoc;
};

/// @brief Represent a struct/union.
class RecordDecl : public TagDecl {
public:
	~RecordDecl() override = default;

	bool isAnonymous() const { return m_isAnonymous; }
	void setAnonymous(bool isAnonymous) { m_isAnonymous = isAnonymous; }

	bool hasObjectMember() const { return m_hasObjectMember; }
	void setHasObjectMember(bool has) { m_hasObjectMember = has; }
private:
	bool m_isAnonymous;
	bool m_hasObjectMember;
};

/// @brief Represent an enum.
/*class EnumDecl : public TagDecl {
public:
	~EnumDecl() override = default;

	std::shared_ptr<QualType> getIntegerType() const { return m_integerType; }
	void setIntegerType(std::shared_ptr<QualType> type) { m_integerType = type; }

private:
	std::shared_ptr<QualType> m_integerType;
};*/

#endif
