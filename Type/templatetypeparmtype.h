#ifndef TEMPLATETYPEPARMTYPE_H
#define TEMPLATETYPEPARMTYPE_H

#include "type.h"

class TemplateTypeParmType : public Type
{
public:
    TemplateTypeParmType();
private:
    unsigned Depth : 15;
    unsigned Index : 16;
    unsigned ParameterPack : 1;
};

#endif // TEMPLATETYPEPARMTYPE_H
