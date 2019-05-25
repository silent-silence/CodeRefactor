#ifndef UNARYOPERATOR_H
#define UNARYOPERATOR_H

#include "expr.h"

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

#endif // UNARYOPERATOR_H
