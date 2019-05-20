#include "indirectgotostmt.h"

IndirectGotoStmt::IndirectGotoStmt(std::shared_ptr<Expr> target)
    : Stmt(IndirectGotoStmtClass), Target(target)
{}

IndirectGotoStmt::IndirectGotoStmt(Stmt::EmptyShell Empty)
    : Stmt(IndirectGotoStmtClass, Empty)
{ }
