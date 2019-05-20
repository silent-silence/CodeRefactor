#include "expr.h"

Expr::Expr(StmtClass SC, QualType T)
    :Stmt (SC), TypeDependent(false), ValueDependent(false)
{
}

Expr::Expr(Stmt::StmtClass SC, Stmt::EmptyShell)
    :Stmt(SC)
{
}

Expr::Expr(Stmt::StmtClass SC, QualType T, bool TD, bool VD)
    :Stmt (SC), TypeDependent(TD), ValueDependent(VD)
{
    setType(T);
}

QualType Expr::getType() const
{
    return TR;
}

void Expr::setType(const QualType &value)
{
    TR = value;
}

bool Expr::isTypeDependent() const
{
    return TypeDependent;
}

void Expr::setTypeDependent(bool value)
{
    TypeDependent = value;
}

bool Expr::isValueDependent() const
{
    return ValueDependent;
}

void Expr::setValueDependent(bool value)
{
    ValueDependent = value;
}


