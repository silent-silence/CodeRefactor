#ifndef DECL_H
#define DECL_H

#include "DeclBase.h"
#include "DeclName.h"
#include "AST/Type.h"
#include <vector>
#include <memory>

class QualType;
class Expr;
class Type;
class CompoundStmt;
class SourceRange;
class SourceLocation;
class Parameter;

/// @brief Represent a declaration with a name.
class NamedDecl : public Decl {
public:
	NamedDecl(std::weak_ptr<DeclContext> context, SourceLocation location, DeclName name)
		: Decl(context, location), m_name{name}
	{}
	~NamedDecl() override = default;

	DeclName getDeclarationName() const { return m_name; }
	std::string getIdentifier() const { return m_name.getIdentifier(); }
	std::string getSotredName() const { return m_name.getStoredName(); }

private:
	DeclName m_name;
};

/*/// @brief Represent a declaration of type.
class TypeDecl : public NamedDecl {
public:
	std::weak_ptr<Type> getTypeForDecl() const { return m_typeForDecl; }
	void setTypeForDecl(std::weak_ptr<Type> type) { m_typeForDecl = type; }

private:
	std::weak_ptr<Type> m_typeForDecl;
};*/

/// @brief Represent a declaration of variable.
class VariableDecl : public NamedDecl {
public:
	VariableDecl(std::weak_ptr<DeclContext> context, SourceLocation location, QualType type, DeclName name)
		: NamedDecl(context, location, name), m_type{type}
	{}
	~VariableDecl() override = default;

	QualType getType() const { return m_type; }
	void setType(QualType newType) { m_type = newType; }

	std::weak_ptr<Expr> getDefaultArg() { return m_initializer; }
	const std::weak_ptr<Expr> getDefaultArg() const { return m_initializer; }
	void setDefaultArg(std::weak_ptr<Expr> defaultArg) { m_initializer = defaultArg; }

	bool hasDefaultArg() const { return m_initializer.expired(); }

private:
	QualType m_type;
	std::weak_ptr<Expr> m_initializer;
};

/// @brief Represent a declaration of enum constant, for example, enum X {a, b}, a/b is an EnumConstantDecl.
class EnumConstantDecl : public VariableDecl {
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
};

/// @brief Represent a block of declarations.
class BlockDecl : public Decl, public DeclContext {
public:
	BlockDecl(std::weak_ptr<DeclContext> context, SourceLocation location)
		: Decl(context, location)
	{}
	~BlockDecl() override = default;

	std::weak_ptr<CompoundStmt> getBody() const { return m_body; }
	void setBody(std::weak_ptr<CompoundStmt> body) { m_body = body; }

private:
	std::weak_ptr<CompoundStmt> m_body;
};

/// @brief Represent a function declaration.
class FunctionDecl : public DeclContext, public NamedDecl {
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

	QualType getResultType() const { return m_returnType; };
private:
	QualType m_returnType;
	std::vector<std::weak_ptr<Parameter>> m_parameters;
	std::weak_ptr<CompoundStmt> m_body;
	SourceLocation endRangeLoc;
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

	SourceLocation getRBraceLoc() const;
	void setRBraceLoc(SourceLocation L);
	SourceLocation getTagKeyWordLoc() const;
	void setTageKeyWordLoc(SourceLocation L);
	SourceRange getSourceRange() const;

	bool isDefinition() const { return m_isDefinition; }

	std::string getKindName() const {
		switch (getTagKind()) {
			case TagKind::KindStruct: return "struct";
			case TagKind::KindEnum: return "enum";
			case TagKind::KindUnion: return "union";
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
	QualType m_type;
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
class EnumDecl : public TagDecl {
public:
	~EnumDecl() override = default;

	QualType getIntegerType() const { return m_integerType; }
	void setIntegerType(QualType type) { m_integerType = type; }

private:
	QualType m_integerType;
};
#endif
