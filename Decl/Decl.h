#ifndef DECL_H
#define DECL_H

#include "DeclBase.h"
#include "DeclName.h"
#include <vector>
#include <memory>

class QualType;
class Expr;
class Type;
class Stmt;
class CompoundStmt;
class SourceRange;
class SourceLocation;
class Parameter;

class FunctionDecl;
class ParmVarDecl;

/*class DeclaratorInfo {

};*/

/// @brief The top decl context
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
	~NamedDecl() override = 0;

	DeclName getDeclarationName() const;
	std::string getNameAsString() const;
	std::weak_ptr<IdentifierInfo> getIdentifier() const;

private:
	DeclName m_name;
};

/// @abstract
/// @brief Represent a variable/function/enum constant
class ValueDecl : public NamedDecl {
public:
	ValueDecl(Kind declKind, std::weak_ptr<DeclContext> context, SourceLocation location,
			DeclName name,
			std::shared_ptr<QualType> type);
	~ValueDecl() override = 0;

	std::weak_ptr<QualType> getType() const;
	void setType(std::shared_ptr<QualType> newType);

private:
	std::shared_ptr<QualType> m_declType;
};

/// @abstract
/// @brief Represent ValueDecl declared with declarator
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

	void setInitExpr(std::shared_ptr<Expr> init);
	std::weak_ptr<Expr> getInitExpr() const;

private:
	std::shared_ptr<Expr> m_init;
	StorageClass SClass;
	bool DeclaredInCondition ;
};

/// @brief Represent a declaration of enum constant, for example, enum X {a, b}, a/b is an EnumConstantDecl.
class EnumConstantDecl : public ValueDecl {
public:
	EnumConstantDecl(std::weak_ptr<DeclContext> context, SourceLocation location,
					 DeclName name,
					 std::shared_ptr<QualType> type,
					 std::shared_ptr<Expr> init);
	~EnumConstantDecl() override = default;

	std::weak_ptr<Expr> getInitExpr();
	const std::weak_ptr<Expr> getInitExpr() const;

	const int getInitValue() const;

	void setInitExpr(std::weak_ptr<Expr> e);
	void setInitValue(const int value);

private:
	std::shared_ptr<Expr> m_init;
	int m_value;
};

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

/// @brief Represent a declaration of type.
class TypeDecl : public NamedDecl {
public:
	TypeDecl(Kind kind, std::weak_ptr<DeclContext> context, SourceLocation l,
			std::shared_ptr<IdentifierInfo> info);
	~TypeDecl() override = 0;

	std::weak_ptr<QualType> getTypeForDecl() const;
	void setTypeForDecl(std::shared_ptr<QualType> type);

private:
	std::shared_ptr<QualType> m_typeForDecl;
};

/// @brief Represent a typedef type
class TypedefDecl : public TypeDecl {
	friend class DeclContextHolder;
public:
	TypedefDecl(std::weak_ptr<DeclContext> context, SourceLocation l, std::shared_ptr<IdentifierInfo> info,
			std::shared_ptr<QualType> type);
	~TypedefDecl() override = default;
};

/// @brief Represent a declaration of a struct/enum/union
class TagDecl : public TypeDecl, public DeclContext {
public:
	enum class TagKind {
		KindStruct,
		KindEnum,
		KindUnion
	};

	TagDecl(Kind declKind, TagKind tagKind, std::weak_ptr<DeclContext> context,
			SourceLocation location, std::shared_ptr<IdentifierInfo> info,
			SourceLocation tkl);
	~TagDecl() override = 0;

	std::weak_ptr<DeclContext> getParent() override;
	const std::weak_ptr<DeclContext> getParent() const override;

	bool isDefinition() const;

	std::string getKindName() const;
	TagKind getTagKind() const;
	void setTagKind(TagKind kind);
	bool isStruct() const;
	bool isUnion() const;
	bool isEnum() const;

	std::weak_ptr<TypedefDecl> getTypedefForAnonDecl() const;
	void setTypedefForAnonDecl(std::shared_ptr<TypedefDecl> TDD);

private:
	TagKind m_kind;
	/// @brief Is definition or declaration
	bool m_isDefinition;
	std::shared_ptr<TypedefDecl> m_typedefForAnonDecl;
	SourceLocation TagKeyWordLoc;
	SourceLocation RBraceLoc;
};

