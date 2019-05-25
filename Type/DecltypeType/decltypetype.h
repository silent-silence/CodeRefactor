#ifndef DECLTYPETYPE_H
#define DECLTYPETYPE_H

#include "../type.h"
#include "../qualtype.h"
#include "Expr/expr.h"

class DecltypeType : public Type
{
public:
protected:
    DecltypeType(std::shared_ptr<Expr> E, QualType underlyingType, QualType can = QualType());
private:
    Expr *E;
    QualType UnderlyingType;
};

#endif // DECLTYPETYPE_H
