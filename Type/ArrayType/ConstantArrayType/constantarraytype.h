#ifndef CONSTANTARRAYTYPE_H
#define CONSTANTARRAYTYPE_H

#include "../arraytype.h"

class ConstantArrayType : public ArrayType
{
public:
    ConstantArrayType();
private:
    int Size;
};

#endif // CONSTANTARRAYTYPE_H
