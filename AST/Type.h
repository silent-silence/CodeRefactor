#ifndef AST_TYPE_H
#define AST_TYPE_H

#include <memory>
#include <vector>
#include "Basic/SourceLocation.h"

class Expr;
class Stmt;
class ASTContext;
class TagDecl;

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
    QualType(const std::weak_ptr<Type> Ptr, unsigned Quals);
    std::shared_ptr<Type> getTypePtr() const;
    unsigned getCVRQualifiers() const;
    std::shared_ptr<Type> operator->() const;
    bool isNull() const;
    bool isConstQualified() const;
    bool isVolatileQualified() const;
    bool isRestrictQualified() const;
private:
    std::pair<std::weak_ptr<Type>, unsigned> Value;
};

/// @brief This is the base class of the type hierarchy
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
    Type(TypeClass tc, bool dependent);
    bool isDependentType() const;
    Type(std::shared_ptr<Type> type);
	QualType getCanonicalType() const { return CanonicalType; }

protected:
    enum { TypeClassBitSize = 6 };
    static std::shared_ptr<Type> creator(TypeClass tc, QualType Canonical, bool dependent);

private:
    Type(const Type &)=default;
    void setCanonicalType(const QualType &value);

    QualType CanonicalType;
    bool Dependent : 1;
    unsigned TC : TypeClassBitSize;

};

/// @extends
/// @brief C embedded extensions
class ExtQualType : public Type
{
public:
    ExtQualType(std::shared_ptr<Type> Base,QualType CanonicalPtr, unsigned AddrSpace,QualType::GCAttrTypes gcAttr);
private:
    std::shared_ptr<Type> BaseType;
    unsigned AddressSpace;
    QualType::GCAttrTypes GCAttrType;
};

/// @brief This class is used to collect qualifiers: const, volatile, and restrict
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

/// @brief This class is used for builtin types
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
    BuiltinType(Kind k);
private:
    Kind TypeKind;
};

/// @brief Used for arbitrary width types that we either don't want to or can't map to named integer types
class FixedWidthIntType : public Type
{
public:
    FixedWidthIntType(unsigned W,bool S);
private:
    unsigned Width;
    bool Signed;
};

/// @brief Complex values
class ComplexType : public Type
{
public:
    ComplexType(QualType Element, QualType CanonicalPtr);
private:
    QualType ElementType;
};

/// @brief Pointer Declarators
class PointerType : public Type
{
public:
    PointerType(QualType Pointee, QualType CanonicalPtr);
private:
    QualType PointeeType;
};

/// @brief Pointer to a block type. Pointee is required to always be a function type
/// @example "void (^)(int)"
class BlockPointerType : public Type
{
public:
    BlockPointerType(QualType Pointee, QualType CanonicalCls);
private:
    QualType PointeeType;
};

/// @C++
/// @brief Base for LValueReferenceType and RValueReferenceType
class ReferenceType : public Type
{
public:
    ReferenceType(TypeClass tc, QualType Referencee, QualType CanonicalRef);
protected:
private:
    QualType PointeeType;
};

/// @C++
/// @brief C++ Lvalue reference
class LValueReferenceType : public ReferenceType
{
public:
    LValueReferenceType(QualType Referencee, QualType CanonicalRef);
};

/// @C++
/// @brief C++0x Rvalue reference
class RValueReferenceType : public ReferenceType
{
public:
    RValueReferenceType(QualType Referencee, QualType CanonicalRef);
};

/// @C++
/// @brief C++ pointers to members
/// @example "A::B"
class MemberPointerType : public Type
{
public:
    MemberPointerType(QualType Pointee, const std::shared_ptr<Type>Cls, QualType CanonicalPtr);
private:
    QualType PointeeType;
    const std::shared_ptr<Type> Class;
};

/// @brief Array Declarators
class ArrayType : public Type
{
public:
    enum ArraySizeModifier {
        Normal, Static, Star
    };
protected:
    ArrayType(TypeClass tc, QualType et, QualType can,
              ArraySizeModifier sm, unsigned tq);

private:
    QualType ElementType;
    unsigned SizeModifier : 2;
    unsigned IndexTypeQuals : 3;
};

