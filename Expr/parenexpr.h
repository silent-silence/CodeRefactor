#ifndef PARENEXPR_H
#define PARENEXPR_H

#include "expr.h"

class ParenExpr : public Expr
{
public:
    ParenExpr(std::shared_ptr<Expr> val);
    explicit ParenExpr(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Val;
};

#endif // PARENEXPR_H
