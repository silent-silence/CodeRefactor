#include "AST/Type.h"
#include "AST/Expr.h"
#include "AST/ASTContext.h"

using std::vector;
using std::move;					using std::string;
using std::shared_ptr;				using std::make_shared;
using std::to_string;               using std::weak_ptr;
using std::dynamic_pointer_cast;

/// @QualType
QualType::QualType(const shared_ptr<Type> Ptr, unsigned Quals)
    : Value(Ptr, Quals)
{}

shared_ptr<Type> QualType::getTypePtr() const
{
    return Value.first;
}

unsigned QualType::getCVRQualifiers() const
{
    return Value.second;
}

shared_ptr<Type> QualType::operator->() const
{
    return getTypePtr();
}

bool QualType::isNull() const
{
    return getTypePtr() == nullptr;
}

bool QualType::isConstQualified() const
{
    return (getCVRQualifiers() & Const) != 0;
}

bool QualType::isVolatileQualified() const
{
    return (getCVRQualifiers() & Volatile) != 0;
}

bool QualType::isRestrictQualified() const
{
    return (getCVRQualifiers() & Restrict) != 0;
}

/// @Type
Type::Type(Type::TypeClass tc, bool dependent)
    : Dependent(dependent), TC(tc)
{}

Type::~Type()
{}

bool Type::isDependentType() const
{
    return Dependent;
}

weak_ptr<QualType> Type::getCanonicalType() const
{
    return CanonicalType;
}

void Type::setCanonicalType(const weak_ptr<QualType> &value)
{
    auto ptr = value.lock();
    CanonicalType = ptr->isNull() ? make_shared<QualType>(shared_from_this(), 0) : ptr;
}

Type::TypeClass Type::getTypeClass() const
{
    return static_cast<Type::TypeClass>(TC);
}

/// @ExtQualType
shared_ptr<QualType> ExtQualType::creator(shared_ptr<Type> Base,
                                          shared_ptr<QualType> CanonicalPtr,
                                          unsigned AddrSpace,
                                          QualType::GCAttrTypes gcAttr)
{
    auto ptr=make_shared<ExtQualType>(Base, AddrSpace, gcAttr);
    ptr->setCanonicalType(CanonicalPtr);
    return ptr->getCanonicalType().lock();
}

ExtQualType::ExtQualType(shared_ptr<Type> Base,
                         unsigned AddrSpace,
                         QualType::GCAttrTypes gcAttr)
    :Type(TypeClass::ExtQual, Base->isDependentType()),
      BaseType(Base), AddressSpace(AddrSpace), GCAttrType(gcAttr)
{}

weak_ptr<Type> ExtQualType::getBaseType() const
{
    return BaseType;
}

QualType::GCAttrTypes ExtQualType::getObjCGCAttr() const
{
    return GCAttrType;
}

unsigned ExtQualType::getAddressSpace() const
{
    return AddressSpace;
}

bool ExtQualType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::ExtQual;
}

bool ExtQualType::classof(const weak_ptr<ExtQualType>)
{
    return true;
}

/// @BuiltinType
shared_ptr<QualType> BuiltinType::creator(BuiltinType::Kind k, QualType::TQ tq)
{
    auto ptr=make_shared<BuiltinType>(k);
    auto ptr_qual=make_shared<QualType>(ptr, tq);
    ptr->setCanonicalType(ptr_qual);
    return ptr_qual;
}

BuiltinType::BuiltinType(Kind k)
    :Type(TypeClass::Builtin, (k==Dependent)), TypeKind(k)
{}

BuiltinType::Kind BuiltinType::getKind() const
{
    return TypeKind;
}

bool BuiltinType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::Builtin;
}

bool BuiltinType::classof(const weak_ptr<BuiltinType>)
{
    return true;
}

/// @FixedWidthIntType
shared_ptr<QualType> FixedWidthIntType::creator(unsigned W, bool S)
{
    auto ptr=make_shared<FixedWidthIntType>(W, S);
    auto ptr_qual=make_shared<QualType>(ptr, 0);
    ptr->setCanonicalType(ptr_qual);
    return ptr_qual;
}

FixedWidthIntType::FixedWidthIntType(unsigned W,bool S)
    :Type(TypeClass::FixedWidthInt, false), Width(W), Signed(S)
{}

unsigned FixedWidthIntType::getWidth() const
{
    return Width;
}

