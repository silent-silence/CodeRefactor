#ifndef CONTINUESTMT_H
#define CONTINUESTMT_H

#include "stmt.h"

class ContinueStmt : public Stmt
{
public:
    ContinueStmt() : Stmt(ContinueStmtClass){}
    explicit ContinueStmt(EmptyShell Empty) : Stmt(ContinueStmtClass, Empty) { }
};

#endif // CONTINUESTMT_H
