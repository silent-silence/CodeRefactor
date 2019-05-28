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

class ExtQualType;//例子
class QualifierSet;//
class BuiltinType;//
class FixedWidthIntType;//
class ComplexType;//
class PointerType;//
class BlockPointerType;//
class ReferenceType;//
class LValueReferenceType;//
class RValueReferenceType;//
class MemberPointerType;//
class ArrayType;//
class ConstantArrayType;//
class ConstantArrayWithExprType;//
class ConstantArrayWithoutExprType;//
class IncompleteArrayType;//
class VariableArrayType;//
class DependentSizedArrayType;//

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

    std::weak_ptr<QualType> getCanonicalType() const;
    void setCanonicalType(const std::weak_ptr<QualType> &value);

protected:
    enum { TypeClassBitSize = 6 };
private:
    //    Type(const Type&)=default;           // DO NOT IMPLEMENT.
    //    void operator=(const Type&)=default;

    std::weak_ptr<QualType> CanonicalType;
    bool Dependent : 1;
    unsigned TC : TypeClassBitSize;

};

/// @extends
/// @brief C embedded extensions
class ExtQualType : public Type
{
public:
    static std::shared_ptr<Type> creator(
            std::shared_ptr<Type> Base,
            std::shared_ptr<QualType> CanonicalPtr,
            unsigned AddrSpace,
            QualType::GCAttrTypes gcAttr);

    ExtQualType(std::shared_ptr<Type> Base,
                unsigned AddrSpace,
                QualType::GCAttrTypes gcAttr);
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
    static std::shared_ptr<Type> creator(Kind k);
    BuiltinType(Kind k);
private:

    Kind TypeKind;
};

/// @brief Used for arbitrary width types that we either don't want to or can't map to named integer types
class FixedWidthIntType : public Type
{
public:
    static std::shared_ptr<Type> creator(unsigned W,bool S);

    FixedWidthIntType(unsigned W,bool S);
private:
    unsigned Width;
    bool Signed;
};

/// @brief Complex values
class ComplexType : public Type
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> Element,
                                         std::shared_ptr<QualType> CanonicalPtr);

    ComplexType(std::shared_ptr<QualType> Element);
private:
    std::shared_ptr<QualType> ElementType;
};

/// @brief Pointer Declarators
class PointerType : public Type
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> Pointee,
                                         std::shared_ptr<QualType> CanonicalPtr);

    PointerType(std::shared_ptr<QualType> Pointee);
private:
    std::shared_ptr<QualType> PointeeType;
};

/// @brief Pointer to a block type. Pointee is required to always be a function type
/// @example "void (^)(int)"
class BlockPointerType : public Type
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> Pointee,
                                         std::shared_ptr<QualType> CanonicalCls);
    BlockPointerType(std::shared_ptr<QualType> Pointee);
private:
    std::shared_ptr<QualType> PointeeType;
};

/// @C++
/// @brief Base for LValueReferenceType and RValueReferenceType
class ReferenceType : public Type
{
protected:
    ReferenceType(TypeClass tc, std::shared_ptr<QualType> Referencee);
private:
    std::shared_ptr<QualType> PointeeType;
};

/// @C++
/// @brief C++ Lvalue reference
class LValueReferenceType : public ReferenceType
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> Referencee,
                                         std::shared_ptr<QualType> CanonicalRef);
    LValueReferenceType(std::shared_ptr<QualType> Referencee);
};

/// @C++
/// @brief C++0x Rvalue reference
class RValueReferenceType : public ReferenceType
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> Referencee,
                                         std::shared_ptr<QualType> CanonicalRef);
    RValueReferenceType(std::shared_ptr<QualType> Referencee);
};

/// @C++
/// @brief C++ pointers to members
/// @example "A::B"
class MemberPointerType : public Type
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> Pointee,
                                         const std::shared_ptr<Type>Cls,
                                         std::shared_ptr<QualType> CanonicalPtr);

    MemberPointerType(std::shared_ptr<QualType> Pointee,
                      const std::shared_ptr<Type>Cls);
private:
    std::shared_ptr<QualType> PointeeType;
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
    ArrayType(TypeClass tc,
              std::shared_ptr<QualType> et,
              ArraySizeModifier sm,
              unsigned tq);
private:
    std::shared_ptr<QualType> ElementType;
    unsigned SizeModifier : 2;
    unsigned IndexTypeQuals : 3;
};

