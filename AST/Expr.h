#ifndef AST_EXPR_H
#define AST_EXPR_H

#include "AST/Type.h"
#include "AST/Stmt.h"
#include "Declarations/Decl.h"

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
private:
    IdentType Type;
    SourceLocation Loc;
};

class IntegerLiteral : public Expr
{
public:
    IntegerLiteral(const int &V, QualType type, SourceLocation l);
    explicit IntegerLiteral(EmptyShell Empty);
private:
    int Value;
    SourceLocation Loc;
};

class CharacterLiteral : public Expr
{
public:
    CharacterLiteral(unsigned value, bool iswide, QualType type, SourceLocation l);
    CharacterLiteral(EmptyShell Empty);
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
protected:
    CastExpr(StmtClass SC, QualType ty, const CastInfo &info, std::shared_ptr<Expr>op) ;
    CastExpr(StmtClass SC, EmptyShell Empty);
private:
    CastKind Kind;
    std::shared_ptr<Stmt> Op;
};

class ImplicitCastExpr : public CastExpr
{
public:
    ImplicitCastExpr(QualType ty, const CastInfo &info, std::shared_ptr<Expr>op, bool Lvalue);
    explicit ImplicitCastExpr(EmptyShell Shell);
private:
    bool LvalueCast;
};

class ExplicitCastExpr : public CastExpr
{
protected:
    ExplicitCastExpr(StmtClass SC, QualType exprTy, const CastInfo &info,
                     std::shared_ptr<Expr>op, QualType writtenTy);
    ExplicitCastExpr(StmtClass SC, EmptyShell Shell);
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
private:
    SourceLocation TokenLoc;
};

class VAArgExpr : public Expr
{
public:
    VAArgExpr(SourceLocation BLoc, std::shared_ptr<Expr> e, QualType t, SourceLocation RPLoc);

    explicit VAArgExpr(EmptyShell Empty);
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


    //    DesignatedInitExpr(QualType Ty, unsigned NumDesignators,
    //                       const Designator *Designators,
    //                       SourceLocation EqualOrColonLoc, bool GNUSyntax,
    //                       Expr **IndexExprs, unsigned NumIndexExprs,
    //                       Expr *Init);

    //    explicit DesignatedInitExpr(unsigned NumSubExprs)
    //        : Expr(DesignatedInitExprClass, EmptyShell()),
    //          NumDesignators(0), Designators(0), NumSubExprs(NumSubExprs) { }

};

class ImplicitValueInitExpr : public Expr
{
public:
    explicit ImplicitValueInitExpr(QualType ty);

    explicit ImplicitValueInitExpr(EmptyShell Empty);
};

class ParenListExpr : public Expr
{
public:
    ParenListExpr(SourceLocation lparenloc, std::vector<std::shared_ptr<Expr>> exprs,
                  unsigned numexprs, SourceLocation rparenloc);
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
