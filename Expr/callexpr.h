#ifndef CALLEXPR_H
#define CALLEXPR_H

#include "expr.h"

class CallExpr : public Expr
{
public:
    CallExpr(std::shared_ptr<Expr> fn, std::shared_ptr<Expr> *args, unsigned numargs, QualType t);
    CallExpr(StmtClass SC, EmptyShell Empty);
protected:
    CallExpr(StmtClass SC, std::shared_ptr<Expr> fn, std::shared_ptr<Expr> *args, unsigned numargs, QualType t);
private:
    enum { FN=0, ARGS_START=1 };
    //std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    unsigned NumArgs;
};

#endif // CALLEXPR_H
