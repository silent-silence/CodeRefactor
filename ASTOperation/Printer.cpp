//
// Created by 17271 on 2019/5/26.
//

#include "Printer.h"
#include "AST/Stmt.h"
#include "AST/Expr.h"
#include "OpenHelper/OpenHelper.h"
#include "Decl/DeclGroup.h"
#include "Basic/IdentifierTable.h"

using std::string;					using std::dynamic_pointer_cast;
using std::to_string;

Printer::Printer(OpenHelper &openHelper)
	: m_openHelper{openHelper}, m_tabNum{0}, stmtInOneLine{false}
{}

void Printer::print(std::shared_ptr<Stmt> s)
{
	switch(s->getStmtClass())
	{
		case Stmt::NoStmtClass:break;
		case Stmt::NullStmtClass:				processNullStmt(s);					break;
		case Stmt::CompoundStmtClass:			processCompoundStmt(s);				break;
		case Stmt::CaseStmtClass:				processCaseStmt(s);					break;
		case Stmt::DefaultStmtClass:			processDefaultStmt(s);				break;
		case Stmt::LabelStmtClass:break;
		case Stmt::IfStmtClass:					processIfStmt(s);					break;
		case Stmt::SwitchStmtClass:				processSwitchStmt(s);				break;
		case Stmt::WhileStmtClass:				processWhileStmt(s);				break;
		case Stmt::DoStmtClass:					processDoStmt(s);					break;
		case Stmt::ForStmtClass:				processForStmt(s);					break;
		case Stmt::GotoStmtClass:break;
		case Stmt::IndirectGotoStmtClass:break;
		case Stmt::ContinueStmtClass:			processContinueStmt(s);				break;
		case Stmt::BreakStmtClass:				processBreakStmt(s);				break;
		case Stmt::ReturnStmtClass:				processReturnStmt(s);				break;
		case Stmt::DeclStmtClass:				processDeclStmt(s);					break;
		case Stmt::SwitchCaseClass:break;
		case Stmt::AsmStmtClass:break;
		case Stmt::CXXCatchStmtClass:break;
		case Stmt::CXXTryStmtClass:break;
		case Stmt::ExprClass:break;
		case Stmt::PredefinedExprClass:break;
		case Stmt::DeclRefExprClass:			processRefExpr(s);					break;
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
		case Stmt::CStyleCastExprClass:			processCStyleCastExpr(s);			break;
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
		case Stmt::ShuffleVectorExprClass:break;
		case Stmt::BlockExprClass:break;
		case Stmt::BlockDeclRefExprClass:break;
	}
}

#ifdef ENV_TEST
void Printer::resetPrinter()
{
	m_tabNum = 0;
}
#endif

void Printer::formatExprAsStmt(std::shared_ptr<Stmt> s)
{
	if(dynamic_pointer_cast<Expr>(s))
		m_openHelper.getOutputStream()<< string(m_tabNum * 2, tabType);
	print(s);
	// if is a expr
	if(dynamic_pointer_cast<Expr>(s))
			m_openHelper.getOutputStream() << ";\n";
}

std::string Printer::getTypeName(std::shared_ptr<Type> type) const
{
	std::string typeName;
	// TODO: support more types
	if(type->getTypeClass() == Type::Builtin)
	{
		auto builtin = dynamic_pointer_cast<BuiltinType>(type);
		switch(builtin->getKind())
		{
			case BuiltinType::Void:			typeName = "void";					break;
			case BuiltinType::Bool:			typeName = "bool";					break;
			case BuiltinType::Char_U:		typeName = "unsigned char";			break;
			case BuiltinType::UChar:		typeName = "unsigned char";			break;
			case BuiltinType::Char16:		typeName = "void";					break;
			case BuiltinType::Char32:		typeName = "void";					break;
			case BuiltinType::UShort:		typeName = "unsigned short";		break;
			case BuiltinType::UInt:			typeName = "unsigned int";			break;
			case BuiltinType::ULong:		typeName = "unsigned long";			break;
			case BuiltinType::ULongLong:	typeName = "unsigned long long";	break;
			case BuiltinType::UInt128:		typeName = "void";break;
			case BuiltinType::Char_S:		typeName = "signed char";			break;
			case BuiltinType::SChar:		typeName = "signed char";			break;
			case BuiltinType::WChar:		typeName = "void";break;
			case BuiltinType::Short:		typeName = "short";					break;
			case BuiltinType::Int:			typeName = "int";					break;
			case BuiltinType::Long:			typeName = "long";					break;
			case BuiltinType::LongLong:		typeName = "long long";				break;
			case BuiltinType::Int128:		typeName = "void";break;
			case BuiltinType::Float:		typeName = "float";					break;
			case BuiltinType::Double:		typeName = "double";				break;
			case BuiltinType::LongDouble:	typeName = "long double";			break;
			case BuiltinType::NullPtr:		typeName = "void";break;
			case BuiltinType::Overload:		typeName = "void";break;
			case BuiltinType::Dependent:	typeName = "void";break;
			case BuiltinType::UndeducedAuto:typeName = "void";break;
		}
	}
	return typeName;
}

