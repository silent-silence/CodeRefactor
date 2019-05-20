#ifndef AST_TYPE_H
#define AST_TYPE_H

#include <memory>

class Type;

class QualType
{
public:
    enum TQ {
        Const    = 0x1,
        Restrict = 0x2,
        Volatile = 0x4,
        CVRFlags = Const|Restrict|Volatile
    };

    enum GCAttrTypes {
        GCNone = 0,
        Weak,
        Strong
    };
    static const unsigned MaxAddressSpace = 0xffffffu;

    QualType();
    QualType(const std::shared_ptr<Type> Ptr, unsigned Quals);
    std::shared_ptr<Type> getTypePtr() const;
    unsigned getCVRQualifiers() const;
    bool isNull() const;
    bool isConstQualified() const;
    bool isVolatileQualified() const;
    bool isRestrictQualified() const;
private:
    std::pair<std::shared_ptr<Type>, unsigned> Value;
};

#endif // AST_TYPE_H