/// @brief This class represents the canonical version of C arrays with a specified constant size
/// @example "int a[2 + 1];"
class ConstantArrayType : public ArrayType
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> et, std::shared_ptr<QualType> can, int size,
                                         ArraySizeModifier sm, unsigned tq);
    ConstantArrayType(std::shared_ptr<QualType> et, int size,
                      ArraySizeModifier sm, unsigned tq);
protected:
    ConstantArrayType(TypeClass tc, std::shared_ptr<QualType> et,
                      int size, ArraySizeModifier sm, unsigned tq);
private:
    int Size;
};

/// @brief This class represents C arrays with a constant size specified by means of an integer constant expression
/// @example "int a[sizeof(int)];"
class ConstantArrayWithExprType : public ConstantArrayType
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> et, std::shared_ptr<QualType> can,
                                         int size, std::shared_ptr<Expr>e,
                                         ArraySizeModifier sm, unsigned tq);

    ConstantArrayWithExprType(std::shared_ptr<QualType> et,
                              int size, std::shared_ptr<Expr>e,
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
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> et, std::shared_ptr<QualType> can,
                                         int size,
                                         ArraySizeModifier sm, unsigned tq);
    ConstantArrayWithoutExprType(std::shared_ptr<QualType> et,
                                 int size,
                                 ArraySizeModifier sm, unsigned tq);
};

/// @brief This class represents C arrays with an unspecified size
/// @example "int a[];"
class IncompleteArrayType : public ArrayType
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> et, std::shared_ptr<QualType> can,
                                         ArraySizeModifier sm, unsigned tq);
    IncompleteArrayType(std::shared_ptr<QualType> et,
                        ArraySizeModifier sm, unsigned tq);
};

/// @brief This class represents C arrays with a specified size
/// which is not an integer-constant-expression
/// @example "int a[x + foo()];"
class VariableArrayType : public ArrayType
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> et, std::shared_ptr<QualType> can,
                                         std::shared_ptr<Expr>e,
                                         ArraySizeModifier sm, unsigned tq);

    VariableArrayType(std::shared_ptr<QualType> et, std::shared_ptr<Expr>e,
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
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> et,
                                         std::shared_ptr<QualType> can,
                                         std::shared_ptr<Expr>e,
                                         ArraySizeModifier sm,
                                         unsigned tq);


    DependentSizedArrayType(std::shared_ptr<QualType> et,
                            std::shared_ptr<Expr>e,
                            ArraySizeModifier sm, unsigned tq);
private:
    std::shared_ptr<Stmt> SizeExpr;
    std::shared_ptr<QualType> ElementType;
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
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> ElementType,
                                         std::shared_ptr<QualType> can,
                                         std::shared_ptr<Expr> SizeExpr,
                                         SourceLocation loc);

    DependentSizedExtVectorType(std::shared_ptr<QualType> ElementType,
                                std::shared_ptr<Expr> SizeExpr,
                                SourceLocation loc);
private:

    std::shared_ptr<Expr> SizeExpr;
    std::shared_ptr<QualType> ElementType;
    SourceLocation loc;
};

/// @extends
/// @brief GCC generic vector type
class VectorType : public Type
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> vecType,
                                         unsigned nElements,
                                         std::shared_ptr<QualType> canonType);
    VectorType(std::shared_ptr<QualType> vecType,
               unsigned nElements);
protected:
    VectorType(TypeClass tc,
               std::shared_ptr<QualType> vecType,
               unsigned nElements);
private:
    std::shared_ptr<QualType> ElementType;
    unsigned NumElements;
};

/// @extends
/// @brief Extended vector type
class ExtVectorType : public VectorType
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> vecType,
                                         unsigned nElements,
                                         std::shared_ptr<QualType> canonType);
    ExtVectorType(std::shared_ptr<QualType> vecType, unsigned nElements);

};

/// @brief Function Declarators
class FunctionType : public Type
{
public:
protected:
    FunctionType(TypeClass tc, std::shared_ptr<QualType> res, bool SubclassInfo,
                 unsigned typeQuals, bool Dependent,
                 bool noReturn = false);
private:
    bool SubClassData : 1;
    unsigned TypeQuals : 3;
    unsigned NoReturn : 1;
    std::shared_ptr<QualType> ResultType;
};

