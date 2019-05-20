#include "Expr.h"
//line 144 151
Expr::Expr(StmtClass SC, QualType T)
    :Stmt (SC), TypeDependent(false), ValueDependent(false)
{
    setType(T);
}

Expr::Expr(Stmt::StmtClass SC, QualType T, bool TD, bool VD)
    :Stmt (SC), TypeDependent(TD), ValueDependent(VD)
{
    setType(T);
}

Expr::Expr(Stmt::StmtClass SC, Stmt::EmptyShell)
    :Stmt(SC)
{
}

QualType Expr::getType() const
{
    return TR;
}

void Expr::setType(const QualType &value)
{
    TR = value;
}

bool Expr::isTypeDependent() const
{
    return TypeDependent;
}

void Expr::setTypeDependent(bool value)
{
    TypeDependent = value;
}

bool Expr::isValueDependent() const
{
    return ValueDependent;
}

void Expr::setValueDependent(bool value)
{
    ValueDependent = value;
}

DeclRefExpr::DeclRefExpr(QualType t):
    Expr (DeclRefExprClass,t)
{}

DeclRefExpr::DeclRefExpr(QualType t, bool TD, bool VD):Expr(DeclRefExprClass,t,TD,VD)
{
}

DeclRefExpr::DeclRefExpr(Stmt::EmptyShell Empty):Expr(DeclRefExprClass,Empty)
{}

DeclRefExpr::DeclRefExpr(StmtClass SC, QualType t):Expr (SC,t)
{}

DeclRefExpr::DeclRefExpr(StmtClass SC, QualType t, bool TD, bool VD):Expr (SC,t,TD,VD)
{}

PredefinedExpr::PredefinedExpr(QualType type, IdentType IT):Expr (PredefinedExprClass,type),Type(IT)
{}

PredefinedExpr::PredefinedExpr(EmptyShell Empty):Expr (PredefinedExprClass,Empty)
{}

IntegerLiteral::IntegerLiteral(const int &V, QualType type):Expr (IntegerLiteralClass,type),Value(V)
{}

IntegerLiteral::IntegerLiteral(EmptyShell Empty):Expr (IntegerLiteralClass,Empty)
{}

CharacterLiteral::CharacterLiteral(unsigned value, bool iswide, QualType type)
    :Expr (CharacterLiteralClass,type),Value(value),IsWide(iswide)
{}

CharacterLiteral::CharacterLiteral(EmptyShell Empty)
    :Expr (CharacterLiteralClass,Empty)
{}

FloatingLiteral::FloatingLiteral(const float &V, bool isexact, QualType Type)
    :Expr (FloatingLiteralClass,Type),Value(V),IsExact(isexact)
{}

FloatingLiteral::FloatingLiteral(EmptyShell Empty)
    :Expr (FloatingLiteralClass,Empty)
{}
ImaginaryLiteral::ImaginaryLiteral(std::shared_ptr<Expr> val, QualType Ty)
    :Expr (ImaginaryLiteralClass,Ty),Val(val)
{}

ImaginaryLiteral::ImaginaryLiteral(EmptyShell Empty)
    :Expr (ImaginaryLiteralClass,Empty)
{}

StringLiteral::StringLiteral(QualType Ty)
    :Expr (StringLiteralClass,Ty)
{}

ParenExpr::ParenExpr(std::shared_ptr<Expr> val)
    :Expr (ParenExprClass,val->getType(),val->isTypeDependent(),val->isValueDependent())
{}

ParenExpr::ParenExpr(EmptyShell Empty)
    :Expr (ParenExprClass,Empty)
{}

UnaryOperator::UnaryOperator(std::shared_ptr<Expr> input, Opcode opc, QualType type)
    :Expr (UnaryOperatorClass,type,input->isTypeDependent()&&opc !=OffsetOf,input->isValueDependent())
{}

UnaryOperator::UnaryOperator(EmptyShell Empty)
    :Expr (UnaryOperatorClass,Empty)
{}

SizeOfAlignOfExpr::SizeOfAlignOfExpr(bool issizeof, QualType T, QualType resultType)
    :Expr (SizeOfAlignOfExprClass,resultType,false,T->isDependentType()),isSizeof(issizeof),isType(true)
{}

SizeOfAlignOfExpr::SizeOfAlignOfExpr(bool issizeof, std::shared_ptr<Expr> E, QualType resultType)
    :Expr (SizeOfAlignOfExprClass,resultType,false,E->isTypeDependent())
{}
SizeOfAlignOfExpr::SizeOfAlignOfExpr(EmptyShell Empty)
    :Expr (SizeOfAlignOfExprClass,Empty)
{}

