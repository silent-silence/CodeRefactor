#ifndef BREAKSTMT_H
#define BREAKSTMT_H

#include "stmt.h"

class BreakStmt : public Stmt
{
public:
    BreakStmt() : Stmt(BreakStmtClass){}
    explicit BreakStmt(EmptyShell Empty) : Stmt(BreakStmtClass, Empty) { }
};

#endif // BREAKSTMT_H
