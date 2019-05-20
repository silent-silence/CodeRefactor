#include "ASTContext.h"
#include "AST/Expr.h"

using std::get;
using std::queue;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

void ASTContext::createStmt(Stmt::StmtClass type, std::vector<var_t> &value){
	shared_ptr<Stmt> stmt;
	vector<shared_ptr<Stmt>> stmts;
	vector<shared_ptr<Expr>> exprs;
	switch (type) {
		case Stmt::DeclStmtClass://
			createDeclStmt(value);
			break;
		case Stmt::NullStmtClass:
			createNullStmt(value);
			break;
		case Stmt::CompoundStmtClass:
			createCompoundStmt(value);
			break;
		case Stmt::CaseStmtClass:
			createCaseStmt(value);
			break;
		case Stmt::DefaultStmtClass:
			createDefaultStmt(value);
			break;
		case Stmt::LabelStmtClass:
			createLabelStmt(value);
			break;
		case Stmt::IfStmtClass:
			createIfStmt(value);
			break;
		case Stmt::SwitchStmtClass:
			createSwitchStmt(value);
			break;
		case Stmt::WhileStmtClass:
			createWhileStmt(value);
			break;
		case Stmt::DoStmtClass:
			createDoStmt(value);
			break;
		case Stmt::ForStmtClass:
			createForStmt(value);
			break;
		case Stmt::GotoStmtClass:
			createGotoStmt(value);
			break;
		case Stmt::IndirectGotoStmtClass:
			createIndirectGotoStmt(value);
			break;
		case Stmt::ContinueStmtClass:
			createContinueStmt(value);
			break;
		case Stmt::BreakStmtClass:
			createBreakStmt(value);
			break;
		case Stmt::ReturnStmtClass:
			createReturnStmt(value);
			break;
		case Expr::DeclRefExprClass:
			createDeclRefExpr(value);
			break;
		case Expr::PredefinedExprClass:
			createPredefinedExpr(value);
			break;
		case Expr::IntegerLiteralClass:
			createIntegerLiteral(value);
			break;
		case Expr::CharacterLiteralClass:
			createCharacterLiteral(value);
			break;
		case Expr::FloatingLiteralClass:
			createFloatingLiteral(value);
			break;
		case Expr::ImaginaryLiteralClass:
			createImaginaryLiteral(value);
			break;
		case Expr::StringLiteralClass:
			createStringLiteral(value);
			break;
		case Expr::ParenExprClass:
			createParenExpr(value);
			break;
		case Expr::UnaryOperatorClass:
			createUnaryOperator(value);
			break;
		case Expr::SizeOfAlignOfExprClass:
			createSizeOfAlignOfExpr(value);
			break;
		case Expr::ArraySubscriptExprClass:
			createArraySubscriptExpr(value);
			break;
		case Expr::CallExprClass:
			createCallExpr(value);
			break;
		case Expr::MemberExprClass:
			createMemberExpr(value);
			break;
		case Expr::CompoundLiteralExprClass:
			createCompoundLiteralExpr(value);
			break;
		case Expr::ImplicitCastExprClass:
			createImplicitCastExpr(value);
			break;
		case Expr::CStyleCastExprClass:
			createCStyleCastExpr(value);
			break;
		case Expr::BinaryOperatorClass:
			createBinaryOperator(value);
			break;
		case Expr::CompoundAssignOperatorClass:
			createCompoundAssignOperator(value);
			break;
		case Expr::ConditionalOperatorClass:
			createConditionalOperator(value);
			break;
		case Expr::AddrLabelExprClass:
			createAddrLabelExpr(value);
			break;
		case Expr::StmtExprClass:
			createStmtExpr(value);
			break;
		case Expr::TypesCompatibleExprClass:
			createTypesCompatibleExpr(value);
			break;
		case Expr::ShuffleVectorExprClass:
			createShuffleVectorExpr(value);
			break;
		case Expr::ChooseExprClass:
			createChooseExpr(value);
			break;
		case Expr::GNUNullExprClass:
			createGNUNullExpr(value);
			break;
		case Expr::VAArgExprClass:
			createVAArgExpr(value);
			break;
		case Expr::InitListExprClass:
			createInitListExpr(value);
			break;
		case Expr::ParenListExprClass:
			createParenListExpr(value);
			break;
		default:break;
	}
}

