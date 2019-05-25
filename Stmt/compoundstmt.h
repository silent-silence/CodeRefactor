#ifndef COMPOUNDSTMT_H
#define COMPOUNDSTMT_H

#include "stmt.h"

class CompoundStmt : public Stmt
{
public:
    CompoundStmt(std::vector<std::shared_ptr<Stmt>> StmtStart, unsigned numStmts);
    explicit CompoundStmt(EmptyShell Empty);
private:
    std::vector<std::shared_ptr<Stmt>> Body;
    unsigned NumStmts;
};

#endif // COMPOUNDSTMT_H
