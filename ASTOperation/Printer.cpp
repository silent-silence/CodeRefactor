//
// Created by 17271 on 2019/5/26.
//

#include "Printer.h"
#include "AST/Stmt.h"
#include "AST/Expr.h"
#include "OpenHelper/OpenHelper.h"
#include "Decl/DeclGroup.h"
#include "Decl/DeclBase.h"
#include "Basic/IdentifierTable.h"

using std::string;					using std::dynamic_pointer_cast;
using std::to_string;				using std::make_shared;

/// @Printer
std::string Printer::indent()
{
	return string(indentNum * 2, indentCharacter);
}

#ifdef ENV_TEST
void Printer::resetPrinter()
{
	indentNum = 0;
}
#endif

/// @ContextPrinter
Printer::ContextPrinter::ContextPrinter(OpenHelper &stream)
	: m_openHelper{stream}, astPrinter{stream}, typePrinter{stream, astPrinter}
{}

void Printer::ContextPrinter::printContext(std::shared_ptr<DeclContext> context)
{
	for (auto it = context->decl_begin(); it != context->decl_end(); ++it)
	{
		switch ((*it)->getKind())
		{
			case Decl::Kind::Var:
				printVar((*it));
				break;
			case Decl::Kind::Typedef:
				printTypedef(*it);
				break;
			case Decl::Kind::Record:
				printRecord(*it);
				break;
			case Decl::Kind::Function:
				printFunction(*it);
			default:
				break;
		}
	}
}

void Printer::ContextPrinter::printVar(std::shared_ptr<Decl> decl)
{
	auto var = dynamic_pointer_cast<VarDecl>(decl);
	typePrinter.printTypePrefix(var->getType().lock());
	m_openHelper.getOutputStream() << " " << var->getNameAsString();
	typePrinter.printTypePostfix(var->getType().lock());
	m_openHelper.getOutputStream() << ";\n";
}

void Printer::ContextPrinter::printTypedef(std::shared_ptr<Decl> decl)
{
	auto typedefDecl = dynamic_pointer_cast<TypedefDecl>(decl);
	auto type = dynamic_pointer_cast<TypedefType>(typedefDecl->getTypeForDecl().lock()->getTypePtr());
	m_openHelper.getOutputStream() << indent();
	m_openHelper.getOutputStream() << "typedef ";
	typePrinter.printTypePrefix(type->getDeclForType().lock());
	m_openHelper.getOutputStream() << " " << typedefDecl->getNameAsString();
	typePrinter.printTypePostfix(type->getDeclForType().lock());
	m_openHelper.getOutputStream() << ";\n";
}

void Printer::ContextPrinter::printRecord(std::shared_ptr<Decl> decl)
{
	auto record = dynamic_pointer_cast<RecordDecl>(decl);
	if(!record->getIdentifier().lock()->isAnonymous())	// remove anonymous struct decl
	{
		typePrinter.printTypePrefix(record->getTypeForDecl().lock());
		m_openHelper.getOutputStream() << " " << record->getNameAsString();
		typePrinter.printTypePostfix(record->getTypeForDecl().lock());
		m_openHelper.getOutputStream() << ";\n";
	}
}

void Printer::ContextPrinter::printFunction(std::shared_ptr<Decl> decl)
{
	auto fun = dynamic_pointer_cast<FunctionDecl>(decl);
	typePrinter.printTypePrefix(fun->getType().lock());
	m_openHelper.getOutputStream() << " " << fun->getNameAsString();
	typePrinter.printTypePostfix(fun->getType().lock());
	if(fun->getBody().lock())
	{
		m_openHelper.getOutputStream() << "\n";
		astPrinter.printAST(fun->getBody().lock());
		m_openHelper.getOutputStream() << "\n";
	}
	else
		m_openHelper.getOutputStream() << ";\n";
}

/// @TypePrinter
Printer::TypePrinter::TypePrinter(OpenHelper &stream, ASTPrinter &printer)
	: m_openHelper{stream}, ast{printer}
{}

void Printer::TypePrinter::printTypePrefix(std::shared_ptr<QualType> type)
{
	switch(type->getTypePtr()->getTypeClass())
	{
		case Type::TypeClass::Record:
			recordPrefix(type);
			break;
		case Type::TypeClass::Enum:
			enumPrefix(type);
			break;
		default:
			m_openHelper.getOutputStream() << type->getTypePrefixAsString();
			break;
	}
}

void Printer::TypePrinter::printTypeInfix(std::shared_ptr<QualType> type)
{
	if(type->getTypePtr()->getTypeClass() == Type::TypeClass::Pointer)
		m_openHelper.getOutputStream() << "*";
}

