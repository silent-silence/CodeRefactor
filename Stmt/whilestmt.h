#ifndef WHILESTMT_H
#define WHILESTMT_H

#include "stmt.h"
#include "../Expr/expr.h"

class WhileStmt : public Stmt
{
public:
    WhileStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> body);
    explicit WhileStmt(EmptyShell Empty);
private:
    enum { COND, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

#endif // WHILESTMT_H
