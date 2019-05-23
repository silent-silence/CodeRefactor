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
#include <vector>

using std::make_shared;					using std::shared_ptr;
using std::vector;						using std::dynamic_pointer_cast;

typedef yy::Parser::token::yytokentype token;

YaccAdapter::YaccAdapter(ASTContext &context,  DeclContextHolder &declContext, OpenHelper &source)
	: m_ASTContext{context}, m_declContextHolder{declContext}, m_source{source}, m_typeSpecifier{0}
{}

void YaccAdapter::parseDone()
{
	if(!m_stmtQueue.empty())
		m_ASTContext.updateASTRoot(m_stmtQueue.top());
}

#ifdef ENV_TEST
void YaccAdapter::clean()
{
	while(!m_stmtQueue.empty())
		m_stmtQueue.pop();
	while(!m_typeQueue.empty())
		m_typeQueue.pop();
}
#endif

void YaccAdapter::makeDeclStmt(yy::location &l, yy::location &r)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	m_stmtQueue.push(
			m_ASTContext.createStmt(Stmt::DeclStmtClass, lp, rp)
	);
}
void YaccAdapter::makeNullStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_stmtQueue.push(
			m_ASTContext.createStmt(Stmt::NullStmtClass, lp)
	);
}

void YaccAdapter::makeCompoundStmt(unsigned stmtNumInBlock, yy::location &l, yy::location &r)
{
	SourceLocation lb = toSourceLocation(l);
	SourceLocation rb = toSourceLocation(r);
	vector<shared_ptr<Stmt>> stmts;
	for(auto i = 0; i != stmtNumInBlock; i++){
		stmts.push_back(pop_stmt());
	}
	reverse(stmts.begin(), stmts.end());
	m_stmtQueue.push(
			m_ASTContext.createStmt(Stmt::CompoundStmtClass, stmts, lb, rb)
	);
}

void YaccAdapter::makeCaseStmt(yy::location &l, yy::location &r, yy::location &c)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	SourceLocation cp = toSourceLocation(c);
	m_stmtQueue.push(
			m_ASTContext.createStmt(
					Stmt::CaseStmtClass,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					dynamic_pointer_cast<Expr>(pop_stmt()),
					lp, rp, cp
			)
	);
}

void YaccAdapter::makeDefaultStmt(yy::location &l, yy::location &r)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	m_stmtQueue.push(
			m_ASTContext.createStmt(
					Stmt::DefaultStmtClass,
					lp, rp,
					pop_stmt()
			)
	);
}

void YaccAdapter::makeSwitchStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_stmtQueue.push(
			m_ASTContext.createStmt(
					Stmt::SwitchStmtClass,
					lp,
					dynamic_pointer_cast<Expr>(pop_stmt())
			)
	);
}

void YaccAdapter::makeWhileStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_stmtQueue.push(
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
	m_stmtQueue.push(
			m_ASTContext.createStmt(
					Stmt::DoStmtClass,
					pop_stmt(),
					dynamic_pointer_cast<Expr>(pop_stmt()),
					dp, wp, lp
			)
	);
}

void YaccAdapter::makeContinueStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_stmtQueue.push(
			m_ASTContext.createStmt(Stmt::ContinueStmtClass, lp)
	);
}

void YaccAdapter::makeBreakStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_stmtQueue.push(
			m_ASTContext.createStmt(Stmt::BreakStmtClass, lp)
	);
}

void YaccAdapter::makeReturnStmt(yy::location &l, bool haveExpr)
{
	SourceLocation lp = toSourceLocation(l);
	m_stmtQueue.push(
			m_ASTContext.createStmt(
					Stmt::ReturnStmtClass,
					lp,
					(haveExpr ? dynamic_pointer_cast<Expr>(pop_stmt()) : nullptr)
			)
	);
}