void Printer::TypePrinter::printTypePostfix(std::shared_ptr<QualType> type)
{
	string ret;
	switch(type->getTypePtr()->getTypeClass())
	{
		case Type::TypeClass::ExtQual:					break;
		case Type::TypeClass::Builtin:					break;/*No postfix*/
		case Type::TypeClass::FixedWidthInt:			break;
		case Type::TypeClass::Complex:					break;
		case Type::TypeClass::Pointer:					break;/*No postfix*/
		case Type::TypeClass::BlockPointer:				break;
		case Type::TypeClass::LValueReference:			break;
		case Type::TypeClass::RValueReference:			break;
		case Type::TypeClass::MemberPointer:			break;
		case Type::TypeClass::ConstantArray:
		case Type::TypeClass::ConstantArrayWithExpr:
			arrayWithExprPostfix(type);
			break;
		case Type::TypeClass::ConstantArrayWithoutExpr:
		case Type::TypeClass::IncompleteArray:
			arrayWithoutExprPostfix(type);
			break;
		case Type::TypeClass::VariableArray:			break;
		case Type::TypeClass::DependentSizedArray:		break;
		case Type::TypeClass::DependentSizedExtVector:	break;
		case Type::TypeClass::Vector:					break;
		case Type::TypeClass::ExtVector:				break;
		case Type::TypeClass::FunctionProto:
			functionProtoPostfix(type);
			break;
		case Type::TypeClass::FunctionNoProto:
			functionNoProtoPostfix(type);
			break;
		case Type::TypeClass::Typedef:					break;/*No postfix*/
		case Type::TypeClass::TypeOfExpr:				break;
		case Type::TypeClass::TypeOf:					break;
		case Type::TypeClass::Decltype:					break;
		case Type::TypeClass::Record:
			//recordPostfix(type);
			break;
		case Type::TypeClass::Enum:
			//enumPostfix(type);
			break;
		case Type::TypeClass::TemplateTypeParm:			break;
		case Type::TypeClass::TemplateSpecialization:	break;
		case Type::TypeClass::QualifiedName:			break;
		case Type::TypeClass::Typename:					break;
	}
}

void Printer::TypePrinter::recordPrefix(std::shared_ptr<QualType> type)
{
	m_openHelper.getOutputStream() << type->getTypePrefixAsString();
	m_openHelper.getOutputStream() << "\n";
	auto recordType = dynamic_pointer_cast<RecordType>(type->getTypePtr());
	auto decl = dynamic_pointer_cast<RecordDecl>(recordType->getDecl().lock());
	++indentNum;
	for(auto it = decl->decl_begin(); it != decl->decl_end(); ++it)
	{
		auto d = dynamic_pointer_cast<VarDecl>(*it);
		m_openHelper.getOutputStream() << indent();
		printTypePrefix(d->getType().lock());
		m_openHelper.getOutputStream() << " ";
		m_openHelper.getOutputStream() << d->getNameAsString();
		printTypePostfix(d->getType().lock());
		m_openHelper.getOutputStream() << ";\n";
	}
	--indentNum;
	m_openHelper.getOutputStream() << "}";
}

void Printer::TypePrinter::arrayWithExprPostfix(std::shared_ptr<QualType> type)
{
	auto arrayType = dynamic_pointer_cast<ConstantArrayWithExprType>(type->getTypePtr());
	m_openHelper.getOutputStream() << "[";
	ast.printAST(arrayType->getSizeSpecifier().lock());
	m_openHelper.getOutputStream() << "]";
}

void Printer::TypePrinter::arrayWithoutExprPostfix(std::shared_ptr<QualType> type)
{
	printTypePostfix(dynamic_pointer_cast<ArrayType>(type->getTypePtr())->getElementType().lock());
	m_openHelper.getOutputStream() << "[]";
}

void Printer::TypePrinter::functionNoProtoPostfix(std::shared_ptr<QualType> type)
{
	m_openHelper.getOutputStream() << "()";
}

void Printer::TypePrinter::functionProtoPostfix(std::shared_ptr<QualType> type)
{
	m_openHelper.getOutputStream() << "(";
	auto fun = dynamic_pointer_cast<FunctionType>(type->getTypePtr());
	auto funDecl = fun->getFunDecl();
	for(int i = 0; i < funDecl->getNumParams(); ++i)
	{
		auto param = funDecl->getParamDecl(i).lock();
		printTypePrefix(param->getType().lock());
		if(param->paramHasName())
			m_openHelper.getOutputStream() << " " << param->getNameAsString();
        printTypePostfix(param->getType().lock());
		if(i != funDecl->getNumParams() - 1)
			m_openHelper.getOutputStream() << ", ";
	}
	m_openHelper.getOutputStream() << ")";
}

