#ifndef POINTERTYPE_H
#define POINTERTYPE_H

#include "type.h"
#include "qualtype.h"

class PointerType : public Type
{
public:
    PointerType();
private:
    QualType PointeeType;
};

#endif // POINTERTYPE_H
