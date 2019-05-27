#include "AST/Type.h"
#include "AST/Expr.h"
#include "AST/ASTContext.h"

using std::move;
using std::shared_ptr;
using std::make_shared;

QualType::QualType()
{

}

QualType::QualType(const std::shared_ptr<Type> Ptr, unsigned Quals)
    : Value(Ptr, Quals)
{}

std::shared_ptr<Type> QualType::getTypePtr() const
{
    return Value.first;
}

unsigned QualType::getCVRQualifiers() const
{
    return Value.second;
}

std::shared_ptr<Type> QualType::operator->() const
{
    return getTypePtr();
}

bool QualType::isNull() const
{
    return getTypePtr() == nullptr;
}

bool QualType::isConstQualified() const
{
    return (getCVRQualifiers() & Const) ? true : false;
}

bool QualType::isVolatileQualified() const
{
    return (getCVRQualifiers() & Volatile) ? true : false;
}

bool QualType::isRestrictQualified() const
{
    return (getCVRQualifiers() & Restrict) ? true : false;
}

std::shared_ptr<Type> ExtQualType::creator(shared_ptr<Type> Base,
                                           std::shared_ptr<QualType> CanonicalPtr,
                                           unsigned AddrSpace,
                                           QualType::GCAttrTypes gcAttr)
{
    auto ptr=make_shared<ExtQualType>(Base, AddrSpace, gcAttr);
    ptr->setCanonicalType(CanonicalPtr);
    return move(ptr);
}

ExtQualType::ExtQualType(shared_ptr<Type> Base,
                         unsigned AddrSpace,
                         QualType::GCAttrTypes gcAttr)
    :Type(ExtQual, Base->isDependentType()),
      BaseType(Base), AddressSpace(AddrSpace), GCAttrType(gcAttr)
{
}

shared_ptr<Type> BuiltinType::creator(BuiltinType::Kind k)
{
    auto ptr=make_shared<BuiltinType>(k);
    ptr->setCanonicalType(std::make_shared<QualType>());
    return move(ptr);
}

BuiltinType::BuiltinType(Kind k)
    :Type(Builtin, (k==Dependent)), TypeKind(k)
{}

shared_ptr<Type> FixedWidthIntType::creator(unsigned W, bool S)
{
    auto ptr=make_shared<FixedWidthIntType>(W, S);
    ptr->setCanonicalType(std::make_shared<QualType>());
    return move(ptr);
}

FixedWidthIntType::FixedWidthIntType(unsigned W,bool S)
    :Type(FixedWidthInt, false), Width(W), Signed(S)
{}

shared_ptr<Type> ComplexType::creator(QualType Element,
                                      std::shared_ptr<QualType> CanonicalPtr)
{
    auto ptr=make_shared<ComplexType>(Element);
    ptr->setCanonicalType(CanonicalPtr);
    return move(ptr);
}

ComplexType::ComplexType(QualType Element)
    : Type(Complex, Element->isDependentType()),
      ElementType(Element)
{}

shared_ptr<Type> PointerType::creator(QualType Pointee, std::shared_ptr<QualType> CanonicalPtr)
{
    auto ptr=make_shared<PointerType>(Pointee);
    ptr->setCanonicalType(CanonicalPtr);
    return move(ptr);
}

PointerType::PointerType(QualType Pointee)
    :Type(Pointer, Pointee->isDependentType()),
      PointeeType(Pointee)
{}

shared_ptr<Type> BlockPointerType::creator(QualType Pointee,std::shared_ptr<QualType> CanonicalCls)
{
    auto ptr=make_shared<BlockPointerType>(Pointee);
    ptr->setCanonicalType(CanonicalCls);
    return move(ptr);
}

BlockPointerType::BlockPointerType(QualType Pointee)
    : Type(BlockPointer, Pointee->isDependentType()),
      PointeeType(Pointee)
{}

ReferenceType::ReferenceType(TypeClass tc, QualType Referencee)
    :Type(tc, Referencee->isDependentType()),
      PointeeType(Referencee)
{}

shared_ptr<Type> LValueReferenceType::creator(QualType Referencee, std::shared_ptr<QualType> CanonicalRef)
{
    auto ptr=make_shared<LValueReferenceType>(Referencee);
    ptr->setCanonicalType(CanonicalRef);
    return move(ptr);
}

