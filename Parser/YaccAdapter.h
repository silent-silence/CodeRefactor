//
// Created by 17271 on 2019/5/20.
//

#ifndef CODEREFACTOR_YACCADAPTER_H
#define CODEREFACTOR_YACCADAPTER_H

#include <stack>
#include <memory>
#include "parser.hpp"

class Stmt;
class Type;
class SourceLocation;
class ASTContext;
class DeclContextHolder;
class OpenHelper;

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
		/// @brief represent storage class specifiers, which can be appear only one time
		STORAGE	=	0x1000,
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
		STORAGE_JMP	=	VOID | CHAR | SHORT | INT | FLOAT | DOUBLE | SIGNED | UNSIGNED | LONG | LONGLONG
	};

	YaccAdapter(ASTContext &context, DeclContextHolder &declContext, OpenHelper &source);

	/// @brief Call this after parse is done, the method will save root node in ASTContext
	void parseDone();

#ifdef ENV_TEST
	void clean();
#endif

	/// @brief Make statements
	void makeDeclStmt(yy::location &l, yy::location &r, bool haveDefinedName);
	void makeNullStmt(yy::location &l);
	void makeCompoundStmt(unsigned stmtNumInBlock, yy::location &l, yy::location &r);
	void makeCaseStmt(yy::location &l, yy::location &r, yy::location &c);
	void makeDefaultStmt(yy::location &d, yy::location &e);
	void makeLabelStmt(yy::location &l, yy::location &r);
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
	void makeMemberExpr(int opc, yy::location &l);
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
	void addTypeSpecifier(TypeSpecifier type, TypeSpecifier storageSpecifier);

	/// @brief Make types
	//void makeExtQualType(std::vector<var_t> &value);
//    void makeQualifierSet(std::vector<var_t> &value);
	void makeBuiltinType();
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
	void makeFunctionProtoType();
	//void makeTypeOfExprType(std::vector<var_t> &value);
	//void makeDependentTypeOfExprType(std::vector<var_t> &value);
	//void makeTypeOfType(std::vector<var_t> &value);
	//void makeDecltypeType(std::vector<var_t> &value);
	//void makeDependentDecltypeType(std::vector<var_t> &value);

	/// @brief Symbol table
	void storeVariable(std::string name, yy::location &l);
	void makeVariable(std::shared_ptr<Type> type);

private:
	/// @brief Check if type specifier is allowed
	bool isTypeSpecifierNotIllegal();

	SourceLocation toSourceLocation(yy::location &location);

	/// @brief Pop a Stmt/Type from the stack
	std::shared_ptr<Stmt> pop_stmt();
	std::shared_ptr<Type> pop_type();

	ASTContext &m_ASTContext;
	DeclContextHolder &m_declContextHolder;
	OpenHelper &m_source;

	/// @brief Temporary save the storage class specifiers(typedef/extern/static/register)
	TypeSpecifier m_storageSpecifier;
	/// @brief Temporary save the basic type of specifiers(int/long/double...)
	unsigned m_typeSpecifier;

	/// @brief The stack of Stmt/Type
	std::stack<std::shared_ptr<Stmt>> m_stmtStack;
	std::stack<std::shared_ptr<Type>> m_typeStack;
	std::stack<std::pair<std::string, SourceLocation>> m_nameStack;

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
