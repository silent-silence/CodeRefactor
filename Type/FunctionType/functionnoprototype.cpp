#include "functionnoprototype.h"

FunctionNoProtoType::FunctionNoProtoType(QualType Result, QualType Canonical, bool NoReturn)
    : FunctionType(FunctionNoProto, Result, false, 0, Canonical, false, NoReturn)
{}
