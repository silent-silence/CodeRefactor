#include "nullstmt.h"

NullStmt::NullStmt()
    : Stmt(NullStmtClass)
{}

NullStmt::NullStmt(Stmt::EmptyShell Empty)
    : Stmt(NullStmtClass, Empty)
{ }
