#include "Expr.h"

using std::array;
using std::vector;
using std::string;
using std::weak_ptr;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::reinterpret_pointer_cast;

Expr::Expr(StmtClass SC, shared_ptr<QualType> T)
    : Stmt (SC), TypeDependent(false), ValueDependent(false)
{
    setType(T);
}

Expr::Expr(Stmt::StmtClass SC, shared_ptr<QualType> T, bool TD, bool VD)
    : Stmt(SC), TypeDependent(TD), ValueDependent(VD)
{
    setType(T);
}

Expr::Expr(Stmt::StmtClass SC, Stmt::EmptyShell)
    : Stmt(SC)
{
}

weak_ptr<QualType> Expr::getType() const
{
    return TR;
}

void Expr::setType(const std::shared_ptr<QualType> &value)
{
    if(TR && (TR->isNull() || !TR->isRestrictQualified()))
        throw "Expressions can't have reference type";
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

bool Expr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() >= firstExprConstant &&
            T.lock()->getStmtClass() <= lastExprConstant;
}

bool Expr::classof(const weak_ptr<Expr>)
{
    return true;
}

DeclRefExpr::DeclRefExpr(shared_ptr<NamedDecl> d, shared_ptr<QualType> t, SourceLocation l)
    : Expr(DeclRefExprClass, t), D(d), Loc(l)
{}

DeclRefExpr::DeclRefExpr(shared_ptr<NamedDecl> d, shared_ptr<QualType> t,
                         SourceLocation l, bool TD, bool VD)
    : Expr(DeclRefExprClass, t, TD, VD), D(d), Loc(l)
{}

DeclRefExpr::DeclRefExpr(Stmt::EmptyShell Empty)
    : Expr(DeclRefExprClass,Empty)
{}

DeclRefExpr::DeclRefExpr(Stmt::StmtClass SC, shared_ptr<NamedDecl> d,
                         shared_ptr<QualType> t, SourceLocation l)
    :Expr(SC, t), D(d), Loc(l)
{}

DeclRefExpr::DeclRefExpr(Stmt::StmtClass SC, shared_ptr<NamedDecl> d,
                         shared_ptr<QualType> t, SourceLocation l,
                         bool TD, bool VD)
    :Expr(SC, t, TD, VD), D(d), Loc(l)
{}

Stmt::child_iterator DeclRefExpr::child_begin()
{
    return child_iterator();
}

Stmt::child_iterator DeclRefExpr::child_end()
{
    return child_iterator();
}

bool DeclRefExpr::classof(const weak_ptr<Stmt> T) {
    return T.lock()->getStmtClass() == DeclRefExprClass ||
            T.lock()->getStmtClass() == CXXConditionDeclExprClass ||
            T.lock()->getStmtClass() == QualifiedDeclRefExprClass;
}

bool DeclRefExpr::classof(const weak_ptr<DeclRefExpr>)
{
    return true;
}

weak_ptr<NamedDecl> DeclRefExpr::getDecl() { return D; }

void DeclRefExpr::setDecl(shared_ptr<NamedDecl> NewD)
{
    D = NewD;
}

SourceLocation DeclRefExpr::getLocation() const
{
    return Loc;
}

void DeclRefExpr::setLocation(SourceLocation L)
{
    Loc = L;
}

PredefinedExpr::PredefinedExpr(SourceLocation l, shared_ptr<QualType> type, IdentType IT)
    : Expr (PredefinedExprClass,type), Loc{l}, Type(IT)
{}

PredefinedExpr::PredefinedExpr(EmptyShell Empty)
    : Expr (PredefinedExprClass, Empty)
{}

Stmt::child_iterator PredefinedExpr::child_begin()
{
    return child_iterator();
}

Stmt::child_iterator PredefinedExpr::child_end()
{
    return child_iterator();
}

bool PredefinedExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == PredefinedExprClass;
}

bool PredefinedExpr::classof(const weak_ptr<PredefinedExpr>)
{
    return true;
}

PredefinedExpr::IdentType PredefinedExpr::getIdentType() const
{
    return Type;
}

void PredefinedExpr::setIdentType(PredefinedExpr::IdentType IT)
{
    Type = IT;
}

SourceLocation PredefinedExpr::getLocation() const
{
    return Loc;
}

void PredefinedExpr::setLocation(SourceLocation L)
{
    Loc = L;
}

IntegerLiteral::IntegerLiteral(const int &V, shared_ptr<QualType> type, SourceLocation l)
    : Expr (IntegerLiteralClass,type), Value(V), Loc(l)
{

}

IntegerLiteral::IntegerLiteral(EmptyShell Empty)
    : Expr (IntegerLiteralClass,Empty)
{}

Stmt::child_iterator IntegerLiteral::child_begin()
{
    return child_iterator();
}

Stmt::child_iterator IntegerLiteral::child_end()
{
    return child_iterator();
}

bool IntegerLiteral::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == IntegerLiteralClass;
}

bool IntegerLiteral::classof(const weak_ptr<IntegerLiteral>)
{
    return true;
}

const int &IntegerLiteral::getValue() const
{
    return Value;
}

SourceLocation IntegerLiteral::getLocation() const
{
    return Loc;
}

void IntegerLiteral::setValue(const int Val)
{
    Value = Val;
}

void IntegerLiteral::setLocation(SourceLocation Location)
{
    Loc = Location;
}

CharacterLiteral::CharacterLiteral(unsigned value, bool iswide,
                                   shared_ptr<QualType> type, SourceLocation l)
    :Expr (CharacterLiteralClass,type), Value(value), Loc(l), IsWide(iswide)
{}

CharacterLiteral::CharacterLiteral(EmptyShell Empty)
    :Expr (CharacterLiteralClass,Empty)
{}

Stmt::child_iterator CharacterLiteral::child_begin()
{
    return child_iterator();
}

Stmt::child_iterator CharacterLiteral::child_end()
{
    return child_iterator();
}

bool CharacterLiteral::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == CharacterLiteralClass;
}

bool CharacterLiteral::classof(const weak_ptr<CharacterLiteral>)
{
    return true;
}

