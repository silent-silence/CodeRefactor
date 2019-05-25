#include "type.h"

bool Type::isDependentType() const
{
    return Dependent;
}

Type::Type(Type::TypeClass tc, QualType Canonical, bool dependent)
    : CanonicalType(Canonical.isNull() ? QualType(shared_from_this(), 0) : Canonical),
      Dependent(dependent), TC(tc) {}

Type::Type(const Type &)
{

}