/// @brief Represents a K&R-style 'int foo()' function, which has
/// no information available about its arguments
class FunctionNoProtoType : public FunctionType
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> Result,
                                         std::shared_ptr<QualType> Canonical,
                                         bool NoReturn = false);
    FunctionNoProtoType(std::shared_ptr<QualType> Result, bool NoReturn = false);
};

/// @brief Represents a prototype with argument type info
/// @example "int foo(int)" or "int foo(void)"
class FunctionProtoType : public FunctionType
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> Result, const std::vector<QualType> ArgArray,
                                         unsigned numArgs,
                                         bool isVariadic, unsigned typeQuals, bool hasExs,
                                         bool hasAnyExs, const std::vector<QualType> ExArray,
                                         unsigned numExs, std::shared_ptr<QualType> Canonical,
                                         bool NoReturn);
    FunctionProtoType(std::shared_ptr<QualType> Result, const std::vector<QualType> ArgArray,
                      unsigned numArgs,
                      bool isVariadic, unsigned typeQuals, bool hasExs,
                      bool hasAnyExs, const std::vector<QualType> ExArray,
                      unsigned numExs, bool NoReturn);
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
    TypedefType(TypeClass tc, std::shared_ptr<QualType> can)
        : Type(tc, (*can)->isDependentType()){
    }
private:
    //TypedefDecl *Decl;
};

/// @extends
/// @brief GCC extension
class TypeOfExprType : public Type
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<Expr> E,
                                         std::shared_ptr<QualType> can = std::make_shared<QualType>());
    TypeOfExprType(std::shared_ptr<Expr> E);
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
    static std::shared_ptr<Type> creator(std::shared_ptr<QualType> T, QualType can);
    TypeOfType(std::shared_ptr<QualType> T);
private:
    std::shared_ptr<QualType> TOType;
};

/// @C++
/// @brief C++0x
class DecltypeType : public Type
{
public:
    static std::shared_ptr<Type> creator(std::shared_ptr<Expr> E,
                                         std::shared_ptr<QualType> underlyingType,
                                         std::shared_ptr<QualType> can=std::make_shared<QualType>());
    DecltypeType(std::shared_ptr<Expr> E,
                 std::shared_ptr<QualType> underlyingType);
private:
    std::shared_ptr<Expr> E;
    std::shared_ptr<QualType> UnderlyingType;
};

/// @C++
class DependentDecltypeType : public DecltypeType
{
public:
    DependentDecltypeType(std::shared_ptr<Expr> E);
private:
};

/// @brief a struct/union/class/enum
class TagType : public Type
{
protected:
    TagType(TypeClass TC, std::shared_ptr<TagDecl> D);
private:
    std::pair<std::shared_ptr<TagDecl> , unsigned> decl;
};

/// @brief structs/unions/classes
class RecordType : public TagType
{
protected:
    //    explicit RecordType(std::shared_ptr<RecordDecl> D)
    //        : TagType(Record, std::dynamic_pointer_cast<TagDecl>(D), QualType()) { }
    //    explicit RecordType(TypeClass TC, std::shared_ptr<RecordDecl> D)
    //        : TagType(TC, std::dynamic_pointer_cast<TagDecl>(D), QualType()) { }
};

/// @brief enum
class EnumType : public TagType
{
private:
    //    explicit EnumType(std::shared_ptr<EnumDecl> D)
    //        : TagType(Enum, std::dynamic_pointer_cast<TagDecl>(D), QualType()) { }
};

/// @C++
class TemplateTypeParmType : public Type
{
public:

private:
    //    TemplateTypeParmType(unsigned D, unsigned I, bool PP,
    //                         std::shared_ptr<IdentifierInfo> N,
    //                         QualType Canon)
    //        : Type(TemplateTypeParm, true),
    //          Depth(D), Index(I), ParameterPack(PP), Name(N) { }

    //    TemplateTypeParmType(unsigned D, unsigned I, bool PP)
    //        : Type(TemplateTypeParm, QualType(this, 0), true),
    //          Depth(D), Index(I), ParameterPack(PP), Name(0) { }

    unsigned Depth : 15;
    unsigned Index : 16;
    unsigned ParameterPack : 1;
    //    std::shared_ptr<IdentifierInfo> Name;
};

/// @C++
/// @brief Represents the type of a template specialization as written in the source code
class TemplateSpecializationType : public Type
{
public:
private:
    /*TemplateSpecializationType(TemplateName T,
                                 const TemplateArgument *Args,
                                 unsigned NumArgs, QualType Canon);*/

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

#endif // AST_TYPE_H
