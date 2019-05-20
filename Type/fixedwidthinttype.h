#ifndef FIXEDWIDTHINTTYPE_H
#define FIXEDWIDTHINTTYPE_H

#include "type.h"

class FixedWidthIntType : public Type
{
public:
    FixedWidthIntType();
private:
  unsigned Width;
  bool Signed;
};

#endif // FIXEDWIDTHINTTYPE_H