void Printer::TypePrinter::enumPrefix(std::shared_ptr<QualType> type)
{
	m_openHelper.getOutputStream() << type->getTypePrefixAsString();
	m_openHelper.getOutputStream() << "\n";
	auto enumType = dynamic_pointer_cast<EnumType>(type->getTypePtr());
	auto decl = dynamic_pointer_cast<EnumDecl>(enumType->getDecl().lock());
	++indentNum;
	m_openHelper.getOutputStream() << indent();
	unsigned enumConstantLocation = 0;
	for(auto it = decl->decl_begin(); it != decl->decl_end(); ++it, ++enumConstantLocation)
	{
		auto d = dynamic_pointer_cast<EnumConstantDecl>(*it);
		m_openHelper.getOutputStream() << d->getNameAsString();
		if(++it != decl->decl_end())
			m_openHelper.getOutputStream() << ", ";
		--it;
	}
	--indentNum;
	m_openHelper.getOutputStream() << "\n}";
}

/// @ASTPrinter
Printer::ASTPrinter::ASTPrinter(OpenHelper &stream)
	: m_openHelper{stream}, m_typePrinter{stream, *this}, stmtInOneLine{false}, noIntent{false}
{}

void Printer::ASTPrinter::printAST(std::shared_ptr<Stmt> root)
{
    if(root == nullptr)
        return;
	switch(root->getStmtClass())
	{
		case Stmt::StmtClass::NoStmtClass:break;
		case Stmt::StmtClass::NullStmtClass:				processNullStmt(root);					break;
		case Stmt::StmtClass::CompoundStmtClass:			processCompoundStmt(root);				break;
		case Stmt::StmtClass::CaseStmtClass:				processCaseStmt(root);					break;
		case Stmt::StmtClass::DefaultStmtClass:				processDefaultStmt(root);				break;
		case Stmt::StmtClass::LabelStmtClass:break;
		case Stmt::StmtClass::IfStmtClass:					processIfStmt(root);					break;
		case Stmt::StmtClass::SwitchStmtClass:				processSwitchStmt(root);				break;
		case Stmt::StmtClass::WhileStmtClass:				processWhileStmt(root);					break;
		case Stmt::StmtClass::DoStmtClass:					processDoStmt(root);					break;
		case Stmt::StmtClass::ForStmtClass:					processForStmt(root);					break;
		case Stmt::StmtClass::GotoStmtClass:break;
		case Stmt::StmtClass::IndirectGotoStmtClass:break;
		case Stmt::StmtClass::ContinueStmtClass:			processContinueStmt(root);				break;
		case Stmt::StmtClass::BreakStmtClass:				processBreakStmt(root);					break;
		case Stmt::StmtClass::ReturnStmtClass:				processReturnStmt(root);				break;
		case Stmt::StmtClass::DeclStmtClass:				processDeclStmt(root);					break;
		case Stmt::StmtClass::SwitchCaseClass:break;
		case Stmt::StmtClass::AsmStmtClass:break;
		case Stmt::StmtClass::CommentStmtClass:             processCommentStmt(root);                  break;
		case Stmt::StmtClass::CXXCatchStmtClass:break;
		case Stmt::StmtClass::CXXTryStmtClass:break;
		case Stmt::StmtClass::ExprClass:break;
		case Stmt::StmtClass::PredefinedExprClass:break;
		case Stmt::StmtClass::DeclRefExprClass:				processRefExpr(root);					break;
		case Stmt::StmtClass::IntegerLiteralClass:			processIntergerLiteral(root);			break;
		case Stmt::StmtClass::FloatingLiteralClass:			processFloatingLiteral(root);			break;
		case Stmt::StmtClass::ImaginaryLiteralClass:break;
		case Stmt::StmtClass::StringLiteralClass:			processStringLiteral(root);				break;
		case Stmt::StmtClass::CharacterLiteralClass:		processCharacterLiteral(root);			break;
		case Stmt::StmtClass::ParenExprClass:				processParenExpr(root);					break;
		case Stmt::StmtClass::UnaryOperatorClass:			processUnaryOperator(root);				break;
		case Stmt::StmtClass::SizeOfAlignOfExprClass:		processSizeOfAlignOfExpr(root);			break;
		case Stmt::StmtClass::ArraySubscriptExprClass:		processArraySubscriptExpr(root);		break;
		case Stmt::StmtClass::CallExprClass:				processCallExpr(root);					break;
		case Stmt::StmtClass::MemberExprClass:	break;
		case Stmt::StmtClass::CastExprClass:break;
		case Stmt::StmtClass::BinaryOperatorClass:			processBinaryOperator(root);			break;
		case Stmt::StmtClass::CompoundAssignOperatorClass:	processCompoundAssignOperator(root);	break;
		case Stmt::StmtClass::ConditionalOperatorClass:		processConditionalOperator(root);		break;
		case Stmt::StmtClass::ImplicitCastExprClass:break;
		case Stmt::StmtClass::ExplicitCastExprClass:break;
		case Stmt::StmtClass::CStyleCastExprClass:			processCStyleCastExpr(root);			break;
		case Stmt::StmtClass::CompoundLiteralExprClass:break;
		case Stmt::StmtClass::ExtVectorElementExprClass:break;
		case Stmt::StmtClass::InitListExprClass:break;
		case Stmt::StmtClass::DesignatedInitExprClass:break;
		case Stmt::StmtClass::ImplicitValueInitExprClass:break;
		case Stmt::StmtClass::ParenListExprClass:break;
		case Stmt::StmtClass::VAArgExprClass:break;
		case Stmt::StmtClass::AddrLabelExprClass:break;
		case Stmt::StmtClass::StmtExprClass:break;
		case Stmt::StmtClass::TypesCompatibleExprClass:break;
		case Stmt::StmtClass::ChooseExprClass:break;
		case Stmt::StmtClass::GNUNullExprClass:break;
		case Stmt::StmtClass::CXXOperatorCallExprClass:break;
		case Stmt::StmtClass::CXXMemberCallExprClass:break;
		case Stmt::StmtClass::CXXNamedCastExprClass:break;
		case Stmt::StmtClass::CXXStaticCastExprClass:break;
		case Stmt::StmtClass::CXXDynamicCastExprClass:break;
		case Stmt::StmtClass::CXXReinterpretCastExprClass:break;
		case Stmt::StmtClass::CXXConstCastExprClass:break;
		case Stmt::StmtClass::CXXFunctionalCastExprClass:break;
		case Stmt::StmtClass::CXXTypeidExprClass:break;
		case Stmt::StmtClass::CXXBoolLiteralExprClass:break;
		case Stmt::StmtClass::CXXNullPtrLiteralExprClass:break;
		case Stmt::StmtClass::CXXThisExprClass:break;
		case Stmt::StmtClass::CXXThrowExprClass:break;
		case Stmt::StmtClass::CXXDefaultArgExprClass:break;
		case Stmt::StmtClass::CXXZeroInitValueExprClass:break;
		case Stmt::StmtClass::CXXConditionDeclExprClass:break;
		case Stmt::StmtClass::CXXNewExprClass:break;
		case Stmt::StmtClass::CXXDeleteExprClass:break;
		case Stmt::StmtClass::UnresolvedFunctionNameExprClass:break;
		case Stmt::StmtClass::UnaryTypeTraitExprClass:break;
		case Stmt::StmtClass::QualifiedDeclRefExprClass:break;
		case Stmt::StmtClass::UnresolvedDeclRefExprClass:break;
		case Stmt::StmtClass::TemplateIdRefExprClass:break;
		case Stmt::StmtClass::CXXConstructExprClass:break;
		case Stmt::StmtClass::CXXBindTemporaryExprClass:break;
		case Stmt::StmtClass::CXXExprWithTemporariesClass:break;
		case Stmt::StmtClass::CXXTemporaryObjectExprClass:break;
		case Stmt::StmtClass::CXXUnresolvedConstructExprClass:break;
		case Stmt::StmtClass::CXXUnresolvedMemberExprClass:break;
		case Stmt::StmtClass::ShuffleVectorExprClass:break;
		case Stmt::StmtClass::BlockExprClass:break;
		case Stmt::StmtClass::BlockDeclRefExprClass:break;
	}
}

