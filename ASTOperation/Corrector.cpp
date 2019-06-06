#include "Corrector.h"
#include "AST/Stmt.h"
#include "AST/Expr.h"
#include "OpenHelper/OpenHelper.h"

#include <exception>

using std::exception;

using std::weak_ptr;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

Corrector::Corrector(OpenHelper &openHelper)
    : m_openHelper{openHelper}
{}

void Corrector::correct(std::weak_ptr<Stmt> root)
{
//    auto s = TypeConversion(root);
//    switch(s->getStmtClass())
//        {
//            case Stmt::NoStmtClass:break;
//            case Stmt::NullStmtClass:				processNullStmt(s);					break;
//            case Stmt::CompoundStmtClass:			processCompoundStmt(s);				break;
//            case Stmt::CaseStmtClass:				processCaseStmt(s);					break;
//            case Stmt::DefaultStmtClass:break;
//            case Stmt::LabelStmtClass:break;
//            case Stmt::IfStmtClass:					processIfStmt(s);					break;
//            case Stmt::SwitchStmtClass:break;
//            case Stmt::WhileStmtClass:				processWhileStmt(s);				break;
//            case Stmt::DoStmtClass:					processDoStmt(s);					break;
//            case Stmt::ForStmtClass:				processForStmt(s);					break;
//            case Stmt::GotoStmtClass:break;
//            case Stmt::IndirectGotoStmtClass:break;
//            case Stmt::ContinueStmtClass:			processContinueStmt(s);				break;
//            case Stmt::BreakStmtClass:				processBreakStmt(s);				break;
//            case Stmt::ReturnStmtClass:				processReturnStmt(s);				break;
//            case Stmt::DeclStmtClass:break;
//            case Stmt::SwitchCaseClass:break;
//            case Stmt::AsmStmtClass:break;
//            case Stmt::CXXCatchStmtClass:break;
//            case Stmt::CXXTryStmtClass:break;
//            case Stmt::ExprClass:break;
//            case Stmt::PredefinedExprClass:break;
//            case Stmt::DeclRefExprClass:break;
//            case Stmt::IntegerLiteralClass:			processIntergerLiteral(s);			break;
//            case Stmt::FloatingLiteralClass:		processFloatingLiteral(s);			break;
//            case Stmt::ImaginaryLiteralClass:break;
//            case Stmt::StringLiteralClass:			processStringLiteral(s);			break;
//            case Stmt::CharacterLiteralClass:		processCharacterLiteral(s);			break;
//            case Stmt::ParenExprClass:				processParenExpr(s);				break;
//            case Stmt::UnaryOperatorClass:			processUnaryOperator(s);			break;
//            case Stmt::SizeOfAlignOfExprClass:		processSizeOfAlignOfExpr(s);		break;
//            case Stmt::ArraySubscriptExprClass:		processArraySubscriptExpr(s);		break;
//            case Stmt::CallExprClass:break;
//            case Stmt::MemberExprClass:	break;
//            case Stmt::CastExprClass:break;
//            case Stmt::BinaryOperatorClass:			processBinaryOperator(s);			break;
//            case Stmt::CompoundAssignOperatorClass:	processCompoundAssignOperator(s);	break;
//            case Stmt::ConditionalOperatorClass:	processConditionalOperator(s);		break;
//            case Stmt::ImplicitCastExprClass:break;
//            case Stmt::ExplicitCastExprClass:break;
//            case Stmt::CStyleCastExprClass:break;
//            case Stmt::CompoundLiteralExprClass:break;
//            case Stmt::ExtVectorElementExprClass:break;
//            case Stmt::InitListExprClass:break;
//            case Stmt::DesignatedInitExprClass:break;
//            case Stmt::ImplicitValueInitExprClass:break;
//            case Stmt::ParenListExprClass:break;
//            case Stmt::VAArgExprClass:break;
//            case Stmt::AddrLabelExprClass:break;
//            case Stmt::StmtExprClass:break;
//            case Stmt::TypesCompatibleExprClass:break;
//            case Stmt::ChooseExprClass:break;
//            case Stmt::GNUNullExprClass:break;
//            case Stmt::CXXOperatorCallExprClass:break;
//            case Stmt::CXXMemberCallExprClass:break;
//            case Stmt::CXXNamedCastExprClass:break;
//            case Stmt::CXXStaticCastExprClass:break;
//            case Stmt::CXXDynamicCastExprClass:break;
//            case Stmt::CXXReinterpretCastExprClass:break;
//            case Stmt::CXXConstCastExprClass:break;
//            case Stmt::CXXFunctionalCastExprClass:break;
//            case Stmt::CXXTypeidExprClass:break;
//            case Stmt::CXXBoolLiteralExprClass:break;
//            case Stmt::CXXNullPtrLiteralExprClass:break;
//            case Stmt::CXXThisExprClass:break;
//            case Stmt::CXXThrowExprClass:break;
//            case Stmt::CXXDefaultArgExprClass:break;
//            case Stmt::CXXZeroInitValueExprClass:break;
//            case Stmt::CXXConditionDeclExprClass:break;
//            case Stmt::CXXNewExprClass:break;
//            case Stmt::CXXDeleteExprClass:break;
//            case Stmt::UnresolvedFunctionNameExprClass:break;
//            case Stmt::UnaryTypeTraitExprClass:break;
//            case Stmt::QualifiedDeclRefExprClass:break;
//            case Stmt::UnresolvedDeclRefExprClass:break;
//            case Stmt::TemplateIdRefExprClass:break;
//            case Stmt::CXXConstructExprClass:break;
//            case Stmt::CXXBindTemporaryExprClass:break;
//            case Stmt::CXXExprWithTemporariesClass:break;
//            case Stmt::CXXTemporaryObjectExprClass:break;
//            case Stmt::CXXUnresolvedConstructExprClass:break;
//            case Stmt::CXXUnresolvedMemberExprClass:break;
//            case Stmt::ShuffleVectorExprClass:break;
//            case Stmt::BlockExprClass:break;
//            case Stmt::BlockDeclRefExprClass:break;
    //    }
}

