#ifndef FUNCTIONTYPE_H
#define FUNCTIONTYPE_H

#include "../type.h"
#include "../qualtype.h"

class FunctionType : public Type
{
public:
protected:
    FunctionType(TypeClass tc, QualType res, bool SubclassInfo,
                   unsigned typeQuals, QualType Canonical, bool Dependent,
                   bool noReturn = false);
private:
    bool SubClassData : 1;
    unsigned TypeQuals : 3;
    unsigned NoReturn : 1;
    QualType ResultType;
};

#endif // FUNCTIONTYPE_H
