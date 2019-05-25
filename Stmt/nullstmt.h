#ifndef NULLSTMT_H
#define NULLSTMT_H

#include "stmt.h"

class NullStmt : public Stmt
{
public:
    NullStmt();
    explicit NullStmt(EmptyShell Empty);
};

#endif // NULLSTMT_H
