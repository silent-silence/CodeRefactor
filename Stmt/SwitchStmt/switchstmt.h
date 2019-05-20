#ifndef SWITCHSTMT_H
#define SWITCHSTMT_H

#include "../stmt.h"
#include "Expr/expr.h"
#include "SwitchCase/switchcase.h"

class SwitchStmt : public Stmt
{
public:
    SwitchStmt(std::shared_ptr<Expr> cond);
    explicit SwitchStmt(EmptyShell Empty);
private:
    enum { COND, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    std::shared_ptr<SwitchCase> FirstCase;
};

#endif // SWITCHSTMT_H