SourceLocation CharacterLiteral::getLoc() const
{
    return Loc;
}

bool CharacterLiteral::isWide() const
{
    return IsWide;
}

unsigned CharacterLiteral::getValue() const
{
    return Value;
}

void CharacterLiteral::setLocation(SourceLocation Location)
{
    Loc = Location;
}

void CharacterLiteral::setWide(bool W)
{
    IsWide = W;
}

void CharacterLiteral::setValue(unsigned Val)
{
    Value = Val;
}

FloatingLiteral::FloatingLiteral(const float &V, bool isexact,
                                 shared_ptr<QualType> Type, SourceLocation L)
    : Expr (FloatingLiteralClass,Type), Value(V), IsExact(isexact), Loc(L)
{}

FloatingLiteral::FloatingLiteral(EmptyShell Empty)
    : Expr (FloatingLiteralClass,Empty)
{}

Stmt::child_iterator FloatingLiteral::child_begin()
{
    return child_iterator();
}

Stmt::child_iterator FloatingLiteral::child_end()
{
    return child_iterator();
}

bool FloatingLiteral::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == FloatingLiteralClass;
}

bool FloatingLiteral::classof(const weak_ptr<FloatingLiteral>)
{
    return true;
}

const float &FloatingLiteral::getValue() const
{
    return Value;
}

void FloatingLiteral::setValue(const float Val)
{
    Value = Val;
}

bool FloatingLiteral::isExact() const
{
    return IsExact;
}

void FloatingLiteral::setExact(bool E)
{
    IsExact = E;
}

SourceLocation FloatingLiteral::getLocation() const
{
    return Loc;
}

void FloatingLiteral::setLocation(SourceLocation L)
{
    Loc = L;
}

ImaginaryLiteral::ImaginaryLiteral(shared_ptr<Expr> val, shared_ptr<QualType> Ty)
    : Expr (ImaginaryLiteralClass,Ty), Val(val)
{}

ImaginaryLiteral::ImaginaryLiteral(EmptyShell Empty)
    :Expr (ImaginaryLiteralClass,Empty)
{}

Stmt::child_iterator ImaginaryLiteral::child_begin()
{
    return &Val;
}

Stmt::child_iterator ImaginaryLiteral::child_end()
{
    return &Val+1;
}

bool ImaginaryLiteral::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == ImaginaryLiteralClass;
}

bool ImaginaryLiteral::classof(const weak_ptr<ImaginaryLiteral>)
{
    return true;
}

weak_ptr<Expr> ImaginaryLiteral::getSubExpr()
{
    return dynamic_pointer_cast<Expr>(Val);
}

void ImaginaryLiteral::setSubExpr(shared_ptr<Expr> E)
{
    Val = E;
}

shared_ptr<StringLiteral> StringLiteral::Create(
        const string StrData, unsigned ByteLength, bool Wide,
        shared_ptr<QualType> Ty, const SourceLocation Loc, unsigned NumStrs)
{
    struct PtrMaker : public StringLiteral {
        PtrMaker(shared_ptr<QualType> Ty) : StringLiteral(Ty) {}
        static shared_ptr<StringLiteral> make(shared_ptr<QualType> Ty) {
            return make_shared<PtrMaker>(Ty);
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

shared_ptr<StringLiteral> StringLiteral::Create(
        const string StrData, unsigned ByteLength, bool Wide,
        shared_ptr<QualType> Ty, SourceLocation Loc)
{
    return Create(StrData, ByteLength, Wide, Ty, Loc, 1);
}

Stmt::child_iterator StringLiteral::child_begin()
{
    return child_iterator();
}

Stmt::child_iterator StringLiteral::child_end()
{
    return child_iterator();
}

bool StringLiteral::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StringLiteralClass;
}

bool StringLiteral::classof(const weak_ptr<StringLiteral>)
{
    return true;
}

StringLiteral::StringLiteral(shared_ptr<QualType> Ty)
    : Expr (StringLiteralClass,Ty)
{}

ParenExpr::ParenExpr(SourceLocation l, SourceLocation r, std::shared_ptr<Expr> val)
    :Expr (ParenExprClass, val->getType().lock(),
           val->isTypeDependent(), val->isValueDependent()),
      Val(val), L(l), R(r)
{}

ParenExpr::ParenExpr(EmptyShell Empty)
    :Expr (ParenExprClass,Empty)
{}

Stmt::child_iterator ParenExpr::child_begin()
{
    return &Val;
}

Stmt::child_iterator ParenExpr::child_end()
{
    return &Val+1;
}

bool ParenExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == ParenExprClass;
}

bool ParenExpr::classof(const weak_ptr<ParenExpr>)
{
    return true;
}

weak_ptr<Expr> ParenExpr::getSubExpr()
{
    return dynamic_pointer_cast<Expr>(Val);
}

void ParenExpr::setSubExpr(shared_ptr<Expr> E)
{
    Val = E;
}

SourceLocation ParenExpr::getLParen() const
{
    return L;
}

void ParenExpr::setLParen(SourceLocation Loc)
{
    L = Loc;
}

SourceLocation ParenExpr::getRParen() const
{
    return R;
}

void ParenExpr::setRParen(SourceLocation Loc)
{
    R = Loc;
}

UnaryOperator::UnaryOperator(shared_ptr<Expr> input, Opcode opc,
                             shared_ptr<QualType> type, SourceLocation l)
    : Expr (UnaryOperatorClass,type,input->isTypeDependent()&&opc !=OffsetOf,input->isValueDependent()),
      Val(input), Opc(opc), Loc(l)
{}

UnaryOperator::UnaryOperator(EmptyShell Empty)
    :Expr (UnaryOperatorClass,Empty)
{}

Stmt::child_iterator UnaryOperator::child_begin()
{
    return &Val;
}

Stmt::child_iterator UnaryOperator::child_end()
{
    return &Val+1;
}

bool UnaryOperator::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == UnaryOperatorClass;
}

bool UnaryOperator::classof(const weak_ptr<UnaryOperator>)
{
    return true;
}

