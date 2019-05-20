#ifndef FLOATINGLITERAL_H
#define FLOATINGLITERAL_H

#include "expr.h"

class FloatingLiteral : public Expr
{
public:
    FloatingLiteral(const float &V, bool isexact, QualType Type);
    explicit FloatingLiteral(EmptyShell Empty);
private:
    float Value;
    bool IsExact : 1;
};

#endif // FLOATINGLITERAL_H
