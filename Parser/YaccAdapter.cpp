//
// Created by 17271 on 2019/5/20.
//

#include "YaccAdapter.h"
#include "AST/Stmt.h"
#include "AST/Type.h"
#include "AST/ASTContext.h"
#include "Decl/DeclContextHolder.h"
#include "OpenHelper/OpenHelper.h"
#include "AST/Type.h"
#include "Errors/TypeError.hpp"
#include "Decl/DeclGroup.h"
#include <vector>
#include <list>
#include <iostream>

using std::make_shared;					using std::shared_ptr;
using std::vector;						using std::dynamic_pointer_cast;
using std::cerr;						using std::endl;
using std::make_pair;					using std::stack;
using std::list;						using std::string;
using std::pair;

typedef yy::Parser::token::yytokentype token;

YaccAdapter::YaccAdapter(ASTContext &context,  DeclContextHolder &declContext, OpenHelper &source)
	: m_ASTContext{context}, m_declContextHolder{declContext}, m_source{source}
{
	// init context stack with context holder's root
	m_declContextStack.push(m_declContextHolder.getContextRoot());
}

void YaccAdapter::parseDone()
{
	if(!m_stmtStack.empty())
		m_ASTContext.updateASTRoot(m_stmtStack.top());
}

#ifdef ENV_TEST
void YaccAdapter::clean()
{
	while(!m_stmtStack.empty())
		m_stmtStack.pop();
	while(!m_typeStack.empty())
		m_typeStack.pop();
}
#endif

void YaccAdapter::makeDeclStmt(yy::location &l, yy::location &r)
{
	// make DeclGroupRef
	shared_ptr<DeclGroupRef> dg;
	if(m_varDecls.empty())			// There's no variable declared
		dg = nullptr;
	else if(m_varDecls.size() == 1)	// Only one variable declared
		dg = make_shared<DeclGroupRef>(popVarDecl());
	else							// A group of declaration were made
	{
		vector<shared_ptr<Decl>> decls;
		while(!m_varDecls.empty())
			decls.push_back(popVarDecl());
		dg = make_shared<DeclGroupRef>(decls);
	}

	// make decl stmt
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	m_stmtStack.push(
			m_ASTContext.createStmt(Stmt::DeclStmtClass, dg, lp, rp)
	);

	// pop type here
	pop_type();
	// pop corresponding type specifiers
	if(!m_typeSpecifier.empty())
		m_typeSpecifier.pop();
}

void YaccAdapter::makeNullStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_stmtStack.push(
			m_ASTContext.createStmt(Stmt::NullStmtClass, lp)
	);
}

void YaccAdapter::makeCompoundStmt(unsigned stmtNumInBlock, yy::location &l, yy::location &r)
{
	SourceLocation lb = toSourceLocation(l);
	SourceLocation rb = toSourceLocation(r);
	list<shared_ptr<Stmt>> stmts;
	for(auto i = 0; i != stmtNumInBlock; i++){
		stmts.push_back(pop_stmt());
	}
	reverse(stmts.begin(), stmts.end());
	m_stmtStack.push(
			m_ASTContext.createStmt(Stmt::CompoundStmtClass, stmts, lb, rb)
	);
	dynamic_pointer_cast<BlockDecl>(popDeclContext())->setBody(
			dynamic_pointer_cast<CompoundStmt>(m_stmtStack.top())
	);
}

void YaccAdapter::makeCaseStmt(yy::location &l, yy::location &r, yy::location &c)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	SourceLocation cp = toSourceLocation(c);
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::CaseStmtClass,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					lp, rp, cp
			)
	);
}

void YaccAdapter::makeDefaultStmt(yy::location &l, yy::location &r)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::DefaultStmtClass,
					lp, rp
			)
	);
}

void YaccAdapter::makeIfStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::IfStmtClass,
					lp,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					pop_stmt()
			)
	);
}

void YaccAdapter::makeIfElseStmt(yy::location &l, yy::location &r)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::IfStmtClass,
					lp,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					pop_stmt(),
					rp,
					pop_stmt()
			)
	);
}

void YaccAdapter::makeSwitchStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::SwitchStmtClass,
					lp,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					pop_stmt()
			)
	);
}

void YaccAdapter::makeWhileStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::WhileStmtClass,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					pop_stmt(),
					lp
			)
	);
}

