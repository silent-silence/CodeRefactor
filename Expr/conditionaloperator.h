#ifndef CONDITIONALOPERATOR_H
#define CONDITIONALOPERATOR_H

#include "expr.h"

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

#endif // CONDITIONALOPERATOR_H
