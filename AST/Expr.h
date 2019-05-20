#ifndef AST_EXPR_H
#define AST_EXPR_H

#include "AST/Type.h"
#include "AST/Stmt.h"

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
    DeclRefExpr(QualType t);
    DeclRefExpr(QualType t, bool TD, bool VD);
    explicit DeclRefExpr(EmptyShell Empty);
protected:
    DeclRefExpr(StmtClass SC, QualType t);
    DeclRefExpr(StmtClass SC, QualType t, bool TD, bool VD);
private:
    enum { COND, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

class PredefinedExpr : public Expr
{
public:
    enum IdentType {
        Func,
        Function,
        PrettyFunction
    };
    PredefinedExpr(QualType type, IdentType IT);
    explicit PredefinedExpr(EmptyShell Empty);
private:
    IdentType Type;
};

class IntegerLiteral : public Expr
{
public:
    IntegerLiteral(const int &V, QualType type);
    explicit IntegerLiteral(EmptyShell Empty);
private:
    int Value;
};

class CharacterLiteral : public Expr
{
public:
    CharacterLiteral(unsigned value, bool iswide, QualType type);
    CharacterLiteral(EmptyShell Empty);
private:
    unsigned Value;
    bool IsWide;
};

class FloatingLiteral : public Expr
{
public:
    FloatingLiteral(const float &V, bool isexact, QualType Type);
    explicit FloatingLiteral(EmptyShell Empty);
private:
    float Value;
    bool IsExact : 1;
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
                                                 unsigned ByteLength,
                                                 bool Wide,
                                                 QualType Ty,
                                                 unsigned NumStrs);
    static std::shared_ptr<StringLiteral> Create(const char *StrData,
                                                 unsigned ByteLength,
                                                 bool Wide,
                                                 QualType Ty);
private:
    StringLiteral(QualType Ty);

    const char *StrData;
    unsigned ByteLength;
    bool IsWide;
    unsigned NumConcatenated;
};

class ParenExpr : public Expr
{
public:
    ParenExpr(std::shared_ptr<Expr> val);
    explicit ParenExpr(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Val;
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
    UnaryOperator(std::shared_ptr<Expr> input, Opcode opc, QualType type);
    explicit UnaryOperator(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Val;
    Opcode Opc;
};

class SizeOfAlignOfExpr : public Expr
{
public:
    SizeOfAlignOfExpr(bool issizeof, QualType T, QualType resultType);
    SizeOfAlignOfExpr(bool issizeof, std::shared_ptr<Expr> E, QualType resultType);
    explicit SizeOfAlignOfExpr(EmptyShell Empty);
protected:
    bool isSizeof : 1;  // true if sizeof, false if alignof.
    bool isType : 1;    // true if operand is a type, false if an expression
private:
    std::shared_ptr<void> Ty;
    std::shared_ptr<Stmt> Ex;
};

class ArraySubscriptExpr : public Expr
{
public:
    ArraySubscriptExpr(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, QualType t);
    explicit ArraySubscriptExpr(EmptyShell Shell);
private:
    enum { LHS, RHS, END_EXPR=2 };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

class CallExpr : public Expr
{
public:
    CallExpr(std::shared_ptr<Expr> fn, std::shared_ptr<Expr> *args, unsigned numargs, QualType t);
    CallExpr(StmtClass SC, EmptyShell Empty);
protected:
    CallExpr(StmtClass SC, std::shared_ptr<Expr> fn, std::shared_ptr<Expr> *args, unsigned numargs, QualType t);
private:
    enum { FN=0, ARGS_START=1 };
    //std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    unsigned NumArgs;
};

class MemberExpr : public Expr
{
public:
    MemberExpr(std::shared_ptr<Expr> base, bool isarrow, QualType ty);
    explicit MemberExpr(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Base;
    bool IsArrow;
};

class CompoundLiteralExpr : public Expr
{
public:
    CompoundLiteralExpr(QualType ty, std::shared_ptr<Expr> init, bool fileScope);
    explicit CompoundLiteralExpr(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Init;
    bool FileScope;
};

class CastExpr : public Expr
{

};

class ImplicitCastExpr : public CastExpr
{

};

class ExplicitCastExpr : public CastExpr
{

};

class CStyleCastExpr : public ExplicitCastExpr
{

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
    BinaryOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, Opcode opc, QualType ResTy);
    explicit BinaryOperator(EmptyShell Empty);
protected:
    BinaryOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, Opcode opc, QualType ResTy, bool dead);
    BinaryOperator(StmtClass SC, EmptyShell Empty);
private:
    enum { LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    Opcode Opc;
};

class CompoundAssignOperator : public BinaryOperator
{
public:
    CompoundAssignOperator(std::shared_ptr<Expr> lhs,
                           std::shared_ptr<Expr> rhs,
                           Opcode opc,
                           QualType ResType,
                           QualType CompLHSType,
                           QualType CompResultType);
    explicit CompoundAssignOperator(EmptyShell Empty);
private:
    QualType ComputationLHSType;
    QualType ComputationResultType;
};

class ConditionalOperator : public Expr
{
public:
    ConditionalOperator(std::shared_ptr<Expr> cond,std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, QualType t);
    explicit ConditionalOperator(EmptyShell Empty)
        : Expr(ConditionalOperatorClass, Empty) { }
private:
    enum { COND, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

class AddrLabelExpr : public Expr
{

};

class StmtExpr : public Expr
{

};

class TypesCompatibleExpr : public Expr
{

};

class ShuffleVectorExpr : public Expr
{

};

class ChooseExpr : public Expr
{

};

class GNUNullExpr : public Expr
{

};

class VAArgExpr : public Expr
{

};

class InitListExpr : public Expr
{

};

class DesignatedInitExpr : public Expr
{

};

class ImplicitValueInitExpr : public Expr
{

};

class ParenListExpr : public Expr
{

};

class ExtVectorElementExpr : public Expr
{

};

class BlockExpr : public Expr
{

};

class BlockDeclRefExpr : public Expr
{

};

#endif // AST_EXPR_H