std::weak_ptr<Stmt> ASTContext::getTop() const
{
	return queue.front();
}

void ASTContext::cleanAST()
{
	queue = std::queue<std::shared_ptr<Stmt>>();
}

void ASTContext::createType(Type::TypeClass type, vector<ASTContext::var_t> value)
{

}

void ASTContext::createDeclStmt(std::vector<var_t> &value)
{
	queue.push(
			make_shared<DeclStmt>(
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1])));
}

void ASTContext::createNullStmt(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<NullStmt>(
					get<SourceLocation>(value[0])));
}

void ASTContext::createCompoundStmt(std::vector<ASTContext::var_t> &value)
{
	vector<shared_ptr<Stmt>> stmts;
	for(auto i = 0; i != get<unsigned>(value[0]); i++){
		stmts.push_back(pop_back());
	}
	queue.push(
			make_shared<CompoundStmt>(
					stmts,
					get<SourceLocation>(value[1]),
					get<SourceLocation>(value[2])));
}

void ASTContext::createCaseStmt(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<CaseStmt>(
					pop_back<Expr>(),
					pop_back<Expr>(),
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					get<SourceLocation>(value[2])));
}

void ASTContext::createDefaultStmt(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<DefaultStmt>(
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					pop_back()));
}

void ASTContext::createLabelStmt(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<LabelStmt>(
					get<SourceLocation>(value[0]),
					pop_back()));
}

void ASTContext::createIfStmt(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<IfStmt>(
					get<SourceLocation>(value[0]),
					pop_back<Expr>(),
					pop_back(),
					value[1].emplace<SourceLocation>(SourceLocation())));
}

void ASTContext::createSwitchStmt(std::vector<ASTContext::var_t> &value)
{
	queue.push(make_shared<SwitchStmt>(pop_back<Expr>()));
}

void ASTContext::createWhileStmt(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<WhileStmt>(
					pop_back<Expr>(),
					pop_back(),
					get<SourceLocation>(value[0])));
}

void ASTContext::createDoStmt(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<DoStmt>(
					pop_back(),
					pop_back<Expr>(),
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					get<SourceLocation>(value[2])));
}

void ASTContext::createForStmt(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<ForStmt>(
					pop_back(),
					pop_back<Expr>(),
					pop_back<Expr>(),
					pop_back(),
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					get<SourceLocation>(value[2])));
}

void ASTContext::createGotoStmt(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<GotoStmt>(
					pop_back<LabelStmt>(),
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1])));
}

void ASTContext::createIndirectGotoStmt(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<IndirectGotoStmt>(
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					pop_back<Expr>()));
}

void ASTContext::createContinueStmt(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<ContinueStmt>(
					get<SourceLocation>(value[0])));
}

void ASTContext::createBreakStmt(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<BreakStmt>(
					get<SourceLocation>(value[0])));
}

void ASTContext::createReturnStmt(std::vector<ASTContext::var_t> &value)
{
	// TODO: Dose return has any expression
	if(get<bool>(value[1]))
		pop_back();
	queue.push(
			make_shared<ReturnStmt>(
					get<SourceLocation>(value[0])));
}

void ASTContext::createDeclRefExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<DeclRefExpr>(
					pop_back<NamedDecl>(),//error
					get<QualType>(value[0]),
					get<SourceLocation>(value[1]),
					get<bool>(value[2]),
					get<bool>(value[3])));
}

void ASTContext::createPredefinedExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<PredefinedExpr>(
					get<SourceLocation>(value[0]),
					get<QualType>(value[1]),
					get<PredefinedExpr::IdentType>(value[2])));
}