bool FixedWidthIntType::isSigned() const
{
    return Signed;
}

bool FixedWidthIntType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::FixedWidthInt;
}

bool FixedWidthIntType::classof(const weak_ptr<FixedWidthIntType>)
{
    return true;
}

/// @ComplexType
shared_ptr<QualType> ComplexType::creator(shared_ptr<QualType> Element,
                                          shared_ptr<QualType> CanonicalPtr)
{
    auto ptr=make_shared<ComplexType>(Element);
    ptr->setCanonicalType(CanonicalPtr);
    return ptr->getCanonicalType().lock();
}

ComplexType::ComplexType(shared_ptr<QualType> Element)
    : Type(TypeClass::Complex, (*Element)->isDependentType()),
      ElementType(Element)
{}

weak_ptr<QualType> ComplexType::getElementType() const
{
    return ElementType;
}

bool ComplexType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::Complex;
}

bool ComplexType::classof(const std::weak_ptr<ComplexType>)
{
    return true;
}

/// @PointerType
shared_ptr<Type> PointerType::creator(shared_ptr<QualType> Pointee, shared_ptr<QualType> CanonicalPtr)
{
    auto ptr=make_shared<PointerType>(Pointee);
    ptr->setCanonicalType(CanonicalPtr);
    return std::move(ptr);
}

PointerType::PointerType(shared_ptr<QualType> Pointee)
    :Type(TypeClass::Pointer, true),
      PointeeType(Pointee)
{}

weak_ptr<QualType> PointerType::getPointeeType() const
{
    return PointeeType;
}

bool PointerType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::Pointer;
}

bool PointerType::classof(const std::weak_ptr<PointerType>)
{
    return true;
}

/// @BlockPointerType
shared_ptr<QualType> BlockPointerType::creator(shared_ptr<QualType> Pointee,
                                               shared_ptr<QualType> CanonicalCls)
{
    auto ptr=make_shared<BlockPointerType>(Pointee);
    ptr->setCanonicalType(CanonicalCls);
    return ptr->getCanonicalType().lock();
}

BlockPointerType::BlockPointerType(shared_ptr<QualType> Pointee)
    : Type(TypeClass::BlockPointer, (*Pointee)->isDependentType()),
      PointeeType(Pointee)
{}

weak_ptr<QualType> BlockPointerType::getPointeeType() const
{
    return PointeeType;
}

bool BlockPointerType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::BlockPointer;
}

bool BlockPointerType::classof(const weak_ptr<BlockPointerType>)
{
    return true;
}

/// @ReferenceType
ReferenceType::ReferenceType(TypeClass tc, shared_ptr<QualType> Referencee)
    :Type(tc, (*Referencee)->isDependentType()),
      PointeeType(Referencee)
{}

weak_ptr<QualType> ReferenceType::getPointeeType() const
{
    return PointeeType;
}

bool ReferenceType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::LValueReference ||
            T.lock()->getTypeClass() == TypeClass::RValueReference;
}

bool ReferenceType::classof(const weak_ptr<ReferenceType>)
{
    return true;
}

/// @LValueReferenceType
shared_ptr<QualType> LValueReferenceType::creator(shared_ptr<QualType> Referencee, shared_ptr<QualType> CanonicalRef)
{
    auto ptr=make_shared<LValueReferenceType>(Referencee);
    ptr->setCanonicalType(CanonicalRef);
    return ptr->getCanonicalType().lock();
}

LValueReferenceType::LValueReferenceType(shared_ptr<QualType> Referencee)
    :ReferenceType(TypeClass::LValueReference, Referencee)
{}

bool LValueReferenceType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::LValueReference;
}

bool LValueReferenceType::classof(const weak_ptr<LValueReferenceType>)
{
    return true;
}

/// @RValueReferenceType
shared_ptr<QualType> RValueReferenceType::creator(shared_ptr<QualType> Referencee, shared_ptr<QualType> CanonicalRef)
{
    auto ptr=make_shared<RValueReferenceType>(Referencee);
    ptr->setCanonicalType(CanonicalRef);
    return ptr->getCanonicalType().lock();
}

RValueReferenceType::RValueReferenceType(shared_ptr<QualType> Referencee)
    :ReferenceType(TypeClass::RValueReference, Referencee)
{}

bool RValueReferenceType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::RValueReference;
}

