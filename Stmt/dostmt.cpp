#include "dostmt.h"

DoStmt::DoStmt(std::shared_ptr<Stmt> body, std::shared_ptr<Expr> cond)
    : Stmt(DoStmtClass){
    SubExprs[COND] = cond;
    SubExprs[BODY] = body;
}

DoStmt::DoStmt(Stmt::EmptyShell Empty)
    : Stmt(DoStmtClass, Empty) { }
