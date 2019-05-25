#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include "../type.h"
#include "../qualtype.h"

class ArrayType : public Type
{
public:
    enum ArraySizeModifier {
        Normal, Static, Star
    };
protected:
private:
  QualType ElementType;
  unsigned SizeModifier : 2;
  unsigned IndexTypeQuals : 3;
};

#endif // ARRAYTYPE_H