bool RValueReferenceType::classof(const weak_ptr<RValueReferenceType>)
{
    return true;
}

/// @MemberPointerType
shared_ptr<QualType> MemberPointerType::creator(shared_ptr<QualType> Pointee,
                                                const shared_ptr<Type> Cls,
                                                shared_ptr<QualType> CanonicalPtr)
{
    auto ptr=make_shared<MemberPointerType>(Pointee, Cls);
    ptr->setCanonicalType(CanonicalPtr);
    return ptr->getCanonicalType().lock();
}

MemberPointerType::MemberPointerType(shared_ptr<QualType> Pointee,
                                     const shared_ptr<Type>Cls)
    :Type(TypeClass::MemberPointer, Cls->isDependentType() || (*Pointee)->isDependentType()),
      PointeeType(Pointee), Class(Cls)
{}

weak_ptr<QualType> MemberPointerType::getPointeeType() const
{
    return PointeeType;
}

const weak_ptr<Type> MemberPointerType::getClass() const
{
    return Class;
}

bool MemberPointerType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::MemberPointer;
}

bool MemberPointerType::classof(const weak_ptr<MemberPointerType>)
{
    return true;
}

/// @ArrayType
ArrayType::ArrayType(TypeClass tc, shared_ptr<QualType> et,
                     ArraySizeModifier sm, unsigned tq)
    : Type(tc, (*et)->isDependentType() || tc == TypeClass::DependentSizedArray),
      ElementType(et), SizeModifier(sm), IndexTypeQuals(tq)
{}

weak_ptr<QualType> ArrayType::getElementType() const
{
    return ElementType;
}

ArrayType::ArraySizeModifier ArrayType::getSizeModifier() const
{
    return ArraySizeModifier(SizeModifier);
}

unsigned ArrayType::getIndexTypeQualifier() const
{
    return IndexTypeQuals;
}

bool ArrayType::classof(const weak_ptr<Type> T)
{
    auto ptr=T.lock();
    return ptr->getTypeClass() == TypeClass::ConstantArray ||
            ptr->getTypeClass() == TypeClass::ConstantArrayWithExpr ||
            ptr->getTypeClass() == TypeClass::ConstantArrayWithoutExpr ||
            ptr->getTypeClass() == TypeClass::VariableArray ||
            ptr->getTypeClass() == TypeClass::IncompleteArray ||
            ptr->getTypeClass() == TypeClass::DependentSizedArray;
}

bool ArrayType::classof(const weak_ptr<ArrayType>)
{
    return true;
}

/// @ConstantArrayType
ConstantArrayType::ConstantArrayType(shared_ptr<QualType> et, int size,
                                     ArraySizeModifier sm, unsigned tq)
    :ArrayType(TypeClass::ConstantArray, et, sm, tq),Size(size)
{}

const int &ConstantArrayType::getSize() const
{
    return Size;
}

bool ConstantArrayType::classof(const weak_ptr<Type> T)
{
    auto ptr=T.lock();
    return ptr->getTypeClass() == TypeClass::ConstantArray ||
            ptr->getTypeClass() == TypeClass::ConstantArrayWithExpr ||
            ptr->getTypeClass() == TypeClass::ConstantArrayWithoutExpr;
}

bool ConstantArrayType::classof(const weak_ptr<ConstantArrayType>)
{
    return true;
}

shared_ptr<QualType> ConstantArrayType::creator(shared_ptr<QualType> et,
                                                shared_ptr<QualType> can,
                                                int size,
                                                ArrayType::ArraySizeModifier sm,
                                                unsigned tq)
{
    auto ptr=make_shared<ConstantArrayType>(et, size, sm, tq);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

ConstantArrayType::ConstantArrayType(TypeClass tc, shared_ptr<QualType> et,
                                     int size, ArraySizeModifier sm, unsigned tq)
    :ArrayType(tc, et, sm, tq), Size(size)
{}

/// @ConstantArrayWithExprType
shared_ptr<Type> ConstantArrayWithExprType::creator(shared_ptr<QualType> et,
                                                    shared_ptr<QualType> can,
                                                    shared_ptr<Expr> e,
                                                    ArrayType::ArraySizeModifier sm,
                                                    unsigned tq)
{
    auto ptr=make_shared<ConstantArrayWithExprType>(et, e->EvaluateAsInt(), e, sm, tq);
    ptr->setCanonicalType(can);
    return ptr;
}

ConstantArrayWithExprType::ConstantArrayWithExprType(shared_ptr<QualType> et,
                                                     int size,
                                                     shared_ptr<Expr>e,
                                                     ArraySizeModifier sm,
                                                     unsigned tq)
    :ConstantArrayType(TypeClass::ConstantArrayWithExpr, et, size, sm, tq), SizeExpr(e)
{}

weak_ptr<Expr> ConstantArrayWithExprType::getSizeSpecifier() const
{
    return SizeExpr;
}

bool ConstantArrayWithExprType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::ConstantArrayWithExpr;
}

