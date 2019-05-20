#ifndef INTEGERLITERAL_H
#define INTEGERLITERAL_H

#include "expr.h"

class IntegerLiteral : public Expr
{
public:
    IntegerLiteral(const int &V, QualType type);
    explicit IntegerLiteral(EmptyShell Empty);
private:
    int Value;
};

#endif // INTEGERLITERAL_H