void ASTContext::createIntegerLiteral(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<IntegerLiteral>(
					get<int>(value[0]),
					get<QualType>(value[1]),
					get<SourceLocation>(value[2])));
}

void ASTContext::createCharacterLiteral(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<CharacterLiteral>(
					get<unsigned>(value[0]),
					get<bool>(value[1]),
					get<QualType>(value[2]),
					get<SourceLocation>(value[3])));
}

void ASTContext::createFloatingLiteral(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<FloatingLiteral>(
					get<float>(value[0]),
					get<bool>(value[1]),
					get<QualType>(value[2]),
					get<SourceLocation>(value[3])));
}

void ASTContext::createImaginaryLiteral(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<ImaginaryLiteral>(
					pop_back<Expr>(),
					get<QualType>(value[0])));
}

void ASTContext::createStringLiteral(std::vector<ASTContext::var_t> &value)
{
	shared_ptr<Stmt> stmt;
	if(value.size()==5)
		stmt=StringLiteral::Create(
				get<char *>(value[0]),
				get<unsigned long>(value[1]),
				get<bool>(value[2]),
				get<QualType>(value[3]),
				get<SourceLocation>(value[4]));
	else
		stmt=StringLiteral::Create(
				get<char *>(value[0]),
				get<unsigned long>(value[1]),
				get<bool>(value[2]),
				get<QualType>(value[3]),
				get<SourceLocation>(value[4]),
				get<unsigned>(value[5]));
	queue.push(stmt);
}

void ASTContext::createParenExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<ParenExpr>(
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					pop_back<Expr>()));
}

void ASTContext::createUnaryOperator(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<UnaryOperator>(
					pop_back<Expr>(),
					get<UnaryOperator::Opcode>(value[0]),
					get<QualType>(value[1]),
					get<SourceLocation>(value[2])));
}

void ASTContext::createSizeOfAlignOfExpr(std::vector<ASTContext::var_t> &value)
{
	//TODO: get types here
	queue.push(
			make_shared<SizeOfAlignOfExpr>(
					get<bool>(value[0]),
					get<QualType>(value[1]),
					get<QualType>(value[2]),
					get<SourceLocation>(value[3]),
					get<SourceLocation>(value[4])));
}

void ASTContext::createArraySubscriptExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<ArraySubscriptExpr>(
					pop_back<Expr>(),
					pop_back<Expr>(),
					get<QualType>(value[0]),
					get<SourceLocation>(value[1])));
}

void ASTContext::createCallExpr(std::vector<ASTContext::var_t> &value)
{
	vector<shared_ptr<Expr>> exprs;
	for(auto i=get<unsigned>(value[0]);i!=0;i++){
		exprs.push_back(pop_back<Expr>());
	}
	queue.push(
			make_shared<CallExpr>(
					pop_back<Expr>(),
					exprs,
					get<unsigned>(value[1]),
					get<QualType>(value[2]),
					get<SourceLocation>(value[3])));
}

void ASTContext::createMemberExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<MemberExpr>(
					pop_back<Expr>(),
					get<bool>(value[0]),
					pop_back<NamedDecl>(),
					get<SourceLocation>(value[1]),
					get<QualType>(value[2])));
}

void ASTContext::createCompoundLiteralExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<CompoundLiteralExpr>(
					get<SourceLocation>(value[0]),
					get<QualType>(value[1]),
					pop_back<Expr>(),
					get<bool>(value[2])));
}

void ASTContext::createImplicitCastExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<ImplicitCastExpr>(
					get<QualType>(value[0]),
					static_cast<CastExpr::CastKind>(get<int>(value[1])),
					pop_back<Expr>(),
					get<bool>(value[2])));
}

void ASTContext::createCStyleCastExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<CStyleCastExpr>(
					get<QualType>(value[0]),
					static_cast<CastExpr::CastKind>(get<int>(value[1])),
					pop_back<Expr>(),
					get<QualType>(value[2]),
					get<SourceLocation>(value[3]),
					get<SourceLocation>(value[4])));
}