void Printer::ASTPrinter::processCallExpr(std::shared_ptr<Stmt> &s)
{
	auto call = dynamic_pointer_cast<CallExpr>(s);
	printAST(call->getCallee());
	m_openHelper.getOutputStream() << "(";
	auto args = call->getArgs();
	// TODO: use child_begin/child_end instead
	unsigned argPosition = 0;
	for(auto it = ++args.begin(); it != args.end(); ++it, ++argPosition)
	{
		printAST(*it);
		// skip last ', '
		if(argPosition != call->getNumArgs() - 1)
			m_openHelper.getOutputStream() << ", ";
	}
	m_openHelper.getOutputStream() << ")";
}

void Printer::ASTPrinter::formatExprAsStmt(std::shared_ptr<Stmt> s)
{
	if(dynamic_pointer_cast<Expr>(s) && !noIntent)
		m_openHelper.getOutputStream() << indent();
	printAST(s);
	// if is a expr
	if(dynamic_pointer_cast<Expr>(s))
	{
		if(stmtInOneLine)
			m_openHelper.getOutputStream() << ";";
		else
			m_openHelper.getOutputStream() << ";\n";
	}
}

void Printer::ASTPrinter::processNullStmt(std::shared_ptr<Stmt> &s)
{
	if(noIntent)
		m_openHelper.getOutputStream() << ";";
	else
		m_openHelper.getOutputStream() << indent() << ";";
	if(!stmtInOneLine)
		m_openHelper.getOutputStream() << "\n";
}