void YaccAdapter::makeDoStmt(yy::location &d, yy::location &w, yy::location &l)
{
	SourceLocation dp = toSourceLocation(d);
	SourceLocation wp = toSourceLocation(w);
	SourceLocation lp = toSourceLocation(l);
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::DoStmtClass,
					pop_stmt(),
					dynamic_pointer_cast<Expr>(pop_stmt()),
					dp, wp, lp
			)
	);
}

void YaccAdapter::makeForStmt(yy::location &f, yy::location &l, yy::location &r, bool haveInc)
{
	SourceLocation fp = toSourceLocation(f);
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);

	if (haveInc)
	{
		auto body = pop_stmt();
		auto inc = dynamic_pointer_cast<Expr>(pop_stmt());
		auto cond = pop_stmt();
		auto init = pop_stmt();
		m_stmtStack.push(
				m_ASTContext.createStmt(
						Stmt::ForStmtClass,
						haveInc,
						init,
						cond,
						body,
						fp, lp, rp,
						inc
				)
		);
	}
	else
		m_stmtStack.push(
				m_ASTContext.createStmt(
						Stmt::ForStmtClass,
						haveInc,
						pop_stmt(),
						pop_stmt(),
						pop_stmt(),
						fp, lp, rp
				)
		);
}

void YaccAdapter::makeContinueStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_stmtStack.push(
			m_ASTContext.createStmt(Stmt::ContinueStmtClass, lp)
	);
}

void YaccAdapter::makeBreakStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_stmtStack.push(
			m_ASTContext.createStmt(Stmt::BreakStmtClass, lp)
	);
}

void YaccAdapter::makeReturnStmt(yy::location &l, bool haveExpr)
{
	SourceLocation lp = toSourceLocation(l);
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::ReturnStmtClass,
					lp,
					(haveExpr ? dynamic_pointer_cast<Expr>(pop_stmt()) : nullptr)
			)
	);
}

void YaccAdapter::makeDeclRefExpr(std::string &name, yy::location &l)
{
	shared_ptr<ValueDecl> decl = dynamic_pointer_cast<ValueDecl>(
			m_declContextStack.top()->lookup(name).lock()
	);
	SourceLocation lp = toSourceLocation(l);
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::DeclRefExprClass,
					dynamic_pointer_cast<NamedDecl>(decl),
					decl->getType().lock(),
					lp
			)
	);
}

void YaccAdapter::makeIntegerLiteral(int val, yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	/// @note The type of integer literal should specified by ASTContext
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::IntegerLiteralClass,
					val, lp
			)
	);
}

void YaccAdapter::makeCharacterLiteral(unsigned val, yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	/// @note The type of character literal should specified by ASTContext
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::CharacterLiteralClass,
					val, false, lp
			)
	);
}

void YaccAdapter::makeFloatingLiteral(float val, yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	/// @note The type of floating literal should specified by ASTContext
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::FloatingLiteralClass,
					val, false, lp
			)
	);
}

void YaccAdapter::makeStringLiteral(std::string str, yy::location &location)
{
	SourceLocation l = toSourceLocation(location);
	/// @note The type of string literal should specified by ASTContext
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::StringLiteralClass,
					str, sizeof(str.data()), false, l, str.length()
			)
	);
}

void YaccAdapter::makeParenExpr(yy::location &l, yy::location &r)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::ParenExprClass,
					lp, rp, dynamic_pointer_cast<Expr>(pop_stmt())
			)
	);
}

void YaccAdapter::makeUnaryOperator(int opc, yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	UnaryOperator::Opcode operatorCode;
	switch (opc)
	{
		case token::TOK_POST_INC:	operatorCode = UnaryOperator::PostInc;	break;
		case token::TOK_POST_DEC:	operatorCode = UnaryOperator::PostDec;	break;
		case token::TOK_PRE_INC:	operatorCode = UnaryOperator::PreInc;	break;
		case token::TOK_PRE_DEC:	operatorCode = UnaryOperator::PreDec;	break;
		case '&':					operatorCode = UnaryOperator::AddrOf;	break;
		case '*':					operatorCode = UnaryOperator::Deref;	break;
		case '+':					operatorCode = UnaryOperator::Plus;		break;
		case '-':					operatorCode = UnaryOperator::Minus;	break;
		case '~':					operatorCode = UnaryOperator::Not;		break;
		case '!':					operatorCode = UnaryOperator::LNot;		break;
	}
	/// @note The type of the operation should be calculated by ASTContext
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::UnaryOperatorClass,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					operatorCode, lp
			)
	);
}

