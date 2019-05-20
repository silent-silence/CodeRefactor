#ifndef DEPENDENTSIZEDARRAYTYPE_H
#define DEPENDENTSIZEDARRAYTYPE_H

#include "arraytype.h"
#include "Stmt/stmt.h"

class DependentSizedArrayType : public ArrayType
{
public:
    DependentSizedArrayType();
private:
    std::shared_ptr<Stmt> SizeExpr;
};

#endif // DEPENDENTSIZEDARRAYTYPE_H
