#ifndef INDIRECTGOTOSTMT_H
#define INDIRECTGOTOSTMT_H

#include "stmt.h"
#include "../Expr/expr.h"

class IndirectGotoStmt : public Stmt
{
public:
    IndirectGotoStmt(std::shared_ptr<Expr> target);

    explicit IndirectGotoStmt(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Target;
};

#endif // INDIRECTGOTOSTMT_H
