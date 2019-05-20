#ifndef IMAGINARYLITERAL_H
#define IMAGINARYLITERAL_H

#include "expr.h"

class ImaginaryLiteral : public Expr
{
public:
    ImaginaryLiteral(std::shared_ptr<Expr> val, QualType Ty);
    explicit ImaginaryLiteral(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Val;
};

#endif // IMAGINARYLITERAL_H