UnaryOperator::Opcode UnaryOperator::getOpcode() const
{
    return Opc;
}

void UnaryOperator::setOpcode(UnaryOperator::Opcode O)
{
    Opc = O;
}

weak_ptr<Expr> UnaryOperator::getSubExpr() const
{
    return dynamic_pointer_cast<Expr>(Val);
}

void UnaryOperator::setSubExpr(shared_ptr<Expr> E)
{
    Val = E;
}

SourceLocation UnaryOperator::getOperatorLoc() const
{
    return Loc;
}

void UnaryOperator::setOperatorLoc(SourceLocation L)
{
    Loc = L;
}

SourceLocation UnaryOperator::getExprLoc() const
{
    return Loc;
}

ArraySubscriptExpr::ArraySubscriptExpr(shared_ptr<Expr> lhs, shared_ptr<Expr> rhs,
                                       shared_ptr<QualType> t, SourceLocation rbracketloc)
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

Stmt::child_iterator ArraySubscriptExpr::child_begin()
{
    return &SubExprs[0];
}

Stmt::child_iterator ArraySubscriptExpr::child_end()
{
    return &SubExprs[0]+END_EXPR;
}

bool ArraySubscriptExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == ArraySubscriptExprClass;
}

bool ArraySubscriptExpr::classof(const weak_ptr<ArraySubscriptExpr>)
{
    return true;
}

weak_ptr<Expr> ArraySubscriptExpr::getLHS()
{
    return dynamic_pointer_cast<Expr>(SubExprs[LHS]);
}

const weak_ptr<Expr> ArraySubscriptExpr::getLHS() const
{
    return dynamic_pointer_cast<Expr>(SubExprs[LHS]);
}

void ArraySubscriptExpr::setLHS(shared_ptr<Expr> E)
{
    SubExprs[LHS] = E;
}

weak_ptr<Expr> ArraySubscriptExpr::getRHS()
{
    return dynamic_pointer_cast<Expr>(SubExprs[RHS]);
}

const weak_ptr<Expr> ArraySubscriptExpr::getRHS() const
{
    return dynamic_pointer_cast<Expr>(SubExprs[RHS]);
}

void ArraySubscriptExpr::setRHS(shared_ptr<Expr> E)
{
    SubExprs[RHS] = E;
}

SourceLocation ArraySubscriptExpr::getRBracketLoc() const
{
    return RBracketLoc;
}

void ArraySubscriptExpr::setRBracketLoc(SourceLocation L)
{
    RBracketLoc = L;
}

CallExpr::CallExpr(shared_ptr<Expr> fn, vector<shared_ptr<Expr>> args,
                   unsigned numargs, shared_ptr<QualType> t, SourceLocation rparenloc)
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

Stmt::child_iterator CallExpr::child_begin()
{
    return &SubExprs[0];
}

Stmt::child_iterator CallExpr::child_end()
{
    return &SubExprs[0]+NumArgs+ARGS_START;
}

bool CallExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == CallExprClass ||
            T.lock()->getStmtClass() == CXXOperatorCallExprClass ||
            T.lock()->getStmtClass() == CXXMemberCallExprClass;
}

bool CallExpr::classof(const weak_ptr<CallExpr>)
{
    return true;
}

const weak_ptr<Expr> CallExpr::getCallee() const
{
    return dynamic_pointer_cast<Expr>(SubExprs[FN]);
}

weak_ptr<Expr> CallExpr::getCallee()
{
    return dynamic_pointer_cast<Expr>(SubExprs[FN]);
}

void CallExpr::setCallee(shared_ptr<Expr> F)
{
    SubExprs[FN] = F;
}

unsigned CallExpr::getNumArgs() const
{
    return NumArgs;
}

void CallExpr::setNumArgs(unsigned NumArgs)
{
    if (NumArgs == getNumArgs()) return;
    if (NumArgs < getNumArgs()) {
        //        for (unsigned i = NumArgs, e = getNumArgs(); i != e; ++i)
        //            getArg(i)->Destroy(C);
        this->NumArgs = NumArgs;
        return;
    }
}

SourceLocation CallExpr::getRParenLoc() const
{
    return RParenLoc;
}

void CallExpr::setRParenLoc(SourceLocation L)
{
    RParenLoc = L;
}

CallExpr::CallExpr(Stmt::StmtClass SC, shared_ptr<Expr> fn,
                   vector<shared_ptr<Expr>> args, unsigned numargs,
                   shared_ptr<QualType> t, SourceLocation rparenloc)
    : Expr(SC, t), NumArgs(numargs)
{
    SubExprs[FN] = fn;
    for (unsigned i = 0; i != numargs; ++i)
        SubExprs[i+ARGS_START] = args[i];

    RParenLoc = rparenloc;
}

MemberExpr::MemberExpr(shared_ptr<Expr> base, bool isarrow,
                       shared_ptr<NamedDecl> memberdecl, SourceLocation l,
                       shared_ptr<QualType> ty)
    :Expr (MemberExprClass,ty,base->isTypeDependent(),base->isValueDependent()),
      Base(base), MemberDecl(memberdecl), MemberLoc(l), IsArrow(isarrow)
{}

MemberExpr::MemberExpr(EmptyShell Empty)
    :Expr (MemberExprClass,Empty)
{}

Stmt::child_iterator MemberExpr::child_begin()
{
    return &Base;
}

Stmt::child_iterator MemberExpr::child_end()
{
    return &Base+1;
}

bool MemberExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == MemberExprClass;
}

bool MemberExpr::classof(const weak_ptr<MemberExpr>)
{
    return true;
}

void MemberExpr::setBase(shared_ptr<Expr> E)
{
    Base = E;
}

weak_ptr<Expr> MemberExpr::getBase() const
{
    return dynamic_pointer_cast<Expr>(Base);
}

weak_ptr<NamedDecl> MemberExpr::getMemberDecl() const
{
    return MemberDecl;
}

void MemberExpr::setMemberDecl(shared_ptr<NamedDecl> D)
{
    MemberDecl = D;
}

