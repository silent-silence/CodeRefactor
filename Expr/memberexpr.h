#ifndef MEMBEREXPR_H
#define MEMBEREXPR_H

#include "expr.h"

class MemberExpr : public Expr
{
public:
    MemberExpr(std::shared_ptr<Expr> base, bool isarrow, QualType ty);
    explicit MemberExpr(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Base;
    bool IsArrow;
};

#endif // MEMBEREXPR_H