bool ConstantArrayWithExprType::classof(const weak_ptr<ConstantArrayWithExprType>)
{
    return true;
}

/// @ConstantArrayWithoutExprType
shared_ptr<QualType> ConstantArrayWithoutExprType::creator(shared_ptr<QualType> et,
                                                           shared_ptr<QualType> can,
                                                           int size,
                                                           ArrayType::ArraySizeModifier sm,
                                                           unsigned tq)
{
    auto ptr=make_shared<ConstantArrayWithoutExprType>(et, size, sm, tq);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

ConstantArrayWithoutExprType::ConstantArrayWithoutExprType(shared_ptr<QualType> et,
                                                           int size,
                                                           ArraySizeModifier sm,
                                                           unsigned tq)
    :ConstantArrayType(TypeClass::ConstantArrayWithoutExpr, et, size, sm, tq)
{}

bool ConstantArrayWithoutExprType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::ConstantArrayWithoutExpr;
}

bool ConstantArrayWithoutExprType::classof(const weak_ptr<ConstantArrayWithoutExprType>)
{
    return true;
}

/// @IncompleteArrayType
shared_ptr<Type> IncompleteArrayType::creator(shared_ptr<QualType> et,
                                              shared_ptr<QualType> can,
                                              ArrayType::ArraySizeModifier sm,
                                              unsigned tq)
{
    auto ptr=make_shared<IncompleteArrayType>(et, sm, tq);
    ptr->setCanonicalType(can);
    return move(ptr);
}

IncompleteArrayType::IncompleteArrayType(shared_ptr<QualType> et,
                                         ArraySizeModifier sm, unsigned tq)
    :ArrayType(TypeClass::IncompleteArray, et, sm, tq)
{}

bool IncompleteArrayType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::IncompleteArray;
}

bool IncompleteArrayType::classof(const weak_ptr<IncompleteArrayType>)
{
    return true;
}

/// @VariableArrayType
shared_ptr<QualType> VariableArrayType::creator(shared_ptr<QualType> et,
                                                shared_ptr<QualType> can,
                                                shared_ptr<Expr> e,
                                                ArrayType::ArraySizeModifier sm,
                                                unsigned tq)
{
    auto ptr=make_shared<VariableArrayType>(et, e, sm, tq);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

VariableArrayType::VariableArrayType(shared_ptr<QualType> et,
                                     shared_ptr<Expr>e,
                                     ArraySizeModifier sm, unsigned tq)
    :ArrayType(TypeClass::VariableArray, et, sm, tq),SizeExpr(e)
{}

weak_ptr<Expr> VariableArrayType::getSizeExpr() const
{
    return dynamic_pointer_cast<Expr>(SizeExpr);
}

bool VariableArrayType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::VariableArray;
}

bool VariableArrayType::classof(const weak_ptr<VariableArrayType>)
{
    return true;
}

/// @DependentSizedArrayType
shared_ptr<QualType> DependentSizedArrayType::creator(shared_ptr<QualType> et,
                                                      shared_ptr<QualType> can,
                                                      shared_ptr<Expr> e,
                                                      ArrayType::ArraySizeModifier sm,
                                                      unsigned tq)
{
    auto ptr=make_shared<DependentSizedArrayType>(et, e, sm, tq);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

DependentSizedArrayType::DependentSizedArrayType(shared_ptr<QualType> et,
                                                 shared_ptr<Expr>e,
                                                 ArraySizeModifier sm, unsigned tq)
    :ArrayType(TypeClass::DependentSizedArray, et, sm,tq), SizeExpr(e)
{}

weak_ptr<Expr> DependentSizedArrayType::getSizeExpr() const
{
    return dynamic_pointer_cast<Expr>(SizeExpr);
}

bool DependentSizedArrayType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::DependentSizedArray;
}

