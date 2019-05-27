//
// Created by 17271 on 2019/5/26.
//

#include "Printer.h"
#include "AST/Stmt.h"
#include "AST/Expr.h"
#include "OpenHelper/OpenHelper.h"

using std::string;					using std::dynamic_pointer_cast;
using std::to_string;

Printer::Printer(OpenHelper &openHelper)
	: m_openHelper{openHelper}, m_tabNum{0}
{}

void Printer::print(std::shared_ptr<Stmt> s)
{
	switch(s->getStmtClass())
	{
		case Stmt::NoStmtClass:break;
		case Stmt::NullStmtClass:				processNullStmt(s);					break;
		case Stmt::CompoundStmtClass:			processCompoundStmt(s);				break;
		case Stmt::CaseStmtClass:				processCaseStmt(s);					break;
		case Stmt::DefaultStmtClass:break;
		case Stmt::LabelStmtClass:break;
		case Stmt::IfStmtClass:					processIfStmt(s);					break;
		case Stmt::SwitchStmtClass:break;
		case Stmt::WhileStmtClass:				processWhileStmt(s);				break;
		case Stmt::DoStmtClass:					processDoStmt(s);					break;
		case Stmt::ForStmtClass:				processForStmt(s);					break;
		case Stmt::GotoStmtClass:break;
		case Stmt::IndirectGotoStmtClass:break;
		case Stmt::ContinueStmtClass:			processContinueStmt(s);				break;
		case Stmt::BreakStmtClass:				processBreakStmt(s);				break;
		case Stmt::ReturnStmtClass:				processReturnStmt(s);				break;
		case Stmt::DeclStmtClass:break;
		case Stmt::SwitchCaseClass:break;
		case Stmt::AsmStmtClass:break;
		case Stmt::ObjCAtTryStmtClass:break;
		case Stmt::ObjCAtCatchStmtClass:break;
		case Stmt::ObjCAtFinallyStmtClass:break;
		case Stmt::ObjCAtThrowStmtClass:break;
		case Stmt::ObjCAtSynchronizedStmtClass:break;
		case Stmt::ObjCForCollectionStmtClass:break;
		case Stmt::CXXCatchStmtClass:break;
		case Stmt::CXXTryStmtClass:break;
		case Stmt::ExprClass:break;
		case Stmt::PredefinedExprClass:break;
		case Stmt::DeclRefExprClass:break;
		case Stmt::IntegerLiteralClass:			processIntergerLiteral(s);			break;
		case Stmt::FloatingLiteralClass:		processFloatingLiteral(s);			break;
		case Stmt::ImaginaryLiteralClass:break;
		case Stmt::StringLiteralClass:			processStringLiteral(s);			break;
		case Stmt::CharacterLiteralClass:		processCharacterLiteral(s);			break;
		case Stmt::ParenExprClass:				processParenExpr(s);				break;
		case Stmt::UnaryOperatorClass:			processUnaryOperator(s);			break;
		case Stmt::SizeOfAlignOfExprClass:		processSizeOfAlignOfExpr(s);		break;
		case Stmt::ArraySubscriptExprClass:		processArraySubscriptExpr(s);		break;
		case Stmt::CallExprClass:break;
		case Stmt::MemberExprClass:	break;
		case Stmt::CastExprClass:break;
		case Stmt::BinaryOperatorClass:			processBinaryOperator(s);			break;
		case Stmt::CompoundAssignOperatorClass:	processCompoundAssignOperator(s);	break;
		case Stmt::ConditionalOperatorClass:	processConditionalOperator(s);		break;
		case Stmt::ImplicitCastExprClass:break;
		case Stmt::ExplicitCastExprClass:break;
		case Stmt::CStyleCastExprClass:break;
		case Stmt::CompoundLiteralExprClass:break;
		case Stmt::ExtVectorElementExprClass:break;
		case Stmt::InitListExprClass:break;
		case Stmt::DesignatedInitExprClass:break;
		case Stmt::ImplicitValueInitExprClass:break;
		case Stmt::ParenListExprClass:break;
		case Stmt::VAArgExprClass:break;
		case Stmt::AddrLabelExprClass:break;
		case Stmt::StmtExprClass:break;
		case Stmt::TypesCompatibleExprClass:break;
		case Stmt::ChooseExprClass:break;
		case Stmt::GNUNullExprClass:break;
		case Stmt::CXXOperatorCallExprClass:break;
		case Stmt::CXXMemberCallExprClass:break;
		case Stmt::CXXNamedCastExprClass:break;
		case Stmt::CXXStaticCastExprClass:break;
		case Stmt::CXXDynamicCastExprClass:break;
		case Stmt::CXXReinterpretCastExprClass:break;
		case Stmt::CXXConstCastExprClass:break;
		case Stmt::CXXFunctionalCastExprClass:break;
		case Stmt::CXXTypeidExprClass:break;
		case Stmt::CXXBoolLiteralExprClass:break;
		case Stmt::CXXNullPtrLiteralExprClass:break;
		case Stmt::CXXThisExprClass:break;
		case Stmt::CXXThrowExprClass:break;
		case Stmt::CXXDefaultArgExprClass:break;
		case Stmt::CXXZeroInitValueExprClass:break;
		case Stmt::CXXConditionDeclExprClass:break;
		case Stmt::CXXNewExprClass:break;
		case Stmt::CXXDeleteExprClass:break;
		case Stmt::UnresolvedFunctionNameExprClass:break;
		case Stmt::UnaryTypeTraitExprClass:break;
		case Stmt::QualifiedDeclRefExprClass:break;
		case Stmt::UnresolvedDeclRefExprClass:break;
		case Stmt::TemplateIdRefExprClass:break;
		case Stmt::CXXConstructExprClass:break;
		case Stmt::CXXBindTemporaryExprClass:break;
		case Stmt::CXXExprWithTemporariesClass:break;
		case Stmt::CXXTemporaryObjectExprClass:break;
		case Stmt::CXXUnresolvedConstructExprClass:break;
		case Stmt::CXXUnresolvedMemberExprClass:break;
		case Stmt::ObjCStringLiteralClass:break;
		case Stmt::ObjCEncodeExprClass:break;
		case Stmt::ObjCMessageExprClass:break;
		case Stmt::ObjCSelectorExprClass:break;
		case Stmt::ObjCProtocolExprClass:break;
		case Stmt::ObjCIvarRefExprClass:break;
		case Stmt::ObjCPropertyRefExprClass:break;
		case Stmt::ObjCImplicitSetterGetterRefExprClass:break;
		case Stmt::ObjCSuperExprClass:break;
		case Stmt::ObjCIsaExprClass:break;
		case Stmt::ShuffleVectorExprClass:break;
		case Stmt::BlockExprClass:break;
		case Stmt::BlockDeclRefExprClass:break;
	}
}

