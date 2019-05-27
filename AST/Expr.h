#ifndef AST_EXPR_H
#define AST_EXPR_H

#include "AST/Type.h"
#include "AST/Stmt.h"
#include "Decl/Decl.h"

class Expr;
class Stmt;

class DeclRefExpr;
class PredefinedExpr;
class IntegerLiteral;
class CharacterLiteral;
class FloatingLiteral;
class ImaginaryLiteral;
class StringLiteral;
class ParenExpr;
class UnaryOperator;
class SizeOfAlignOfExpr;
class ArraySubscriptExpr;
class CallExpr;
class MemberExpr;
class CompoundLiteralExpr;
class CastExpr;
class ImplicitCastExpr;
class ExplicitCastExpr;//
class CStyleCastExpr;

class BinaryOperator;
class CompoundAssignOperator;
class ConditionalOperator;
class AddrLabelExpr;
class StmtExpr;
class TypesCompatibleExpr;
class ShuffleVectorExpr;//
class ChooseExpr;
class GNUNullExpr;
class VAArgExpr;
class InitListExpr;
class DesignatedInitExpr;
class ImplicitValueInitExpr;
class ParenListExpr;
class ExtVectorElementExpr;
class BlockExpr;
class BlockDeclRefExpr;

//Expr  - 这代表一个表达式。
//Expr是的Stmt子类，这允许表达式在Stmt的任何地方透明地使用。
class Expr : public Stmt
{
public:
    QualType getType() const;
    void setType(const QualType &value);

    bool isTypeDependent() const;
    void setTypeDependent(bool value);

    bool isValueDependent() const;
    void setValueDependent(bool value);

    static bool classof(const std::shared_ptr<Stmt> T) {
        return T->getStmtClass() >= firstExprConstant &&
                T->getStmtClass() <= lastExprConstant;
    }
    static bool classof(const std::shared_ptr<Expr>) { return true; }
protected:
    Expr(StmtClass SC, QualType T);
    Expr(StmtClass SC, QualType T, bool TD, bool VD);

    explicit Expr(StmtClass SC, EmptyShell);

    bool TypeDependent : 1;//TypeDependent  - 此表达式是否依赖于类型
    bool ValueDependent : 1;//ValueDependent  - 此表达式是否与值有关
private:
    QualType TR;
};

class DeclRefExpr : public Expr
{
public:
    DeclRefExpr(std::shared_ptr<NamedDecl> d, QualType t, SourceLocation l);
    DeclRefExpr(std::shared_ptr<NamedDecl> d, QualType t, SourceLocation l, bool TD, bool VD);
    explicit DeclRefExpr(EmptyShell Empty);
protected:
    DeclRefExpr(StmtClass SC, std::shared_ptr<NamedDecl> d, QualType t, SourceLocation l);
    DeclRefExpr(StmtClass SC, std::shared_ptr<NamedDecl> d, QualType t, SourceLocation l, bool TD, bool VD);
    virtual child_iterator child_begin() { return child_iterator(); }
    virtual child_iterator child_end(){ return child_iterator(); }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == DeclRefExprClass ||
             T->getStmtClass() == CXXConditionDeclExprClass ||
             T->getStmtClass() == QualifiedDeclRefExprClass;
    }
     static bool classof(const DeclRefExpr *) { return true; }
private:
    std::shared_ptr<NamedDecl> D;
    SourceLocation Loc;
};

class PredefinedExpr : public Expr
{
public:
    enum IdentType {
        Func,
        Function,
        PrettyFunction
    };
    PredefinedExpr(SourceLocation l, QualType type, IdentType IT);
    explicit PredefinedExpr(EmptyShell Empty);
    virtual child_iterator child_begin(){ return child_iterator(); }
    virtual child_iterator child_end() { return child_iterator(); }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == PredefinedExprClass;
    }
    static bool classof(const PredefinedExpr *) { return true; }

private:
    IdentType Type;
    SourceLocation Loc;
};