void YaccAdapter::makeSizeofExpr(yy::location &l, yy::location &r, bool isSizeofType)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	/// @note The return type of sizeof should be specified by ASTContext
	std::shared_ptr<Stmt> sizeofExpr;
	if(isSizeofType)
	{
		auto type = pop_type();
		sizeofExpr = m_ASTContext.createStmt(
				Stmt::SizeOfAlignOfExprClass,
				lp, rp,
				type
		);
	}
	else
	{
		sizeofExpr = m_ASTContext.createStmt(
				Stmt::SizeOfAlignOfExprClass,
				lp, rp,
				dynamic_pointer_cast<Expr>(pop_stmt())
		);
	}
	m_stmtStack.push(sizeofExpr);
}

void YaccAdapter::makeArraySubscripExpr(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	/// @note The type of array should be specified by ASTContext
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::ArraySubscriptExprClass,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					dynamic_pointer_cast<Expr>(pop_stmt()),
					lp
			)
	);
}

void YaccAdapter::makeCallExpr(unsigned parameterNum, yy::location &l)
{
	// TODO Find member name in DeclContext
	SourceLocation lp = toSourceLocation(l);
	list<shared_ptr<Expr>> exprs;
	for(auto i = 0; i != parameterNum; i++) {
		exprs.push_back(dynamic_pointer_cast<Expr>(pop_stmt()));
	}
	/// @note The return type and parameter number can be found by ASTContext
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::CallExprClass,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					exprs, lp
			)
	);
}

void YaccAdapter::makeMemberExpr(int opc, yy::location &l, std::string &name)
{
	bool isArrow;
	switch(opc)
	{
		case '.':					isArrow = false;	break;
		case token::TOK_POINT_OP:	isArrow = true;		break;
		default:					isArrow = false;
	}

	// get base and decl
	shared_ptr<Expr> base = dynamic_pointer_cast<Expr>(pop_stmt());
	auto baseType = dynamic_pointer_cast<TagType>(base->getType().lock()->getTypePtr());
	if(!baseType)
		throw TypeError("Used member accese on type not a struct or union");
	shared_ptr<NamedDecl> decl = dynamic_pointer_cast<NamedDecl>(
			baseType->getDecl().lock()->lookup(name).lock()
	);
	SourceLocation lp = toSourceLocation(l);
	/// @note The type of member can be found by ASTContext
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::MemberExprClass,
					base,
					isArrow, decl, lp
			)
	);
}

void YaccAdapter::makeCStyleCastExpr(yy::location &l, yy::location &r)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	/// @note Type of expr should be found by ASTContext
	auto exprType = pop_type();
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::CStyleCastExprClass,
					CastExpr::CastKind::CK_Unknown,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					exprType,
					lp, rp
			)
	);
}

void YaccAdapter::makeBinaryOperator(int opc, yy::location &location)
{
	BinaryOperator::Opcode operatorCode;
	switch(opc)
	{
		case token::TOK_OR_OP:				operatorCode = BinaryOperator::LOr;		break;
		case token::TOK_AND_OP:				operatorCode = BinaryOperator::LAnd;	break;
		case '|':							operatorCode = BinaryOperator::Or;		break;
		case '^':							operatorCode = BinaryOperator::Xor;		break;
		case '&':							operatorCode = BinaryOperator::And;		break;
		case token::TOK_EQ_OP:				operatorCode = BinaryOperator::EQ;		break;
		case token::TOK_NE_OP:				operatorCode = BinaryOperator::NE;		break;
		case '<':							operatorCode = BinaryOperator::LT;		break;
		case '>':							operatorCode = BinaryOperator::GT;		break;
		case token::TOK_LE_OP:				operatorCode = BinaryOperator::LE;		break;
		case token::TOK_GE_OP:				operatorCode = BinaryOperator::GE;		break;
		case token::TOK_LEFT_SHIFT_OP:		operatorCode = BinaryOperator::Shl;		break;
		case token::TOK_RIGHT_SHIFT_OP:		operatorCode = BinaryOperator::Shr;		break;
		case '+':							operatorCode = BinaryOperator::Add;		break;
		case '-':							operatorCode = BinaryOperator::Sub;		break;
		case '*':							operatorCode = BinaryOperator::Mul;		break;
		case '/':							operatorCode = BinaryOperator::Div;		break;
		case '%':							operatorCode = BinaryOperator::Rem;		break;
		case ',':							operatorCode = BinaryOperator::Comma;	break;
	}
	SourceLocation l = toSourceLocation(location);
	/// @note Type of the operation should by found by ASTContext
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::BinaryOperatorClass,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					dynamic_pointer_cast<Expr>(pop_stmt()),
					operatorCode, l
			)
	);
}

