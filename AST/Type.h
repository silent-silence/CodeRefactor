//  This file defines the Type interface and subclasses.
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


//instead each reference to a type stores the qualifiers.
// This greatly reduces the number of nodes we need to allocate for types (for example we only need one for 'int', 'const int', 'volatile int','const volatile int', etc).
//we make QualType be a simple wrapper class that acts like a smart pointer.
class QualType
{
public:
    //These flags must be kept in sync with DeclSpec::TQ
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
    bool isNull() const;//Return true if this QualType doesn't point to a type yet.
    bool isConstQualified() const;
    bool isVolatileQualified() const;
    bool isRestrictQualified() const;
private:
    //value store Type and Qualifiers
    std::pair<std::shared_ptr<Type>, unsigned> Value;
};

// This is the base class of the type hierarchy.A central concept with types is that each type always has a canonical type.
//A canonical type is the type with any typedef names stripped out of it or the types it references.
//Non-canonical types are useful for emitting diagnostics, without losing information about typedefs being used.  Canonical types are useful for type comparisons (they allow by-pointer equality tests) and useful for reasoning about whether something has a particular form (e.g. is a function type),because they implicitly, recursively, strip all typedefs out of a type.

// Types, once created, are immutable.

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
    //Whether this type is a dependent type, meaning that its definition somehow depends on a template parameter
    bool isDependentType() const;
protected:
    enum { TypeClassBitSize = 6 };
    Type(TypeClass tc, QualType Canonical, bool dependent);
private:
    Type(const Type&);

    QualType CanonicalType;
    //Whether this type is a dependent type.
    bool Dependent : 1;
     // Enum that specifies what subclass this belongs to.Note that this should stay at the end of the ivars for Type so that subclasses can pack their bitfields into the same word.
    unsigned TC : TypeClassBitSize;



};
//This supports all kinds of type attributes; including, address space qualified types, objective-c's __weak and __strong attributes.
class ExtQualType : public Type
{
public:
    ExtQualType(std::shared_ptr<Type> Base,QualType CanonicalPtr, unsigned AddrSpace,QualType::GCAttrTypes gcAttr);
private:
    //This is the underlying type that this qualifies.  All CVR qualifiers are stored on the QualType that references this type, so we can't have any here. basetype
    std::shared_ptr<Type> BaseType;
    //The address space ID this type is qualified with
    unsigned AddressSpace;
    //__strong attributes
    QualType::GCAttrTypes GCAttrType;
};

// This class is used to collect qualifiers
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

//This class is used for builtin types like 'int'.  Builtin types are always canonical and have a literal name field.
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

//Used for arbitrary width types that we either don't want to or can't map to named integer types.  These always have a lower integer rank than builtin types of the same width.
class FixedWidthIntType : public Type
{
public:
    FixedWidthIntType(unsigned W,bool S);
private:
    unsigned Width;
    bool Signed;
};

//This supports the C99 complex types (_Complex float etc) as well as the GCC integer complex extensions。
class ComplexType : public Type
{
public:
    ComplexType(QualType Element, QualType CanonicalPtr);
private:
    QualType ElementType;
};
//Pointer Declarators
class PointerType : public Type
{
public:
    PointerType(QualType Pointee, QualType CanonicalPtr);
private:
    QualType PointeeType;
};

// pointer to a block type
//This type is to represent types syntactically represented as "void (^)(int)"etc. Pointee is required to always be a function type.
class BlockPointerType : public Type
{
public:
    BlockPointerType(QualType Pointee, QualType CanonicalCls);
private:
    QualType PointeeType;
};

//Base for LValueReferenceType and RValueReferenceType

class ReferenceType : public Type
{
public:
    ReferenceType(TypeClass tc, QualType Referencee, QualType CanonicalRef);
protected:
private:
    QualType PointeeType;
};
//Lvalue reference
class LValueReferenceType : public ReferenceType
{
public:
    LValueReferenceType(QualType Referencee, QualType CanonicalRef);
};

//Rvalue reference
class RValueReferenceType : public ReferenceType
{
public:
    RValueReferenceType(QualType Referencee, QualType CanonicalRef);
};

//Pointers to members
class MemberPointerType : public Type
{
public:
    MemberPointerType(QualType Pointee, const std::shared_ptr<Type>Cls, QualType CanonicalPtr);
private:
    QualType PointeeType;  

    //The class of which the pointee is a member. Must ultimately be a RecordType, but could be a typedef or a template parameter too.
    const std::shared_ptr<Type> Class;
};