void ASTContext::createBinaryOperator(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<BinaryOperator>(
					pop_back<Expr>(),
					pop_back<Expr>(),
					get<BinaryOperator::Opcode>(value[0]),
					get<QualType>(value[1]),
					get<SourceLocation>(value[2])));
}

void ASTContext::createCompoundAssignOperator(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<CompoundAssignOperator>(
					pop_back<Expr>(),
					pop_back<Expr>(),
					get<CompoundAssignOperator::Opcode>(value[0]),
					get<QualType>(value[1]),
					get<QualType>(value[2]),
					get<QualType>(value[3]),
					get<SourceLocation>(value[4])));
}

void ASTContext::createConditionalOperator(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<ConditionalOperator>(
					pop_back<Expr>(),
					pop_back<Expr>(),
					pop_back<Expr>(),
					get<QualType>(value[0])));
}

void ASTContext::createAddrLabelExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<AddrLabelExpr>(
					get<SourceLocation>(value[0]),
					get<SourceLocation>(value[1]),
					pop_back<LabelStmt>(),
					get<QualType>(value[2])));
}

void ASTContext::createStmtExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<StmtExpr>(
					pop_back<CompoundStmt>(),
					get<QualType>(value[0]),
					get<SourceLocation>(value[1]),
					get<SourceLocation>(value[2])));
}

void ASTContext::createTypesCompatibleExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<TypesCompatibleExpr>(
					get<QualType>(value[0]),
					get<SourceLocation>(value[1]),
					get<QualType>(value[2]),
					get<QualType>(value[3]),
					get<SourceLocation>(value[4])));
}

void ASTContext::createShuffleVectorExpr(std::vector<ASTContext::var_t> &value)
{
	vector<shared_ptr<Expr>> exprs;
	for(auto i=get<unsigned>(value[0]);i!=0;i++){
		exprs.push_back(pop_back<Expr>());
	}
	queue.push(
			make_shared<ShuffleVectorExpr>(
					exprs,
					get<unsigned>(value[1]),
					get<QualType>(value[2]),
					get<SourceLocation>(value[3]),
					get<SourceLocation>(value[4])));
}

void ASTContext::createChooseExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<ChooseExpr>(
					get<SourceLocation>(value[0]),
					pop_back<Expr>(),
					pop_back<Expr>(),
					pop_back<Expr>(),
					get<QualType>(value[1]),
					get<SourceLocation>(value[2])));
}

void ASTContext::createGNUNullExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<GNUNullExpr>(
					get<QualType>(value[0]),
					get<SourceLocation>(value[1])));
}

void ASTContext::createVAArgExpr(std::vector<ASTContext::var_t> &value)
{
	queue.push(
			make_shared<VAArgExpr>(
					get<SourceLocation>(value[0]),
					pop_back<Expr>(),
					get<QualType>(value[1]),
					get<SourceLocation>(value[2])));
}

void ASTContext::createInitListExpr(std::vector<ASTContext::var_t> &value)
{
	/*vector<shared_ptr<Expr>> exprs;
	for(auto i=get<unsigned>(value[0]);i!=0;i++){
		exprs.push_back(pop_back<Expr>());
	}
	queue.push(
			make_shared<InitListExpr>(
					get<SourceLocation>(value[0]),
					exprs,
					get<unsigned>(value[1]),
					get<SourceLocation>(value[2])));*/
}

void ASTContext::createParenListExpr(std::vector<ASTContext::var_t> &value)
{
	/*vector<shared_ptr<Expr>> exprs;
	for(auto i=get<unsigned>(value[0]);i!=0;i++){
		exprs.push_back(pop_back<Expr>());
	}
	queue.push(
			make_shared<ParenListExpr>(
					get<SourceLocation>(value[0]),
					exprs,
					get<unsigned>(value[1]),
					get<SourceLocation>(value[2])));*/
}
