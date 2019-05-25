#ifndef QUALIFIEDNAMETYPE_H
#define QUALIFIEDNAMETYPE_H

#include "type.h"
#include "qualtype.h"

class QualifiedNameType : public Type
{
public:
    QualifiedNameType();
private:
    QualType NamedType;
};

#endif // QUALIFIEDNAMETYPE_H