void Printer::resetPrinter()
{
	m_tabNum = 0;
}

void Printer::childIteration(std::shared_ptr<Stmt> &root)
{
	for(auto s = root->child_begin(); s != root->child_end(); ++s)
		print(*s);
}

void Printer::processNullStmt(std::shared_ptr<Stmt> &s)
{
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << ";\n";
}

void Printer::processCompoundStmt(std::shared_ptr<Stmt> &s)
{
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "{\n";
	++m_tabNum;
	childIteration(s);
	--m_tabNum;
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "}\n";
}

void Printer::processUnaryOperator(std::shared_ptr<Stmt> &s)
{
	auto unary = dynamic_pointer_cast<UnaryOperator>(s);
	switch (unary->getOpcode())
	{
		case UnaryOperator::PostInc:
			childIteration(s);
			m_openHelper.getOutputStream() << "++";
			break;
		case UnaryOperator::PostDec:
			childIteration(s);
			m_openHelper.getOutputStream() << "--";
			break;
		case UnaryOperator::PreInc:
			m_openHelper.getOutputStream() << "++";
			childIteration(s);
			break;
		case UnaryOperator::PreDec:
			m_openHelper.getOutputStream() << "--";
			childIteration(s);
			break;
		case UnaryOperator::AddrOf:
			m_openHelper.getOutputStream() << "&";
			childIteration(s);
			break;
		case UnaryOperator::Deref:
			m_openHelper.getOutputStream() << "*";
			childIteration(s);
			break;
		case UnaryOperator::Plus:
			m_openHelper.getOutputStream() << "+";
			childIteration(s);
			break;
		case UnaryOperator::Minus:
			m_openHelper.getOutputStream() << "-";
			childIteration(s);
			break;
		case UnaryOperator::Not:
			m_openHelper.getOutputStream() << "!";
			childIteration(s);
			break;
		case UnaryOperator::LNot:
			m_openHelper.getOutputStream() << "~";
			childIteration(s);
			break;
		case UnaryOperator::Real:break;
		case UnaryOperator::Imag:break;
		case UnaryOperator::Extension:break;
		case UnaryOperator::OffsetOf:break;
	}
}