/// @brief Represent a struct/union.
class RecordDecl : public TagDecl {
public:
	RecordDecl(Kind declKind, TagKind tagKind, std::weak_ptr<DeclContext> context,
			   SourceLocation l, std::shared_ptr<IdentifierInfo> info,
			   SourceLocation tkl = SourceLocation());
	~RecordDecl() override = default;

	bool isAnonymous() const;
	void setAnonymous(bool isAnonymous);

	bool hasObjectMember() const;
	void setHasObjectMember(bool has);

private:
	bool m_isAnonymous;
	bool m_hasObjectMember;
};

/// @brief Represent an enum.
class EnumDecl : public TagDecl {
public:
	EnumDecl(std::weak_ptr<DeclContext> context,
			SourceLocation l, std::shared_ptr<IdentifierInfo> info, SourceLocation tkl = SourceLocation());
	~EnumDecl() override = default;

	std::shared_ptr<QualType> getIntegerType() const;
	void setIntegerType(std::shared_ptr<QualType> type);

private:
	std::shared_ptr<QualType> m_integerType;
};

/// @brief Represent a function declaration.
class FunctionDecl : public DeclaratorDecl, public DeclContext
{
public:
    enum class StorageClass {
        None, Extern, Static, PrivateExtern
    };
    FunctionDecl(Kind declKind, std::shared_ptr<DeclContext> context, SourceLocation location,
                 DeclName name, std::shared_ptr<QualType> type,
                 StorageClass S, bool isInline);
    ~FunctionDecl() override = default;
    /*static std::shared_ptr<FunctionDecl> Create(DeclContext *DC, SourceLocation L,
                                                DeclName N, std::shared_ptr<QualType> T,
                                                StorageClass S = None, bool isInline = false,
                                                bool hasWrittenPrototype = true);*/

	std::weak_ptr<DeclContext> getParent() override;
	const std::weak_ptr<DeclContext> getParent() const override;

    void setLocEnd(SourceLocation E);

    /*std::weak_ptr<Stmt> getBody(const std::weak_ptr<FunctionDecl> &Definition) const;*/
    std::weak_ptr<Stmt> getBody() const;

    /*std::weak_ptr<Stmt> getBodyIfAvailable() const;*/
    void setBody(std::shared_ptr<Stmt> B);

    /*unsigned getNumParmVarDeclsFromType() const;*/
    unsigned getNumParams() const;

    const std::weak_ptr<ParmVarDecl> getParamDecl(unsigned i) const;
    std::weak_ptr<ParmVarDecl> getParamDecl(unsigned i);
    void setParams(std::vector<std::shared_ptr<ParmVarDecl>> &NewParamInfo);
    void addArg(std::shared_ptr<ParmVarDecl> newParam);

	FunctionDecl::StorageClass getStorageClass() const;
    void setStorageClass(FunctionDecl::StorageClass SC);

    bool isC99InlineDefinition() const;
    void setC99InlineDefinition(bool I);

private:
	std::vector<std::shared_ptr<ParmVarDecl>> ParamInfo;
	std::shared_ptr<Stmt> Body;
	StorageClass SClass;
	/*bool IsInline : 1;*/
	bool C99InlineDefinition;
	/*bool IsVirtualAsWritten : 1;
	bool IsPure : 1;
	bool HasInheritedPrototype : 1;
	bool HasWrittenPrototype : 1;
	bool IsDeleted : 1;
	bool IsTrivial : 1; // sunk from CXXMethodDecl
	bool IsCopyAssignment : 1;  // sunk from CXXMethodDecl
	bool HasImplicitReturnZero : 1;*/
	SourceLocation EndRangeLoc;

	/*std::shared_ptr<FunctionDecl> TemplateOrSpecialization;*/
};

class ParmVarDecl : public VarDecl {
public:
    ParmVarDecl(Kind DK, std::shared_ptr<DeclContext> DC, SourceLocation L,
                std::shared_ptr<IdentifierInfo> Id, std::shared_ptr<QualType> T,
                StorageClass S);
    /*static std::shared_ptr<ParmVarDecl> Create(std::shared_ptr<DeclContext> DC,
                               SourceLocation L,std::shared_ptr<IdentifierInfo> Id,
                               std::shared_ptr<QualType> T,
                               StorageClass S, std::shared_ptr<Expr> DefArg);*/
    bool paramHasName() const;
    std::weak_ptr<QualType> getOriginalType() const;
};

#endif
