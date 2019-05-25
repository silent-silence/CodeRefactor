#include "declrefexpr.h"

DeclRefExpr::DeclRefExpr(QualType t)
    : Expr(DeclRefExprClass, t)
{}

DeclRefExpr::DeclRefExpr(QualType t, bool TD, bool VD)
    : Expr(DeclRefExprClass, t, TD, VD)
{}

DeclRefExpr::DeclRefExpr(Stmt::EmptyShell Empty)
    : Expr(DeclRefExprClass, Empty)
{}

DeclRefExpr::DeclRefExpr(Stmt::StmtClass SC, QualType t)
    : Expr(SC, t)
{}

DeclRefExpr::DeclRefExpr(Stmt::StmtClass SC, QualType t, bool TD, bool VD)
    : Expr(SC, t, TD, VD)
{}