ArraySubscriptExpr::ArraySubscriptExpr(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, QualType t)
    :Expr (ArraySubscriptExprClass,t,lhs->isTypeDependent()||rhs->isTypeDependent(),lhs->isValueDependent()||rhs->isValueDependent())
{
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
}

ArraySubscriptExpr::ArraySubscriptExpr(EmptyShell Shell)
    :Expr (ArraySubscriptExprClass,Shell)
{}

CallExpr::CallExpr(std::shared_ptr<Expr> fn, std::shared_ptr<Expr> *args, unsigned numargs, QualType t)
    :Expr (CallExprClass,t,fn->isTypeDependent(),fn->isValueDependent()),NumArgs(numargs)
{}//资料里该部分没有写 ：之后的部分，看是否需要写，以及args赋值给谁

CallExpr::CallExpr(StmtClass SC, EmptyShell Empty)
    :Expr (SC,Empty)
{}
CallExpr::CallExpr(StmtClass SC, std::shared_ptr<Expr> fn, std::shared_ptr<Expr> *args, unsigned numargs, QualType t)
    :Expr (SC,t,fn->isTypeDependent(),fn->isValueDependent()),NumArgs(numargs)
{}//同上

MemberExpr::MemberExpr(std::shared_ptr<Expr> base, bool isarrow, QualType ty)
    :Expr (MemberExprClass,ty,base->isTypeDependent(),base->isValueDependent()),IsArrow(isarrow)
{}

MemberExpr::MemberExpr(EmptyShell Empty)
    :Expr (MemberExprClass,Empty)
{}

CompoundLiteralExpr::CompoundLiteralExpr(QualType ty, std::shared_ptr<Expr> init, bool fileScope)
    :Expr (CompoundLiteralExprClass,ty),Init(init),FileScope(fileScope)
{}
CompoundLiteralExpr::CompoundLiteralExpr(EmptyShell Empty)
    :Expr (CompoundLiteralExprClass,Empty)
{}

CastExpr::CastExpr(StmtClass SC, QualType ty, const CastInfo &info, std::shared_ptr<Expr>op)
    :Expr (SC,ty,ty->isDependentType(),ty->isDependentType()||(op&&op->isValueDependent())),Kind(info.Kind),Op(op)
{}

CastExpr::CastExpr(StmtClass SC, EmptyShell Empty)
  : Expr(SC, Empty)
{ }

ImplicitCastExpr::ImplicitCastExpr(QualType ty, const CastInfo &info, std::shared_ptr<Expr>op, bool Lvalue)
    : CastExpr(ImplicitCastExprClass, ty, info, op), LvalueCast(Lvalue)
{}

ImplicitCastExpr::ImplicitCastExpr(EmptyShell Shell)
      : CastExpr(ImplicitCastExprClass, Shell) { }

ExplicitCastExpr::ExplicitCastExpr(StmtClass SC, QualType exprTy, const CastInfo &info,
                                   std::shared_ptr<Expr>op, QualType writtenTy)
    : CastExpr(SC, exprTy, info, op), TypeAsWritten(writtenTy)
{}

ExplicitCastExpr::ExplicitCastExpr(StmtClass SC, EmptyShell Shell)
  : CastExpr(SC, Shell)
{ }

CStyleCastExpr::CStyleCastExpr(QualType exprTy, CastKind kind, std::shared_ptr<Expr> op, QualType writtenTy) :
  ExplicitCastExpr(CStyleCastExprClass, exprTy, kind, op, writtenTy)
 {}

CStyleCastExpr::CStyleCastExpr(EmptyShell Shell)
      : ExplicitCastExpr(CStyleCastExprClass, Shell)
{ }

BinaryOperator::BinaryOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, BinaryOperator::Opcode opc, QualType ResTy, SourceLocation opLoc)
    : Expr(BinaryOperatorClass, ResTy,
           lhs->isTypeDependent() || rhs->isTypeDependent(),
           lhs->isValueDependent() || rhs->isValueDependent()),
      Opc(opc), OpLoc(opLoc)
{
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
}


BinaryOperator::BinaryOperator(Stmt::EmptyShell Empty)
    : Expr(BinaryOperatorClass, Empty), Opc(Comma) { }

BinaryOperator::BinaryOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, BinaryOperator::Opcode opc, QualType ResTy, SourceLocation oploc, bool dead)
    : Expr(CompoundAssignOperatorClass, ResTy), Opc(opc), OpLoc(oploc)
{
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
}

BinaryOperator::BinaryOperator(Stmt::StmtClass SC, Stmt::EmptyShell Empty)
    : Expr(SC, Empty), Opc(MulAssign) { }

CompoundAssignOperator::CompoundAssignOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, BinaryOperator::Opcode opc, QualType ResType, QualType CompLHSType, QualType CompResultType, SourceLocation OpLoc)
    : BinaryOperator(lhs, rhs, opc, ResType, OpLoc, true),
      ComputationLHSType(CompLHSType),
      ComputationResultType(CompResultType)
{
}

