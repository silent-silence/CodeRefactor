#ifndef COMPOUNDLITERALEXPR_H
#define COMPOUNDLITERALEXPR_H

#include "expr.h"

class CompoundLiteralExpr : public Expr
{
public:
    CompoundLiteralExpr(QualType ty, std::shared_ptr<Expr> init, bool fileScope);
    explicit CompoundLiteralExpr(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Init;
    bool FileScope;
};

#endif // COMPOUNDLITERALEXPR_H
