#ifndef SIZEOFALIGNOFEXPR_H
#define SIZEOFALIGNOFEXPR_H

#include "expr.h"

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

#endif // SIZEOFALIGNOFEXPR_H
