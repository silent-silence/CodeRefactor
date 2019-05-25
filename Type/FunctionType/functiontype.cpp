#include "functiontype.h"

FunctionType::FunctionType(Type::TypeClass tc, QualType res, bool SubclassInfo, unsigned typeQuals, QualType Canonical, bool Dependent, bool noReturn)
    : Type(tc, Canonical, Dependent),
      SubClassData(SubclassInfo),
      TypeQuals(typeQuals),
      NoReturn(noReturn),
      ResultType(res)
{}