void Printer::ASTPrinter::processCompoundStmt(std::shared_ptr<Stmt> &s)
{
	if(noIntent)
		m_openHelper.getOutputStream() << "{\n";
	else
		m_openHelper.getOutputStream() << indent() << "{\n";
	++indentNum;
	for(auto stmt = s->child_begin(); stmt != s->child_end(); ++stmt)
		formatExprAsStmt(*stmt);
	--indentNum;
	if(noIntent)
		m_openHelper.getOutputStream() << "}\n";
	else
		m_openHelper.getOutputStream() << indent() << "}\n";
}

void Printer::ASTPrinter::processUnaryOperator(std::shared_ptr<Stmt> &s)
{
	auto unary = dynamic_pointer_cast<UnaryOperator>(s);
	auto subExpr = unary->getSubExpr().lock();
	switch (unary->getOpcode())
	{
		case UnaryOperator::PostInc: printAST(subExpr);
			m_openHelper.getOutputStream() << "++";
			break;
		case UnaryOperator::PostDec: printAST(subExpr);
			m_openHelper.getOutputStream() << "--";
			break;
		case UnaryOperator::PreInc:
			m_openHelper.getOutputStream() << "++";
			printAST(subExpr);
			break;
		case UnaryOperator::PreDec:
			m_openHelper.getOutputStream() << "--";
			printAST(subExpr);
			break;
		case UnaryOperator::AddrOf:
			m_openHelper.getOutputStream() << "&";
			printAST(subExpr);
			break;
		case UnaryOperator::Deref:
			m_openHelper.getOutputStream() << "*";
			printAST(subExpr);
			break;
		case UnaryOperator::Plus:
			m_openHelper.getOutputStream() << "+";
			printAST(subExpr);
			break;
		case UnaryOperator::Minus:
			m_openHelper.getOutputStream() << "-";
			printAST(subExpr);
			break;
		case UnaryOperator::Not:
			m_openHelper.getOutputStream() << "~";
			printAST(subExpr);
			break;
		case UnaryOperator::LNot:
			m_openHelper.getOutputStream() << "!";
			printAST(subExpr);
			break;
		case UnaryOperator::Real:break;
		case UnaryOperator::Imag:break;
		case UnaryOperator::Extension:break;
		case UnaryOperator::OffsetOf:break;
	}
}

void Printer::ASTPrinter::processIntergerLiteral(std::shared_ptr<Stmt> &s)
{
	auto integer = dynamic_pointer_cast<IntegerLiteral>(s);
	m_openHelper.getOutputStream() << to_string(integer->getValue());
}

void Printer::ASTPrinter::processFloatingLiteral(std::shared_ptr<Stmt> &s)
{
	auto floating = dynamic_pointer_cast<FloatingLiteral>(s);
	m_openHelper.getOutputStream() << toString(floating->getValue());
}

void Printer::ASTPrinter::processBinaryOperator(std::shared_ptr<Stmt> &s)
{
	auto binary = dynamic_pointer_cast<BinaryOperator>(s);
	printAST(binary->getLHS().lock());
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
	printAST(binary->getRHS().lock());
}

void Printer::ASTPrinter::processCompoundAssignOperator(std::shared_ptr<Stmt> &s)
{
	auto binary = dynamic_pointer_cast<BinaryOperator>(s);
	printAST(binary->getLHS().lock());
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
	printAST(binary->getRHS().lock());
}

void Printer::ASTPrinter::processConditionalOperator(std::shared_ptr<Stmt> &s)
{
	auto condition = dynamic_pointer_cast<ConditionalOperator>(s);
	printAST(condition->getCond().lock());
	m_openHelper.getOutputStream() << "?";
	printAST(condition->getLHS().lock());
	m_openHelper.getOutputStream() << ":";
	printAST(condition->getRHS().lock());
}