//Array Declarators
class ArrayType : public Type
{
public:
    // Capture whether this is a normal array (e.g. int X[4]) an array with a static size (e.g. int X[static 4]), or an array with a star size (e.g. int X[*]).'static' is only allowed on function parameters.
    enum ArraySizeModifier {
        Normal, Static, Star
    };
protected:
    ArrayType(TypeClass tc, QualType et, QualType can,
              ArraySizeModifier sm, unsigned tq);

private:
    //The element type of the array。
    QualType ElementType;
    //VC++ treats enums as signed, avoid using the ArraySizeModifier enum
    //NOTE: These fields are packed into the bitfields space in the Type class
    unsigned SizeModifier : 2;
    //Capture qualifiers in declarations
    unsigned IndexTypeQuals : 3;
};

//This class represents the canonical version of C arrays with a specified constant size.
class ConstantArrayType : public ArrayType
{
public:
    ConstantArrayType(QualType et, QualType can,int size,
                      ArraySizeModifier sm, unsigned tq);

protected:
    ConstantArrayType(TypeClass tc, QualType et, QualType can,
int size, ArraySizeModifier sm, unsigned tq);
private:
    int Size; // Allows us to unique the type.
};

//This class represents C arrays with a constant size specified by means of an integer constant expression.
class ConstantArrayWithExprType : public ConstantArrayType
{
public:
    ConstantArrayWithExprType(QualType et, QualType can,
                              int size,std::shared_ptr<Expr>e,
                              ArraySizeModifier sm, unsigned tq);
private:
    //An assignment expression,a function block
    std::shared_ptr<Expr> SizeExpr;
};

// This class represents C arrays with a constant size that was not specified by an integer constant expression,but inferred by static semantics
class ConstantArrayWithoutExprType : public ConstantArrayType
{
public:
    ConstantArrayWithoutExprType(QualType et, QualType can,
                                 int size,
                                 ArraySizeModifier sm, unsigned tq);
};

//This class represents C arrays with an unspecified size.
class IncompleteArrayType : public ArrayType
{
public:
    IncompleteArrayType(QualType et, QualType can,
                        ArraySizeModifier sm, unsigned tq);
};

//This class represents C arrays with a specified size which is not an integer-constant-expression.
//Note: VariableArrayType's aren't uniqued (since the expressions aren't) and should not be: two lexically equivalent variable array types could mean different things.
class VariableArrayType : public ArrayType
{
public:
    VariableArrayType(QualType et, QualType can, std::shared_ptr<Expr>e,
                      ArraySizeModifier sm, unsigned tq);
private:
    //An assignment expression
    std::shared_ptr<Stmt> SizeExpr;
};

//This type represents an array type in C++ whose size is a value-dependent expression.
//For these types, we won't actually know what the array bound is until template instantiation occurs, at which point this will become either a ConstantArrayType or a VariableArrayType.
class DependentSizedArrayType : public ArrayType
{
public:
    DependentSizedArrayType( QualType et, QualType can,
                            std::shared_ptr<Expr>e, ArraySizeModifier sm, unsigned tq);
private:
    //An assignment expression that will instantiate to the size of the array.
    std::shared_ptr<Stmt> SizeExpr;
    QualType ElementType;
    SourceLocation loc;
};


//This type represent an extended vector type where either the type or size is dependent.
class DependentSizedExtVectorType : public Type
{
private:
    DependentSizedExtVectorType(QualType ElementType, QualType can, std::shared_ptr<Expr> SizeExpr, SourceLocation loc);

    std::shared_ptr<Expr> SizeExpr;
    QualType ElementType;//The element type of the array.
    SourceLocation loc;
};

//GCC generic vector type.This type is created using __attribute__((vector_size(n)), where "n" specifies the vector size in bytes. Since the constructor takes the number of vector elements, the client is responsible for converting the size into the number of elements.
class VectorType : public Type
{
public:
protected:
    VectorType(QualType vecType, unsigned nElements, QualType canonType);
    VectorType(TypeClass tc, QualType vecType, unsigned nElements, QualType canonType);
    QualType ElementType;//The element type of the vector
    unsigned NumElements;//The number of elements in the vector
};

//Extended vector type. This type is created using __attribute__((ext_vector_type(n)), where "n" is the number of elements.
//Unlike vector_size, ext_vector_type is only allowed on typedef's. This class enables syntactic extensions, like Vector Components for accessing points, colors, and textures
class ExtVectorType : public VectorType
{
public:
private:
    ExtVectorType(QualType vecType, unsigned nElements, QualType canonType);
};