LValueReferenceType::LValueReferenceType(QualType Referencee)
    :ReferenceType(LValueReference, Referencee)
{}

shared_ptr<Type> RValueReferenceType::creator(QualType Referencee, std::shared_ptr<QualType> CanonicalRef)
{
    auto ptr=make_shared<RValueReferenceType>(Referencee);
    ptr->setCanonicalType(CanonicalRef);
    return move(ptr);
}

RValueReferenceType::RValueReferenceType(QualType Referencee)
    :ReferenceType(RValueReference, Referencee)
{}

shared_ptr<Type> MemberPointerType::creator(QualType Pointee,
                                            const shared_ptr<Type> Cls,
                                            std::shared_ptr<QualType> CanonicalPtr)
{
    auto ptr=make_shared<MemberPointerType>(Pointee, Cls);
    ptr->setCanonicalType(CanonicalPtr);
    return move(ptr);
}

MemberPointerType::MemberPointerType(QualType Pointee,
                                     const shared_ptr<Type>Cls)
    :Type(MemberPointer, Cls->isDependentType() || Pointee->isDependentType()),
      PointeeType(Pointee), Class(Cls)
{}

ArrayType::ArrayType(TypeClass tc, QualType et,
                     ArraySizeModifier sm, unsigned tq)
    : Type(tc, et->isDependentType() || tc == DependentSizedArray),
      ElementType(et), SizeModifier(sm), IndexTypeQuals(tq)
{}

ConstantArrayType::ConstantArrayType(QualType et, int size,
                                     ArraySizeModifier sm, unsigned tq)
    :ArrayType(ConstantArray, et, sm, tq),Size(size)
{}

shared_ptr<Type> ConstantArrayType::creator(QualType et,
                                            std::shared_ptr<QualType> can,
                                            int size,
                                            ArrayType::ArraySizeModifier sm,
                                            unsigned tq)
{
    auto ptr=make_shared<ConstantArrayType>(et, size, sm, tq);
    ptr->setCanonicalType(can);
    return move(ptr);
}

ConstantArrayType::ConstantArrayType(TypeClass tc, QualType et,
                                     int size, ArraySizeModifier sm, unsigned tq)
    :ArrayType(tc, et, sm, tq), Size(size)
{}

shared_ptr<Type> ConstantArrayWithExprType::creator(QualType et,
                                                    std::shared_ptr<QualType> can,
                                                    int size,
                                                    shared_ptr<Expr> e,
                                                    ArrayType::ArraySizeModifier sm,
                                                    unsigned tq)
{
    auto ptr=make_shared<ConstantArrayWithExprType>(et, size, e, sm, tq);
    ptr->setCanonicalType(can);
    return move(ptr);
}

ConstantArrayWithExprType::ConstantArrayWithExprType(QualType et,
                                                     int size,
                                                     std::shared_ptr<Expr>e,
                                                     ArraySizeModifier sm,
                                                     unsigned tq)
    :ConstantArrayType(ConstantArrayWithExpr, et, size, sm, tq),
      SizeExpr(e)
{}

shared_ptr<Type> ConstantArrayWithoutExprType::creator(QualType et,
                                                       std::shared_ptr<QualType> can,
                                                       int size,
                                                       ArrayType::ArraySizeModifier sm,
                                                       unsigned tq)
{
    auto ptr=make_shared<ConstantArrayWithoutExprType>(et, size, sm, tq);
    ptr->setCanonicalType(can);
    return move(ptr);
}

ConstantArrayWithoutExprType::ConstantArrayWithoutExprType(QualType et,
                                                           int size,
                                                           ArraySizeModifier sm,
                                                           unsigned tq)
    :ConstantArrayType(ConstantArrayWithoutExpr, et, size, sm, tq)
{}

shared_ptr<Type> IncompleteArrayType::creator(QualType et,
                                              std::shared_ptr<QualType> can,
                                              ArrayType::ArraySizeModifier sm,
                                              unsigned tq)
{
    auto ptr=make_shared<IncompleteArrayType>(et, sm, tq);
    ptr->setCanonicalType(can);
    return move(ptr);
}

IncompleteArrayType::IncompleteArrayType(QualType et,
                                         ArraySizeModifier sm, unsigned tq)
    :ArrayType(IncompleteArray, et, sm, tq)
{}

