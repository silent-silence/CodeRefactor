#include "ifstmt.h"

IfStmt::IfStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> then, std::shared_ptr<Stmt> elsev)
    : Stmt(IfStmtClass)
{
    SubExprs[COND] = cond;
    SubExprs[THEN] = then;
    SubExprs[ELSE] = elsev;
}

IfStmt::IfStmt(Stmt::EmptyShell Empty)
    : Stmt(IfStmtClass, Empty)
{ }
