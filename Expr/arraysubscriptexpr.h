#ifndef ARRAYSUBSCRIPTEXPR_H
#define ARRAYSUBSCRIPTEXPR_H

#include "expr.h"

class ArraySubscriptExpr : public Expr
{
public:
    ArraySubscriptExpr(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, QualType t);
    explicit ArraySubscriptExpr(EmptyShell Shell);
private:
    enum { LHS, RHS, END_EXPR=2 };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

#endif // ARRAYSUBSCRIPTEXPR_H