void Printer::processIntergerLiteral(std::shared_ptr<Stmt> &s)
{
	auto integer = dynamic_pointer_cast<IntegerLiteral>(s);
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << to_string(integer->getValue());
}

void Printer::processFloatingLiteral(std::shared_ptr<Stmt> &s)
{
	auto floating = dynamic_pointer_cast<FloatingLiteral>(s);
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << toString(floating->getValue());
}

void Printer::processBinaryOperator(std::shared_ptr<Stmt> &s)
{
	auto binary = dynamic_pointer_cast<BinaryOperator>(s);
	print(binary->getLHS());
	switch (binary->getOpcode())
	{
		case BinaryOperator::PtrMemD:
			m_openHelper.getOutputStream() << ".";
			break;
		case BinaryOperator::PtrMemI:
			m_openHelper.getOutputStream() << "->";
			break;
		case BinaryOperator::Mul:
			m_openHelper.getOutputStream() << "*";
			break;
		case BinaryOperator::Div:
			m_openHelper.getOutputStream() << "/";
			break;
		case BinaryOperator::Rem:
			m_openHelper.getOutputStream() << "%";
			break;
		case BinaryOperator::Add:
			m_openHelper.getOutputStream() << "+";
			break;
		case BinaryOperator::Sub:
			m_openHelper.getOutputStream() << "-";
			break;
		case BinaryOperator::Shl:
			m_openHelper.getOutputStream() << "<<";
			break;
		case BinaryOperator::Shr:
			m_openHelper.getOutputStream() << ">>";
			break;
		case BinaryOperator::LT:
			m_openHelper.getOutputStream() << "<";
			break;
		case BinaryOperator::GT:
			m_openHelper.getOutputStream() << ">";
			break;
		case BinaryOperator::LE:
			m_openHelper.getOutputStream() << "<=";
			break;
		case BinaryOperator::GE:
			m_openHelper.getOutputStream() << ">=";
			break;
		case BinaryOperator::EQ:
			m_openHelper.getOutputStream() << "==";
			break;
		case BinaryOperator::NE:
			m_openHelper.getOutputStream() << "!=";
			break;
		case BinaryOperator::And:
			m_openHelper.getOutputStream() << "&";
			break;
		case BinaryOperator::Xor:
			m_openHelper.getOutputStream() << "^";
			break;
		case BinaryOperator::Or:
			m_openHelper.getOutputStream() << "|";
			break;
		case BinaryOperator::LAnd:
			m_openHelper.getOutputStream() << "&&";
			break;
		case BinaryOperator::LOr:
			m_openHelper.getOutputStream() << "||";
			break;

		case BinaryOperator::Comma:
			m_openHelper.getOutputStream() << ",";
			break;
	}
	print(binary->getRHS());
}

void Printer::processCompoundAssignOperator(std::shared_ptr<Stmt> &s)
{
	auto binary = dynamic_pointer_cast<BinaryOperator>(s);
	print(binary->getLHS());
	switch (binary->getOpcode())
	{
		case BinaryOperator::Assign:
			m_openHelper.getOutputStream() << "=";
			break;
		case BinaryOperator::MulAssign:
			m_openHelper.getOutputStream() << "*=";
			break;
		case BinaryOperator::DivAssign:
			m_openHelper.getOutputStream() << "/=";
			break;
		case BinaryOperator::RemAssign:
			m_openHelper.getOutputStream() << "%=";
			break;
		case BinaryOperator::AddAssign:
			m_openHelper.getOutputStream() << "+=";
			break;
		case BinaryOperator::SubAssign:
			m_openHelper.getOutputStream() << "-=";
			break;
		case BinaryOperator::ShlAssign:
			m_openHelper.getOutputStream() << "<<=";
			break;
		case BinaryOperator::ShrAssign:
			m_openHelper.getOutputStream() << ">>=";
			break;
		case BinaryOperator::AndAssign:
			m_openHelper.getOutputStream() << "&=";
			break;
		case BinaryOperator::XorAssign:
			m_openHelper.getOutputStream() << "^=";
			break;
		case BinaryOperator::OrAssign:
			m_openHelper.getOutputStream() << "|=";
			break;
	}
	print(binary->getRHS());
}

void Printer::processConditionalOperator(std::shared_ptr<Stmt> &s)
{
	auto condition = dynamic_pointer_cast<ConditionalOperator>(s);
	print(condition->getCond());
	m_openHelper.getOutputStream() << "?";
	print(condition->getLHS());
	m_openHelper.getOutputStream() << ":";
	print(condition->getRHS());
}