bool MemberExpr::isArrow() const
{
    return IsArrow;
}

void MemberExpr::setArrow(bool A)
{
    IsArrow = A;
}

SourceLocation MemberExpr::getMemberLoc() const
{
    return MemberLoc;
}

void MemberExpr::setMemberLoc(SourceLocation L)
{
    MemberLoc = L;
}

SourceLocation MemberExpr::getExprLoc() const
{
    return MemberLoc;
}

CompoundLiteralExpr::CompoundLiteralExpr(SourceLocation lparenloc, shared_ptr<QualType> ty,
                                         shared_ptr<Expr> init, bool fileScope)
    : Expr (CompoundLiteralExprClass,ty), LParenLoc(lparenloc), Init(init), FileScope(fileScope)
{}

CompoundLiteralExpr::CompoundLiteralExpr(EmptyShell Empty)
    :Expr (CompoundLiteralExprClass,Empty)
{}

Stmt::child_iterator CompoundLiteralExpr::child_begin()
{
    return &Init;
}

Stmt::child_iterator CompoundLiteralExpr::child_end()
{
    return &Init+1;
}

bool CompoundLiteralExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == CompoundLiteralExprClass;
}

bool CompoundLiteralExpr::classof(const weak_ptr<CompoundLiteralExpr>)
{
    return true;
}

const weak_ptr<Expr> CompoundLiteralExpr::getInitializer() const
{
    return std::dynamic_pointer_cast<Expr>(Init);
}

weak_ptr<Expr> CompoundLiteralExpr::getInitializer()
{
    return std::dynamic_pointer_cast<Expr>(Init);
}

void CompoundLiteralExpr::setInitializer(shared_ptr<Expr> E)
{
    Init = E;
}

bool CompoundLiteralExpr::isFileScope() const
{
    return FileScope;
}

void CompoundLiteralExpr::setFileScope(bool FS)
{
    FileScope = FS;
}

SourceLocation CompoundLiteralExpr::getLParenLoc() const
{
    return LParenLoc;
}

void CompoundLiteralExpr::setLParenLoc(SourceLocation L)
{
    LParenLoc = L;
}

Stmt::child_iterator CastExpr::child_begin()
{
    return &Op;
}

Stmt::child_iterator CastExpr::child_end()
{
    return &Op+1;
}

CastExpr::CastExpr(StmtClass SC, shared_ptr<QualType> ty,
                   const CastInfo &info, shared_ptr<Expr>op)
    :Expr (SC, ty, (*ty)->isDependentType(),
           (*ty)->isDependentType()||(op&&op->isValueDependent())), Kind(info.Kind),Op(op)
{}

CastExpr::CastExpr(StmtClass SC, EmptyShell Empty)
    : Expr(SC, Empty)
{ }

bool CastExpr::classof(const weak_ptr<Stmt> T)
{
    StmtClass SC = T.lock()->getStmtClass();
    if (SC >= CXXNamedCastExprClass && SC <= CXXFunctionalCastExprClass)
        return true;
    if (SC >= ImplicitCastExprClass && SC <= CStyleCastExprClass)
        return true;
    return false;
}

bool CastExpr::classof(const weak_ptr<CastExpr>)
{
    return true;
}

CastExpr::CastKind CastExpr::getCastKind() const
{
    return Kind;
}

void CastExpr::setCastKind(CastExpr::CastKind K)
{
    Kind = K;
}

weak_ptr<Expr> CastExpr::getSubExpr()
{
    return dynamic_pointer_cast<Expr>(Op);
}

void CastExpr::setSubExpr(shared_ptr<Expr> E)
{
    Op = E;
}

ImplicitCastExpr::ImplicitCastExpr(shared_ptr<QualType> ty, const CastInfo &info,
                                   shared_ptr<Expr>op, bool Lvalue)
    : CastExpr(ImplicitCastExprClass, ty, info, op), LvalueCast(Lvalue)
{}

ImplicitCastExpr::ImplicitCastExpr(EmptyShell Shell)
    : CastExpr(ImplicitCastExprClass, Shell) { }

bool ImplicitCastExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == ImplicitCastExprClass;
}

bool ImplicitCastExpr::classof(const weak_ptr<ImplicitCastExpr>)
{
    return true;
}

bool ImplicitCastExpr::isLvalueCast() const
{
    return LvalueCast;
}

void ImplicitCastExpr::setLvalueCast(bool Lvalue)
{
    LvalueCast = Lvalue;
}

ExplicitCastExpr::ExplicitCastExpr(StmtClass SC, shared_ptr<QualType> exprTy, const CastInfo &info,
                                   shared_ptr<Expr>op, shared_ptr<QualType> writtenTy)
    : CastExpr(SC, exprTy, info, op), TypeAsWritten(writtenTy)
{}

ExplicitCastExpr::ExplicitCastExpr(StmtClass SC, EmptyShell Shell)
    : CastExpr(SC, Shell)
{ }

bool ExplicitCastExpr::classof(const weak_ptr<Stmt> T)
{
    StmtClass SC = T.lock()->getStmtClass();
    if (SC >= ExplicitCastExprClass && SC <= CStyleCastExprClass)
        return true;
    if (SC >= CXXNamedCastExprClass && SC <= CXXFunctionalCastExprClass)
        return true;
    return false;
}

weak_ptr<QualType> ExplicitCastExpr::getTypeAsWritten() const
{
    return TypeAsWritten;
}

void ExplicitCastExpr::setTypeAsWritten(shared_ptr<QualType> T)
{
    TypeAsWritten = T;
}

CStyleCastExpr::CStyleCastExpr(shared_ptr<QualType> exprTy, CastKind kind,
                               shared_ptr<Expr> op, shared_ptr<QualType> writtenTy,
                               SourceLocation l, SourceLocation r) :
    ExplicitCastExpr(CStyleCastExprClass, exprTy, kind, op, writtenTy),
    LPLoc(l), RPLoc(r)
{}

CStyleCastExpr::CStyleCastExpr(EmptyShell Shell)
    : ExplicitCastExpr(CStyleCastExprClass, Shell)
{ }