void YaccAdapter::makeCompoundAssignOperator(int opc, yy::location &location)
{
	CompoundAssignOperator::Opcode operatorCode;
	switch (opc)
	{
		case '=':							operatorCode = BinaryOperator::Assign;		break;
		case token::TOK_MUL_ASSIGN:			operatorCode = BinaryOperator::MulAssign;	break;
		case token::TOK_DIV_ASSIGN:			operatorCode = BinaryOperator::DivAssign;	break;
		case token::TOK_MOD_ASSIGN:			operatorCode = BinaryOperator::RemAssign;	break;
		case token::TOK_ADD_ASSIGN:			operatorCode = BinaryOperator::AddAssign;	break;
		case token::TOK_SUB_ASSIGN:			operatorCode = BinaryOperator::SubAssign;	break;
		case token::TOK_LEFT_SHIFT_ASSIGN:	operatorCode = BinaryOperator::ShlAssign;	break;
		case token::TOK_RIGHT_SHIFT_ASSIGN:	operatorCode = BinaryOperator::ShrAssign;	break;
		case token::TOK_AND_ASSIGN:			operatorCode = BinaryOperator::AndAssign;	break;
		case token::TOK_XOR_ASSIGN:			operatorCode = BinaryOperator::XorAssign;	break;
		case token::TOK_OR_ASSIGN:			operatorCode = BinaryOperator::OrAssign;	break;
	}
	SourceLocation l = toSourceLocation(location);
	/// @note Types should be found by ASTContext
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::CompoundAssignOperatorClass,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					dynamic_pointer_cast<Expr>(pop_stmt()),
					operatorCode, l
			)
	);
}

void YaccAdapter::makeConditionalOperator()
{
	/// @note Type of result should be found by ASTContext
	m_stmtStack.push(
			m_ASTContext.createStmt(
					Stmt::ConditionalOperatorClass,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					dynamic_pointer_cast<Expr>(pop_stmt()),
					dynamic_pointer_cast<Expr>(pop_stmt())
			)
	);
}

void YaccAdapter::makeFunctionNoProtoType()
{
	auto retType = pop_type();
	m_typeSpecifier.pop();

	auto type = m_ASTContext.createType(
			Type::FunctionNoProto,
			retType,
			false
	);
	auto name = m_nameStack.top();
	m_nameStack.pop();
	// Sine a function can be definition or declaration, we can not push the func into decl context now
	m_varDecls.push(
			m_declContextHolder.createFunction(
					m_declContextStack.top(), name.first, name.second, type
			)
	);
	m_typeStack.push(type);
}

void YaccAdapter::makeFunctionProtoType(int paramNum)
{
	// set FunctionProtoType's args type
	vector<shared_ptr<QualType>> argsType;
	for(int i = 1; i < paramNum; ++i)
		argsType.push_back(pop_type());
	auto funDecl = dynamic_pointer_cast<FunctionDecl>(m_declContextStack.top());
	dynamic_pointer_cast<FunctionProtoType>(funDecl->getType().lock()->getTypePtr())->setArgs(argsType);

	// reset current context because we called enterFunctioParamDecl early
	m_declContextStack.pop();
}