class IntegerLiteral : public Expr
{
public:
    IntegerLiteral(const int &V, QualType type, SourceLocation l);
    explicit IntegerLiteral(EmptyShell Empty);
    virtual child_iterator child_begin(){ return child_iterator(); }
    virtual child_iterator child_end() { return child_iterator(); }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == IntegerLiteralClass;
    }
    static bool classof(const IntegerLiteral *) { return true; }
private:
    int Value;
    SourceLocation Loc;
};

class CharacterLiteral : public Expr
{
public:
    CharacterLiteral(unsigned value, bool iswide, QualType type, SourceLocation l);
    CharacterLiteral(EmptyShell Empty);
    virtual child_iterator child_begin(){ return child_iterator();}
    virtual child_iterator child_end(){ return child_iterator();}
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == CharacterLiteralClass;
    }
    static bool classof(const CharacterLiteral *) { return true; }
private:
    unsigned Value;
    bool IsWide;
    SourceLocation Loc;
};

class FloatingLiteral : public Expr
{
public:
    FloatingLiteral(const float &V, bool isexact, QualType Type, SourceLocation L);
    explicit FloatingLiteral(EmptyShell Empty);
    virtual child_iterator child_begin(){ return child_iterator(); }
    virtual child_iterator child_end(){ return child_iterator(); }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == FloatingLiteralClass;
    }
    static bool classof(const FloatingLiteral *) { return true; }
private:
    float Value;
    bool IsExact : 1;
    SourceLocation Loc;
};

class ImaginaryLiteral : public Expr
{
public:
    ImaginaryLiteral(std::shared_ptr<Expr> val, QualType Ty);
    explicit ImaginaryLiteral(EmptyShell Empty);
    virtual child_iterator child_begin(){ return &Val; }
    virtual child_iterator child_end(){ return &Val+1; }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == ImaginaryLiteralClass;
    }
    static bool classof(const ImaginaryLiteral *) { return true; }
private:
    std::shared_ptr<Stmt> Val;
};

class StringLiteral : public Expr
{
public:
    static std::shared_ptr<StringLiteral> Create(const char *StrData,
                                                 unsigned ByteLength, bool Wide, QualType Ty,
                                                 const SourceLocation Loc, unsigned NumStrs);
    static std::shared_ptr<StringLiteral> Create(const char *StrData,
                                                 unsigned ByteLength,
                                                 bool Wide, QualType Ty, SourceLocation Loc);
    virtual child_iterator child_begin(){ return child_iterator(); }
    virtual child_iterator child_end(){ return child_iterator(); }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == StringLiteralClass;
    }
    static bool classof(const StringLiteral *) { return true; }

private:
    StringLiteral(QualType Ty);

    const char *StrData;
    unsigned ByteLength;
    bool IsWide;
    unsigned NumConcatenated;
    SourceLocation TokLocs;
};

class ParenExpr : public Expr
{
public:
    ParenExpr(SourceLocation l, SourceLocation r, std::shared_ptr<Expr> val);
    explicit ParenExpr(EmptyShell Empty);
    virtual child_iterator child_begin(){ return &Val; }
    virtual child_iterator child_end(){ return &Val+1; }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == ParenExprClass;
    }
    static bool classof(const ParenExpr *) { return true; }
private:
    std::shared_ptr<Stmt> Val;
    SourceLocation L;
    SourceLocation R;
};

class UnaryOperator : public Expr
{
public:
    enum Opcode {
        PostInc, PostDec, // [C99 6.5.2.4] Postfix increment and decrement operators
        PreInc, PreDec,   // [C99 6.5.3.1] Prefix increment and decrement operators.
        AddrOf, Deref,    // [C99 6.5.3.2] Address and indirection operators.
        Plus, Minus,      // [C99 6.5.3.3] Unary arithmetic operators.
        Not, LNot,        // [C99 6.5.3.3] Unary arithmetic operators.
        Real, Imag,       // "__real expr"/"__imag expr" Extension.
        Extension,        // __extension__ marker.
        OffsetOf          // __builtin_offsetof
    };
    UnaryOperator(std::shared_ptr<Expr> input, Opcode opc, QualType type, SourceLocation l);
    explicit UnaryOperator(EmptyShell Empty);
    virtual child_iterator child_begin(){ return &Val; }
    virtual child_iterator child_end(){ return &Val+1; }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == UnaryOperatorClass;
    }
    static bool classof(const UnaryOperator *) { return true; }
