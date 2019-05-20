#ifndef AST_TYPE_H
#define AST_TYPE_H

#include <memory>
#include <vector>
#include "Basic/SourceLocation.h"

class Expr;
class Stmt;

class QualType;
class Type;

class ExtQualType;
class QualifierSet;
class BuiltinType;
class FixedWidthIntType;
class ComplexType;
class PointerType;
class BlockPointerType;
class ReferenceType;
class LValueReferenceType;
class RValueReferenceType;
class MemberPointerType;
class ArrayType;
class ConstantArrayType;
class ConstantArrayWithExprType;
class ConstantArrayWithoutExprType;
class IncompleteArrayType;
class VariableArrayType;
class DependentSizedArrayType;

class DependentSizedExtVectorType;
class VectorType;
class ExtVectorType;
class FunctionType;
class FunctionNoProtoType;
class FunctionProtoType;
class TypedefType;
class TypeOfExprType;
class DependentTypeOfExprType;
class TypeOfType;
class DecltypeType;
class DependentDecltypeType;
class TagType;
class RecordType;
class EnumType;
class TemplateTypeParmType;
class TemplateSpecializationType;
class QualifiedNameType;
class TypenameType;
class ObjCInterfaceType;
class ObjCObjectPointerType;

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
    std::shared_ptr<Type> operator->() const;
    bool isNull() const;
    bool isConstQualified() const;
    bool isVolatileQualified() const;
    bool isRestrictQualified() const;
private:
    std::pair<std::shared_ptr<Type>, unsigned> Value;
};

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

class ExtQualType : public Type
{
public:
    ExtQualType(std::shared_ptr<Type> Base,QualType CanonicalPtr, unsigned AddrSpace,QualType::GCAttrTypes gcAttr);
private:
    std::shared_ptr<Type> BaseType;
    unsigned AddressSpace;
    QualType::GCAttrTypes GCAttrType;
};

class QualifierSet
{
public:
    QualifierSet();
private:
    unsigned int Mask;

    static const unsigned int CVRMask = 0x07;
    static const unsigned int GCAttrMask = 0x18;
    static const unsigned int GCAttrShift = 3;
    static const unsigned int AddressSpaceMask = ~(CVRMask | GCAttrMask);
    static const unsigned int AddressSpaceShift = 5;
    static const unsigned MaxAddressSpace = QualType::MaxAddressSpace;
};

class BuiltinType : public Type
{
public:
    enum Kind {
        Void,

        Bool,     // This is bool and/or _Bool.
        Char_U,   // This is 'char' for targets where char is unsigned.
        UChar,    // This is explicitly qualified unsigned char.
        Char16,   // This is 'char16_t' for C++.
        Char32,   // This is 'char32_t' for C++.
        UShort,
        UInt,
        ULong,
        ULongLong,
        UInt128,  // __uint128_t

        Char_S,   // This is 'char' for targets where char is signed.
        SChar,    // This is explicitly qualified signed char.
        WChar,    // This is 'wchar_t' for C++.
        Short,
        Int,
        Long,
        LongLong,
        Int128,   // __int128_t

        Float, Double, LongDouble,

        NullPtr,  // This is the type of C++0x 'nullptr'.

        Overload,  // This represents the type of an overloaded function declaration.
        Dependent, // This represents the type of a type-dependent expression.

        UndeducedAuto, // In C++0x, this represents the type of an auto variable
        // that has not been deduced yet.
        ObjCId,    // This represents the ObjC 'id' type.
        ObjCClass  // This represents the ObjC 'Class' type.
    };
    BuiltinType();
private:
    Kind TypeKind;
};

class FixedWidthIntType : public Type
{
public:
    FixedWidthIntType();
private:
    unsigned Width;
    bool Signed;
};

class ComplexType : public Type
{
public:
    ComplexType();
private:
    QualType ElementType;
};

class PointerType : public Type
{
public:
    PointerType();
private:
    QualType PointeeType;
};

class BlockPointerType : public Type
{
public:
    BlockPointerType();
private:
    QualType PointeeType;
};

class ReferenceType : public Type
{
public:
    ReferenceType();
protected:
private:
    QualType PointeeType;
};

class LValueReferenceType : public ReferenceType
{
public:
    LValueReferenceType();
};

class RValueReferenceType : public ReferenceType
{
public:
    RValueReferenceType();
};

class MemberPointerType : public Type
{
public:
    MemberPointerType();
private:
    QualType PointeeType;
    const std::shared_ptr<Type> Class;
};

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

class ConstantArrayType : public ArrayType
{
public:
    ConstantArrayType();
private:
    int Size;
};

class ConstantArrayWithExprType : public ConstantArrayType
{
public:
    ConstantArrayWithExprType();
private:
    std::shared_ptr<Expr> SizeExpr;
};

class ConstantArrayWithoutExprType : public ConstantArrayType
{
public:
    ConstantArrayWithoutExprType();
};

class IncompleteArrayType : public ArrayType
{
public:
    IncompleteArrayType();
};

class VariableArrayType : public ArrayType
{
public:
    VariableArrayType();
private:
    std::shared_ptr<Stmt> SizeExpr;
};

class DependentSizedArrayType : public ArrayType
{
public:
    DependentSizedArrayType();
private:
    std::shared_ptr<Stmt> SizeExpr;
    QualType ElementType;
    SourceLocation loc;
};