void YaccAdapter::addTypeSpecifier(YaccAdapter::TypeSpecifier type)
{
	// init type specifier stack if empty
	if(m_typeSpecifier.empty())
		m_typeSpecifier.push(make_pair(static_cast<unsigned>(0), static_cast<TypeSpecifier>(0)));

	// storage specifier
	switch(type)
	{
		case TYPEDEF:
		case EXTERN:
		case STATIC:
		case AUTO:
		case REGISTER:
			m_typeSpecifier.top().second = type;
			type = TypeSpecifier::STORAGE;
			break;
		default:
			// do nothing
			break;
	}

	// long and long long need special operation
	if ((type == YaccAdapter::TypeSpecifier::LONG) &&
		(m_typeSpecifier.top().first & YaccAdapter::TypeSpecifier::LONGLONG) == 0)
	{
		m_typeSpecifier.top().first += type;
	}
	else if ((type != YaccAdapter::TypeSpecifier::LONG) && (m_typeSpecifier.top().first & type) == 0)
	{
		m_typeSpecifier.top().first |= type;
	}
	else
	{
		// reset specifiers before throw
		m_typeSpecifier.pop();
		throw TypeError("Type specifiers error");
	}
}

void YaccAdapter::makeType()
{
	// pop struct's next specifiers for struct member
	if(m_typeSpecifier.top().first == 0)
		m_typeSpecifier.pop();

	if(!isTypeSpecifierNotIllegal())
	{
		// reset specifiers before throw
		m_typeSpecifier.pop();
		throw TypeError("Type specifiers error");
	}

	// Left builtin type specifiers only
	unsigned types = m_typeSpecifier.top().first & (
			VOID | CHAR | SHORT | INT | FLOAT | DOUBLE | SIGNED | UNSIGNED | LONG | LONGLONG | STRUCT | UNION
	);
	// make which type
	switch(types)
	{
		case UNSIGNED | CHAR:			m_typeStack.push(makeBuiltin(BuiltinType::UChar));		break;
		case UNSIGNED | SHORT:			m_typeStack.push(makeBuiltin(BuiltinType::UShort));		break;
		case UNSIGNED | INT:			m_typeStack.push(makeBuiltin(BuiltinType::UInt));		break;
		case UNSIGNED | LONG:			m_typeStack.push(makeBuiltin(BuiltinType::ULong));		break;
		case UNSIGNED | LONG | INT:		m_typeStack.push(makeBuiltin(BuiltinType::ULong));		break;
		case UNSIGNED | LONGLONG:		m_typeStack.push(makeBuiltin(BuiltinType::ULongLong));	break;
		case UNSIGNED | LONGLONG | INT:	m_typeStack.push(makeBuiltin(BuiltinType::ULongLong));	break;
		case SIGNED | CHAR:				m_typeStack.push(makeBuiltin(BuiltinType::SChar));		break;
		case SIGNED | SHORT:			m_typeStack.push(makeBuiltin(BuiltinType::Short));		break;
		case SIGNED | INT:				m_typeStack.push(makeBuiltin(BuiltinType::Int));		break;
		case SIGNED | LONG:				m_typeStack.push(makeBuiltin(BuiltinType::Long));		break;
		case SIGNED | LONGLONG:			m_typeStack.push(makeBuiltin(BuiltinType::LongLong));	break;
		case SHORT | INT:				m_typeStack.push(makeBuiltin(BuiltinType::Short));		break;
		case LONGLONG | INT:			m_typeStack.push(makeBuiltin(BuiltinType::LongLong));	break;
		case LONG | DOUBLE:				m_typeStack.push(makeBuiltin(BuiltinType::LongDouble));	break;
		case LONG | INT:				m_typeStack.push(makeBuiltin(BuiltinType::Long));		break;
		case VOID:						m_typeStack.push(makeBuiltin(BuiltinType::Void));		break;
		case CHAR:						m_typeStack.push(makeBuiltin(BuiltinType::Char_S));		break;
		case SHORT:						m_typeStack.push(makeBuiltin(BuiltinType::Short));		break;
		case INT:						m_typeStack.push(makeBuiltin(BuiltinType::Int));		break;
		case FLOAT:						m_typeStack.push(makeBuiltin(BuiltinType::Float));		break;
		case DOUBLE:					m_typeStack.push(makeBuiltin(BuiltinType::Double));		break;
		case SIGNED:					m_typeStack.push(makeBuiltin(BuiltinType::Int));		break;
		case UNSIGNED:					m_typeStack.push(makeBuiltin(BuiltinType::UInt));		break;
		case LONG:						m_typeStack.push(makeBuiltin(BuiltinType::Long));		break;
		case LONGLONG:					m_typeStack.push(makeBuiltin(BuiltinType::LongLong));	break;
		case STRUCT:					m_typeStack.push(makeStruct());							break;
		case UNION:
		default:						break;
	}
}

