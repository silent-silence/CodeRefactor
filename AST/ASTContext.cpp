#include "ASTContext.h"
#include "AST/Expr.h"

using std::get;
using std::queue;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

void ASTContext::createStmt(Stmt::StmtClass type, std::vector<var_t> &value){
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

void ASTContext::createType(Type::TypeClass type, vector<ASTContext::var_t> value)
{
    switch (type) {
    case Type::ExtQual:
        createExtQualType(value);
        break;
    case Type::QualifiedName:
        break;
    default:break;
    }
}

void ASTContext::createDeclStmt(std::vector<var_t> &value)
{
    queue_stmt.push(
                make_shared<DeclStmt>(
                    get<SourceLocation>(value[0]),
                get<SourceLocation>(value[1])));
}

void ASTContext::createNullStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<NullStmt>(
                    get<SourceLocation>(value[0])));
}

void ASTContext::createCompoundStmt(std::vector<ASTContext::var_t> &value)
{
    vector<shared_ptr<Stmt>> stmts;
    for(auto i=get<unsigned>(value[0]);i!=0;i++){
        stmts.push_back(pop_stmt());
    }
    queue_stmt.push(
                make_shared<CompoundStmt>(
                    stmts,
                    get<SourceLocation>(value[0]),
                get<SourceLocation>(value[1])));
}

void ASTContext::createCaseStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<CaseStmt>(
                    pop_stmt<Expr>(),
                    pop_stmt<Expr>(),
                    get<SourceLocation>(value[0]),
                get<SourceLocation>(value[1]),
            get<SourceLocation>(value[2])));
}

void ASTContext::createDefaultStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<DefaultStmt>(
                    get<SourceLocation>(value[0]),
                get<SourceLocation>(value[1]),
            pop_stmt()));
}

void ASTContext::createLabelStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<LabelStmt>(
                    get<SourceLocation>(value[0]),
                pop_stmt()));
}

void ASTContext::createIfStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<IfStmt>(
                    get<SourceLocation>(value[0]),
                pop_stmt<Expr>(),
                pop_stmt(),
                value[1].emplace<SourceLocation>(SourceLocation())));
}

void ASTContext::createSwitchStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(make_shared<SwitchStmt>(pop_stmt<Expr>()));
}

void ASTContext::createWhileStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<WhileStmt>(
                    pop_stmt<Expr>(),
                    pop_stmt(),
                    get<SourceLocation>(value[0])));
}

void ASTContext::createDoStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<DoStmt>(
                    pop_stmt(),
                    pop_stmt<Expr>(),
                    get<SourceLocation>(value[0]),
                get<SourceLocation>(value[1]),
            get<SourceLocation>(value[2])));
}

void ASTContext::createForStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<ForStmt>(
                    pop_stmt(),
                    pop_stmt<Expr>(),
                    pop_stmt<Expr>(),
                    pop_stmt(),
                    get<SourceLocation>(value[0]),
                get<SourceLocation>(value[1]),
            get<SourceLocation>(value[2])));
}

void ASTContext::createGotoStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<GotoStmt>(
                    pop_stmt<LabelStmt>(),
                    get<SourceLocation>(value[0]),
                get<SourceLocation>(value[1])));
}

void ASTContext::createIndirectGotoStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<IndirectGotoStmt>(
                    get<SourceLocation>(value[0]),
                get<SourceLocation>(value[1]),
            pop_stmt<Expr>()));
}

void ASTContext::createContinueStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<ContinueStmt>(
                    get<SourceLocation>(value[0])));
}

void ASTContext::createBreakStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<BreakStmt>(
                    get<SourceLocation>(value[0])));
}

void ASTContext::createReturnStmt(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<ReturnStmt>(
                    get<SourceLocation>(value[0]),pop_stmt<Expr>()));
}

void ASTContext::createDeclRefExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<DeclRefExpr>(
                    pop_stmt<NamedDecl>(),//error
                    get<QualType>(value[0]),
                get<SourceLocation>(value[1]),
            get<bool>(value[2]),
            get<bool>(value[3])));
}

void ASTContext::createPredefinedExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<PredefinedExpr>(
                    get<SourceLocation>(value[0]),
                get<QualType>(value[1]),
            get<PredefinedExpr::IdentType>(value[2])));
}