private:
    std::shared_ptr<Stmt> Val;
    Opcode Opc;
    SourceLocation Loc;
};

class SizeOfAlignOfExpr : public Expr
{
public:
    SizeOfAlignOfExpr(bool issizeof, QualType T,
                      QualType resultType, SourceLocation op,
                      SourceLocation rp);

    SizeOfAlignOfExpr(bool issizeof, std::shared_ptr<Expr> E,
                      QualType resultType, SourceLocation op,
                      SourceLocation rp);

    explicit SizeOfAlignOfExpr(EmptyShell Empty)
        : Expr(SizeOfAlignOfExprClass, Empty) { }
    virtual child_iterator child_begin(){
        //        if (isArgumentType()) {
        //          if (VariableArrayType* T = dyn_cast<VariableArrayType>(
        //                                         getArgumentType().getTypePtr()))
        //            return child_iterator(T);
        //          return child_iterator();
        //        }
        //        return child_iterator(&Argument.Ex);
    }
    virtual child_iterator child_end(){
        //        if (isArgumentType())
        //          return child_iterator();
        //        return child_iterator(&Argument.Ex + 1);
    }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == SizeOfAlignOfExprClass;
    }
    static bool classof(const SizeOfAlignOfExpr *) { return true; }

private:
    bool isSizeof : 1;  // true if sizeof, false if alignof.
    bool isType : 1;    // true if operand is a type, false if an expression
    std::shared_ptr<void> Ty;
    std::shared_ptr<Stmt> Ex;
    SourceLocation OpLoc, RParenLoc;
};

class ArraySubscriptExpr : public Expr
{
public:
    ArraySubscriptExpr(std::shared_ptr<Expr> lhs,
                       std::shared_ptr<Expr> rhs,
                       QualType t,
                       SourceLocation rbracketloc);
    explicit ArraySubscriptExpr(EmptyShell Shell);
    virtual child_iterator child_begin(){return &SubExprs[0];}
    virtual child_iterator child_end() {return &SubExprs[0]+END_EXPR;}
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == ArraySubscriptExprClass;
    }
    static bool classof(const ArraySubscriptExpr *) { return true; }
private:
    enum { LHS, RHS, END_EXPR=2 };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation RBracketLoc;
};

class CallExpr : public Expr
{
public:
    CallExpr(std::shared_ptr<Expr> fn,
             std::vector<std::shared_ptr<Expr>> args,
             unsigned numargs,
             QualType t,
             SourceLocation rparenloc);
    CallExpr(StmtClass SC, EmptyShell Empty);
    virtual child_iterator child_begin() {
        return &SubExprs[0];
    }
    virtual child_iterator child_end() {
        return &SubExprs[0]+NumArgs+ARGS_START;
    }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == CallExprClass ||
             T->getStmtClass() == CXXOperatorCallExprClass ||
             T->getStmtClass() == CXXMemberCallExprClass;
    }
    static bool classof(const CallExpr *) { return true; }
//    static bool classof(const CXXOperatorCallExpr *) { return true; }
//    static bool classof(const CXXMemberCallExpr *) { return true; }
protected:
    CallExpr(StmtClass SC,
             std::shared_ptr<Expr> fn,
             std::vector<std::shared_ptr<Expr>> args,
             unsigned numargs,
             QualType t,
             SourceLocation rparenloc);
private:
    enum { FN=0, ARGS_START=1 };
    std::vector<std::shared_ptr<Stmt>> SubExprs;
    unsigned NumArgs;
    SourceLocation RParenLoc;
};

class MemberExpr : public Expr
{
public:
    MemberExpr(std::shared_ptr<Expr> base,
               bool isarrow,
               std::shared_ptr<NamedDecl> memberdecl,
               SourceLocation l,
               QualType ty);
    explicit MemberExpr(EmptyShell Empty);
    virtual child_iterator child_begin(){ return &Base; }
    virtual child_iterator child_end(){ return &Base+1; }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == MemberExprClass;
    }
    static bool classof(const MemberExpr *) { return true; }