void YaccAdapter::makePointerType()
{
	m_typeStack.push(
			m_ASTContext.createType(Type::Pointer, pop_type())
	);
}

void YaccAdapter::makeConstantArrayType()
{
	auto elementType = pop_type();
	auto arrayType = m_ASTContext.createType(
			Type::Pointer,
			elementType,
			elementType
	);
	// TODO Calculate array size
	m_typeStack.push(
			m_ASTContext.createType(
					Type::ConstantArray,
					elementType,
					arrayType,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					ArrayType::Normal,
					0
			)
	);
}

void YaccAdapter::makeIncompleteArrayType()
{
	auto elementType = pop_type();
	auto arrayType = m_ASTContext.createType(
			Type::Pointer,
			elementType,
			elementType
	);
	m_typeStack.push(
			m_ASTContext.createType(
					Type::IncompleteArray,
					elementType,
					arrayType,
					ArrayType::Normal,
					static_cast<unsigned>(0)
			)
	);
}

void YaccAdapter::enterCompoundBlock(yy::location &l)
{
	m_declContextStack.push(
			m_declContextHolder.createBlock(m_declContextStack.top(), toSourceLocation(l))
	);
}

void YaccAdapter::enterStructBlock(yy::location &l)
{
	m_declContextStack.push(
			m_declContextHolder.createStruct(m_declContextStack.top(), toSourceLocation(l))
	);
	// ready new type specifier env for struct member
	m_typeSpecifier.push(make_pair(static_cast<unsigned>(0), static_cast<TypeSpecifier>(0)));
}

void YaccAdapter::enterFunctionParamDecl()
{
	auto retType = pop_type();
	m_typeSpecifier.pop();

	// temporary a function with no parameter
	auto type = m_ASTContext.createType(
			Type::FunctionProto,
			retType,
			vector<shared_ptr<QualType>>()
	);

	auto name = m_nameStack.top();
	m_nameStack.pop();
	// Sine a function can be definition or declaration, we can not push the func into decl context now
	auto funDecl = m_declContextHolder.createFunction(
			m_declContextStack.top(), name.first, name.second, type
	);
	m_varDecls.push(funDecl);
	m_typeStack.push(type);

	// temporary make the function a context for it's parameters
	m_declContextStack.push(dynamic_pointer_cast<FunctionDecl>(funDecl));
}

void YaccAdapter::enterFunctionBlock()
{
	auto func = dynamic_pointer_cast<FunctionDecl>(m_varDecls.front());
	m_varDecls.pop();
	pop_type();
	m_declContextStack.push(func);
}

void YaccAdapter::storeVariable(std::string name, yy::location &l)
{
	m_nameStack.push(make_pair(name, toSourceLocation(l)));
}

void YaccAdapter::makeVariables()
{
	shared_ptr<QualType> type;
	// Get type for var, except function(Do not pop type here)
	if(!m_nameStack.empty())
		type = m_typeStack.top();

	// all name in stack is the same type
	while(!m_nameStack.empty())
	{
		auto varName = m_nameStack.top();
		m_nameStack.pop();

		if(!m_typeSpecifier.empty() && (m_typeSpecifier.top().first & TYPEDEF))	// if is typedef type
		{
			m_varDecls.push(
					m_declContextHolder.createTypedefDecl(m_declContextStack.top(), type, varName.first, varName.second)
			);
		}
		else										// is normal variable
		{
			m_varDecls.push(
					m_declContextHolder.createVariable(m_declContextStack.top(), varName.first, varName.second, type)
			);
		}
	}
}

void YaccAdapter::makeInStructDeclStmt(yy::location &l, yy::location &r)
{
	makeDeclStmt(l, r);
	// ready new type specifier env for struct member
	m_typeSpecifier.push(make_pair(static_cast<unsigned>(0), static_cast<TypeSpecifier>(0)));
}

void YaccAdapter::makeFunParam()
{
	// get type and reset type specifers
	auto type = m_typeStack.top();
	m_typeSpecifier.top().first = 0;
	m_typeSpecifier.top().second = static_cast<TypeSpecifier>(0);
	// get name if defined
	auto name = m_nameStack.top();
	m_nameStack.pop();
	m_varDecls.push(
			m_declContextHolder.createVariable(m_declContextStack.top(), name.first, name.second, type)
	);
}

