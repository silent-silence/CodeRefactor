//
#ifndef EXTQUALTYPE_H
#define EXTQUALTYPE_H

#include "type.h"
#include "qualtype.h"

class ExtQualType : public Type
{
public:
    ExtQualType();
private:
    std::shared_ptr<Type> BaseType;
    unsigned AddressSpace;
    QualType::GCAttrTypes GCAttrType;
};

#endif // EXTQUALTYPE_H