bool CStyleCastExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == CStyleCastExprClass;
}

bool CStyleCastExpr::classof(const weak_ptr<CStyleCastExpr>)
{
    return true;
}

SourceLocation CStyleCastExpr::getLParenLoc() const
{
    return LPLoc;
}

void CStyleCastExpr::setLParenLoc(SourceLocation L)
{
    LPLoc = L;
}

SourceLocation CStyleCastExpr::getRParenLoc() const
{
    return RPLoc;
}

void CStyleCastExpr::setRParenLoc(SourceLocation L)
{
    RPLoc = L;
}

BinaryOperator::BinaryOperator(shared_ptr<Expr> lhs, shared_ptr<Expr> rhs,
                               BinaryOperator::Opcode opc, shared_ptr<QualType> ResTy,
                               SourceLocation opLoc)
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

SourceLocation BinaryOperator::getOperatorLoc() const
{
    return OpLoc;
}

void BinaryOperator::setOperatorLoc(SourceLocation L)
{
    OpLoc = L;
}

BinaryOperator::Opcode BinaryOperator::getOpcode() const
{
    return Opc;
}

void BinaryOperator::setOpcode(BinaryOperator::Opcode O)
{
    Opc = O;
}

weak_ptr<Expr> BinaryOperator::getLHS() const
{
    return dynamic_pointer_cast<Expr>(SubExprs[LHS]);
}

void BinaryOperator::setLHS(shared_ptr<Expr> E)
{
    SubExprs[LHS] = E;
}

weak_ptr<Expr> BinaryOperator::getRHS() const
{
    return dynamic_pointer_cast<Expr>(SubExprs[RHS]);
}

void BinaryOperator::setRHS(shared_ptr<Expr> E)
{
    SubExprs[RHS] = E;
}

bool BinaryOperator::classof(const weak_ptr<Stmt> S)
{
    return S.lock()->getStmtClass() == BinaryOperatorClass ||
            S.lock()->getStmtClass() == CompoundAssignOperatorClass;
}

bool BinaryOperator::classof(const weak_ptr<BinaryOperator>)
{
    return true;
}

Stmt::child_iterator BinaryOperator::child_begin()
{
    return &SubExprs[0];
}

Stmt::child_iterator BinaryOperator::child_end()
{
    return &SubExprs[0]+END_EXPR;
}

BinaryOperator::BinaryOperator(shared_ptr<Expr> lhs, shared_ptr<Expr> rhs,
                               BinaryOperator::Opcode opc, shared_ptr<QualType> ResTy,
                               SourceLocation oploc, bool dead)
    : Expr(CompoundAssignOperatorClass, ResTy), Opc(opc), OpLoc(oploc)
{
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
}

BinaryOperator::BinaryOperator(Stmt::StmtClass SC, Stmt::EmptyShell Empty)
    : Expr(SC, Empty), Opc(MulAssign) { }

CompoundAssignOperator::CompoundAssignOperator(shared_ptr<Expr> lhs, shared_ptr<Expr> rhs,
                                               BinaryOperator::Opcode opc, shared_ptr<QualType> ResType,
                                               shared_ptr<QualType> CompLHSType,
                                               shared_ptr<QualType> CompResultType, SourceLocation OpLoc)
    : BinaryOperator(lhs, rhs, opc, ResType, OpLoc, true),
      ComputationLHSType(CompLHSType),
      ComputationResultType(CompResultType)
{
}

CompoundAssignOperator::CompoundAssignOperator(Stmt::EmptyShell Empty)
    : BinaryOperator(CompoundAssignOperatorClass, Empty) { }

weak_ptr<QualType> CompoundAssignOperator::getComputationLHSType() const
{
    return ComputationLHSType;
}

void CompoundAssignOperator::setComputationLHSType(shared_ptr<QualType> T)
{
    ComputationLHSType = T;
}

weak_ptr<QualType> CompoundAssignOperator::getComputationResultType() const
{
    return ComputationResultType;
}

void CompoundAssignOperator::setComputationResultType(shared_ptr<QualType> T)
{
    ComputationResultType = T;
}

bool CompoundAssignOperator::classof(const weak_ptr<CompoundAssignOperator>)
{
    return true;
}

bool CompoundAssignOperator::classof(const weak_ptr<Stmt> S)
{
    return S.lock()->getStmtClass() == CompoundAssignOperatorClass;
}

ConditionalOperator::ConditionalOperator(shared_ptr<Expr> cond, shared_ptr<Expr> lhs,
                                         shared_ptr<Expr> rhs, shared_ptr<QualType> t)
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

weak_ptr<Expr> ConditionalOperator::getCond() const
{
    return dynamic_pointer_cast<Expr>(SubExprs[COND]);
}

void ConditionalOperator::setCond(shared_ptr<Expr> E)
{
    SubExprs[COND] = E;
}

weak_ptr<Expr> ConditionalOperator::getTrueExpr() const
{
    return reinterpret_pointer_cast<Expr>(SubExprs[LHS] ? SubExprs[LHS] : SubExprs[COND]);
}

weak_ptr<Expr> ConditionalOperator::getFalseExpr() const
{
    return reinterpret_pointer_cast<Expr>(SubExprs[RHS]);
}

weak_ptr<Expr> ConditionalOperator::getLHS() const
{
    return reinterpret_pointer_cast<Expr>(SubExprs[LHS]);
}

void ConditionalOperator::setLHS(shared_ptr<Expr> E)
{
    SubExprs[LHS] = E;
}

weak_ptr<Expr> ConditionalOperator::getRHS() const
{
    return reinterpret_pointer_cast<Expr>(SubExprs[RHS]);
}

void ConditionalOperator::setRHS(shared_ptr<Expr> E)
{
    SubExprs[RHS] = E;
}

bool ConditionalOperator::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == ConditionalOperatorClass;
}

bool ConditionalOperator::classof(const weak_ptr<ConditionalOperator>)
{
    return true;
}

