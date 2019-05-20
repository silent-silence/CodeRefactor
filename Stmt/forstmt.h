#ifndef FORSTMT_H
#define FORSTMT_H

#include "stmt.h"
#include "../Expr/expr.h"

class ForStmt : public Stmt
{
public:
    ForStmt(std::shared_ptr<Stmt> Init, std::shared_ptr<Expr> Cond, std::shared_ptr<Expr> Inc, std::shared_ptr<Stmt> Body);
    explicit ForStmt(EmptyShell Empty);
private:
    enum { INIT, COND, INC, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

#endif // FORSTMT_H
