#include "memberexpr.h"

using std::shared_ptr;

MemberExpr::MemberExpr(shared_ptr<Expr> base, bool isarrow, QualType ty)
    : Expr(MemberExprClass, ty, base->isTypeDependent(), base->isValueDependent()),
      Base(base),
      IsArrow(isarrow)
{}

MemberExpr::MemberExpr(Stmt::EmptyShell Empty)
    : Expr(MemberExprClass, Empty)
{ }