shared_ptr<Type> VariableArrayType::creator(QualType et,
                                            std::shared_ptr<QualType> can,
                                            shared_ptr<Expr> e,
                                            ArrayType::ArraySizeModifier sm,
                                            unsigned tq)
{
    auto ptr=make_shared<VariableArrayType>(et, e, sm, tq);
    ptr->setCanonicalType(can);
    return move(ptr);
}

VariableArrayType::VariableArrayType(QualType et,
                                     std::shared_ptr<Expr>e,
                                     ArraySizeModifier sm, unsigned tq)
    :ArrayType(VariableArray, et, sm, tq),SizeExpr(e)
{}

shared_ptr<Type> DependentSizedArrayType::creator(QualType et,
                                                  std::shared_ptr<QualType> can,
                                                  shared_ptr<Expr> e,
                                                  ArrayType::ArraySizeModifier sm,
                                                  unsigned tq)
{
    auto ptr=make_shared<DependentSizedArrayType>(et, e, sm, tq);
    ptr->setCanonicalType(can);
    return move(ptr);
}

DependentSizedArrayType::DependentSizedArrayType(QualType et,
                                                 shared_ptr<Expr>e,
                                                 ArraySizeModifier sm, unsigned tq)
    :ArrayType(DependentSizedArray, et, sm,tq),SizeExpr(e)
{}

std::shared_ptr<Type> DependentSizedExtVectorType::creator(QualType ElementType,
                                                           std::shared_ptr<QualType> can,
                                                           std::shared_ptr<Expr> SizeExpr,
                                                           SourceLocation loc)
{
    auto ptr=std::make_shared<DependentSizedExtVectorType>(ElementType, SizeExpr, loc);
    ptr->setCanonicalType(can);
    return std::move(ptr);
}

DependentSizedExtVectorType::DependentSizedExtVectorType(QualType ElementType,
                                                         std::shared_ptr<Expr> SizeExpr,
                                                         SourceLocation loc)
    : Type (DependentSizedExtVector, true),
      SizeExpr(SizeExpr),
      ElementType(ElementType),
      loc(loc)
{}

std::shared_ptr<Type> VectorType::creator(QualType vecType, unsigned nElements, std::shared_ptr<QualType> canonType)
{
    auto ptr=std::make_shared<VectorType>(vecType, nElements);
    ptr->setCanonicalType(canonType);
    return std::move(ptr);
}

VectorType::VectorType(QualType vecType, unsigned nElements)
    : Type(Vector, vecType->isDependentType()),
      ElementType(vecType),
      NumElements(nElements)
{}

VectorType::VectorType(Type::TypeClass tc, QualType vecType, unsigned nElements)
	: Type(tc, vecType->isDependentType()), ElementType(vecType), NumElements(nElements)
{}

std::shared_ptr<Type> ExtVectorType::creator(QualType vecType, unsigned nElements, std::shared_ptr<QualType> canonType)
{
    auto ptr=std::make_shared<ExtVectorType>(vecType, nElements);
    ptr->setCanonicalType(canonType);
    return std::move(ptr);
}

ExtVectorType::ExtVectorType(QualType vecType, unsigned nElements)
    : VectorType(ExtVector, vecType, nElements) {}

FunctionType::FunctionType(Type::TypeClass tc,
                           QualType res,
                           bool SubclassInfo,
                           unsigned typeQuals,
                           bool Dependent,
                           bool noReturn)
    : Type(tc, Dependent),
      SubClassData(SubclassInfo), TypeQuals(typeQuals), NoReturn(noReturn),
      ResultType(res) {}

std::shared_ptr<Type> FunctionNoProtoType::creator(QualType Result,
                                                   std::shared_ptr<QualType> Canonical,
                                                   bool NoReturn)
{
    auto ptr=std::make_shared<FunctionNoProtoType>(Result, NoReturn);
    ptr->setCanonicalType(Canonical);
    return std::move(ptr);
}

FunctionNoProtoType::FunctionNoProtoType(QualType Result,bool NoReturn)
    : FunctionType(FunctionNoProto, Result, false, 0, false, NoReturn) {}

