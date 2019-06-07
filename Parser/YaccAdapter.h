//
// Created by 17271 on 2019/5/20.
//

#ifndef CODEREFACTOR_YACCADAPTER_H
#define CODEREFACTOR_YACCADAPTER_H

#include <stack>
#include <memory>
#include <queue>
#include "AST/Type.h"
#include "parser.hpp"

class Stmt;
class QualType;
class SourceLocation;
class ASTContext;
class DeclContextHolder;
class OpenHelper;
class Decl;
class DeclContext;

/// @brief An Adapter which is called by yacc rules, and create AST via ASTContext/DeclContextHolder interface.
class YaccAdapter {
public:
	/// @brief Each enumerator hold one bit, represents a basic type specifier
	enum TypeSpecifier : unsigned {
		VOID	=	0x0001,
		CHAR	=	0x0002,
		SHORT	=	0x0004,
		INT		=	0x0008,
		FLOAT	=	0x0010,
		DOUBLE	=	0x0020,
		SIGNED	=	0x0040,
		UNSIGNED=	0x0080,
		LONG	=	0x0100,
		/// @brief An exception is long long, which can be specified as two long
		LONGLONG=	0x0200,
		STRUCT	=	0x0400,
		UNION	=	0x0800,
		ENUM	=	0x1000,
		CONST	=	0x2000,
		VOLATILE=	0x4000,
		/// @brief represent storage class specifiers, which can be appear only one time
		STORAGE	=	0x8000,
		TYPEDEF, EXTERN, STATIC, AUTO, REGISTER
	};

	/// @brief This enumerator hold if a basic type can be specified by another type
	enum TypeJumpTable : unsigned {
		VOID_JMP	=	0,
		CHAR_JMP	=	SIGNED | UNSIGNED | STORAGE,
		SHORT_JMP	=	INT | SIGNED | UNSIGNED | STORAGE,
		INT_JMP		=	SHORT | SIGNED | UNSIGNED | LONG | LONGLONG | STORAGE,
		FLOAT_JMP	=	STORAGE,
		DOUBLE_JMP	=	LONG | STORAGE,
		SIGNED_JMP	=	CHAR | SHORT | INT | LONG | LONGLONG | STORAGE,
		UNSIGNED_JMP=	CHAR | SHORT | INT | LONG | LONGLONG | STORAGE,
		LONG_JMP	=	INT | DOUBLE | UNSIGNED | SIGNED | LONG | STORAGE,
		LONGLONG_JMP=	INT  | UNSIGNED | SIGNED | STORAGE,
		STRUCT_JMP	=	STORAGE | CONST | VOLATILE,
		UNION_JMP	=	STORAGE | CONST | VOLATILE,
		ENUM_JMP	=	STORAGE | CONST | VOLATILE,
		STORAGE_JMP	=	VOID | CHAR | SHORT | INT | FLOAT | DOUBLE | SIGNED | UNSIGNED | LONG | LONGLONG
	};

	YaccAdapter(ASTContext &context, DeclContextHolder &declContext, OpenHelper &source);

	/// @brief Call this after parse is done, the method will save root node in ASTContext
	void parseDone();

#ifdef ENV_TEST
	void clean();
#endif

	/// @brief Make statements
	void makeDeclStmt(yy::location &l, yy::location &r);
	void makeNullStmt(yy::location &l);
	void makeCompoundStmt(unsigned stmtNumInBlock, yy::location &l, yy::location &r);
	void makeCaseStmt(yy::location &l, yy::location &r, yy::location &c);
	void makeDefaultStmt(yy::location &d, yy::location &e);
	void makeLabelStmt(yy::location &l);
	void makeIfStmt(yy::location &l);
	void makeIfElseStmt(yy::location &lf, yy::location &le);
	void makeSwitchStmt(yy::location &l);
	void makeWhileStmt(yy::location &l);
	void makeDoStmt(yy::location &d, yy::location &w, yy::location &l);
	void makeForStmt(yy::location &f, yy::location &l, yy::location &r, bool haveInc);
	void makeGotoStmt(yy::location &l, yy::location &r);
	void makeIndirectGotoStmt();
	void makeContinueStmt(yy::location &l);
	void makeBreakStmt(yy::location &l);
	void makeReturnStmt(yy::location &l, bool haveExpr = false);
    void makeCommentStmt(std::string &comment, yy::location &l);

	/// @brief Make expressions
	//void makeStmtExpr(yy::location &l, yy::location &r);
	void makeDeclRefExpr(std::string &name, yy::location &l);
	void makePredefinedExpr();
	void makeIntegerLiteral(int val, yy::location &l);
	void makeCharacterLiteral(unsigned val, yy::location &l);
	void makeFloatingLiteral(float val, yy::location &l);
	void makeImaginaryLiteral();
	void makeStringLiteral(std::string str, yy::location &location);
	void makeParenExpr(yy::location &l, yy::location &r);
	void makeUnaryOperator(int opc, yy::location &l);
	void makeSizeofExpr(yy::location &l, yy::location &r, bool isSizeofType);
	void makeArraySubscripExpr(yy::location &l);
	void makeCallExpr(unsigned parameterNum, yy::location &l);
	void makeMemberExpr(int opc, yy::location &l, std::string &name);
	void makeCompoundLiteralExpr();
	void makeImplicitCastExpr();
	void makeCStyleCastExpr(yy::location &l, yy::location &r);
	void makeBinaryOperator(int opc, yy::location &location);
	void makeCompoundAssignOperator(int opc, yy::location &location);
	void makeConditionalOperator();
	void makeVAArgExpr();
	void makeInitListExpr();
	void makeParenListExpr();