private:
    std::shared_ptr<Stmt> Base;
    std::shared_ptr<NamedDecl> MemberDecl;
    SourceLocation MemberLoc;
    bool IsArrow;
};

class CompoundLiteralExpr : public Expr
{
public:
    CompoundLiteralExpr(SourceLocation lparenloc,
                        QualType ty,
                        std::shared_ptr<Expr> init,
                        bool fileScope);
    explicit CompoundLiteralExpr(EmptyShell Empty);
    virtual child_iterator child_begin(){ return &Init; }
    virtual child_iterator child_end(){ return &Init+1; }

    static bool classof(const Stmt *T) {
      return T->getStmtClass() == CompoundLiteralExprClass;
    }
    static bool classof(const CompoundLiteralExpr *) { return true; }
private:
    std::shared_ptr<Stmt> Init;
    bool FileScope;
    SourceLocation LParenLoc;
};

class CastExpr : public Expr
{
public:
    enum CastKind {
        CK_Unknown,
        CK_BitCast,
        CK_NoOp,
        CK_DerivedToBase,
        CK_Dynamic,
        CK_ToUnion,
        CK_ArrayToPointerDecay
    };
    struct CastInfo {
        const CastKind Kind;
        CastInfo(CastKind Kind)
            : Kind(Kind) { }
    };
    virtual child_iterator child_begin(){ return &Op; }
    virtual child_iterator child_end(){ return &Op+1; }
protected:
    CastExpr(StmtClass SC, QualType ty, const CastInfo &info, std::shared_ptr<Expr>op) ;
    CastExpr(StmtClass SC, EmptyShell Empty);
    static bool classof(const Stmt *T) {
      StmtClass SC = T->getStmtClass();
      if (SC >= CXXNamedCastExprClass && SC <= CXXFunctionalCastExprClass)
        return true;

      if (SC >= ImplicitCastExprClass && SC <= CStyleCastExprClass)
        return true;

      return false;
    }
    static bool classof(const CastExpr *) { return true; }
private:
    CastKind Kind;
    std::shared_ptr<Stmt> Op;
};

class ImplicitCastExpr : public CastExpr
{
public:
    ImplicitCastExpr(QualType ty, const CastInfo &info, std::shared_ptr<Expr>op, bool Lvalue);
    explicit ImplicitCastExpr(EmptyShell Shell);
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == ImplicitCastExprClass;
    }
    static bool classof(const ImplicitCastExpr *) { return true; }

private:
    bool LvalueCast;
};

class ExplicitCastExpr : public CastExpr
{
protected:
    ExplicitCastExpr(StmtClass SC, QualType exprTy, const CastInfo &info,
                     std::shared_ptr<Expr>op, QualType writtenTy);
    ExplicitCastExpr(StmtClass SC, EmptyShell Shell);
    static bool classof(const Stmt *T) {
      StmtClass SC = T->getStmtClass();
      if (SC >= ExplicitCastExprClass && SC <= CStyleCastExprClass)
        return true;
      if (SC >= CXXNamedCastExprClass && SC <= CXXFunctionalCastExprClass)
        return true;

      return false;
    }
    static bool classof(const ExplicitCastExpr *) { return true; }
private:
    QualType TypeAsWritten;
};

class CStyleCastExpr : public ExplicitCastExpr
{
public:
    CStyleCastExpr(QualType exprTy,
                   CastKind kind,
                   std::shared_ptr<Expr> op,
                   QualType writtenTy,
                   SourceLocation l,
                   SourceLocation r);
    explicit CStyleCastExpr(EmptyShell Shell);
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == CStyleCastExprClass;
    }
    static bool classof(const CStyleCastExpr *) { return true; }
private:
    SourceLocation LPLoc;
    SourceLocation RPLoc;
};

