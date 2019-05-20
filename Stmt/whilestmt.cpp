#include "whilestmt.h"

WhileStmt::WhileStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> body)
    : Stmt(WhileStmtClass) {
    SubExprs[COND] = cond;
    SubExprs[BODY] = body;
}

WhileStmt::WhileStmt(Stmt::EmptyShell Empty)
    : Stmt(WhileStmtClass, Empty)
{ }
