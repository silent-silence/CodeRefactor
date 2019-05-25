#include "integerliteral.h"

IntegerLiteral::IntegerLiteral(const int &V, QualType type)
    : Expr(IntegerLiteralClass, type), Value(V)
{
}

IntegerLiteral::IntegerLiteral(Stmt::EmptyShell Empty)
    : Expr(IntegerLiteralClass, Empty)
{ }
