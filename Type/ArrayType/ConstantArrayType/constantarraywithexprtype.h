#ifndef CONSTANTARRAYWITHEXPRTYPE_H
#define CONSTANTARRAYWITHEXPRTYPE_H

#include "constantarraytype.h"
#include "Expr/expr.h"

class ConstantArrayWithExprType : public ConstantArrayType
{
public:
    ConstantArrayWithExprType();
private:
    std::shared_ptr<Expr> SizeExpr;
};

#endif // CONSTANTARRAYWITHEXPRTYPE_H
