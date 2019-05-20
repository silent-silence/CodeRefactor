#ifndef BLOCKPOINTERTYPE_H
#define BLOCKPOINTERTYPE_H

#include "type.h"
#include "qualtype.h"

class BlockPointerType : public Type
{
public:
    BlockPointerType();
private:
    QualType PointeeType;
};

#endif // BLOCKPOINTERTYPE_H