/// @brief This class represents the canonical version of C arrays with a specified constant size
/// @example "int a[2 + 1];"
class ConstantArrayType : public ArrayType
{
public:
    ConstantArrayType(QualType et, QualType can,int size,
                      ArraySizeModifier sm, unsigned tq);

protected:
    ConstantArrayType(TypeClass tc, QualType et, QualType can,
                      int size, ArraySizeModifier sm, unsigned tq);
private:
    int Size;
};

/// @brief This class represents C arrays with a constant size specified by means of an integer constant expression
/// @example "int a[sizeof(int)];"
class ConstantArrayWithExprType : public ConstantArrayType
{
public:
    ConstantArrayWithExprType(QualType et, QualType can,
                              int size,std::shared_ptr<Expr>e,
                              ArraySizeModifier sm, unsigned tq);
private:
    std::shared_ptr<Expr> SizeExpr;
};

/// @brief This class represents C arrays with a
/// constant size that was not specified by an integer constant expression,
/// but inferred by static semantics.
/// @example "int a[] = {1, 2, 3};"
class ConstantArrayWithoutExprType : public ConstantArrayType
{
public:
    ConstantArrayWithoutExprType(QualType et, QualType can,
                                 int size,
                                 ArraySizeModifier sm, unsigned tq);
};

/// @brief This class represents C arrays with an unspecified size
/// @example "int a[];"
class IncompleteArrayType : public ArrayType
{
public:
    IncompleteArrayType(QualType et, QualType can,
                        ArraySizeModifier sm, unsigned tq);
};

/// @brief This class represents C arrays with a specified size
/// which is not an integer-constant-expression
/// @example "int a[x + foo()];"
class VariableArrayType : public ArrayType
{
public:
    VariableArrayType(QualType et, QualType can, std::shared_ptr<Expr>e,
                      ArraySizeModifier sm, unsigned tq);
private:
    std::shared_ptr<Stmt> SizeExpr;
};

/// @C++
/// @brief C++ This type represents an array type
/// whose size is a value-dependent expression
/// @example
/// @code
/// template<typename T, int Size>
/// class array {
///   T data[Size];
/// };
/// @endcode
class DependentSizedArrayType : public ArrayType
{
public:
    DependentSizedArrayType( QualType et, QualType can,
                             std::shared_ptr<Expr>e, ArraySizeModifier sm, unsigned tq);
private:
    std::shared_ptr<Stmt> SizeExpr;
    QualType ElementType;
    SourceLocation loc;
};

/// @C++
/// @brief C++ This type represent an extended vector type
/// where either the type or size is dependent. For example:
/// @code
/// template<typename T, int Size>
/// class vector {
///   typedef T __attribute__((ext_vector_type(Size))) type;
/// }
/// @endcode
class DependentSizedExtVectorType : public Type
{
private:
    DependentSizedExtVectorType(QualType ElementType, QualType can, std::shared_ptr<Expr> SizeExpr, SourceLocation loc);

    std::shared_ptr<Expr> SizeExpr;
    QualType ElementType;
    SourceLocation loc;
};

/// @extends
/// @brief GCC generic vector type
class VectorType : public Type
{
public:
    VectorType(QualType vecType, unsigned nElements, QualType canonType);
protected:
    VectorType(TypeClass tc, QualType vecType, unsigned nElements, QualType canonType);
    QualType ElementType;
    unsigned NumElements;
};

/// @extends
/// @brief Extended vector type
class ExtVectorType : public VectorType
{
public:
    ExtVectorType(QualType vecType, unsigned nElements, QualType canonType);
private:

};

