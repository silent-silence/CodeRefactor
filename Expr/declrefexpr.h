#ifndef DECLREFEXPR_H
#define DECLREFEXPR_H

#include "expr.h"

class DeclRefExpr : public Expr
{
public:
    DeclRefExpr(QualType t);
    DeclRefExpr(QualType t, bool TD, bool VD);
    explicit DeclRefExpr(EmptyShell Empty);
protected:
    DeclRefExpr(StmtClass SC, QualType t);
    DeclRefExpr(StmtClass SC, QualType t, bool TD, bool VD);
private:
    enum { COND, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

#endif // DECLREFEXPR_H