void ASTContext::createIntegerLiteral(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<IntegerLiteral>(
                    get<int>(value[0]),
                get<QualType>(value[1]),
            get<SourceLocation>(value[2])));
}

void ASTContext::createCharacterLiteral(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<CharacterLiteral>(
                    get<unsigned>(value[0]),
                get<bool>(value[1]),
            get<QualType>(value[2]),
            get<SourceLocation>(value[3])));
}

void ASTContext::createFloatingLiteral(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<FloatingLiteral>(
                    get<float>(value[0]),
                get<bool>(value[1]),
            get<QualType>(value[2]),
            get<SourceLocation>(value[3])));
}

void ASTContext::createImaginaryLiteral(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<ImaginaryLiteral>(
                    pop_stmt<Expr>(),
                    get<QualType>(value[0])));
}

void ASTContext::createStringLiteral(std::vector<ASTContext::var_t> &value)
{
    shared_ptr<Stmt> stmt;
    if(value.size()==5)
        stmt=StringLiteral::Create(
                    get<char *>(value[0]),
                get<unsigned>(value[1]),
                get<bool>(value[2]),
                get<QualType>(value[3]),
                get<SourceLocation>(value[4]));
    else
        stmt=StringLiteral::Create(
                    get<char *>(value[0]),
                get<unsigned>(value[1]),
                get<bool>(value[2]),
                get<QualType>(value[3]),
                get<SourceLocation>(value[4]),
                get<unsigned>(value[5]));
    queue_stmt.push(stmt);
}

void ASTContext::createParenExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<ParenExpr>(
                    get<SourceLocation>(value[0]),
                get<SourceLocation>(value[1]),
            pop_stmt<Expr>()));
}

void ASTContext::createUnaryOperator(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<UnaryOperator>(
                    pop_stmt<Expr>(),
                    get<UnaryOperator::Opcode>(value[0]),
                get<QualType>(value[1]),
            get<SourceLocation>(value[2])));
}

void ASTContext::createSizeOfAlignOfExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<SizeOfAlignOfExpr>(
                    get<bool>(value[0]),
                get<QualType>(value[1]),
            get<QualType>(value[2]),
            get<SourceLocation>(value[3]),
            get<SourceLocation>(value[4])));
}

void ASTContext::createArraySubscriptExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<ArraySubscriptExpr>(
                    pop_stmt<Expr>(),
                    pop_stmt<Expr>(),
                    get<QualType>(value[0]),
                get<SourceLocation>(value[1])));
}

void ASTContext::createCallExpr(std::vector<ASTContext::var_t> &value)
{
    vector<shared_ptr<Expr>> exprs;
    for(auto i=get<unsigned>(value[0]);i!=0;i++){
        exprs.push_back(pop_stmt<Expr>());
    }
    queue_stmt.push(
                make_shared<CallExpr>(
                    pop_stmt<Expr>(),
                    exprs,
                    get<unsigned>(value[1]),
                get<QualType>(value[2]),
            get<SourceLocation>(value[3])));
}

void ASTContext::createMemberExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<MemberExpr>(
                    pop_stmt<Expr>(),
                    get<bool>(value[0]),
                pop_stmt<NamedDecl>(),
                get<SourceLocation>(value[1]),
            get<QualType>(value[2])));
}

void ASTContext::createCompoundLiteralExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<CompoundLiteralExpr>(
                    get<SourceLocation>(value[0]),
                get<QualType>(value[1]),
            pop_stmt<Expr>(),
            get<bool>(value[2])));
}

void ASTContext::createImplicitCastExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<ImplicitCastExpr>(
                    get<QualType>(value[0]),
                static_cast<CastExpr::CastKind>(get<int>(value[1])),
            pop_stmt<Expr>(),
            get<bool>(value[2])));
}

void ASTContext::createCStyleCastExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<CStyleCastExpr>(
                    get<QualType>(value[0]),
                static_cast<CastExpr::CastKind>(get<int>(value[1])),
            pop_stmt<Expr>(),
            get<QualType>(value[2]),
            get<SourceLocation>(value[3]),
            get<SourceLocation>(value[4])));
}