class BinaryOperator : public Expr
{
public:
    enum Opcode {
        // Operators listed in order of precedence.
        PtrMemD, PtrMemI, // [C++ 5.5] Pointer-to-member operators.
        Mul, Div, Rem,    // [C99 6.5.5] Multiplicative operators.
        Add, Sub,         // [C99 6.5.6] Additive operators.
        Shl, Shr,         // [C99 6.5.7] Bitwise shift operators.
        LT, GT, LE, GE,   // [C99 6.5.8] Relational operators.
        EQ, NE,           // [C99 6.5.9] Equality operators.
        And,              // [C99 6.5.10] Bitwise AND operator.
        Xor,              // [C99 6.5.11] Bitwise XOR operator.
        Or,               // [C99 6.5.12] Bitwise OR operator.
        LAnd,             // [C99 6.5.13] Logical AND operator.
        LOr,              // [C99 6.5.14] Logical OR operator.
        Assign, MulAssign,// [C99 6.5.16] Assignment operators.
        DivAssign, RemAssign,
        AddAssign, SubAssign,
        ShlAssign, ShrAssign,
        AndAssign, XorAssign,
        OrAssign,
        Comma             // [C99 6.5.17] Comma operator.
    };
    BinaryOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, Opcode opc, QualType ResTy, SourceLocation opLoc);

    explicit BinaryOperator(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> S) {
        return S->getStmtClass() == BinaryOperatorClass ||
                S->getStmtClass() == CompoundAssignOperatorClass;
    }
    static bool classof(const std::shared_ptr<BinaryOperator>) { return true; }
    virtual child_iterator child_begin() { return &SubExprs[0]; }
    virtual child_iterator child_end() { return &SubExprs[0]+END_EXPR; }
protected:
    BinaryOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, Opcode opc, QualType ResTy, SourceLocation oploc, bool dead);

    BinaryOperator(StmtClass SC, EmptyShell Empty);
private:
    enum { LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    Opcode Opc;
    SourceLocation OpLoc;
};

class CompoundAssignOperator : public BinaryOperator
{
public:
    CompoundAssignOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, Opcode opc,
                           QualType ResType, QualType CompLHSType,
                           QualType CompResultType,
                           SourceLocation OpLoc);

    explicit CompoundAssignOperator(EmptyShell Empty);
    static bool classof(const std::shared_ptr<CompoundAssignOperator>) { return true; }
    static bool classof(const std::shared_ptr<Stmt> S) {
        return S->getStmtClass() == CompoundAssignOperatorClass;
    }
private:
    QualType ComputationLHSType;
    QualType ComputationResultType;
};

class ConditionalOperator : public Expr
{
public:
    ConditionalOperator(std::shared_ptr<Expr> cond, std::shared_ptr<Expr> lhs,
                        std::shared_ptr<Expr> rhs, QualType t);

    explicit ConditionalOperator(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
        return T->getStmtClass() == ConditionalOperatorClass;
    }
    static bool classof(const std::shared_ptr<ConditionalOperator>) { return true; }
    virtual child_iterator child_begin(){return &SubExprs[0];}
    virtual child_iterator child_end(){return &SubExprs[0]+END_EXPR;}
private:
    enum { COND, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

class AddrLabelExpr : public Expr
{
public:
    AddrLabelExpr(SourceLocation AALoc, SourceLocation LLoc, std::shared_ptr<LabelStmt> L,
                  QualType t);

