#include "compoundstmt.h"

CompoundStmt::CompoundStmt(std::vector<std::shared_ptr<Stmt> > StmtStart, unsigned numStmts)
    : Stmt(CompoundStmtClass), Body(StmtStart), NumStmts(numStmts)
{
}

CompoundStmt::CompoundStmt(Stmt::EmptyShell Empty)
    : Stmt(CompoundStmtClass, Empty), Body(0), NumStmts(0)
{ }
