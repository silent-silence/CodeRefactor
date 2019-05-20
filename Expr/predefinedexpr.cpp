#include "predefinedexpr.h"

PredefinedExpr::PredefinedExpr(QualType type, PredefinedExpr::IdentType IT)
    : Expr(PredefinedExprClass, type), Type(IT)
{}

PredefinedExpr::PredefinedExpr(Stmt::EmptyShell Empty)
    : Expr(PredefinedExprClass, Empty)
{ }
