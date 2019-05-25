#include "parenexpr.h"

ParenExpr::ParenExpr(std::shared_ptr<Expr> val)
    : Expr(ParenExprClass, val->getType(), val->isTypeDependent(), val->isValueDependent()),
      Val(val)
{}

ParenExpr::ParenExpr(Stmt::EmptyShell Empty)
    : Expr(ParenExprClass, Empty)
{ }
