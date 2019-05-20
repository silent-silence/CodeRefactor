#ifndef FUNCTIONNOPROTOTYPE_H
#define FUNCTIONNOPROTOTYPE_H

#include "functiontype.h"

class FunctionNoProtoType : public FunctionType
{
public:
private:
    FunctionNoProtoType(QualType Result, QualType Canonical,
                          bool NoReturn = false);
};

#endif // FUNCTIONNOPROTOTYPE_H
