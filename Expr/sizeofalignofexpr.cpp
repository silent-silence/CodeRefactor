#include "sizeofalignofexpr.h"

SizeOfAlignOfExpr::SizeOfAlignOfExpr(bool issizeof, QualType T, QualType resultType)
    : Expr(SizeOfAlignOfExprClass, resultType, false, true),
      isSizeof(issizeof),
      isType(true)
{
}

SizeOfAlignOfExpr::SizeOfAlignOfExpr(bool issizeof, std::shared_ptr<Expr> E, QualType resultType)
    : Expr(SizeOfAlignOfExprClass, resultType, false, E->isTypeDependent()),
      isSizeof(issizeof),
      isType(false)
{
}

SizeOfAlignOfExpr::SizeOfAlignOfExpr(Stmt::EmptyShell Empty)
    : Expr(SizeOfAlignOfExprClass, Empty)
{ }