Stmt::child_iterator ConditionalOperator::child_begin()
{
    return &SubExprs[0];
}

Stmt::child_iterator ConditionalOperator::child_end()
{
    return &SubExprs[0]+END_EXPR;
}

AddrLabelExpr::AddrLabelExpr(SourceLocation AALoc, SourceLocation LLoc,
                             shared_ptr<LabelStmt> L, shared_ptr<QualType> t)
    : Expr(AddrLabelExprClass, t), AmpAmpLoc(AALoc), LabelLoc(LLoc), Label(L) {}

AddrLabelExpr::AddrLabelExpr(Stmt::EmptyShell Empty)
    : Expr(AddrLabelExprClass, Empty) { }

SourceLocation AddrLabelExpr::getAmpAmpLoc() const
{
    return AmpAmpLoc;
}

void AddrLabelExpr::setAmpAmpLoc(SourceLocation L)
{
    AmpAmpLoc = L;
}

SourceLocation AddrLabelExpr::getLabelLoc() const
{
    return LabelLoc;
}

void AddrLabelExpr::setLabelLoc(SourceLocation L)
{
    LabelLoc = L;
}

weak_ptr<LabelStmt> AddrLabelExpr::getLabel() const
{
    return Label;
}

void AddrLabelExpr::setLabel(shared_ptr<LabelStmt> S)
{
    Label = S;
}

bool AddrLabelExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == AddrLabelExprClass;
}

bool AddrLabelExpr::classof(const weak_ptr<AddrLabelExpr>)
{
    return true;
}

Stmt::child_iterator AddrLabelExpr::child_begin()
{
    return child_iterator();
}

Stmt::child_iterator AddrLabelExpr::child_end()
{
    return child_iterator();
}

StmtExpr::StmtExpr(shared_ptr<CompoundStmt> substmt, shared_ptr<QualType> T,
                   SourceLocation lp, SourceLocation rp) :
    Expr(StmtExprClass, T), SubStmt(substmt),  LParenLoc(lp), RParenLoc(rp) { }

StmtExpr::StmtExpr(Stmt::EmptyShell Empty)
    : Expr(StmtExprClass, Empty) { }

weak_ptr<CompoundStmt> StmtExpr::getSubStmt()
{
    return reinterpret_pointer_cast<CompoundStmt>(SubStmt);
}

const weak_ptr<CompoundStmt> StmtExpr::getSubStmt() const
{
    return reinterpret_pointer_cast<CompoundStmt>(SubStmt);
}

void StmtExpr::setSubStmt(shared_ptr<CompoundStmt> S)
{
    SubStmt = S;
}

SourceLocation StmtExpr::getLParenLoc() const
{
    return LParenLoc;
}

void StmtExpr::setLParenLoc(SourceLocation L)
{
    LParenLoc = L;
}

SourceLocation StmtExpr::getRParenLoc() const
{
    return RParenLoc;
}

void StmtExpr::setRParenLoc(SourceLocation L)
{
    RParenLoc = L;
}

bool StmtExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtExprClass;
}

bool StmtExpr::classof(const weak_ptr<StmtExpr>)
{
    return true;
}

TypesCompatibleExpr::TypesCompatibleExpr(shared_ptr<QualType> ReturnType, SourceLocation BLoc,
                                         shared_ptr<QualType> t1, shared_ptr<QualType> t2,
                                         SourceLocation RP)
    : Expr(TypesCompatibleExprClass, ReturnType), Type1(t1), Type2(t2), BuiltinLoc(BLoc), RParenLoc(RP) {}

TypesCompatibleExpr::TypesCompatibleExpr(Stmt::EmptyShell Empty)
    : Expr(TypesCompatibleExprClass, Empty) { }

weak_ptr<QualType> TypesCompatibleExpr::getArgType1() const
{
    return Type1;
}

void TypesCompatibleExpr::setArgType1(shared_ptr<QualType> T)
{
    Type1 = T;
}

weak_ptr<QualType> TypesCompatibleExpr::getArgType2() const
{
    return Type2;
}

void TypesCompatibleExpr::setArgType2(shared_ptr<QualType> T)
{
    Type2 = T;
}

SourceLocation TypesCompatibleExpr::getBuiltinLoc() const
{
    return BuiltinLoc;
}

void TypesCompatibleExpr::setBuiltinLoc(SourceLocation L)
{
    BuiltinLoc = L;
}

SourceLocation TypesCompatibleExpr::getRParenLoc() const
{
    return RParenLoc;
}

void TypesCompatibleExpr::setRParenLoc(SourceLocation L)
{
    RParenLoc = L;
}

bool TypesCompatibleExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == TypesCompatibleExprClass;
}

bool TypesCompatibleExpr::classof(const weak_ptr<TypesCompatibleExpr>)
{
    return true;
}

Stmt::child_iterator TypesCompatibleExpr::child_begin()
{
    return child_iterator();
}

Stmt::child_iterator TypesCompatibleExpr::child_end()
{
    return child_iterator();
}

ShuffleVectorExpr::ShuffleVectorExpr(vector<std::shared_ptr<Expr> > args, unsigned nexpr,
                                     shared_ptr<QualType> Type, SourceLocation BLoc, SourceLocation RP)
    : Expr(ShuffleVectorExprClass, Type), BuiltinLoc(BLoc), RParenLoc(RP), NumExprs(nexpr)
{
    SubExprs.resize(nexpr);
    for (unsigned i = 0; i < nexpr; i++)
        SubExprs[i] = args[i];
}

ShuffleVectorExpr::ShuffleVectorExpr(Stmt::EmptyShell Empty)
    : Expr(ShuffleVectorExprClass, Empty), SubExprs(0) { }

weak_ptr<Expr> ShuffleVectorExpr::getExpr(unsigned Index)
{
    return reinterpret_pointer_cast<Expr>(SubExprs[Index]);
}

const weak_ptr<Expr> ShuffleVectorExpr::getExpr(unsigned Index) const
{
    return reinterpret_pointer_cast<Expr>(SubExprs[Index]);
}