bool DependentSizedArrayType::classof(const weak_ptr<DependentSizedArrayType>)
{
    return true;
}

/// @DependentSizedExtVectorType
shared_ptr<QualType> DependentSizedExtVectorType::creator(shared_ptr<QualType> ElementType,
                                                          shared_ptr<QualType> can,
                                                          shared_ptr<Expr> SizeExpr,
                                                          SourceLocation loc)
{
    auto ptr=make_shared<DependentSizedExtVectorType>(ElementType, SizeExpr, loc);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

DependentSizedExtVectorType::DependentSizedExtVectorType(shared_ptr<QualType> ElementType,
                                                         shared_ptr<Expr> SizeExpr,
                                                         SourceLocation loc)
    : Type (TypeClass::DependentSizedExtVector, true),
      SizeExpr(SizeExpr),
      ElementType(ElementType),
      loc(loc)
{}

weak_ptr<Expr> DependentSizedExtVectorType::getSizeExpr() const
{
    return SizeExpr;
}

weak_ptr<QualType> DependentSizedExtVectorType::getElementType() const
{
    return ElementType;
}

SourceLocation DependentSizedExtVectorType::getAttributeLoc() const
{
    return loc;
}

bool DependentSizedExtVectorType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::DependentSizedExtVector;
}

bool DependentSizedExtVectorType::classof(const weak_ptr<DependentSizedExtVectorType>)
{
    return true;
}

/// @VectorType
shared_ptr<QualType> VectorType::creator(shared_ptr<QualType> vecType, unsigned nElements, shared_ptr<QualType> canonType)
{
    auto ptr=make_shared<VectorType>(vecType, nElements);
    ptr->setCanonicalType(canonType);
    return ptr->getCanonicalType().lock();
}

VectorType::VectorType(shared_ptr<QualType> vecType, unsigned nElements)
    : Type(TypeClass::Vector, (*vecType)->isDependentType()),
      ElementType(vecType),
      NumElements(nElements)
{}

weak_ptr<QualType> VectorType::getElementType() const
{
    return ElementType;
}

unsigned VectorType::getNumElements() const
{
    return NumElements;
}

bool VectorType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::Vector ||
            T.lock()->getTypeClass() == TypeClass::ExtVector;
}

bool VectorType::classof(const weak_ptr<VectorType>)
{
    return true;
}

VectorType::VectorType(Type::TypeClass tc, shared_ptr<QualType> vecType, unsigned nElements)
    : Type(tc, (*vecType)->isDependentType()), ElementType(vecType), NumElements(nElements)
{}

/// @ExtVectorType
shared_ptr<QualType> ExtVectorType::creator(shared_ptr<QualType> vecType, unsigned nElements, shared_ptr<QualType> canonType)
{
    auto ptr=make_shared<ExtVectorType>(vecType, nElements);
    ptr->setCanonicalType(canonType);
    return ptr->getCanonicalType().lock();
}

ExtVectorType::ExtVectorType(shared_ptr<QualType> vecType, unsigned nElements)
    : VectorType(TypeClass::ExtVector, vecType, nElements)
{}

bool ExtVectorType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::ExtVector;
}

bool ExtVectorType::classof(const weak_ptr<ExtVectorType>)
{
    return true;
}

/// @FunctionType
FunctionType::FunctionType(Type::TypeClass tc,
                           shared_ptr<QualType> res,
                           bool SubclassInfo,
                           unsigned typeQuals,
                           bool Dependent,
                           bool noReturn)
    : Type(tc, Dependent), SubClassData(SubclassInfo), TypeQuals(typeQuals), NoReturn(noReturn), ResultType(res)
{}

shared_ptr<FunctionDecl> FunctionType::getFunDecl() const
{
    return m_funDecl;
}

void FunctionType::setFunDecl(shared_ptr<FunctionDecl> decl)
{
    m_funDecl = decl;
}

weak_ptr<QualType> FunctionType::getResultType() const
{
    return ResultType;
}

bool FunctionType::getNoReturnAttr() const
{
    return NoReturn;
}

bool FunctionType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::FunctionNoProto ||
            T.lock()->getTypeClass() == TypeClass::FunctionProto;
}

bool FunctionType::classof(const weak_ptr<FunctionType>)
{
    return true;
}

