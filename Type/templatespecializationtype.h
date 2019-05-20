#ifndef TEMPLATESPECIALIZATIONTYPE_H
#define TEMPLATESPECIALIZATIONTYPE_H

#include "type.h"

class TemplateSpecializationType : public Type
{
public:
    TemplateSpecializationType();
private:
    unsigned NumArgs;
};

#endif // TEMPLATESPECIALIZATIONTYPE_H