SourceLocation ShuffleVectorExpr::getBuiltinLoc() const
{
    return BuiltinLoc;
}

void ShuffleVectorExpr::setBuiltinLoc(SourceLocation L)
{
    BuiltinLoc = L;
}

SourceLocation ShuffleVectorExpr::getRParenLoc() const
{
    return RParenLoc;
}

void ShuffleVectorExpr::setRParenLoc(SourceLocation L)
{
    RParenLoc = L;
}

bool ShuffleVectorExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == ShuffleVectorExprClass;
}

bool ShuffleVectorExpr::classof(const weak_ptr<ShuffleVectorExpr>)
{
    return true;
}

Stmt::child_iterator ShuffleVectorExpr::child_begin()
{
    return &SubExprs[0];
}

Stmt::child_iterator ShuffleVectorExpr::child_end()
{
    return &SubExprs[0]+NumExprs;
}

ChooseExpr::ChooseExpr(SourceLocation BLoc, shared_ptr<Expr> cond,
                       shared_ptr<Expr> lhs, shared_ptr<Expr> rhs,
                       shared_ptr<QualType> t, SourceLocation RP)
    : Expr(ChooseExprClass, t),
      BuiltinLoc(BLoc), RParenLoc(RP)
{
    SubExprs[COND] = cond;
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
}

ChooseExpr::ChooseExpr(Stmt::EmptyShell Empty)
    : Expr(ChooseExprClass, Empty) { }

weak_ptr<Expr> ChooseExpr::getCond() const
{
    return reinterpret_pointer_cast<Expr>(SubExprs[COND]);
}

void ChooseExpr::setCond(shared_ptr<Expr> E)
{
    SubExprs[COND] = E;
}

weak_ptr<Expr> ChooseExpr::getLHS() const
{
    return reinterpret_pointer_cast<Expr>(SubExprs[LHS]);
}

void ChooseExpr::setLHS(shared_ptr<Expr> E)
{
    SubExprs[LHS] = E;
}

weak_ptr<Expr> ChooseExpr::getRHS() const
{
    return reinterpret_pointer_cast<Expr>(SubExprs[RHS]);
}

void ChooseExpr::setRHS(shared_ptr<Expr> E)
{
    SubExprs[RHS] = E;
}

SourceLocation ChooseExpr::getBuiltinLoc() const
{
    return BuiltinLoc;
}

void ChooseExpr::setBuiltinLoc(SourceLocation L)
{
    BuiltinLoc = L;
}

SourceLocation ChooseExpr::getRParenLoc() const
{
    return RParenLoc;
}

void ChooseExpr::setRParenLoc(SourceLocation L)
{
    RParenLoc = L;
}

bool ChooseExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == ChooseExprClass;
}

bool ChooseExpr::classof(const weak_ptr<ChooseExpr>)
{
    return true;
}

GNUNullExpr::GNUNullExpr(shared_ptr<QualType> Ty, SourceLocation Loc)
    : Expr(GNUNullExprClass, Ty), TokenLoc(Loc)
{ }

GNUNullExpr::GNUNullExpr(Stmt::EmptyShell Empty)
    : Expr(GNUNullExprClass, Empty) { }

SourceLocation GNUNullExpr::getTokenLocation() const
{
    return TokenLoc;
}

void GNUNullExpr::setTokenLocation(SourceLocation L)
{
    TokenLoc = L;
}

bool GNUNullExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == GNUNullExprClass;
}

bool GNUNullExpr::classof(const weak_ptr<GNUNullExpr>)
{
    return true;
}

VAArgExpr::VAArgExpr(SourceLocation BLoc, shared_ptr<Expr> e,
                     shared_ptr<QualType> t, SourceLocation RPLoc)
    : Expr(VAArgExprClass, t),
      Val(e),
      BuiltinLoc(BLoc),
      RParenLoc(RPLoc)
{ }

VAArgExpr::VAArgExpr(Stmt::EmptyShell Empty)
    : Expr(VAArgExprClass, Empty) { }

const weak_ptr<Expr> VAArgExpr::getSubExpr() const
{
    return reinterpret_pointer_cast<Expr>(Val);
}

weak_ptr<Expr> VAArgExpr::getSubExpr()
{
    return reinterpret_pointer_cast<Expr>(Val);
}

void VAArgExpr::setSubExpr(shared_ptr<Expr> E)
{
    Val = E;
}

SourceLocation VAArgExpr::getBuiltinLoc() const
{
    return BuiltinLoc;
}

void VAArgExpr::setBuiltinLoc(SourceLocation L)
{
    BuiltinLoc = L;
}

SourceLocation VAArgExpr::getRParenLoc() const
{
    return RParenLoc;
}

void VAArgExpr::setRParenLoc(SourceLocation L)
{
    RParenLoc = L;
}

bool VAArgExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == VAArgExprClass;
}

bool VAArgExpr::classof(const weak_ptr<VAArgExpr>)
{
    return true;
}

InitListExpr::InitListExpr(SourceLocation lbraceloc,
                           vector<shared_ptr<Expr> > initexprs,
                           unsigned numinits,
                           SourceLocation rbraceloc)
    : Expr(InitListExprClass, make_shared<QualType>()),
      LBraceLoc(lbraceloc), RBraceLoc(rbraceloc), SyntacticForm(0),
      HadArrayRangeDesignator(false)
{
    for(unsigned i=0;i!=numinits;i++){
        InitExprs.push_back(initexprs[i]);
    }
}

InitListExpr::InitListExpr(Stmt::EmptyShell Empty)
    : Expr(InitListExprClass, Empty) { }

SourceLocation InitListExpr::getLBraceLoc() const
{
    return LBraceLoc;
}

void InitListExpr::setLBraceLoc(SourceLocation Loc)
{
    LBraceLoc = Loc;
}

SourceLocation InitListExpr::getRBraceLoc() const
{
    return RBraceLoc;
}

void InitListExpr::setRBraceLoc(SourceLocation Loc)
{
    RBraceLoc = Loc;
}

weak_ptr<InitListExpr> InitListExpr::getSyntacticForm() const
{
    return SyntacticForm;
}

