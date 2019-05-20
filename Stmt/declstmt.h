#ifndef DECLSTMT_H
#define DECLSTMT_H

#include "stmt.h"

class DeclStmt : public Stmt
{
public:
    DeclStmt() : Stmt(DeclStmtClass){}
    explicit DeclStmt(EmptyShell Empty) : Stmt(DeclStmtClass, Empty) { }
};

#endif // DECLSTMT_H
