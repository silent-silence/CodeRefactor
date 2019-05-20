#include "characterliteral.h"

CharacterLiteral::CharacterLiteral(unsigned value, bool iswide, QualType type)
    : Expr(CharacterLiteralClass, type), Value(value), IsWide(iswide)
{
}

CharacterLiteral::CharacterLiteral(Stmt::EmptyShell Empty)
    : Expr(CharacterLiteralClass, Empty)
{
}