/// @FunctionNoProtoType
shared_ptr<Type> FunctionNoProtoType::creator(shared_ptr<QualType> Result,
                                              shared_ptr<QualType> Canonical,
                                              bool NoReturn)
{
    auto ptr=make_shared<FunctionNoProtoType>(Result, NoReturn);
    ptr->setCanonicalType(Canonical);
    return ptr;
}

FunctionNoProtoType::FunctionNoProtoType(shared_ptr<QualType> Result, bool NoReturn)
    : FunctionType(TypeClass::FunctionNoProto, Result, false, 0, false, NoReturn)
{}

bool FunctionNoProtoType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::FunctionNoProto;
}

bool FunctionNoProtoType::classof(const weak_ptr<FunctionNoProtoType>)
{
    return true;
}

/// @FunctionProtoType
shared_ptr<Type> FunctionProtoType::creator(shared_ptr<QualType> Result,
                                            const vector<shared_ptr<QualType>> ArgArray,
                                            unsigned numArgs, bool isVariadic,
                                            unsigned typeQuals, bool hasExs,
                                            bool hasAnyExs,
                                            const vector<QualType> ExArray,
                                            unsigned numExs,
                                            shared_ptr<QualType> Canonical,
                                            bool NoReturn)
{
    auto ptr = make_shared<FunctionProtoType>(Result, ArgArray, numArgs,
                                              isVariadic, typeQuals, hasExs,
                                              hasAnyExs, ExArray, numExs, NoReturn
                                              );
    ptr->setCanonicalType(Canonical);
    return ptr;
}

FunctionProtoType::FunctionProtoType(shared_ptr<QualType> Result,
                                     const vector<shared_ptr<QualType>> ArgArray,
                                     unsigned numArgs,
                                     bool isVariadic,
                                     unsigned typeQuals,
                                     bool hasExs,
                                     bool hasAnyExs,
                                     const vector<QualType> ExArray,
                                     unsigned numExs,
                                     bool NoReturn)
    : FunctionType(TypeClass::FunctionProto, Result, isVariadic, typeQuals,
                   ((*Result)->isDependentType() || hasAnyDependentType(ArgArray)), NoReturn),
      NumArgs(numArgs), NumExceptions(numExs), HasExceptionSpec(hasExs),
      AnyExceptionSpec(hasAnyExs), Args{ArgArray}
{}

void FunctionProtoType::setArgs(vector<shared_ptr<QualType>> ArgArray)
{
    Args = ArgArray;
}

unsigned FunctionProtoType::getNumArgs() const
{
    return NumArgs;
}

bool FunctionProtoType::hasExceptionSpec() const
{
    return HasExceptionSpec;
}

bool FunctionProtoType::hasAnyExceptionSpec() const
{
    return AnyExceptionSpec;
}

unsigned FunctionProtoType::getNumExceptions() const
{
    return NumExceptions;
}

bool FunctionProtoType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::FunctionProto;
}

bool FunctionProtoType::classof(const weak_ptr<FunctionProtoType>)
{
    return true;
}

bool FunctionProtoType::hasAnyDependentType(const vector<shared_ptr<QualType>> ArgArray)
{
    for(auto &i:ArgArray)
        if (i->getTypePtr()->isDependentType())
            return true;
    return false;
}

/// @TypeOfExprType
shared_ptr<QualType> TypeOfExprType::creator(shared_ptr<Expr> E, shared_ptr<QualType> can)
{
    auto ptr=make_shared<TypeOfExprType>(E);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

TypeOfExprType::TypeOfExprType(shared_ptr<Expr> E)
    : Type(TypeClass::TypeOfExpr, E->isTypeDependent()), TOExpr(E)
{}

weak_ptr<Expr> TypeOfExprType::getUnderlyingExpr() const
{
    return TOExpr;
}

bool TypeOfExprType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::TypeOfExpr;
}

bool TypeOfExprType::classof(const weak_ptr<TypeOfExprType>)
{
    return true;
}

/// @DependentTypeOfExprType
DependentTypeOfExprType::DependentTypeOfExprType(shared_ptr<Expr> E)
    : TypeOfExprType(E)
{}

/// @TypeOfType
shared_ptr<QualType> TypeOfType::creator(shared_ptr<QualType> T, QualType can)
{
    auto ptr=make_shared<TypeOfType>(T);
    //ptr->setCanonicalType(can.isNull()?QualType(ptr, 0):can);
    return ptr->getCanonicalType().lock();
}

