#ifndef TYPEOFTYPE_H
#define TYPEOFTYPE_H

#include "type.h"
#include "qualtype.h"

class TypeOfType  : public Type
{
public:
    TypeOfType();
private:
    QualType TOType;
};

#endif // TYPEOFTYPE_H
