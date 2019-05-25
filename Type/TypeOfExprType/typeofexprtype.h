#ifndef TYPEOFEXPRTYPE_H
#define TYPEOFEXPRTYPE_H

#include "../type.h"
#include "Expr/expr.h"

class TypeOfExprType : public Type
{
public:
    TypeOfExprType();
protected:
    TypeOfExprType(std::shared_ptr<Expr> E, QualType can = QualType());
private:
    std::shared_ptr<Expr> TOExpr;
};

#endif // TYPEOFEXPRTYPE_H
