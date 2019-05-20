#include "type.h"
#include "qualtype.h"

QualType::QualType()
{

}

QualType::QualType(const std::shared_ptr<Type> Ptr, unsigned Quals)
    : Value(Ptr, Quals)
{}

std::shared_ptr<Type> QualType::getTypePtr() const
{
    return Value.first;
}

unsigned QualType::getCVRQualifiers() const
{
    return Value.second;
}

bool QualType::isNull() const
{
    return getTypePtr() == nullptr;
}

bool QualType::isConstQualified() const
{
    return (getCVRQualifiers() & Const) ? true : false;
}

bool QualType::isVolatileQualified() const
{
    return (getCVRQualifiers() & Volatile) ? true : false;
}

bool QualType::isRestrictQualified() const
{
    return (getCVRQualifiers() & Restrict) ? true : false;
}
