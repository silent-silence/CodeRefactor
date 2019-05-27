#include "Expr.h"

using std::vector;
using std::shared_ptr;

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

DeclRefExpr::DeclRefExpr(std::shared_ptr<NamedDecl> d, QualType t, SourceLocation l)
    :Expr(DeclRefExprClass, t), D(d), Loc(l)
{}

DeclRefExpr::DeclRefExpr(std::shared_ptr<NamedDecl> d, QualType t, SourceLocation l, bool TD, bool VD)
    :Expr(DeclRefExprClass, t, TD, VD), D(d), Loc(l)
{}

DeclRefExpr::DeclRefExpr(Stmt::EmptyShell Empty)
    :Expr(DeclRefExprClass,Empty)
{}

DeclRefExpr::DeclRefExpr(Stmt::StmtClass SC,
                         shared_ptr<NamedDecl> d,
                         QualType t,
                         SourceLocation l)
    :Expr(SC, t), D(d), Loc(l)
{}

DeclRefExpr::DeclRefExpr(Stmt::StmtClass SC,
                         shared_ptr<NamedDecl> d,
                         QualType t,
                         SourceLocation l, bool TD, bool VD)
    :Expr(SC, t, TD, VD), D(d), Loc(l)
{}

PredefinedExpr::PredefinedExpr(SourceLocation l, QualType type, IdentType IT)
    :Expr (PredefinedExprClass,type), Loc{l}, Type(IT)
{}

PredefinedExpr::PredefinedExpr(EmptyShell Empty)
    :Expr (PredefinedExprClass, Empty)
{}


IntegerLiteral::IntegerLiteral(const int &V, QualType type, SourceLocation l)
    :Expr (IntegerLiteralClass,type),Value(V), Loc(l)
{

}

IntegerLiteral::IntegerLiteral(EmptyShell Empty):Expr (IntegerLiteralClass,Empty)
{}

CharacterLiteral::CharacterLiteral(unsigned value, bool iswide, QualType type, SourceLocation l)
    :Expr (CharacterLiteralClass,type), Value(value), Loc(l), IsWide(iswide)
{}

CharacterLiteral::CharacterLiteral(EmptyShell Empty)
    :Expr (CharacterLiteralClass,Empty)
{}

FloatingLiteral::FloatingLiteral(const float &V, bool isexact, QualType Type, SourceLocation L)
    :Expr (FloatingLiteralClass,Type), Value(V), IsExact(isexact), Loc(L)
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

std::shared_ptr<StringLiteral> StringLiteral::Create(const std::string StrData,
                                                     unsigned ByteLength,
                                                     bool Wide, QualType Ty,
                                                     const SourceLocation Loc,
                                                     unsigned NumStrs)
{
	// TODO: maybe move this into constructor
	struct PtrMaker : public StringLiteral {
		PtrMaker(QualType Ty) : StringLiteral(Ty) {}
		static std::shared_ptr<StringLiteral> make(QualType Ty) {
			return std::make_shared<PtrMaker>(Ty);
		}
	};
	auto stringLiteral = PtrMaker::make(Ty);
	stringLiteral->StrData = StrData;
	stringLiteral->ByteLength = ByteLength;
	stringLiteral->IsWide = Wide;
	stringLiteral->NumConcatenated = NumStrs;
	stringLiteral->TokLocs = Loc;
	return stringLiteral;
}

std::shared_ptr<StringLiteral> StringLiteral::Create(const std::string StrData, unsigned ByteLength, bool Wide, QualType Ty, SourceLocation Loc)
{
    return Create(StrData, ByteLength, Wide, Ty, Loc, 1);
}

StringLiteral::StringLiteral(QualType Ty)
    :Expr (StringLiteralClass,Ty)
{}

ParenExpr::ParenExpr(SourceLocation l, SourceLocation r, std::shared_ptr<Expr> val)
    :Expr (ParenExprClass,val->getType(),val->isTypeDependent(),val->isValueDependent()),
      L(l), R(r),Val(val)
{}

ParenExpr::ParenExpr(EmptyShell Empty)
    :Expr (ParenExprClass,Empty)
{}

UnaryOperator::UnaryOperator(std::shared_ptr<Expr> input, Opcode opc, QualType type, SourceLocation l)
    :Expr (UnaryOperatorClass,type,input->isTypeDependent()&&opc !=OffsetOf,input->isValueDependent()),
      Val(input), Opc(opc), Loc(l)
{}

UnaryOperator::UnaryOperator(EmptyShell Empty)
    :Expr (UnaryOperatorClass,Empty)
{}

ArraySubscriptExpr::ArraySubscriptExpr(shared_ptr<Expr> lhs,
                                       shared_ptr<Expr> rhs,
                                       QualType t,
                                       SourceLocation rbracketloc)
    :Expr (ArraySubscriptExprClass, t,
           lhs->isTypeDependent()||rhs->isTypeDependent(),
           lhs->isValueDependent()||rhs->isValueDependent()),
      RBracketLoc(rbracketloc)
{
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
}