void YaccAdapter::makeUnnamedFunParam(yy::location &l)
{
	// get type and reset type specifers
	auto type = m_typeStack.top();
	m_typeSpecifier.top().first = 0;
	m_typeSpecifier.top().second = static_cast<TypeSpecifier>(0);
	// get name if defined
	string nameString = StoredDecl().getDeclAsString();
	SourceLocation location = toSourceLocation(l);
	m_varDecls.push(
			m_declContextHolder.createVariable(m_declContextStack.top(), nameString, location, type)
	);
}

void YaccAdapter::makeFunctionDefinition()
{
	auto func = dynamic_pointer_cast<FunctionDecl>(m_declContextStack.top());
	func->setBody(pop_stmt());
}

bool YaccAdapter::isTypeSpecifierNotIllegal()
{
// define begin
#define updateJumpTable(specifier_type)									\
	if((jumpTable & specifier_type) && (specifiers & specifier_type))	\
	{																	\
		jumpTable = specifier_type##_JMP;								\
		specifiers &= (~specifier_type);								\
	}																	\
	else
// end define

	unsigned specifiers = m_typeSpecifier.top().first;
	unsigned jumpTable = ~static_cast<unsigned>(0);
	while(specifiers != 0)
	{
		updateJumpTable(VOID)
		updateJumpTable(CHAR)
		updateJumpTable(SHORT)
		updateJumpTable(INT)
		updateJumpTable(FLOAT)
		updateJumpTable(DOUBLE)
		updateJumpTable(SIGNED)
		updateJumpTable(UNSIGNED)
		updateJumpTable(LONG)
		updateJumpTable(LONGLONG)
		updateJumpTable(STRUCT)
		updateJumpTable(UNION)
		updateJumpTable(STORAGE)
		return false;
	}

	return true;
}

std::shared_ptr<QualType> YaccAdapter::makeBuiltin(BuiltinType::Kind kind)
{
	QualType::TQ typeQualifier = QualType::None;
	switch(m_typeSpecifier.top().first)
	{
		case CONST:		typeQualifier = QualType::Const;	break;
		case VOLATILE:	typeQualifier = QualType::Volatile;	break;
	}

	return m_ASTContext.createType(Type::Builtin, kind);
}

std::shared_ptr<QualType> YaccAdapter::makeStruct()
{
	// if is a struct decl, pop it
	shared_ptr<RecordDecl> structDecl = dynamic_pointer_cast<RecordDecl>(popDeclContext());

	QualType::TQ typeQualifier = QualType::None;
	switch(m_typeSpecifier.top().first)
	{
		case CONST:		typeQualifier = QualType::Const;	break;
		case VOLATILE:	typeQualifier = QualType::Volatile;	break;
	}

	return m_ASTContext.createType(Type::Record, structDecl);
}

SourceLocation YaccAdapter::toSourceLocation(yy::location &location)
{
	return SourceLocation(m_source.getOpenedName(), location.begin.line, location.begin.column);
}

std::shared_ptr<Stmt> YaccAdapter::pop_stmt()
{
	if(m_stmtStack.empty())
		throw std::range_error("Stmt stack empty");
	std::shared_ptr<Stmt> ptr=m_stmtStack.top();
	m_stmtStack.pop();
	return ptr;
}

std::shared_ptr<QualType> YaccAdapter::pop_type()
{
	if(m_typeStack.empty())
		throw std::range_error("Type stack empty");
	std::shared_ptr<QualType> ptr = m_typeStack.top();
	m_typeStack.pop();
	return ptr;
}

std::shared_ptr<DeclContext> YaccAdapter::popDeclContext()
{
	if(m_declContextStack.empty())
		throw std::range_error("Decl context stack empty");
	std::shared_ptr<DeclContext> ptr = m_declContextStack.top();
	m_declContextStack.pop();
	return ptr;
}

std::shared_ptr<Decl> YaccAdapter::popVarDecl()
{
	if(m_varDecls.empty())
		throw std::range_error("Decl stack empty");
	std::shared_ptr<Decl> ptr = m_varDecls.front();
	m_varDecls.pop();
	return ptr;
}