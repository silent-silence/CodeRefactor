#ifndef DOSTMT_H
#define DOSTMT_H

#include "stmt.h"
#include "Expr/expr.h"

class DoStmt : public Stmt
{
public:
    DoStmt(std::shared_ptr<Stmt> body, std::shared_ptr<Expr> cond);
    explicit DoStmt(EmptyShell Empty);
private:
    enum { COND, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

#endif // DOSTMT_H
