#ifndef VARIABLEARRAYTYPE_H
#define VARIABLEARRAYTYPE_H

#include "arraytype.h"
#include "Stmt/stmt.h"

class VariableArrayType : public ArrayType
{
public:
    VariableArrayType();
private:
    std::shared_ptr<Stmt> SizeExpr;
};

#endif // VARIABLEARRAYTYPE_H