void InitListExpr::setSyntacticForm(shared_ptr<InitListExpr> Init)
{
    SyntacticForm = Init;
}

bool InitListExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == InitListExprClass;
}

bool InitListExpr::classof(const weak_ptr<InitListExpr>)
{
    return true;
}

Stmt::child_iterator InitListExpr::child_begin() {
    return (InitExprs.size() ? &InitExprs[0] : nullptr);
}

Stmt::child_iterator InitListExpr::child_end()
{
    return (InitExprs.size() ? &InitExprs[0] + InitExprs.size() : nullptr);
}

ImplicitValueInitExpr::ImplicitValueInitExpr(shared_ptr<QualType> ty)
    : Expr(ImplicitValueInitExprClass, ty)
{ }

ImplicitValueInitExpr::ImplicitValueInitExpr(Stmt::EmptyShell Empty)
    : Expr(ImplicitValueInitExprClass, Empty) { }

bool ImplicitValueInitExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == ImplicitValueInitExprClass;
}

bool ImplicitValueInitExpr::classof(const weak_ptr<ImplicitValueInitExpr>)
{
    return true;
}

Stmt::child_iterator ImplicitValueInitExpr::child_begin()
{
    return child_iterator();
}

Stmt::child_iterator ImplicitValueInitExpr::child_end()
{
    return child_iterator();
}

SizeOfAlignOfExpr::SizeOfAlignOfExpr(bool issizeof, shared_ptr<QualType> T,
                                     shared_ptr<QualType> resultType,
                                     SourceLocation op, SourceLocation rp)
    : Expr(SizeOfAlignOfExprClass, resultType, false, (*T.get())->isDependentType()),
      isSizeof(issizeof), isType(true), OpLoc(op), RParenLoc(rp)
{
    Ty = T;
}

SizeOfAlignOfExpr::SizeOfAlignOfExpr(bool issizeof, shared_ptr<Expr> E,
                                     shared_ptr<QualType> resultType,
                                     SourceLocation op, SourceLocation rp)
    : Expr(SizeOfAlignOfExprClass, resultType, false, E->isTypeDependent()),
      isSizeof(issizeof), isType(false), OpLoc(op), RParenLoc(rp)
{
    Ex = E;
}

SizeOfAlignOfExpr::SizeOfAlignOfExpr(Stmt::EmptyShell Empty)
    : Expr(SizeOfAlignOfExprClass, Empty)
{ }

Stmt::child_iterator SizeOfAlignOfExpr::child_begin(){
    if (isArgumentType()) {
        if (shared_ptr<VariableArrayType> T =
                reinterpret_pointer_cast<VariableArrayType>(getArgumentType().lock()->getTypePtr()))
            return nullptr;//Stmt::child_iterator(T);
        return child_iterator();
    }
    return child_iterator(&Ex);
}

Stmt::child_iterator SizeOfAlignOfExpr::child_end(){
    if (isArgumentType())
        return child_iterator();
    return child_iterator(&Ex + 1);
}

bool SizeOfAlignOfExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == SizeOfAlignOfExprClass;
}

bool SizeOfAlignOfExpr::classof(const weak_ptr<SizeOfAlignOfExpr>)
{
    return true;
}

bool SizeOfAlignOfExpr::isSizeOf() const
{
    return isSizeof;
}

void SizeOfAlignOfExpr::setSizeof(bool S)
{
    isSizeof = S;
}

bool SizeOfAlignOfExpr::isArgumentType() const
{
    return isType;
}

SourceLocation SizeOfAlignOfExpr::getOperatorLoc() const
{
    return OpLoc;
}

void SizeOfAlignOfExpr::setOperatorLoc(SourceLocation L)
{
    OpLoc = L;
}

weak_ptr<QualType> SizeOfAlignOfExpr::getArgumentType() const
{
    return reinterpret_pointer_cast<QualType>(Ty);
}

weak_ptr<Expr> SizeOfAlignOfExpr::getArgumentExpr()
{
    return dynamic_pointer_cast<Expr>(Ex);
}

const weak_ptr<Expr> SizeOfAlignOfExpr::getArgumentExpr() const
{
    return this->getArgumentExpr();
}

weak_ptr<QualType> SizeOfAlignOfExpr::getTypeOfArgument() const {
    return isArgumentType() ? getArgumentType() : getArgumentExpr().lock()->getType();
}

void SizeOfAlignOfExpr::setArgument(shared_ptr<Expr> E)
{
    Ex = E;
    isType = false;
}

void SizeOfAlignOfExpr::setArgument(shared_ptr<QualType> T)
{
    Ty = T;
    isType = true;
}

SourceLocation SizeOfAlignOfExpr::getRParenLoc() const
{
    return RParenLoc;
}

void SizeOfAlignOfExpr::setRParenLoc(SourceLocation L)
{
    RParenLoc = L;
}

ParenListExpr::ParenListExpr(SourceLocation lparenloc,
                             vector<shared_ptr<Expr> > exprs,
                             unsigned numexprs,
                             SourceLocation rparenloc)
    : Expr(ParenListExprClass, make_shared<QualType>()),
      NumExprs(numexprs), LParenLoc(lparenloc), RParenLoc(rparenloc)
{
    for (unsigned i = 0; i != numexprs; ++i)
        Exprs.push_back(exprs[i]);
}

const weak_ptr<Expr> ParenListExpr::getExpr(unsigned Init) const
{
    return reinterpret_pointer_cast<Expr>(Exprs[Init]);
}

weak_ptr<Expr> ParenListExpr::getExpr(unsigned Init)
{
    return reinterpret_pointer_cast<Expr>(Exprs[Init]);
}

SourceLocation ParenListExpr::getLParenLoc() const
{
    return LParenLoc;
}

SourceLocation ParenListExpr::getRParenLoc() const
{
    return RParenLoc;
}

bool ParenListExpr::classof(const weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == ParenListExprClass;
}

bool ParenListExpr::classof(const std::weak_ptr<ParenListExpr>)
{
    return true;
}
