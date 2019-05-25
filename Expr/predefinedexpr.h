#ifndef PREDEFINEDEXPR_H
#define PREDEFINEDEXPR_H

#include "expr.h"

class PredefinedExpr : public Expr
{
public:
    enum IdentType {
        Func,
        Function,
        PrettyFunction
    };
    PredefinedExpr(QualType type, IdentType IT);
    explicit PredefinedExpr(EmptyShell Empty);
private:
    IdentType Type;
};

#endif // PREDEFINEDEXPR_H
