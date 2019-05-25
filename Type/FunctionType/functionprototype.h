#ifndef FUNCTIONPROTOTYPE_H
#define FUNCTIONPROTOTYPE_H

#include "functiontype.h"

class FunctionProtoType : public FunctionType
{
public:
    //FunctionProtoType();
private:
    unsigned NumArgs : 20;
    unsigned NumExceptions : 10;
    bool HasExceptionSpec : 1;
    bool AnyExceptionSpec : 1;
};

#endif // FUNCTIONPROTOTYPE_H
