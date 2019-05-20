#include "returnstmt.h"

ReturnStmt::ReturnStmt(std::shared_ptr<Expr> E)
    : Stmt(ReturnStmtClass), RetExpr(E)
{}

ReturnStmt::ReturnStmt(Stmt::EmptyShell Empty)
    : Stmt(ReturnStmtClass, Empty)
{ }
