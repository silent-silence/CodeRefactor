#include "gotostmt.h"

GotoStmt::GotoStmt(LabelStmt *label)
    : Stmt(GotoStmtClass), Label(label)
{}

GotoStmt::GotoStmt(Stmt::EmptyShell Empty)
    : Stmt(GotoStmtClass, Empty) { }
