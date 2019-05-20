//
// Created by gaojian on 19-4-6.
//

#include "OpenHelper/OpenHelper.h"
#include "Driver.h"
#include "Basic/SourceLocation.h"
#include "AST/ASTContext.h"

using std::make_shared;

typedef yy::Parser::token::yytokentype token;

Driver::Driver(OpenHelper &opener, ASTContext &context)
	: m_opener{opener}, trace_scanning{false}, trace_parsing{false}, m_context{context}
{}

int Driver::parse()
{
	scan_begin();
	yy::Parser parser(*this);
	parser.set_debug_level(trace_parsing);
	int res = parser.parse();
	return res;
}

void Driver::error(const yy::location& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}

void Driver::error(const std::string& m)
{
	std::cerr << m << std::endl;
}

Scanner& Driver::getSacnner()
{
	return m_scanner;
}

OpenHelper& Driver::getOpenHelper()
{
	return m_opener;
}

std::string &Driver::getOpenedFrom() const
{
	return m_opener.getOpenedName();
}

void Driver::scan_begin()
{
	m_scanner.set_debug(trace_scanning);
	m_scanner.switch_streams(&m_opener.getInputStream(), &m_opener.getOutputStream());
}

void Driver::makeCompoundStmt(unsigned stmtNumInBlock, yy::location &l, yy::location &r)
{
	SourceLocation lb = toSourceLocation(l);
	SourceLocation rb = toSourceLocation(r);
	m_context.create<Stmt::CompoundStmtClass>(stmtNumInBlock, lb, rb);
}

void Driver::makeNullStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_context.create<Stmt::NullStmtClass>(lp);
}

void Driver::makeContinueStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_context.create<Stmt::ContinueStmtClass>(lp);
}

void Driver::makeBreakStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_context.create<Stmt::BreakStmtClass>(lp);
}

void Driver::makeReturnStmt(yy::location &l, bool haveExpr)
{
	SourceLocation lp = toSourceLocation(l);
	m_context.create<Stmt::ReturnStmtClass>(lp, haveExpr);
}

void Driver::makeWhileStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_context.create<Stmt::WhileStmtClass>(lp);
}

void Driver::makeDoStmt(yy::location &d, yy::location &w, yy::location &l)
{
	SourceLocation dp = toSourceLocation(d);
	SourceLocation wp = toSourceLocation(w);
	SourceLocation lp = toSourceLocation(l);
	m_context.create<Stmt::DoStmtClass>(dp, wp, lp);
}

void Driver::makeSwitchStmt(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_context.create<Stmt::SwitchStmtClass>(lp);
}

void Driver::makeCaseStmt(yy::location &l, yy::location &r, yy::location &c)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	SourceLocation cp = toSourceLocation(c);
	m_context.create<Stmt::CaseStmtClass>(lp, rp, cp);
}

void Driver::makeDefaultStmt(yy::location &l, yy::location &r)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	m_context.create<Stmt::DefaultStmtClass>(lp, rp);
}

void Driver::makeIntegerLiteral(int val, yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_context.create<Stmt::IntegerLiteralClass>(val, QualType(make_shared<Type>(), 0), lp);
}

void Driver::makeCharacterLiteral(unsigned val, yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_context.create<Stmt::CharacterLiteralClass>(val, false, QualType(make_shared<Type>(), 0), lp);
}

void Driver::makeFloatingLiteral(const float &val, yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_context.create<Stmt::FloatingLiteralClass>(val, false, QualType(make_shared<Type>(), 0), lp);
}

void Driver::makeStringLiteral(std::string str, yy::location &location)
{
	SourceLocation l = toSourceLocation(location);
	m_context.create<Stmt::StringLiteralClass>(str.data(), str.length(), false, QualType(make_shared<Type>(), 0), l);
}

void Driver::makeUnaryOperator(int opc, yy::location &l)
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
	m_context.create<Stmt::UnaryOperatorClass>(operatorCode, QualType(make_shared<Type>(), 0), lp);
}

void Driver::makeBinaryOperator(int opc, yy::location &location)
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
	m_context.create<Stmt::BinaryOperatorClass>(operatorCode, QualType(make_shared<Type>(), 0), l);
}

void Driver::makeCompoundAssignOperator(int opc, yy::location &location)
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
	m_context.create<Stmt::CompoundAssignOperatorClass>(
			operatorCode,
			QualType(make_shared<Type>(), 0),
			QualType(make_shared<Type>(), 0),
			QualType(make_shared<Type>(), 0),
			l
			);
}

void Driver::makeConditionalOperator()
{
	m_context.create<Stmt::ConditionalOperatorClass>(QualType(make_shared<Type>(), 0));
}

void Driver::makeParenExpr(yy::location &l, yy::location &r)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	m_context.create<Stmt::ParenExprClass>(lp, rp);
}

void Driver::makeSizeofExpr(yy::location &l, yy::location &r, bool isSizeofType)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	m_context.create<Stmt::SizeOfAlignOfExprClass>(lp, rp, isSizeofType);
}

void Driver::makeArraySubscripExpr(yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_context.create<Stmt::ArraySubscriptExprClass>(lp);
}

void Driver::makeCallExpr(unsigned parameterNum, yy::location &l)
{
	SourceLocation lp = toSourceLocation(l);
	m_context.create<Stmt::CallExprClass>(parameterNum, lp);
}

void Driver::makeMemberExpr(int opc, yy::location &l)
{
	bool isArrow;
	switch(opc)
	{
		case '.':					isArrow = false;	break;
		case token::TOK_POINT_OP:	isArrow = true;		break;
	}
	SourceLocation lp = toSourceLocation(l);
	m_context.create<Stmt::MemberExprClass>(isArrow, lp);
}

void Driver::makeCStyleCastExpr(yy::location &l, yy::location &r)
{
	SourceLocation lp = toSourceLocation(l);
	SourceLocation rp = toSourceLocation(r);
	m_context.create<Stmt::CStyleCastExprClass>(lp, rp);
}

SourceLocation Driver::toSourceLocation(yy::location &location)
{
	return SourceLocation(getOpenedFrom(), location.begin.line, location.begin.column);
}