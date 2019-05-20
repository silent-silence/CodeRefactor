#ifndef VECTORTYPE_H
#define VECTORTYPE_H

#include "../type.h"
#include "../qualtype.h"

class VectorType : public Type
{
public:
    VectorType();
private:
    QualType ElementType;
    unsigned NumElements;
};

#endif // VECTORTYPE_H