void Printer::ASTPrinter::processContinueStmt(std::shared_ptr<Stmt> &s)
{
	m_openHelper.getOutputStream() << indent() << "continue;\n";
}

void Printer::ASTPrinter::processBreakStmt(std::shared_ptr<Stmt> &s)
{
	m_openHelper.getOutputStream() << indent() << "break;\n";
}

void Printer::ASTPrinter::processReturnStmt(std::shared_ptr<Stmt> &s)
{
	auto ret = dynamic_pointer_cast<ReturnStmt>(s);
	m_openHelper.getOutputStream() << indent() << "return ";
	if(ret->getRetValue().lock())
		printAST(ret->getRetValue().lock());
	m_openHelper.getOutputStream() << ";\n";
}

void Printer::ASTPrinter::processParenExpr(std::shared_ptr<Stmt> &s)
{
	auto paren = dynamic_pointer_cast<ParenExpr>(s);
	m_openHelper.getOutputStream() << "(";
	printAST(paren->getSubExpr().lock());
	m_openHelper.getOutputStream() << ")";
}

void Printer::ASTPrinter::processSizeOfAlignOfExpr(std::shared_ptr<Stmt> &s)
{
	m_openHelper.getOutputStream() << "sizeof ";
	auto sizeofExp = dynamic_pointer_cast<SizeOfAlignOfExpr>(s);
	if(sizeofExp->isArgumentType())
		;		// TODO: sizeof type
	else
		printAST(sizeofExp->getArgumentExpr().lock());
}

void Printer::ASTPrinter::processArraySubscriptExpr(std::shared_ptr<Stmt> &s)
{
	auto arr = dynamic_pointer_cast<ArraySubscriptExpr>(s);
	printAST(arr->getLHS().lock());
	m_openHelper.getOutputStream() << "[";
	printAST(arr->getRHS().lock());
	m_openHelper.getOutputStream() << "]";
}

void Printer::ASTPrinter::processStringLiteral(std::shared_ptr<Stmt> &s)
{
	auto literal = dynamic_pointer_cast<StringLiteral>(s);
	m_openHelper.getOutputStream() << string(literal->getStrData());
}

void Printer::ASTPrinter::processCharacterLiteral(std::shared_ptr<Stmt> &s)
{
	auto character = dynamic_pointer_cast<CharacterLiteral>(s);
	string output;
	switch(character->getValue())
	{
		case '\a':	output = "\\a";	break;
		case '\b':	output = "\\b";	break;
		case '\f':	output = "\\f";	break;
		case '\n':	output = "\\n";	break;
		case '\r':	output = "\\r";	break;
		case '\t':	output = "\\t";	break;
		case '\v':	output = "\\v";	break;
		case '\'':	output = "\\\'";break;
		case '\"':	output = "\\\"";break;
		case '\\':	output = "\\\\";break;
		case '\0':	output = "\\0";	break;
		default:	output = static_cast<char>(character->getValue());	break;
	}
	m_openHelper.getOutputStream() << "'" << output << "'";
}

void Printer::ASTPrinter::processWhileStmt(std::shared_ptr<Stmt> &s)
{
	auto whileStmt = dynamic_pointer_cast<WhileStmt>(s);
	m_openHelper.getOutputStream() << indent() << "while (";
	printAST(whileStmt->getCond().lock());
	m_openHelper.getOutputStream() << ")\n";
	formatExprAsStmt(whileStmt->getBody().lock());
}

void Printer::ASTPrinter::processIfStmt(std::shared_ptr<Stmt> &s, bool isElseCond)
{
	auto ifStmt = dynamic_pointer_cast<IfStmt>(s);
	if(isElseCond)
		m_openHelper.getOutputStream() << "if (";
	else
		m_openHelper.getOutputStream() << indent() << "if (";
	printAST(ifStmt->getCond().lock());
	m_openHelper.getOutputStream() << ")\n";
	auto thenBlock = ifStmt->getThen().lock();
	if(CompoundStmt::classof(thenBlock))
		printAST(thenBlock);
	else
	{
		indentNum++;
		formatExprAsStmt(thenBlock);
		indentNum--;
	}

	auto elseBlock = ifStmt->getElse().lock();
	if(elseBlock)
	{
		m_openHelper.getOutputStream() << indent() << "else ";
		if(IfStmt::classof(elseBlock))
			processIfStmt(elseBlock, true);
		else
		{
			m_openHelper.getOutputStream() << "\n";
			printAST(elseBlock);
		}
	}
}