// Function Declarators.
//This is the common base class of FunctionNoProtoType and FunctionProtoType.
class FunctionType : public Type
{
public:
protected:
    FunctionType(TypeClass tc, QualType res, bool SubclassInfo,
                 unsigned typeQuals, QualType Canonical, bool Dependent,
                 bool noReturn = false);
private:
    //This field is owned by the subclass, put here to pack tightly with the ivars in Type.
    bool SubClassData : 1;
    // Used only by FunctionProtoType, put here to pack with the other bitfields.
    unsigned TypeQuals : 3;
    //Indicates if the function type is attribute noreturn
    unsigned NoReturn : 1;
    //The type returned by the function
    QualType ResultType;
};
//Represents a K&R-style 'int foo()' function, which has no information available about its arguments.
class FunctionNoProtoType : public FunctionType
{
private:
    FunctionNoProtoType(QualType Result, QualType Canonical, bool NoReturn = false);
};
//Represents a prototype with argument type info
// Such a type can have an exception specification, but this specification is not part of the canonical type.

class FunctionProtoType : public FunctionType
{
public:
private:
    FunctionProtoType(QualType Result, const std::vector<QualType> ArgArray,
                      bool isVariadic, unsigned typeQuals, bool hasExs,
                      bool hasAnyExs, const QualType *ExArray,
                      unsigned numExs, QualType Canonical, bool NoReturn);
    //Determine whether there are any dependent types within the arguments passed in.
    static bool hasAnyDependentType(const std::vector<QualType> ArgArray);
    unsigned NumArgs : 20;//The number of arguments this function has, not counting '...'
    unsigned NumExceptions : 10;//The number of types in the exception spec, if any.
    bool HasExceptionSpec : 1;//Whether this function has an exception spec at all.
    bool AnyExceptionSpec : 1;//Whether this function has a throw(...) spec
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

//Subclass of DecltypeType that is used for canonical, dependent C++0x decltype types.
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

// This is a helper class that allows the use of isa/cast/dyncast to detect TagType objects of structs/unions/classes.
class RecordType : public TagType
{
public:
protected:
    /*explicit RecordType(RecordDecl *D)
        : TagType(Record, reinterpret_cast<TagDecl*>(D), QualType()) { }
    explicit RecordType(TypeClass TC, RecordDecl *D)
        : TagType(TC, reinterpret_cast<TagDecl*>(D), QualType()) { }*/
};

// This is a helper class that allows the use of isa/cast/dyncast to detect TagType objects of enums.
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

// \brief Represents the type of a template specialization as written in the source code.
//Template specialization types represent the syntactic form of a template-id that refers to a type.Some template specialization types are syntactic sugar, whose canonical type will point to some other type node that represents the instantiation or class template specialization.
//Other template specialization types, for which the template name is dependent, may be canonical types. These types are always dependent.
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

// \brief Represents a type that was referred to via a qualified name
//This type is used to keep track of a type name as written in the source code, including any nested-name-specifiers. The type itself is always "sugar", used to express what was written in the source code but containing no additional semantic information.
class QualifiedNameType : public Type
{
public:
    QualifiedNameType();
private:
    // \brief The type that this qualified name refers to.
    QualType NamedType;
};

// \brief Represents a 'typename' specifier that names a type within a dependent type.
//TypenameType has a very similar structure to QualifiedNameType，which also involves a nested-name-specifier following by a type,and (FIXME!) both can even be prefixed by the 'typename' keyword.
//However, the two types serve very different roles: QualifiedNameType is a non-semantic type that serves only as sugar to show how a particular type was written in the source code.
//TypenameType, on the other hand, only occurs when the nested-name-specifier is dependent, such that we cannot resolve the actual type until after instantiation.
class TypenameType : public Type
{
public:
    TypenameType();
};

// Interfaces are the core concept in Objective-C for object oriented design. They basically correspond to C++ classes.
// There are two kinds of interface types, normal interfaces like "NSString" and qualified interfaces, which are qualified with a protocol list like "NSString<NSCopyable, NSAmazing>".
class ObjCInterfaceType : public Type
{

};
//Used to represent 'id', 'Interface *', 'id <p>',and 'Interface <p> *'.
//Duplicate protocols are removed and protocol list is canonicalized to be in alphabetical order.
class ObjCObjectPointerType : public Type
{
private:
    QualType PointeeType;
};

#endif // AST_TYPE_H