/*void Printer::childIteration(std::shared_ptr<Stmt> &root)
{
	for(auto s = root->child_begin(); s != root->child_end(); ++s)
		print(*s);
}*/

void Printer::processNullStmt(std::shared_ptr<Stmt> &s)
{
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << ";";
	if(!stmtInOneLine)
		m_openHelper.getOutputStream() << "\n";
}

void Printer::processCompoundStmt(std::shared_ptr<Stmt> &s)
{
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "{\n";
	++m_tabNum;
	for(auto stmt = s->child_begin(); stmt != s->child_end(); ++stmt)
		formatExprAsStmt(*stmt);
	--m_tabNum;
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "}\n";
}

void Printer::processUnaryOperator(std::shared_ptr<Stmt> &s)
{
	auto unary = dynamic_pointer_cast<UnaryOperator>(s);
	auto subExpr = unary->getSubExpr().lock();
	switch (unary->getOpcode())
	{
		case UnaryOperator::PostInc:
			print(subExpr);
			m_openHelper.getOutputStream() << "++";
			break;
		case UnaryOperator::PostDec:
			print(subExpr);
			m_openHelper.getOutputStream() << "--";
			break;
		case UnaryOperator::PreInc:
			m_openHelper.getOutputStream() << "++";
			print(subExpr);
			break;
		case UnaryOperator::PreDec:
			m_openHelper.getOutputStream() << "--";
			print(subExpr);
			break;
		case UnaryOperator::AddrOf:
			m_openHelper.getOutputStream() << "&";
			print(subExpr);
			break;
		case UnaryOperator::Deref:
			m_openHelper.getOutputStream() << "*";
			print(subExpr);
			break;
		case UnaryOperator::Plus:
			m_openHelper.getOutputStream() << "+";
			print(subExpr);
			break;
		case UnaryOperator::Minus:
			m_openHelper.getOutputStream() << "-";
			print(subExpr);
			break;
		case UnaryOperator::Not:
			m_openHelper.getOutputStream() << "!";
			print(subExpr);
			break;
		case UnaryOperator::LNot:
			m_openHelper.getOutputStream() << "~";
			print(subExpr);
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
	m_openHelper.getOutputStream() << to_string(integer->getValue());
}

void Printer::processFloatingLiteral(std::shared_ptr<Stmt> &s)
{
	auto floating = dynamic_pointer_cast<FloatingLiteral>(s);
	m_openHelper.getOutputStream() << toString(floating->getValue());
}

void Printer::processBinaryOperator(std::shared_ptr<Stmt> &s)
{
	auto binary = dynamic_pointer_cast<BinaryOperator>(s);
	print(binary->getLHS().lock());
	switch (binary->getOpcode())
	{
		case BinaryOperator::PtrMemD:	m_openHelper.getOutputStream() << ".";	break;
		case BinaryOperator::PtrMemI:	m_openHelper.getOutputStream() << "->";	break;
		case BinaryOperator::Mul:		m_openHelper.getOutputStream() << "*";	break;
		case BinaryOperator::Div:		m_openHelper.getOutputStream() << "/";	break;
		case BinaryOperator::Rem:		m_openHelper.getOutputStream() << "%";	break;
		case BinaryOperator::Add:		m_openHelper.getOutputStream() << "+";	break;
		case BinaryOperator::Sub:		m_openHelper.getOutputStream() << "-";	break;
		case BinaryOperator::Shl:		m_openHelper.getOutputStream() << "<<";	break;
		case BinaryOperator::Shr:		m_openHelper.getOutputStream() << ">>";	break;
		case BinaryOperator::LT:		m_openHelper.getOutputStream() << "<";	break;
		case BinaryOperator::GT:		m_openHelper.getOutputStream() << ">";	break;
		case BinaryOperator::LE:		m_openHelper.getOutputStream() << "<=";	break;
		case BinaryOperator::GE:		m_openHelper.getOutputStream() << ">=";	break;
		case BinaryOperator::EQ:		m_openHelper.getOutputStream() << "==";	break;
		case BinaryOperator::NE:		m_openHelper.getOutputStream() << "!=";	break;
		case BinaryOperator::And:		m_openHelper.getOutputStream() << "&";	break;
		case BinaryOperator::Xor:		m_openHelper.getOutputStream() << "^";	break;
		case BinaryOperator::Or:		m_openHelper.getOutputStream() << "|";	break;
		case BinaryOperator::LAnd:		m_openHelper.getOutputStream() << "&&";	break;
		case BinaryOperator::LOr:		m_openHelper.getOutputStream() << "||";	break;
		case BinaryOperator::Comma:		m_openHelper.getOutputStream() << ",";	break;
	}
	print(binary->getRHS().lock());
}

void Printer::processCompoundAssignOperator(std::shared_ptr<Stmt> &s)
{
	auto binary = dynamic_pointer_cast<BinaryOperator>(s);
	print(binary->getLHS().lock());
	switch (binary->getOpcode())
	{
		case BinaryOperator::Assign:	m_openHelper.getOutputStream() << "=";	break;
		case BinaryOperator::MulAssign:	m_openHelper.getOutputStream() << "*=";	break;
		case BinaryOperator::DivAssign:	m_openHelper.getOutputStream() << "/=";	break;
		case BinaryOperator::RemAssign:	m_openHelper.getOutputStream() << "%=";	break;
		case BinaryOperator::AddAssign:	m_openHelper.getOutputStream() << "+=";	break;
		case BinaryOperator::SubAssign:	m_openHelper.getOutputStream() << "-=";	break;
		case BinaryOperator::ShlAssign:	m_openHelper.getOutputStream() << "<<=";break;
		case BinaryOperator::ShrAssign:	m_openHelper.getOutputStream() << ">>=";break;
		case BinaryOperator::AndAssign:	m_openHelper.getOutputStream() << "&=";	break;
		case BinaryOperator::XorAssign:	m_openHelper.getOutputStream() << "^=";	break;
		case BinaryOperator::OrAssign:	m_openHelper.getOutputStream() << "|=";	break;
	}
	print(binary->getRHS().lock());
}

void Printer::processConditionalOperator(std::shared_ptr<Stmt> &s)
{
	auto condition = dynamic_pointer_cast<ConditionalOperator>(s);
	print(condition->getCond().lock());
	m_openHelper.getOutputStream() << "?";
	print(condition->getLHS().lock());
	m_openHelper.getOutputStream() << ":";
	print(condition->getRHS().lock());
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
	if(ret->getRetValue().lock())
		print(ret->getRetValue().lock());
	m_openHelper.getOutputStream() << ";\n";
}

void Printer::processParenExpr(std::shared_ptr<Stmt> &s)
{
	auto paren = dynamic_pointer_cast<ParenExpr>(s);
	m_openHelper.getOutputStream() << "(";
	print(paren->getSubExpr().lock());
	m_openHelper.getOutputStream() << ")";
}

void Printer::processSizeOfAlignOfExpr(std::shared_ptr<Stmt> &s)
{
	m_openHelper.getOutputStream() << "sizeof ";
	auto sizeofExp = dynamic_pointer_cast<SizeOfAlignOfExpr>(s);
	if(sizeofExp->isArgumentType())
		;		// TODO: sizeof type
	else
		print(sizeofExp->getArgumentExpr().lock());
}

void Printer::processArraySubscriptExpr(std::shared_ptr<Stmt> &s)
{
	auto arr = dynamic_pointer_cast<ArraySubscriptExpr>(s);
	print(arr->getLHS().lock());
	m_openHelper.getOutputStream() << "[";
	print(arr->getRHS().lock());
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
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "while(";
	print(whileStmt->getCond().lock());
	m_openHelper.getOutputStream() << ")\n";
	formatExprAsStmt(whileStmt->getBody().lock());
}

void Printer::processIfStmt(std::shared_ptr<Stmt> &s)
{
	auto ifStmt = dynamic_pointer_cast<IfStmt>(s);
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "if(";
	print(ifStmt->getCond().lock());
	m_openHelper.getOutputStream() << ")\n";
	formatExprAsStmt(ifStmt->getThen().lock());
	if(ifStmt->getElse().lock())
	{
		m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "else ";
		print(ifStmt->getElse().lock());
	}
}

void Printer::processDoStmt(std::shared_ptr<Stmt> &s)
{
	auto doStmt = dynamic_pointer_cast<DoStmt>(s);
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "do\n";
	formatExprAsStmt(doStmt->getBody().lock());
	m_openHelper.getOutputStream() << "while(";
	print(doStmt->getCond().lock());
	m_openHelper.getOutputStream() << ")\n";
}

void Printer::processForStmt(std::shared_ptr<Stmt> &s)
{
	auto forStmt = dynamic_pointer_cast<ForStmt>(s);
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "for(";
	stmtInOneLine = true;
	print(forStmt->getInit().lock());
	print(forStmt->getCond().lock());
	stmtInOneLine = false;
	if(forStmt->getInc().lock())
		print(forStmt->getInc().lock());
	m_openHelper.getOutputStream() << ")\n";
	formatExprAsStmt(forStmt->getBody().lock());
}

void Printer::processCaseStmt(std::shared_ptr<Stmt> &s)
{
	auto caseStmt = dynamic_pointer_cast<CaseStmt>(s);
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "case ";
	print(caseStmt->getLHS().lock());
	m_openHelper.getOutputStream() << ":\n";
	/*if(caseStmt->getSubStmt())
	{
		++m_tabNum;
		print(caseStmt->getSubStmt());
		--m_tabNum;
	}*/
}

void Printer::processRefExpr(std::shared_ptr<Stmt> &s)
{
	auto ref = dynamic_pointer_cast<DeclRefExpr>(s);
	m_openHelper.getOutputStream() << ref->getDecl().lock()->getNameAsString();
}

void Printer::processSwitchStmt(std::shared_ptr<Stmt> &s)
{
	auto switchStmt = dynamic_pointer_cast<SwitchStmt>(s);
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "switch(";
	print(switchStmt->getCond().lock());
	m_openHelper.getOutputStream() << ")\n";
	print(switchStmt->getBody().lock());
}

void Printer::processDefaultStmt(std::shared_ptr<Stmt> &s)
{
	auto defaultStmt = dynamic_pointer_cast<DefaultStmt>(s);
	m_openHelper.getOutputStream() << string(m_tabNum * 2, tabType) << "default:\n";
}

void Printer::processCStyleCastExpr(std::shared_ptr<Stmt> &s)
{
	auto caseExpr = dynamic_pointer_cast<CStyleCastExpr>(s);
	auto type = caseExpr->getType().lock()->getTypePtr();
	m_openHelper.getOutputStream() << "(";
	m_openHelper.getOutputStream() << getTypeName(type) << ")";
	print(caseExpr->getSubExpr().lock());
}

void Printer::processDeclStmt(std::shared_ptr<Stmt> &s)
{
	auto declStmt = dynamic_pointer_cast<DeclStmt>(s);
	auto declRef = declStmt->getDeclGroup().lock();
	if(declRef->isSingleDecl())
	{
		auto decl = dynamic_pointer_cast<ValueDecl>(declRef->getSingleDecl().lock());
		auto type = decl->getType().lock()->getTypePtr();
		m_openHelper.getOutputStream() << getTypeName(type) << " " << decl->getNameAsString() << ";\n";
	}
	else if(declRef->isDeclGroup())
	{
		auto decls = declRef->getDeclGroup().lock();
		auto type = dynamic_pointer_cast<ValueDecl>((*decls)[0])->getType().lock()->getTypePtr();
		m_openHelper.getOutputStream() << getTypeName(type) << " ";
		for (unsigned i = 0; i < decls->size(); ++i)
		{
			m_openHelper.getOutputStream() << dynamic_pointer_cast<ValueDecl>((*decls)[i])->getNameAsString();
			if (i + 1 != decls->size())
				m_openHelper.getOutputStream() << ", ";
		}
		m_openHelper.getOutputStream() << ";\n";
	}
}