#ifndef BINARYOPERATOR_H
#define BINARYOPERATOR_H

#include "../expr.h"

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

#endif // BINARYOPERATOR_H
