#include "casestmt.h"

CaseStmt::CaseStmt(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs)
    : SwitchCase(CaseStmtClass)
{
    SubExprs[SUBSTMT] = nullptr;
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
}

CaseStmt::CaseStmt(Stmt::EmptyShell Empty)
    : SwitchCase(CaseStmtClass) { }