    explicit AddrLabelExpr(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
        return T->getStmtClass() == AddrLabelExprClass;
    }
    static bool classof(const std::shared_ptr<AddrLabelExpr>) { return true; }
    virtual child_iterator child_begin() { return child_iterator(); }
    virtual child_iterator child_end(){ return child_iterator(); }
private:
    SourceLocation AmpAmpLoc;
    SourceLocation LabelLoc;
    std::shared_ptr<LabelStmt> Label;
};

class StmtExpr : public Expr
{
public:
    StmtExpr(std::shared_ptr<CompoundStmt> substmt, QualType T,
             SourceLocation lp, SourceLocation rp);
    explicit StmtExpr(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
      return T->getStmtClass() == StmtExprClass;
    }
    static bool classof(const std::shared_ptr<StmtExpr>) { return true; }
    virtual child_iterator child_begin() { return &SubStmt; }
    virtual child_iterator child_end(){ return &SubStmt+1; }
private:
    std::shared_ptr<Stmt> SubStmt;
    SourceLocation LParenLoc;
    SourceLocation RParenLoc;
};

class TypesCompatibleExpr : public Expr
{
public:
    TypesCompatibleExpr(QualType ReturnType, SourceLocation BLoc,
                        QualType t1, QualType t2, SourceLocation RP);
    explicit TypesCompatibleExpr(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
      return T->getStmtClass() == TypesCompatibleExprClass;
    }
    static bool classof(const std::shared_ptr<TypesCompatibleExpr>) { return true; }
    virtual child_iterator child_begin(){return child_iterator();}
    virtual child_iterator child_end(){return child_iterator();}
private:
    QualType Type1;
    QualType Type2;
    SourceLocation BuiltinLoc;
    SourceLocation RParenLoc;
};

class ShuffleVectorExpr : public Expr
{
public:
    ShuffleVectorExpr(std::vector<std::shared_ptr<Expr>> args, unsigned nexpr,
                      QualType Type, SourceLocation BLoc,
                      SourceLocation RP);

    explicit ShuffleVectorExpr(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
      return T->getStmtClass() == ShuffleVectorExprClass;
    }
    static bool classof(const std::shared_ptr<ShuffleVectorExpr>) { return true; }
    virtual child_iterator child_begin(){return &SubExprs[0];}
    virtual child_iterator child_end(){return &SubExprs[0]+NumExprs; }
private:
    SourceLocation BuiltinLoc, RParenLoc;
    std::vector<std::shared_ptr<Stmt>> SubExprs;
    unsigned NumExprs;
};

class ChooseExpr : public Expr
{
public:
    ChooseExpr(SourceLocation BLoc, std::shared_ptr<Expr> cond, std::shared_ptr<Expr> lhs,
               std::shared_ptr<Expr> rhs, QualType t,
               SourceLocation RP);

    explicit ChooseExpr(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
      return T->getStmtClass() == ChooseExprClass;
    }
    static bool classof(const std::shared_ptr<ChooseExpr>) { return true; }
    virtual child_iterator child_begin(){ return &SubExprs[0]; }
    virtual child_iterator child_end(){ return &SubExprs[0]+END_EXPR; }

private:
    enum { COND, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation BuiltinLoc;
    SourceLocation RParenLoc;
};

class GNUNullExpr : public Expr
{
public:
    GNUNullExpr(QualType Ty, SourceLocation Loc);

    explicit GNUNullExpr(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
      return T->getStmtClass() == GNUNullExprClass;
    }
    static bool classof(const std::shared_ptr<GNUNullExpr>) { return true; }
    virtual child_iterator child_begin(){return child_iterator();}
    virtual child_iterator child_end(){return child_iterator();}
private:
    SourceLocation TokenLoc;
};

class VAArgExpr : public Expr
{
public:
    VAArgExpr(SourceLocation BLoc, std::shared_ptr<Expr> e, QualType t, SourceLocation RPLoc);

    explicit VAArgExpr(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
      return T->getStmtClass() == VAArgExprClass;
    }
    static bool classof(const std::shared_ptr<VAArgExpr>) { return true; }
    virtual child_iterator child_begin(){return &Val;}
    virtual child_iterator child_end(){return &Val+1;}
private:
    std::shared_ptr<Stmt> Val;
    SourceLocation BuiltinLoc;
    SourceLocation RParenLoc;
};

class InitListExpr : public Expr
{
public:
    InitListExpr(SourceLocation lbraceloc,
                 std::vector<std::shared_ptr<Expr>> initexprs, unsigned numinits,
                 SourceLocation rbraceloc);

