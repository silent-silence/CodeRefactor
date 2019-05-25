#include "labelstmt.h"

LabelStmt::LabelStmt(std::shared_ptr<Stmt> substmt)
    : Stmt(LabelStmtClass), SubStmt(substmt)
{}

LabelStmt::LabelStmt(Stmt::EmptyShell Empty)
    : Stmt(LabelStmtClass, Empty)
{ }