void Corrector::processDeclRefExpr(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<DeclRefExpr>(s);
}

void Corrector::processIntergerLiteral(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<IntegerLiteral>(s);
}

void Corrector::processFloatingLiteral(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<FloatingLiteral>(s);
}

void Corrector::processImaginaryLiteral(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<ImaginaryLiteral>(s);
}

void Corrector::processStringLiteral(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<StringLiteral>(s);
}

void Corrector::processCharacterLiteral(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<CharacterLiteral>(s);
}

void Corrector::processParenExpr(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<ParenExpr>(s);
}

void Corrector::processUnaryOperator(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<UnaryOperator>(s);
}

void Corrector::processSizeOfAlignOfExpr(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<SizeOfAlignOfExpr>(s);
}

void Corrector::processArraySubscriptExpr(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<ArraySubscriptExpr>(s);
}

void Corrector::processCallExpr(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<CallExpr>(s);
}

void Corrector::processBinaryOperator(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<BinaryOperator>(s);
}

void Corrector::processCompoundAssignOperator(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<CompoundAssignOperator>(s);
}

void Corrector::processConditionalOperator(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<ConditionalOperator>(s);
}

void Corrector::processImplicitCastExpr(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<ImplicitCastExpr>(s);
}

void Corrector::processCStyleCastExpr(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<CStyleCastExpr>(s);
}

void Corrector::processVAArgExpr(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<VAArgExpr>(s);
}

void Corrector::processInitListExpr(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<InitListExpr>(s);
}

void Corrector::processParenListExpr(std::weak_ptr<Stmt> s)
{
    auto ptr = TypeConversion<ParenListExpr>(s);
}