class DependentSizedExtVectorType : public Type
{
private:
    DependentSizedExtVectorType(QualType ElementType, QualType can, std::shared_ptr<Expr> SizeExpr, SourceLocation loc);

    std::shared_ptr<Expr> SizeExpr;
    QualType ElementType;
    SourceLocation loc;
};

class VectorType : public Type
{
public:
protected:
    VectorType(QualType vecType, unsigned nElements, QualType canonType);
    VectorType(TypeClass tc, QualType vecType, unsigned nElements, QualType canonType);
    QualType ElementType;
    unsigned NumElements;
};

class ExtVectorType : public VectorType
{
public:
private:
    ExtVectorType(QualType vecType, unsigned nElements, QualType canonType);
};

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

class FunctionNoProtoType : public FunctionType
{
private:
    FunctionNoProtoType(QualType Result, QualType Canonical, bool NoReturn = false);
};

class FunctionProtoType : public FunctionType
{
public:
private:
    FunctionProtoType(QualType Result, const std::vector<QualType> ArgArray,
                      bool isVariadic, unsigned typeQuals, bool hasExs,
                      bool hasAnyExs, const QualType *ExArray,
                      unsigned numExs, QualType Canonical, bool NoReturn);
    static bool hasAnyDependentType(const std::vector<QualType> ArgArray);
    unsigned NumArgs : 20;
    unsigned NumExceptions : 10;
    bool HasExceptionSpec : 1;
    bool AnyExceptionSpec : 1;
};

class TypedefType : public Type
{
public:
protected:
    /*TypedefType(TypeClass tc, TypedefDecl *D, QualType can)
        : Type(tc, can, can->isDependentType()), Decl(D) {
        assert(!isa<TypedefType>(can) && "Invalid canonical type");
    }*/
    TypedefType(TypeClass tc, QualType can)
        : Type(tc, can, can->isDependentType()){
    }
private:
    //TypedefDecl *Decl;
};

class TypeOfExprType : public Type
{
public:
protected:
    TypeOfExprType(std::shared_ptr<Expr> E, QualType can = QualType());
private:
    std::shared_ptr<Expr> TOExpr;
};

class DependentTypeOfExprType : public TypeOfExprType
{
public:
    DependentTypeOfExprType(std::shared_ptr<Expr> E);
};

class TypeOfType  : public Type
{
public:
private:
    TypeOfType(QualType T, QualType can);
    QualType TOType;
};

class DecltypeType : public Type
{
public:
protected:
    DecltypeType(std::shared_ptr<Expr> E, QualType underlyingType, QualType can = QualType());
private:
    std::shared_ptr<Expr> E;
    QualType UnderlyingType;
};

class DependentDecltypeType : public DecltypeType
{
public:
    //DependentDecltypeType(std::shared_ptr<Expr> E);
};

class TagType : public Type
{
public:
protected:
    //TagType(TypeClass TC, TagDecl *D, QualType can);
    TagType(TypeClass TC, QualType can);
private:
    //mutable llvm::PointerIntPair<TagDecl *, 1> decl;
};

class RecordType : public TagType
{
public:
protected:
    /*explicit RecordType(RecordDecl *D)
        : TagType(Record, reinterpret_cast<TagDecl*>(D), QualType()) { }
    explicit RecordType(TypeClass TC, RecordDecl *D)
        : TagType(TC, reinterpret_cast<TagDecl*>(D), QualType()) { }*/
};

class EnumType : public TagType
{
public:
private:
    //explicit EnumType(EnumDecl *D)
    //    : TagType(Enum, reinterpret_cast<TagDecl*>(D), QualType()) { }
};

class TemplateTypeParmType : public Type
{
public:

private:
    /*TemplateTypeParmType(unsigned D, unsigned I, bool PP, IdentifierInfo *N,
                         QualType Canon)
        : Type(TemplateTypeParm, Canon, true),
          Depth(D), Index(I), ParameterPack(PP), Name(N) { }

    TemplateTypeParmType(unsigned D, unsigned I, bool PP)
        : Type(TemplateTypeParm, QualType(this, 0), true),
          Depth(D), Index(I), ParameterPack(PP), Name(0) { }*/
    TemplateTypeParmType(unsigned D, unsigned I, bool PP, QualType Canon);
    TemplateTypeParmType(unsigned D, unsigned I, bool PP);

    unsigned Depth : 15;
    unsigned Index : 16;
    unsigned ParameterPack : 1;
    //IdentifierInfo *Name;
};

class TemplateSpecializationType : public Type
{
public:
private:
    /*TemplateSpecializationType(ASTContext &Context,
                                 TemplateName T,
                                 const TemplateArgument *Args,
                                 unsigned NumArgs, QualType Canon);*/
    TemplateSpecializationType(unsigned NumArgs, QualType Canon);

    unsigned NumArgs;
    //TemplateName Template;
};

class QualifiedNameType : public Type
{
public:
    QualifiedNameType();
private:
    QualType NamedType;
};

class TypenameType : public Type
{
public:
    TypenameType();
};

class ObjCInterfaceType : public Type
{

};

class ObjCObjectPointerType : public Type
{
private:
    QualType PointeeType;
};

#endif // AST_TYPE_H