    explicit InitListExpr(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
      return T->getStmtClass() == InitListExprClass;
    }
    static bool classof(const std::shared_ptr<InitListExpr>) { return true; }
    virtual child_iterator child_begin() {
        return InitExprs.size() ? &InitExprs[0] :nullptr;
        }
        virtual child_iterator child_end(){
        return InitExprs.size() ? &InitExprs[0] + InitExprs.size() : nullptr;
    }
private:
    std::vector<std::shared_ptr<Stmt>> InitExprs;
    SourceLocation LBraceLoc;
    SourceLocation RBraceLoc;

    std::shared_ptr<InitListExpr> SyntacticForm;

    //FieldDecl *UnionFieldInit;

    bool HadArrayRangeDesignator;
};

class DesignatedInitExpr : public Expr
{
public:
    class Designator;
private:
    SourceLocation EqualOrColonLoc;
    bool GNUSyntax : 1;
    unsigned NumDesignators : 15;
    std::shared_ptr<Designator> Designators;
    unsigned NumSubExprs : 16;
    static bool classof(const std::shared_ptr<Stmt> T) {
      return T->getStmtClass() == DesignatedInitExprClass;
    }
    static bool classof(const std::shared_ptr<DesignatedInitExpr>) { return true; }


    //    DesignatedInitExpr(QualType Ty, unsigned NumDesignators,
    //                       const Designator *Designators,
    //                       SourceLocation EqualOrColonLoc, bool GNUSyntax,
    //                       Expr **IndexExprs, unsigned NumIndexExprs,
    //                       Expr *Init);

    //    explicit DesignatedInitExpr(unsigned NumSubExprs)
    //        : Expr(DesignatedInitExprClass, EmptyShell()),
    //          NumDesignators(0), Designators(0), NumSubExprs(NumSubExprs) { }
    //    Stmt::child_iterator DesignatedInitExpr::child_begin() {
    //      char* Ptr = static_cast<char*>(static_cast<void *>(this));
    //      Ptr += sizeof(DesignatedInitExpr);
    //      return reinterpret_cast<Stmt**>(reinterpret_cast<void**>(Ptr));
    //    }
    //    Stmt::child_iterator DesignatedInitExpr::child_end() {
    //      return child_iterator(&*child_begin() + NumSubExprs);
    //    }

};

class ImplicitValueInitExpr : public Expr
{
public:
    explicit ImplicitValueInitExpr(QualType ty);

    explicit ImplicitValueInitExpr(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
      return T->getStmtClass() == ImplicitValueInitExprClass;
    }
    static bool classof(const std::shared_ptr<ImplicitValueInitExpr>) { return true; }
    virtual child_iterator child_begin(){return child_iterator();}
    virtual child_iterator child_end(){return child_iterator();}
};

class ParenListExpr : public Expr
{
public:
    ParenListExpr(SourceLocation lparenloc, std::vector<std::shared_ptr<Expr>> exprs,
                  unsigned numexprs, SourceLocation rparenloc);

    static bool classof(const std::shared_ptr<Stmt> T) {
      return T->getStmtClass() == ParenListExprClass;
    }
    static bool classof(const std::shared_ptr<ParenListExpr>) { return true; }

    virtual child_iterator child_begin(){return &Exprs[0];}
    virtual child_iterator child_end(){return &Exprs[0]+NumExprs;}
private:
    std::vector<std::shared_ptr<Stmt>> Exprs;
    unsigned NumExprs;
    SourceLocation LParenLoc;
    SourceLocation RParenLoc;
};

class ExtVectorElementExpr : public Expr
{
public:
    //    ExtVectorElementExpr(QualType ty, std::shared_ptr<Expr> base, IdentifierInfo &accessor,
    //                           SourceLocation loc)
    //        : Expr(ExtVectorElementExprClass, ty),
    //          Base(base), Accessor(&accessor), AccessorLoc(loc) {}

    explicit ExtVectorElementExpr(EmptyShell Empty);
    virtual child_iterator child_begin() { return &Base; }
    virtual child_iterator child_end() { return &Base+1; }
private:
    std::shared_ptr<Stmt> Base;
    //      IdentifierInfo *Accessor;
    SourceLocation AccessorLoc;
};

class BlockExpr : public Expr
{

};

class BlockDeclRefExpr : public Expr
{

};

#endif // AST_EXPR_H
