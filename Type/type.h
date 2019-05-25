#ifndef TYPE_H
#define TYPE_H

#include <map>
#include <memory>
#include "qualtype.h"

class Type : public std::enable_shared_from_this<Type>
{
public:
    enum TypeClass {
#define TYPE(Class, Base) Class,
#define ABSTRACT_TYPE(Class, Base)
#include "TypeNodes.def"
        TagFirst = Record, TagLast = Enum
    };
    Type(){}
    bool isDependentType() const;
protected:
    enum { TypeClassBitSize = 6 };
    Type(TypeClass tc, QualType Canonical, bool dependent);
private:
    Type(const Type&);

    QualType CanonicalType;
    bool Dependent : 1;
    unsigned TC : TypeClassBitSize;

};

#endif // TYPE_H
