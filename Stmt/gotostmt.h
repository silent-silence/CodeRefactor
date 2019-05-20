#ifndef GOTOSTMT_H
#define GOTOSTMT_H

#include "stmt.h"
#include "labelstmt.h"

class GotoStmt : public Stmt
{
public:
    GotoStmt(LabelStmt *label);
    explicit GotoStmt(EmptyShell Empty);
private:
    std::shared_ptr<LabelStmt> Label;
};

#endif // GOTOSTMT_H