void ASTContext::createBinaryOperator(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<BinaryOperator>(
                    pop_stmt<Expr>(),
                    pop_stmt<Expr>(),
                    get<BinaryOperator::Opcode>(value[0]),
                get<QualType>(value[1]),
            get<SourceLocation>(value[2])));
}

void ASTContext::createCompoundAssignOperator(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<CompoundAssignOperator>(
                    pop_stmt<Expr>(),
                    pop_stmt<Expr>(),
                    get<CompoundAssignOperator::Opcode>(value[0]),
                get<QualType>(value[1]),
            get<QualType>(value[2]),
            get<QualType>(value[3]),
            get<SourceLocation>(value[4])));
}

void ASTContext::createConditionalOperator(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<ConditionalOperator>(
                    pop_stmt<Expr>(),
                    pop_stmt<Expr>(),
                    pop_stmt<Expr>(),
                    get<QualType>(value[0])));
}

void ASTContext::createAddrLabelExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<AddrLabelExpr>(
                    get<SourceLocation>(value[0]),
                get<SourceLocation>(value[1]),
            pop_stmt<LabelStmt>(),
            get<QualType>(value[2])));
}

void ASTContext::createStmtExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<StmtExpr>(
                    pop_stmt<CompoundStmt>(),
                    get<QualType>(value[0]),
                get<SourceLocation>(value[1]),
            get<SourceLocation>(value[2])));
}

void ASTContext::createTypesCompatibleExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
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
        exprs.push_back(pop_stmt<Expr>());
    }
    queue_stmt.push(
                make_shared<ShuffleVectorExpr>(
                    exprs,
                    get<unsigned>(value[1]),
                get<QualType>(value[2]),
            get<SourceLocation>(value[3]),
            get<SourceLocation>(value[4])));
}

void ASTContext::createChooseExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<ChooseExpr>(
                    get<SourceLocation>(value[0]),
                pop_stmt<Expr>(),
                pop_stmt<Expr>(),
                pop_stmt<Expr>(),
                get<QualType>(value[1]),
            get<SourceLocation>(value[2])));
}

void ASTContext::createGNUNullExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<GNUNullExpr>(
                    get<QualType>(value[0]),
                get<SourceLocation>(value[1])));
}

void ASTContext::createVAArgExpr(std::vector<ASTContext::var_t> &value)
{
    queue_stmt.push(
                make_shared<VAArgExpr>(
                    get<SourceLocation>(value[0]),
                pop_stmt<Expr>(),
                get<QualType>(value[1]),
            get<SourceLocation>(value[2])));
}

void ASTContext::createInitListExpr(std::vector<ASTContext::var_t> &value)
{
    vector<shared_ptr<Expr>> exprs;
    for(auto i=get<unsigned>(value[0]);i!=0;i++){
        exprs.push_back(pop_stmt<Expr>());
    }
    queue_stmt.push(
                make_shared<InitListExpr>(
                    get<SourceLocation>(value[0]),
                exprs,
                get<unsigned>(value[1]),
            get<SourceLocation>(value[2])));
}

void ASTContext::createParenListExpr(std::vector<ASTContext::var_t> &value)
{
    vector<shared_ptr<Expr>> exprs;
    for(auto i=get<unsigned>(value[0]);i!=0;i++){
        exprs.push_back(pop_stmt<Expr>());
    }
    queue_stmt.push(
                make_shared<ParenListExpr>(
                    get<SourceLocation>(value[0]),
                exprs,
                get<unsigned>(value[1]),
            get<SourceLocation>(value[2])));
}

