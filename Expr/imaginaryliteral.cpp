#include "imaginaryliteral.h"

using std::shared_ptr;

ImaginaryLiteral::ImaginaryLiteral(shared_ptr<Expr> val, QualType Ty)
    : Expr(ImaginaryLiteralClass, Ty), Val(val)
{}

ImaginaryLiteral::ImaginaryLiteral(Stmt::EmptyShell Empty)
    : Expr(ImaginaryLiteralClass, Empty)
{}