	/// @brief Temporary save type specifier before create a type
	void addTypeSpecifier(TypeSpecifier type);

	/// @brief Make types
	//void makeExtQualType(std::vector<var_t> &value);
//    void makeQualifierSet(std::vector<var_t> &value);
	void makeType();
	void makeFixedWidthIntType();
	void makeComplexType();
	void makePointerType();
	void makeBlockPointerType();
	//void makeReferenceType(std::vector<var_t> &value);//
	//void makeLValueReferenceType(std::vector<var_t> &value);
	//void makeRValueReferenceType(std::vector<var_t> &value);
	//void makeMemberPointerType(std::vector<var_t> &value);
	void makeConstantArrayType();//
	void makeConstantArrayWithExprType();//
	void makeConstantArrayWithoutExprType();//
	void makeIncompleteArrayType();//
	//void makeVariableArrayType(std::vector<var_t> &value);//
	//void makeDependentSizedArrayType(std::vector<var_t> &value);//
	//void makeVectorType(std::vector<var_t> &value);
	//void makeExtVectorType(std::vector<var_t> &value);
	void makeFunctionNoProtoType();
	void makeFunctionProtoType(int paramNum);
	//void makeTypeOfExprType(std::vector<var_t> &value);
	//void makeDependentTypeOfExprType(std::vector<var_t> &value);
	//void makeTypeOfType(std::vector<var_t> &value);
	//void makeDecltypeType(std::vector<var_t> &value);
	//void makeDependentDecltypeType(std::vector<var_t> &value);

	/// @brief Symbol table
	/// @brief Call this meeting a '{', will create a context(block/struct/enum)
	void enterCompoundBlock(yy::location &l);
	void enterStructBlock(yy::location &l);
	void enterFunctionParamDecl();
	void enterFunctionBlock();
	/// @brief For enum decl, named or unnamed
	void makeEnumContext(yy::location &location);
	void makeEnumContext(std::string &name, yy::location &location);

	/// @brief Temporary save the variable name
	void storeVariable(std::string name, yy::location &l);
	/// @brief Pop a name and make variable/typedef decl
	void makeVariables(bool hasInit);
	void makeInStructDeclStmt(yy::location &l, yy::location &r);
	void makeFunParam();
	void makeUnnamedFunParam(yy::location &l);
	void makeFunctionDefinition();
	void makeEnumConstant(std::string &name, yy::location &l, bool hasInit);

private:
	/// @brief A method to create build in type
	std::shared_ptr<QualType> makeBuiltin(BuiltinType::Kind kind);
	std::shared_ptr<QualType> makeStruct();
	std::shared_ptr<QualType> makeEnum();

	/// @brief Check if type specifier is allowed
	bool isTypeSpecifierNotIllegal();

	/// @brief Get CVR qualifier, if empty, return QualType::None
	QualType::TQ getTypeCVRQualifier() const;

	SourceLocation toSourceLocation(yy::location &location);

	/// @brief Pop a Stmt/Type from the stack

	ASTContext &m_ASTContext;
	DeclContextHolder &m_declContextHolder;
	OpenHelper &m_source;

	/// @brief The stack of Stmt
	std::shared_ptr<Stmt> pop_stmt();
	std::stack<std::shared_ptr<Stmt>> m_stmtStack;

	/// @brief The stack of Type
	std::shared_ptr<QualType> pop_type();
	std::stack<std::shared_ptr<QualType>> m_typeStack;

	/// @brief The stack of DeclContext
	std::shared_ptr<DeclContext> popDeclContext();
	std::stack<std::shared_ptr<DeclContext>> m_declContextStack;

	/// @brief first: Temporary save the basic type of specifiers(int/long/double...)
	/// @brief seconde: Temporary save the storage class specifiers(typedef/extern/static/register)
	std::stack<std::pair<unsigned, TypeSpecifier>> m_typeSpecifier;

	/// @brief Temporary save the variable name and it's location
	// TODO: this can be just one 'pair', not a 'stack'
	std::stack<std::pair<std::string, SourceLocation>> m_nameStack;
	/// @brief Temporary save the variable decls
	std::shared_ptr<Decl> popVarDecl();
	std::queue<std::shared_ptr<Decl>> m_varDecls;

	/*template<typename T=Stmt>
	std::shared_ptr<T> pop_stmt(){
		if(stmtQueue.empty())
			throw std::range_error("Queue empty");
		std::shared_ptr<Stmt> ptr=stmtQueue.top();
		stmtQueue.pop();
		return std::dynamic_pointer_cast<T>(ptr);
	}
	template<typename T=Type>
	std::shared_ptr<T> pop_type(){
		if(queue_type.empty())
			return nullptr;
		void ptr=typeQueue.top();
		typeQueue.pop();
		return std::dynamic_pointer_cast<T>(ptr);
	}*/
};

#endif //CODEREFACTOR_YACCADAPTER_H
