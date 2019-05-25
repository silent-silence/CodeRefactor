#include "arraysubscriptexpr.h"

ArraySubscriptExpr::ArraySubscriptExpr(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, QualType t)
    : Expr(ArraySubscriptExprClass,
           t,
           lhs->isTypeDependent() || rhs->isTypeDependent(),
           lhs->isValueDependent() || rhs->isValueDependent())
{
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
}

ArraySubscriptExpr::ArraySubscriptExpr(Stmt::EmptyShell Shell)
    : Expr(ArraySubscriptExprClass, Shell)
{}