TypeOfType::TypeOfType(shared_ptr<QualType> T)
    : Type(TypeClass::TypeOf, (*T)->isDependentType()), TOType(T) {
}

weak_ptr<QualType> TypeOfType::getUnderlyingType() const
{
    return TOType;
}

bool TypeOfType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::TypeOf;
}

bool TypeOfType::classof(const weak_ptr<TypeOfType>)
{
    return true;
}

/// @DecltypeType
shared_ptr<QualType> DecltypeType::creator(shared_ptr<Expr> E, shared_ptr<QualType> underlyingType, shared_ptr<QualType> can)
{
    auto ptr=make_shared<DecltypeType>(E, underlyingType);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

DecltypeType::DecltypeType(shared_ptr<Expr> E, shared_ptr<QualType> underlyingType)
    : Type(TypeClass::Decltype, E->isTypeDependent()), E(E),
      UnderlyingType(underlyingType)
{}

weak_ptr<Expr> DecltypeType::getUnderlyingExpr() const
{
    return E;
}

weak_ptr<QualType> DecltypeType::getUnderlyingType() const
{
    return UnderlyingType;
}

bool DecltypeType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::Decltype;
}

bool DecltypeType::classof(const weak_ptr<DecltypeType>)
{
    return true;
}

/// @QualifierSet
QualifierSet::QualifierSet()
    :Mask(0)
{}

/// @TypedefType
TypedefType::TypedefType(TypeClass tc, shared_ptr<QualType> can, shared_ptr<TypedefDecl> decl, shared_ptr<QualType> declFor)
    : Type(tc, false/*TODO: this should be (*can)->isDependentType()*/), Decl{decl}, declForType{declFor}
{}

shared_ptr<Type> TypedefType::creator(shared_ptr<QualType> can, shared_ptr<TypedefDecl> decl, shared_ptr<QualType> declFor)
{
    auto ptr = make_shared<TypedefType>(TypeClass::Typedef, can, decl, declFor);
    ptr->setCanonicalType(can);
    return ptr;
}

weak_ptr<QualType> TypedefType::getDeclForType() const
{
    return declForType;
}

weak_ptr<TypedefDecl> TypedefType::getDecl() const
{
    return Decl;
}

bool TypedefType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() == TypeClass::Typedef;
}

bool TypedefType::classof(const weak_ptr<TypedefType>)
{
    return true;
}

/// @TagType
TagType::TagType(Type::TypeClass TC, shared_ptr<TagDecl> D, shared_ptr<QualType> can)
    : Type(TC, true/*D->isDependentType()*/), decl{D}
{}

/// @destructor pure-virtual
TagType::~TagType()
{}

weak_ptr<TagDecl> TagType::getDecl() const
{
    return decl;
}

bool TagType::classof(const weak_ptr<Type> T)
{
    return T.lock()->getTypeClass() >= TypeClass::TagFirst &&
            T.lock()->getTypeClass() <= TypeClass::TagLast;
}

bool TagType::classof(const weak_ptr<TagType>)
{
    return true;
}

bool TagType::classof(const weak_ptr<RecordType>)
{
    return true;
}

bool TagType::classof(const weak_ptr<EnumType>)
{
    return true;
}

/// @RecordType
RecordType::RecordType(shared_ptr<RecordDecl> D, shared_ptr<QualType> can)
    : TagType(TypeClass::Record, dynamic_pointer_cast<TagDecl>(D), can)
{}

shared_ptr<Type> RecordType::creator(shared_ptr<RecordDecl> D, shared_ptr<QualType> can)
{
    auto ptr = make_shared<RecordType>(D, can);
    ptr->setCanonicalType(can);
    return ptr;
}

bool RecordType::hasConstFields() const
{
    return false;
}

unsigned RecordType::getAddressSpace() const
{
    return 0;
}

bool RecordType::classof(const weak_ptr<TagType> T)
{

}

bool RecordType::classof(const weak_ptr<Type> T)
{
    //return isa<TagType>(T) && classof(cast<TagType>(T));
}

bool RecordType::classof(const weak_ptr<RecordType>)
{
    return true;
}

/// @EnumType
EnumType::EnumType(shared_ptr<EnumDecl> D, shared_ptr<QualType> can)
    : TagType(TypeClass::Enum, dynamic_pointer_cast<TagDecl>(D), can)
{}