ArraySubscriptExpr::ArraySubscriptExpr(EmptyShell Shell)
    :Expr (ArraySubscriptExprClass,Shell)
{}

CallExpr::CallExpr(shared_ptr<Expr> fn,
                   vector<shared_ptr<Expr> > args,
                   unsigned numargs,
                   QualType t,
                   SourceLocation rparenloc)
    : Expr(CallExprClass, t), NumArgs(numargs)
{
    //SubExprs = new (C) Stmt*[numargs+1];
    SubExprs.push_back(fn);
    for (unsigned i = 0; i != numargs; ++i)
        SubExprs.push_back(args[i]);
    RParenLoc = rparenloc;
}


CallExpr::CallExpr(StmtClass SC, EmptyShell Empty)
    :Expr (SC,Empty)
{}

CallExpr::CallExpr(Stmt::StmtClass SC,
                   shared_ptr<Expr> fn,
                   vector<shared_ptr<Expr> > args,
                   unsigned numargs,
                   QualType t,
                   SourceLocation rparenloc)
    : Expr(SC, t), NumArgs(numargs)
{
    //SubExprs = new (C) Stmt*[numargs+1];
    SubExprs[FN] = fn;
    for (unsigned i = 0; i != numargs; ++i)
        SubExprs[i+ARGS_START] = args[i];

    RParenLoc = rparenloc;
}

MemberExpr::MemberExpr(std::shared_ptr<Expr> base, bool isarrow, std::shared_ptr<NamedDecl> memberdecl, SourceLocation l, QualType ty)
    :Expr (MemberExprClass,ty,base->isTypeDependent(),base->isValueDependent()),
      Base(base), MemberDecl(memberdecl), MemberLoc(l), IsArrow(isarrow)
{}

MemberExpr::MemberExpr(EmptyShell Empty)
    :Expr (MemberExprClass,Empty)
{}

CompoundLiteralExpr::CompoundLiteralExpr(SourceLocation lparenloc, QualType ty, std::shared_ptr<Expr> init, bool fileScope)
    :Expr (CompoundLiteralExprClass,ty), LParenLoc(lparenloc), Init(init), FileScope(fileScope)
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

CStyleCastExpr::CStyleCastExpr(QualType exprTy, CastKind kind, std::shared_ptr<Expr> op, QualType writtenTy, SourceLocation l, SourceLocation r) :
    ExplicitCastExpr(CStyleCastExprClass, exprTy, kind, op, writtenTy),
    LPLoc(l), RPLoc(r)
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

InitListExpr::InitListExpr(SourceLocation lbraceloc,
                           vector<shared_ptr<Expr> > initexprs,
                           unsigned numinits,
                           SourceLocation rbraceloc)
    : Expr(InitListExprClass, QualType()),
      LBraceLoc(lbraceloc), RBraceLoc(rbraceloc), SyntacticForm(0),
      HadArrayRangeDesignator(false)
{
    for(unsigned i=0;i!=numinits;i++){
        InitExprs.push_back(initexprs[i]);
    }
}

InitListExpr::InitListExpr(Stmt::EmptyShell Empty)
    : Expr(InitListExprClass, Empty) { }

ImplicitValueInitExpr::ImplicitValueInitExpr(QualType ty)
    : Expr(ImplicitValueInitExprClass, ty) { }

ImplicitValueInitExpr::ImplicitValueInitExpr(Stmt::EmptyShell Empty)
    : Expr(ImplicitValueInitExprClass, Empty) { }


ExtVectorElementExpr::ExtVectorElementExpr(Stmt::EmptyShell Empty)
    : Expr(ExtVectorElementExprClass, Empty) { }

SizeOfAlignOfExpr::SizeOfAlignOfExpr(bool issizeof,
                                     QualType T,
                                     QualType resultType,
                                     SourceLocation op,
                                     SourceLocation rp)
    : Expr(SizeOfAlignOfExprClass, resultType, false, T->isDependentType()),
      isSizeof(issizeof), isType(true), OpLoc(op), RParenLoc(rp)
{
    //Ty = T.ggetAsOpaquePtr();
}

SizeOfAlignOfExpr::SizeOfAlignOfExpr(bool issizeof,
                                     shared_ptr<Expr> E,
                                     QualType resultType,
                                     SourceLocation op,
                                     SourceLocation rp)
    : Expr(SizeOfAlignOfExprClass, resultType, false, E->isTypeDependent()),
      isSizeof(issizeof), isType(false), OpLoc(op), RParenLoc(rp)
{
    Ex = E;
}

ParenListExpr::ParenListExpr(SourceLocation lparenloc,
                             vector<shared_ptr<Expr> > exprs,
                             unsigned numexprs,
                             SourceLocation rparenloc)
    : Expr(ParenListExprClass, QualType()),
      NumExprs(numexprs), LParenLoc(lparenloc), RParenLoc(rparenloc)
{
    for (unsigned i = 0; i != numexprs; ++i)
        Exprs.push_back(exprs[i]);
}