void YaccAdapter::makeIntegerLiteral(int val, yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	/// @note The type of integer literal should specified by ASTContext
	m_stmtQueue.push(
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
	m_stmtQueue.push(
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
	m_stmtQueue.push(
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
	m_stmtQueue.push(
			m_ASTContext.createStmt(
					Stmt::StringLiteralClass,
					str.data(), sizeof(str.data()), false, l, str.length()
			)
	);
}

void YaccAdapter::makeParenExpr(yy::location &l, yy::location &r)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	m_stmtQueue.push(
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
	m_stmtQueue.push(
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
		sizeofExpr = m_ASTContext.createStmt(
				Stmt::SizeOfAlignOfExprClass,
				lp, rp,
				pop_type()
		);
	}
	else
		sizeofExpr = m_ASTContext.createStmt(
				Stmt::SizeOfAlignOfExprClass,
				lp, rp, dynamic_pointer_cast<Expr>(pop_stmt())
		);
	m_stmtQueue.push(sizeofExpr);
}

void YaccAdapter::makeArraySubscripExpr(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	/// @note The type of array should be specified by ASTContext
	m_stmtQueue.push(
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
	SourceLocation lp = toSourceLocation(l);
	vector<shared_ptr<Expr>> exprs;
	for(auto i = 0; i != parameterNum; i++) {
		exprs.push_back(dynamic_pointer_cast<Expr>(pop_stmt()));
	}
	/// @note The return type and parameter number can be found by ASTContext
	m_stmtQueue.push(
			m_ASTContext.createStmt(
					Stmt::CallExprClass,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					exprs, lp
			)
	);
}

void YaccAdapter::makeMemberExpr(int opc, yy::location &l)
{
	bool isArrow;
	switch(opc)
	{
		case '.':					isArrow = false;	break;
		case token::TOK_POINT_OP:	isArrow = true;		break;
	}
	SourceLocation lp = toSourceLocation(l);
	/// @note The type of member can be found by ASTContext
	// TODO find NamedDecl
	shared_ptr<NamedDecl> decl;
	m_stmtQueue.push(
			m_ASTContext.createStmt(
					Stmt::MemberExprClass,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					isArrow, decl, lp
			)
	);
}

void YaccAdapter::makeCStyleCastExpr(yy::location &l, yy::location &r)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	/// @note Type of expr should be found by ASTContext
	m_stmtQueue.push(
			m_ASTContext.createStmt(
					Stmt::CStyleCastExprClass,
					CastExpr::CastKind::CK_Unknown,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					pop_type(),
					lp, rp
			)
	);
}

void YaccAdapter::makeBinaryOperator(int opc, yy::location &location)
{
	BinaryOperator::Opcode operatorCode;
	switch(opc)
	{
		case token::TOK_OR_OP:				operatorCode = BinaryOperator::LOr;			break;
		case token::TOK_AND_OP:				operatorCode = BinaryOperator::LAnd;		break;
		case '|':							operatorCode = BinaryOperator::Or;			break;
		case '^':							operatorCode = BinaryOperator::Xor;			break;
		case '&':							operatorCode = BinaryOperator::And;			break;
		case token::TOK_EQ_OP:				operatorCode = BinaryOperator::EQ;			break;
		case token::TOK_NE_OP:				operatorCode = BinaryOperator::NE;			break;
		case '<':							operatorCode = BinaryOperator::LT;			break;
		case '>':							operatorCode = BinaryOperator::GT;			break;
		case token::TOK_LE_OP:				operatorCode = BinaryOperator::LE;			break;
		case token::TOK_GE_OP:				operatorCode = BinaryOperator::GE;			break;
		case token::TOK_LEFT_SHIFT_OP:		operatorCode = BinaryOperator::Shl;			break;
		case token::TOK_RIGHT_SHIFT_OP:		operatorCode = BinaryOperator::Shr;			break;
		case '+':							operatorCode = BinaryOperator::Add;			break;
		case '-':							operatorCode = BinaryOperator::Sub;			break;
		case '*':							operatorCode = BinaryOperator::Mul;			break;
		case '/':							operatorCode = BinaryOperator::Div;			break;
		case '%':							operatorCode = BinaryOperator::Rem;			break;
		case ',':							operatorCode = BinaryOperator::Comma;		break;
	}
	SourceLocation l = toSourceLocation(location);
	/// @note Type of the operation should by found by ASTContext
	m_stmtQueue.push(
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
	m_stmtQueue.push(
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
	m_stmtQueue.push(
			m_ASTContext.createStmt(
					Stmt::ConditionalOperatorClass,
					dynamic_pointer_cast<Expr>(pop_stmt()),
					dynamic_pointer_cast<Expr>(pop_stmt()),
					dynamic_pointer_cast<Expr>(pop_stmt())
			)
	);
}

void YaccAdapter::addTypeSpecifier(YaccAdapter::TypeSpecifier type)
{
	// long and long long need special operation
	if ((type == YaccAdapter::TypeSpecifier::LONG) &&
		(m_typeSpecifier & YaccAdapter::TypeSpecifier::LONGLONG) == 0)
		m_typeSpecifier += type;
	else if ((type != YaccAdapter::TypeSpecifier::LONG) && (m_typeSpecifier & type) == 0)
		m_typeSpecifier |= type;
	else
		throw std::runtime_error("");
}

void YaccAdapter::addTypeSpecifier(YaccAdapter::TypeSpecifier type, YaccAdapter::TypeSpecifier storageSpecifier)
{
	addTypeSpecifier(type);
	m_storageSpecifier = storageSpecifier;
}

void YaccAdapter::makeBuiltinType()
{
	if(!isTypeSpecifierNotIllegal())
	{
		// reset specifiers before throw
		m_typeSpecifier = 0;
		throw std::runtime_error("??");
	}

	if(m_typeSpecifier & UNSIGNED && m_typeSpecifier & CHAR)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::UChar));
	else if(m_typeSpecifier & UNSIGNED && m_typeSpecifier & SHORT)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::UShort));
	else if(m_typeSpecifier & UNSIGNED && m_typeSpecifier & INT)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::UInt));
	else if(m_typeSpecifier & UNSIGNED && m_typeSpecifier & LONG)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::ULong));
	else if(m_typeSpecifier & UNSIGNED && m_typeSpecifier & LONGLONG)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::ULongLong));
	else if(m_typeSpecifier & SIGNED && m_typeSpecifier & CHAR)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::SChar));
	else if(m_typeSpecifier & LONG && m_typeSpecifier & DOUBLE)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::LongDouble));
	else if(m_typeSpecifier & LONG && m_typeSpecifier & INT)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::Long));
	else if(m_typeSpecifier & VOID)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::Void));
	else if(m_typeSpecifier & CHAR)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::Char_S));
	else if(m_typeSpecifier & SHORT)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::Short));
	else if(m_typeSpecifier & INT)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::Int));
	else if(m_typeSpecifier & FLOAT)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::Float));
	else if(m_typeSpecifier & DOUBLE)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::Double));
	else if(m_typeSpecifier & SIGNED)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::Int));
	else if(m_typeSpecifier & UNSIGNED)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::UInt));
	else if(m_typeSpecifier & LONG)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::Long));
	else if(m_typeSpecifier & LONGLONG)
		m_typeQueue.push(m_ASTContext.createType(Type::Builtin, BuiltinType::LongLong));

	// reset specifiers after built
	m_typeSpecifier = 0;
}

void YaccAdapter::makeVariable(std::string name, yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_declContextHolder.createVariable(name, lp, pop_type()->getCanonicalType());
	/*
	auto decl = make_shared<VariableDecl>(m_declContext, lp, pop_type(), name);
	m_declContext.addDecl(decl);*/
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

	unsigned specifiers = m_typeSpecifier;
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
		/*updateJumpTable(STRUCT)
		updateJumpTable(UNION)*/
		updateJumpTable(STORAGE)
		return false;
	}
	return true;
}

SourceLocation YaccAdapter::toSourceLocation(yy::location &location)
{
	return SourceLocation(m_source.getOpenedName(), location.begin.line, location.begin.column);
}

std::shared_ptr<Stmt> YaccAdapter::pop_stmt()
{
	if(m_stmtQueue.empty())
		throw std::range_error("Stmt queue empty");
	std::shared_ptr<Stmt> ptr=m_stmtQueue.top();
	m_stmtQueue.pop();
	return ptr;
}

std::shared_ptr<Type> YaccAdapter::pop_type()
{
	if(m_typeQueue.empty())
		throw std::range_error("Type queue empty");
	std::shared_ptr<Type> ptr = m_typeQueue.top();
	m_typeQueue.pop();
	return ptr;
}