void Printer::ASTPrinter::processDoStmt(std::shared_ptr<Stmt> &s)
{
	auto doStmt = dynamic_pointer_cast<DoStmt>(s);
	m_openHelper.getOutputStream() << indent() << "do\n";
	formatExprAsStmt(doStmt->getBody().lock());
	m_openHelper.getOutputStream() << "while (";
	printAST(doStmt->getCond().lock());
	m_openHelper.getOutputStream() << ")\n";
}

void Printer::ASTPrinter::processForStmt(std::shared_ptr<Stmt> &s)
{
	auto forStmt = dynamic_pointer_cast<ForStmt>(s);
	m_openHelper.getOutputStream() << indent() << "for (";
	stmtInOneLine = true;
	noIntent = true;
	formatExprAsStmt(forStmt->getInit().lock());
	formatExprAsStmt(forStmt->getCond().lock());
	stmtInOneLine = false;
	noIntent = false;
	if(forStmt->getInc().lock())
		printAST(forStmt->getInc().lock());
	m_openHelper.getOutputStream() << ")\n";
	if(CompoundStmt::classof(forStmt->getBody().lock()))
		formatExprAsStmt(forStmt->getBody().lock());
	else
	{
		++indentNum;
		formatExprAsStmt(forStmt->getBody().lock());
		--indentNum;
	}
}

void Printer::ASTPrinter::processCaseStmt(std::shared_ptr<Stmt> &s)
{
	auto caseStmt = dynamic_pointer_cast<CaseStmt>(s);
	m_openHelper.getOutputStream() << indent() << "case ";
	printAST(caseStmt->getLHS().lock());
	m_openHelper.getOutputStream() << ":\n";
	/*if(caseStmt->getSubStmt())
	{
		++m_indentNum;
		printAST(caseStmt->getSubStmt());
		--m_indentNum;
	}*/
}

void Printer::ASTPrinter::processRefExpr(std::shared_ptr<Stmt> &s)
{
	auto ref = dynamic_pointer_cast<DeclRefExpr>(s);
	m_openHelper.getOutputStream() << ref->getDecl().lock()->getNameAsString();
}

void Printer::ASTPrinter::processSwitchStmt(std::shared_ptr<Stmt> &s)
{
	auto switchStmt = dynamic_pointer_cast<SwitchStmt>(s);
	m_openHelper.getOutputStream() << indent() << "switch(";
	printAST(switchStmt->getCond().lock());
	m_openHelper.getOutputStream() << ")\n";
	printAST(switchStmt->getBody().lock());
}

void Printer::ASTPrinter::processDefaultStmt(std::shared_ptr<Stmt> &s)
{
	auto defaultStmt = dynamic_pointer_cast<DefaultStmt>(s);
	m_openHelper.getOutputStream() << indent() << "default:\n";
}

void Printer::ASTPrinter::processCStyleCastExpr(std::shared_ptr<Stmt> &s)
{
	auto caseExpr = dynamic_pointer_cast<CStyleCastExpr>(s);
	auto type = caseExpr->getType().lock();
	m_openHelper.getOutputStream() << "(";
	m_typePrinter.printTypePrefix(type);
	m_openHelper.getOutputStream() << ")";
	printAST(caseExpr->getSubExpr().lock());
}