/// @brief Function Declarators
class FunctionType : public Type
{
public:
    FunctionType(TypeClass tc, QualType res, bool SubclassInfo,
                 unsigned typeQuals, QualType Canonical, bool Dependent,
                 bool noReturn = false);
private:
    bool SubClassData : 1;
    unsigned TypeQuals : 3;
    unsigned NoReturn : 1;
    QualType ResultType;
};

/// @brief Represents a K&R-style 'int foo()' function, which has
/// no information available about its arguments
class FunctionNoProtoType : public FunctionType
{
public:
    FunctionNoProtoType(QualType Result, QualType Canonical, bool NoReturn = false);
};

/// @brief Represents a prototype with argument type info
/// @example "int foo(int)" or "int foo(void)"
class FunctionProtoType : public FunctionType
{
public:
    FunctionProtoType(QualType Result, const std::vector<QualType> ArgArray,
                      bool isVariadic, unsigned typeQuals, bool hasExs,
                      bool hasAnyExs, const QualType *ExArray,
                      unsigned numExs, QualType Canonical, bool NoReturn);
private:
    static bool hasAnyDependentType(const std::vector<QualType> ArgArray);
    unsigned NumArgs : 20;
    unsigned NumExceptions : 10;
    bool HasExceptionSpec : 1;
    bool AnyExceptionSpec : 1;
};

/// @brief typedef
class TypedefType : public Type
{
public:
protected:
    /*TypedefType(TypeClass tc, TypedefDecl *D, QualType can)
        : Type(tc, can, can->isDependentType()), Decl(D) {
        assert(!isa<TypedefType>(can) && "Invalid canonical type");
    }*/
    TypedefType(TypeClass tc, QualType can)
        : Type(Type::creator(tc, can, can->isDependentType())){
    }
private:
    //TypedefDecl *Decl;
};

/// @extends
/// @brief GCC extension
class TypeOfExprType : public Type
{
public:
    TypeOfExprType(std::shared_ptr<Expr> E, QualType can = QualType());
private:
    std::shared_ptr<Expr> TOExpr;
};

/// @extends
class DependentTypeOfExprType : public TypeOfExprType
{
public:
    DependentTypeOfExprType(std::shared_ptr<Expr> E);
};

/// @extends
/// @brief GCC extension
class TypeOfType  : public Type
{
public:
    TypeOfType(QualType T, QualType can);
private:
    QualType TOType;
};

/// @C++
/// @brief C++0x
class DecltypeType : public Type
{
public:
    DecltypeType(std::shared_ptr<Expr> E, QualType underlyingType, QualType can = QualType());
private:
    std::shared_ptr<Expr> E;
    QualType UnderlyingType;
};

/// @C++
class DependentDecltypeType : public DecltypeType
{
public:
    DependentDecltypeType(std::shared_ptr<Expr> E);
};

/// @brief a struct/union/class/enum
class TagType : public Type
{
public:
protected:
    TagType(TypeClass TC, std::shared_ptr<TagDecl> D, QualType can);
    TagType(TypeClass TC, QualType can);
private:
    //mutable llvm::PointerIntPair<TagDecl *, 1> decl;
};

/// @brief structs/unions/classes
class RecordType : public TagType
{
public:
protected:
    /*explicit RecordType(RecordDecl *D)
        : TagType(Record, reinterpret_cast<TagDecl*>(D), QualType()) { }
    explicit RecordType(TypeClass TC, RecordDecl *D)
        : TagType(TC, reinterpret_cast<TagDecl*>(D), QualType()) { }*/
};

/// @brief enum
class EnumType : public TagType
{
public:
private:
    //explicit EnumType(EnumDecl *D)
    //    : TagType(Enum, reinterpret_cast<TagDecl*>(D), QualType()) { }
};

/// @C++
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

/// @C++
/// @brief Represents the type of a template specialization as written in the source code
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

/// @C++
/// @brief Represents a type that was referred to via a qualified name
/// @example "N::M::type"
class QualifiedNameType : public Type
{
public:
    QualifiedNameType();
private:
    QualType NamedType;
};

/// C++
/// @brief Represents a 'typename' specifier that names a type within a dependent type
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