void Printer::processContinueStmt(std::shared_ptr<Stmt> &s)
{
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "continue;\n";
}

void Printer::processBreakStmt(std::shared_ptr<Stmt> &s)
{
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "break;\n";
}

void Printer::processReturnStmt(std::shared_ptr<Stmt> &s)
{
	auto ret = dynamic_pointer_cast<ReturnStmt>(s);
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "return ";
	if(ret->getRetValue())
		print(ret->getRetValue());
	m_openHelper.getOutputStream() << ";\n";
}

void Printer::processParenExpr(std::shared_ptr<Stmt> &s)
{
	m_openHelper.getOutputStream() << "(";
	childIteration(s);
	m_openHelper.getOutputStream() << ")";
}

void Printer::processSizeOfAlignOfExpr(std::shared_ptr<Stmt> &s)
{
	m_openHelper.getOutputStream() << "sizeof ";
	auto sizeofExp = dynamic_pointer_cast<SizeOfAlignOfExpr>(s);
	if(sizeofExp->isArgumentType())
		;		// TODO: sizeof type
	else
		print(sizeofExp->getArgumentExpr());
}

void Printer::processArraySubscriptExpr(std::shared_ptr<Stmt> &s)
{
	auto arr = dynamic_pointer_cast<ArraySubscriptExpr>(s);
	print(arr->getLHS());
	m_openHelper.getOutputStream() << "[";
	print(arr->getRHS());
	m_openHelper.getOutputStream() << "]";
}

void Printer::processStringLiteral(std::shared_ptr<Stmt> &s)
{
	auto literal = dynamic_pointer_cast<StringLiteral>(s);
	m_openHelper.getOutputStream() << string(literal->getStrData());
}

void Printer::processCharacterLiteral(std::shared_ptr<Stmt> &s)
{
	auto character = dynamic_pointer_cast<CharacterLiteral>(s);
	m_openHelper.getOutputStream() << "'" << static_cast<char>(character->getValue()) << "'";
}

void Printer::processWhileStmt(std::shared_ptr<Stmt> &s)
{
	auto whileStmt = dynamic_pointer_cast<WhileStmt>(s);
	m_openHelper.getOutputStream() << "while(";
	print(whileStmt->getCond());
	m_openHelper.getOutputStream() << ")\n";
	print(whileStmt->getBody());
}

void Printer::processIfStmt(std::shared_ptr<Stmt> &s)
{
	auto ifStmt = dynamic_pointer_cast<IfStmt>(s);
	m_openHelper.getOutputStream() << "if(";
	print(ifStmt->getCond());
	m_openHelper.getOutputStream() << ")\n";
	print(ifStmt->getThen());
	if(ifStmt->getElse())
	{
		m_openHelper.getOutputStream() << "else ";
		print(ifStmt->getElse());
	}
}

void Printer::processDoStmt(std::shared_ptr<Stmt> &s)
{
	auto doStmt = dynamic_pointer_cast<DoStmt>(s);
	m_openHelper.getOutputStream() << "do\n";
	print(doStmt->getBody());
	m_openHelper.getOutputStream() << "while(";
	print(doStmt->getCond());
	m_openHelper.getOutputStream() << ")\n";
}

void Printer::processForStmt(std::shared_ptr<Stmt> &s)
{
	auto forStmt = dynamic_pointer_cast<ForStmt>(s);
	m_openHelper.getOutputStream() << "for(";
	print(forStmt->getInit());
	print(forStmt->getCond());
	if(forStmt->getInc())
		print(forStmt->getInc());
	m_openHelper.getOutputStream() << ")\n";
	print(forStmt->getBody());
}

void Printer::processCaseStmt(std::shared_ptr<Stmt> &s)
{
	auto caseStmt = dynamic_pointer_cast<CaseStmt>(s);
	m_openHelper.getOutputStream() << "case ";
	print(caseStmt->getLHS());
	m_openHelper.getOutputStream() << ":\n";
	if(caseStmt->getSubStmt())
	{
		++m_tabNum;
		print(caseStmt->getSubStmt());
		--m_tabNum;
	}
}

void Printer::processRefExpr(std::shared_ptr<Stmt> &s)
{
	auto ref = dynamic_pointer_cast<DeclRefExpr>(s);
	m_openHelper.getOutputStream() << ref->getDecl()->getIdentifier();
}
