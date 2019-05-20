#include "switchstmt.h"

SwitchStmt::SwitchStmt(std::shared_ptr<Expr> cond)
    : Stmt(SwitchStmtClass), FirstCase(0)
{
    SubExprs[COND] = cond;
    SubExprs[BODY] = nullptr;
}

SwitchStmt::SwitchStmt(Stmt::EmptyShell Empty)
    : Stmt(SwitchStmtClass, Empty) { }
