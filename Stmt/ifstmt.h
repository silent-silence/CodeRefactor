#ifndef IFSTMT_H
#define IFSTMT_H

#include "stmt.h"
#include "../Expr/expr.h"

class IfStmt : public Stmt
{
public:
    IfStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> then, std::shared_ptr<Stmt> elsev = nullptr);

    explicit IfStmt(EmptyShell Empty);
private:
    enum { COND, THEN, ELSE, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

#endif // IFSTMT_H