std::shared_ptr<Type> FunctionProtoType::creator(QualType Result,
                                                 const std::vector<QualType> ArgArray,
                                                 unsigned numArgs, bool isVariadic,
                                                 unsigned typeQuals, bool hasExs,
                                                 bool hasAnyExs,
                                                 const std::vector<QualType> ExArray,
                                                 unsigned numExs,
                                                 std::shared_ptr<QualType> Canonical,
                                                 bool NoReturn)
{
    auto ptr=std::make_shared<FunctionProtoType>(Result,
                                                 ArgArray,
                                                 numArgs,
                                                 isVariadic,
                                                 typeQuals,
                                                 hasExs,hasAnyExs,
                                                 ExArray,
                                                 numExs,
                                                 NoReturn);
    ptr->setCanonicalType(Canonical);
    return std::move(ptr);
}

FunctionProtoType::FunctionProtoType(QualType Result,
                                     const std::vector<QualType> ArgArray,
                                     unsigned numArgs,
                                     bool isVariadic,
                                     unsigned typeQuals,
                                     bool hasExs,
                                     bool hasAnyExs,
                                     const std::vector<QualType> ExArray,
                                     unsigned numExs,
                                     bool NoReturn)
    : FunctionType(FunctionProto, Result, isVariadic, typeQuals,
                   (Result->isDependentType() ||
                    hasAnyDependentType(ArgArray)), NoReturn),
      NumArgs(numArgs), NumExceptions(numExs), HasExceptionSpec(hasExs),
      AnyExceptionSpec(hasAnyExs) {
    /*QualType *ArgInfo = reinterpret_cast<QualType*>(this+1);
    for (unsigned i = 0; i != numArgs; ++i)
        ArgInfo[i] = ArgArray[i];
    QualType *Ex = ArgInfo + numArgs;
    for (unsigned i = 0; i != numExs; ++i)
        Ex[i] = ExArray[i];*/
}

bool FunctionProtoType::hasAnyDependentType(const std::vector<QualType> ArgArray)
{
    for(auto &i:ArgArray)
        if (i->isDependentType())
            return true;
    return false;
}

std::shared_ptr<Type> TypeOfExprType::creator(std::shared_ptr<Expr> E, std::shared_ptr<QualType> can)
{
    auto ptr=std::make_shared<TypeOfExprType>(E);
    ptr->setCanonicalType(can);
    return std::move(ptr);
}

TypeOfExprType::TypeOfExprType(std::shared_ptr<Expr> E)
    : Type(TypeOfExpr, E->isTypeDependent()), TOExpr(E)
{
}

DependentTypeOfExprType::DependentTypeOfExprType(std::shared_ptr<Expr> E)
    : TypeOfExprType(E){ }

std::shared_ptr<Type> TypeOfType::creator(QualType T, QualType can)
{
    auto ptr=std::make_shared<TypeOfType>(T);
    ptr->setCanonicalType(can.isNull()?QualType(ptr, 0):can);
    return std::move(ptr);
}

TypeOfType::TypeOfType(QualType T)
    : Type(TypeOf, T->isDependentType()), TOType(T) {
}

std::shared_ptr<Type> DecltypeType::creator(std::shared_ptr<Expr> E, QualType underlyingType, std::shared_ptr<QualType> can)
{
    auto ptr=std::make_shared<DecltypeType>(E, underlyingType);
    ptr->setCanonicalType(can);
    return std::move(ptr);
}

DecltypeType::DecltypeType(std::shared_ptr<Expr> E, QualType underlyingType)
    : Type(Decltype,E->isTypeDependent()), E(E),
      UnderlyingType(underlyingType)
{
}

bool Type::isDependentType() const
{
    return Dependent;
}

std::weak_ptr<QualType> Type::getCanonicalType() const
{
    return CanonicalType;
}

void Type::setCanonicalType(const std::weak_ptr<QualType> &value)
{
    auto ptr=value.lock();
    CanonicalType=ptr->isNull()?make_shared<QualType>(shared_from_this(), 0):ptr;
}

Type::Type(Type::TypeClass tc, bool dependent)
    : Dependent(dependent), TC(tc) {}

QualifierSet::QualifierSet()
    :Mask(0)
{

}

TagType::TagType(Type::TypeClass TC, std::shared_ptr<TagDecl> D)
    : Type(TC, true/*D->isDependentType()*/), decl(D, 0)
{}
