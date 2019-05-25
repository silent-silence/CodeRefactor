#ifndef REFERENCETYPE_H
#define REFERENCETYPE_H

#include "../type.h"
#include "../qualtype.h"

class ReferenceType : public Type
{
public:
    ReferenceType();
protected:
private:
    QualType PointeeType;
};

#endif // REFERENCETYPE_H
