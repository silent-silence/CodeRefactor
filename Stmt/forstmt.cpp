#include "forstmt.h"

using std::shared_ptr;

ForStmt::ForStmt(shared_ptr<Stmt> Init, shared_ptr<Expr> Cond, shared_ptr<Expr> Inc, shared_ptr<Stmt> Body)
    : Stmt(ForStmtClass) {
    SubExprs[INIT] = Init;
    SubExprs[COND] = Cond;
    SubExprs[INC] = Inc;
    SubExprs[BODY] = Body;
}

ForStmt::ForStmt(Stmt::EmptyShell Empty)
    : Stmt(ForStmtClass, Empty) { }
