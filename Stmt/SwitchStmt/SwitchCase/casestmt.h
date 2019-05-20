#ifndef CASESTMT_H
#define CASESTMT_H

#include "switchcase.h"
#include "Expr/expr.h"

class CaseStmt : public SwitchCase
{
public:
    CaseStmt(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs);
    explicit CaseStmt(EmptyShell Empty);
private:
    enum { SUBSTMT, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

#endif // CASESTMT_H
