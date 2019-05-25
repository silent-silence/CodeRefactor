#ifndef DEFAULTSTMT_H
#define DEFAULTSTMT_H

#include "switchcase.h"

class DefaultStmt : public SwitchCase
{
public:
    DefaultStmt(std::shared_ptr<Stmt> substmt) :
        SwitchCase(DefaultStmtClass), SubStmt(substmt){}

      explicit DefaultStmt(EmptyShell) : SwitchCase(DefaultStmtClass) { }
private:
    std::shared_ptr<Stmt> SubStmt;
};

#endif // DEFAULTSTMT_H
