#ifndef MEMBERPOINTERTYPE_H
#define MEMBERPOINTERTYPE_H

#include "type.h"
#include "qualtype.h"

class MemberPointerType : public Type
{
public:
    MemberPointerType();
private:
    QualType PointeeType;
    const std::shared_ptr<Type> Class;
};

#endif // MEMBERPOINTERTYPE_H