// TODO: move type print into TypePinter
void Printer::ASTPrinter::processDeclStmt(std::shared_ptr<Stmt> &s)
{
	auto declStmt = dynamic_pointer_cast<DeclStmt>(s);
	auto declRef = declStmt->getDeclGroup().lock();
	if(!declRef)
	{
		/*m_openHelper.getOutputStream() << ";";*/
	}
	else if(declRef->isSingleDecl())
	{
		// normal(struct/variable...)
		if(dynamic_pointer_cast<VarDecl>(declRef->getSingleDecl().lock()))
		{
			auto decl = dynamic_pointer_cast<VarDecl>(declRef->getSingleDecl().lock());
			auto type = decl->getType().lock();
			if(!noIntent)
				m_openHelper.getOutputStream() << indent();
			m_typePrinter.printTypePrefix(type);
			m_openHelper.getOutputStream() << " " << decl->getNameAsString();
			m_typePrinter.printTypePostfix(type);
			if(decl->getInitExpr().lock())
			{
				m_openHelper.getOutputStream() << " = ";
				printAST(decl->getInitExpr().lock());
			}
			m_openHelper.getOutputStream() << ";";
			if (!stmtInOneLine)
				m_openHelper.getOutputStream() << "\n";
		}
		else if(dynamic_pointer_cast<FunctionDecl>(declRef->getSingleDecl().lock()))
		{
			auto decl = dynamic_pointer_cast<FunctionDecl>(declRef->getSingleDecl().lock());
			auto type = decl->getType().lock();
			if(!noIntent)
				m_openHelper.getOutputStream() << indent();
			m_typePrinter.printTypePrefix(type);
			m_openHelper.getOutputStream() << " " << decl->getNameAsString();
			m_typePrinter.printTypePostfix(type);
			m_openHelper.getOutputStream() << ";";
			if (!stmtInOneLine)
				m_openHelper.getOutputStream() << "\n";
		}
		else if(dynamic_pointer_cast<TypedefDecl>(declRef->getSingleDecl().lock()))	// typedef
		{
			auto decl = dynamic_pointer_cast<TypedefDecl>(declRef->getSingleDecl().lock());
			auto type = dynamic_pointer_cast<TypedefType>(decl->getTypeForDecl().lock()->getTypePtr());
			if(!noIntent)
				m_openHelper.getOutputStream() << indent();
			m_openHelper.getOutputStream() << "typedef ";
			m_typePrinter.printTypePrefix(type->getDeclForType().lock());
			m_openHelper.getOutputStream() << " " << decl->getNameAsString();
			m_typePrinter.printTypePostfix(type->getDeclForType().lock());
			m_openHelper.getOutputStream() << ";";
			if (!stmtInOneLine)
				m_openHelper.getOutputStream() << "\n";
		}
	}
	else if(declRef->isDeclGroup())
	{
		auto decls = declRef->getDeclGroup().lock();
		// normal(struct/variable...)
		if(dynamic_pointer_cast<VarDecl>((*decls)[0]))
		{
			auto type = dynamic_pointer_cast<VarDecl>((*decls)[0])->getType().lock();
			if(!noIntent)
				m_openHelper.getOutputStream() << indent();
			m_typePrinter.printTypePrefix(type);
			m_openHelper.getOutputStream() << " ";
			for (unsigned i = 0; i < decls->size(); ++i)
			{
				auto varDecl = dynamic_pointer_cast<VarDecl>((*decls)[i]);
				type = varDecl->getType().lock();
				// skip first type infix
				if (i != 0)
					m_typePrinter.printTypeInfix(type);
				m_openHelper.getOutputStream() << varDecl->getNameAsString();
				m_typePrinter.printTypePostfix(type);
				if(varDecl->getInitExpr().lock())
				{
					m_openHelper.getOutputStream() << " = ";
					printAST(varDecl->getInitExpr().lock());
				}
				// skip last ','
				if (i + 1 != decls->size())
					m_openHelper.getOutputStream() << ", ";
			}
			if (stmtInOneLine)
				m_openHelper.getOutputStream() << ";";
			else
				m_openHelper.getOutputStream() << ";\n";
		}
		else	// typedef
		{
			auto type = dynamic_pointer_cast<TypedefType>(
					dynamic_pointer_cast<TypedefDecl>((*decls)[0])->getTypeForDecl().lock()->getTypePtr()
			);
			if(!noIntent)
				m_openHelper.getOutputStream() << indent();
			m_openHelper.getOutputStream() << "typedef ";
			m_typePrinter.printTypePrefix(type->getDeclForType().lock());
			m_openHelper.getOutputStream() << " ";
			for (unsigned i = 0; i < decls->size(); ++i)
			{
				type = dynamic_pointer_cast<TypedefType>(
						dynamic_pointer_cast<TypedefDecl>((*decls)[i])->getTypeForDecl().lock()->getTypePtr()
				);
				// skip first
				if (i != 0)
					m_typePrinter.printTypeInfix(type->getDeclForType().lock());
				m_openHelper.getOutputStream() << dynamic_pointer_cast<TypedefDecl>((*decls)[i])->getNameAsString();
				m_typePrinter.printTypePostfix(type->getDeclForType().lock());
				if (i + 1 != decls->size())
					m_openHelper.getOutputStream() << ", ";
			}
			if (stmtInOneLine)
				m_openHelper.getOutputStream() << ";";
			else
				m_openHelper.getOutputStream() << ";\n";
		}
	}
}

void Printer::ASTPrinter::processCommentStmt(std::shared_ptr<Stmt> &s)
{
    auto ptr = dynamic_pointer_cast<CommentStmt>(s);
    m_openHelper.getOutputStream() <<indent()<< ptr->getComment() << "\n";
}
