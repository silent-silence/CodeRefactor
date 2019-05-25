#include "conditionaloperator.h"

using std::shared_ptr;

ConditionalOperator::ConditionalOperator(shared_ptr<Expr> cond,
                                         shared_ptr<Expr> lhs,
                                         shared_ptr<Expr> rhs,
                                         QualType t)
    : Expr(ConditionalOperatorClass,
           t,
           ((lhs && lhs->isTypeDependent()) || (rhs && rhs->isTypeDependent())),
           (cond->isValueDependent() || (lhs && lhs->isValueDependent()) || (rhs && rhs->isValueDependent())))
{
    SubExprs[COND] = cond;
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
}
