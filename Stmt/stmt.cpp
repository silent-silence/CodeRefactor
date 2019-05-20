#include "stmt.h"

Stmt::Stmt(Stmt::StmtClass SC)
    : sClass(SC), RefCount(1)
{
}

Stmt::Stmt(Stmt::StmtClass SC, Stmt::EmptyShell)
    : sClass(SC), RefCount(1)
{
}
