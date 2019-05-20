#ifndef LABELSTMT_H
#define LABELSTMT_H

#include "stmt.h"

class LabelStmt : public Stmt
{
public:
    LabelStmt(std::shared_ptr<Stmt> substmt);
    explicit LabelStmt(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> SubStmt;
};

#endif // LABELSTMT_H
