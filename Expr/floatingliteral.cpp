#include "floatingliteral.h"

FloatingLiteral::FloatingLiteral(const float &V, bool isexact, QualType Type)
    : Expr(FloatingLiteralClass, Type), Value(V), IsExact(isexact)
{}

FloatingLiteral::FloatingLiteral(Stmt::EmptyShell Empty)
    : Expr(FloatingLiteralClass, Empty), Value(0.0)
{ }
