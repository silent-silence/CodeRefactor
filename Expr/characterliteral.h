#ifndef CHARACTERLITERAL_H
#define CHARACTERLITERAL_H

#include "expr.h"

class CharacterLiteral : public Expr
{
public:
    CharacterLiteral(unsigned value, bool iswide, QualType type);
    CharacterLiteral(EmptyShell Empty);
private:
    unsigned Value;
    bool IsWide;
};

#endif // CHARACTERLITERAL_H
