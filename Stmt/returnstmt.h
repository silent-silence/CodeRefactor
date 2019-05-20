#ifndef RETURNSTMT_H
#define RETURNSTMT_H

#include "stmt.h"
#include "../Expr/expr.h"

class ReturnStmt : public Stmt
{
public:
    ReturnStmt(std::shared_ptr<Expr> E = 0);
    explicit ReturnStmt(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> RetExpr;
};

#endif // RETURNSTMT_H