void ASTContext::createExtQualType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<ExtQualType>(
                    pop_type(),
                    get<QualType>(value[0]),
                get<unsigned>(value[1]),
            static_cast<QualType::GCAttrTypes>(get<int>(value[2]))));
}
void ASTContext::createBuiltinType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<BuiltinType>(
             get<BuiltinType::Kind>(value[0])));
}
void ASTContext::createFixedWidthIntType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<FixedWidthIntType>(
                    get<unsigned>(value[0]),
                get<bool>(value[1])));
}
void ASTContext::createComplexType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<ComplexType>(
                    get<QualType>(value[0]),
                get<QualType>(value[1])));
}
void ASTContext::createPointerType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<PointerType>(
                    get<QualType>(value[0]),
                get<QualType>(value[1])));
}
void ASTContext::createBlockPointerType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<BlockPointerType>(
                    get<QualType>(value[0]),
                get<QualType>(value[1])));
}
void ASTContext::createReferenceType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<ReferenceType>(
                    get<Type::TypeClass>(value[0]),
                get<QualType>(value[1]),
            get<QualType>(value[2])));
}
void ASTContext::createLValueReferenceType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<LValueReferenceType>(
                    get<QualType>(value[0]),
                get<QualType>(value[1])));
}
void ASTContext::createRValueReferenceType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<RValueReferenceType>(
                    get<QualType>(value[0]),
                get<QualType>(value[1])));
}
void ASTContext::createMemberPointerType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<MemberPointerType>(
                    get<QualType>(value[0]),
                    pop_type(),
                    get<QualType>(value[1])));
}
void ASTContext::createConstantArrayType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<ConstantArrayType>(
                    get<QualType>(value[0]),
                    get<QualType>(value[1]),
            get<int>(value[2]),
            get<ArrayType::ArraySizeModifier>(value[3]),
            get<unsigned>(value[4])));
}
void ASTContext::createConstantArrayWithExprType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<ConstantArrayWithExprType>(
                    get<QualType>(value[0]),
                    get<QualType>(value[1]),
            get<int>(value[2]),
            pop_stmt<Expr>(),
            get<ArrayType::ArraySizeModifier>(value[3]),
            get<unsigned>(value[4])));
}
void ASTContext::createConstantArrayWithoutExprType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<ConstantArrayWithoutExprType>(
                    get<QualType>(value[0]),
                    get<QualType>(value[1]),
            get<int>(value[2]),
            get<ArrayType::ArraySizeModifier>(value[3]),
            get<unsigned>(value[4])));
}
void ASTContext::createIncompleteArrayType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<IncompleteArrayType>(
                    get<QualType>(value[0]),
                    get<QualType>(value[1]),
            get<ArrayType::ArraySizeModifier>(value[2]),
            get<unsigned>(value[3])));
}
void ASTContext::createVariableArrayType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<VariableArrayType>(
                    get<QualType>(value[0]),
                    get<QualType>(value[1]),
            pop_stmt<Expr>(),
            get<ArrayType::ArraySizeModifier>(value[2]),
            get<unsigned>(value[3])));
}
void ASTContext::createDependentSizedArrayType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<DependentSizedArrayType>(
                    get<QualType>(value[0]),
                    get<QualType>(value[1]),
            pop_stmt<Expr>(),
            get<ArrayType::ArraySizeModifier>(value[2]),
            get<unsigned>(value[3])));
}
void ASTContext::createVectorType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<VectorType>(
                    get<QualType>(value[0]),
                get<unsigned>(value[1]),
            get<QualType>(value[2])));
}

void ASTContext::createExtVectorType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<ExtVectorType>(
                    get<QualType>(value[0]),
                get<unsigned>(value[1]),
            get<QualType>(value[2])));
}

void ASTContext::createFunctionNoProtoType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<FunctionNoProtoType>(
                    get<QualType>(value[0]),
                get<QualType>(value[1]),
            get<bool>(value[2])));
}

void ASTContext::createFunctionProtoType(std::vector<ASTContext::var_t> &value)
{
//    queue_type.push(
//                make_shared<FunctionProtoType>(
//                    get<QualType>(value[0]),
//                get<QualType>(value[1]),
    //            get<bool>(value[2])));
}

void ASTContext::createTypeOfExprType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<TypeOfExprType>(
                    pop_stmt<Expr>(),
                    get<QualType>(value[0])));
}

void ASTContext::createDependentTypeOfExprType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<DependentTypeOfExprType>(
                    pop_stmt<Expr>()));
}

void ASTContext::createTypeOfType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<TypeOfType>(
                    get<QualType>(value[0]),
                get<QualType>(value[1])));
}

void ASTContext::createDecltypeType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<DecltypeType>(
                    pop_stmt<Expr>(),
                    get<QualType>(value[0]),
                get<QualType>(value[1])));
}

void ASTContext::createDependentDecltypeType(std::vector<ASTContext::var_t> &value)
{
    queue_type.push(
                make_shared<DependentDecltypeType>(
                    pop_stmt<Expr>()));
}
