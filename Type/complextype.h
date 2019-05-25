#ifndef COMPLEXTYPE_H
#define COMPLEXTYPE_H

#include "type.h"
#include "qualtype.h"

class ComplexType : public Type
{
public:
    ComplexType();
private:
    QualType ElementType;
};

#endif // COMPLEXTYPE_H