CompoundAssignOperator::CompoundAssignOperator(Stmt::EmptyShell Empty)
    : BinaryOperator(CompoundAssignOperatorClass, Empty) { }

ConditionalOperator::ConditionalOperator(std::shared_ptr<Expr> cond, std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, QualType t)
    : Expr(ConditionalOperatorClass, t,
           ((lhs && lhs->isTypeDependent()) || (rhs && rhs->isTypeDependent())),
           (cond->isValueDependent() ||
            (lhs && lhs->isValueDependent()) ||
            (rhs && rhs->isValueDependent())))
{
    SubExprs[COND] = cond;
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
}

ConditionalOperator::ConditionalOperator(Stmt::EmptyShell Empty)
    : Expr(ConditionalOperatorClass, Empty) { }

AddrLabelExpr::AddrLabelExpr(SourceLocation AALoc, SourceLocation LLoc, std::shared_ptr<LabelStmt> L, QualType t)
    : Expr(AddrLabelExprClass, t), AmpAmpLoc(AALoc), LabelLoc(LLoc), Label(L) {}

AddrLabelExpr::AddrLabelExpr(Stmt::EmptyShell Empty)
    : Expr(AddrLabelExprClass, Empty) { }

StmtExpr::StmtExpr(std::shared_ptr<CompoundStmt> substmt, QualType T, SourceLocation lp, SourceLocation rp) :
    Expr(StmtExprClass, T), SubStmt(substmt),  LParenLoc(lp), RParenLoc(rp) { }

StmtExpr::StmtExpr(Stmt::EmptyShell Empty)
    : Expr(StmtExprClass, Empty) { }

TypesCompatibleExpr::TypesCompatibleExpr(QualType ReturnType, SourceLocation BLoc, QualType t1, QualType t2, SourceLocation RP)
    : Expr(TypesCompatibleExprClass, ReturnType), Type1(t1), Type2(t2), BuiltinLoc(BLoc), RParenLoc(RP) {}

TypesCompatibleExpr::TypesCompatibleExpr(Stmt::EmptyShell Empty)
    : Expr(TypesCompatibleExprClass, Empty) { }

ShuffleVectorExpr::ShuffleVectorExpr(std::vector<std::shared_ptr<Expr> > args, unsigned nexpr, QualType Type, SourceLocation BLoc, SourceLocation RP)
    : Expr(ShuffleVectorExprClass, Type), BuiltinLoc(BLoc), RParenLoc(RP), NumExprs(nexpr)
{
    SubExprs.resize(nexpr);
    for (unsigned i = 0; i < nexpr; i++)
        SubExprs[i] = args[i];
}

ShuffleVectorExpr::ShuffleVectorExpr(Stmt::EmptyShell Empty)
    : Expr(ShuffleVectorExprClass, Empty), SubExprs(0) { }

ChooseExpr::ChooseExpr(SourceLocation BLoc, std::shared_ptr<Expr> cond, std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, QualType t, SourceLocation RP)
    : Expr(ChooseExprClass, t),
      BuiltinLoc(BLoc), RParenLoc(RP)
{
    SubExprs[COND] = cond;
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
}

ChooseExpr::ChooseExpr(Stmt::EmptyShell Empty)
    : Expr(ChooseExprClass, Empty) { }

GNUNullExpr::GNUNullExpr(QualType Ty, SourceLocation Loc)
    : Expr(GNUNullExprClass, Ty), TokenLoc(Loc) { }

GNUNullExpr::GNUNullExpr(Stmt::EmptyShell Empty)
    : Expr(GNUNullExprClass, Empty) { }

VAArgExpr::VAArgExpr(SourceLocation BLoc, std::shared_ptr<Expr> e, QualType t, SourceLocation RPLoc)
    : Expr(VAArgExprClass, t),
      Val(e),
      BuiltinLoc(BLoc),
      RParenLoc(RPLoc)
{ }

VAArgExpr::VAArgExpr(Stmt::EmptyShell Empty)
    : Expr(VAArgExprClass, Empty) { }

InitListExpr::InitListExpr(Stmt::EmptyShell Empty)
    : Expr(InitListExprClass, Empty) { }

ImplicitValueInitExpr::ImplicitValueInitExpr(QualType ty)
    : Expr(ImplicitValueInitExprClass, ty) { }

ImplicitValueInitExpr::ImplicitValueInitExpr(Stmt::EmptyShell Empty)
    : Expr(ImplicitValueInitExprClass, Empty) { }


ExtVectorElementExpr::ExtVectorElementExpr(Stmt::EmptyShell Empty)
    : Expr(ExtVectorElementExprClass, Empty) { }
