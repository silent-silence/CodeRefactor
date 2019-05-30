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

std::shared_ptr<QualType> ExtQualType::creator(shared_ptr<Type> Base,
                                           std::shared_ptr<QualType> CanonicalPtr,
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
    :Type(ExtQual, Base->isDependentType()),
      BaseType(Base), AddressSpace(AddrSpace), GCAttrType(gcAttr)
{
}

std::shared_ptr<QualType> BuiltinType::creator(BuiltinType::Kind k)
{
    auto ptr=make_shared<BuiltinType>(k);
    auto ptr_qual=make_shared<QualType>(ptr, 0);
    ptr->setCanonicalType(ptr_qual);
    return ptr_qual;
}

BuiltinType::BuiltinType(Kind k)
    :Type(Builtin, (k==Dependent)), TypeKind(k)
{}

shared_ptr<QualType> FixedWidthIntType::creator(unsigned W, bool S)
{
    auto ptr=make_shared<FixedWidthIntType>(W, S);
    auto ptr_qual=make_shared<QualType>(ptr, 0);
    ptr->setCanonicalType(ptr_qual);
    return ptr_qual;
}

FixedWidthIntType::FixedWidthIntType(unsigned W,bool S)
    :Type(FixedWidthInt, false), Width(W), Signed(S)
{}

shared_ptr<QualType> ComplexType::creator(std::shared_ptr<QualType> Element,
                                      std::shared_ptr<QualType> CanonicalPtr)
{
    auto ptr=make_shared<ComplexType>(Element);
    ptr->setCanonicalType(CanonicalPtr);
    return ptr->getCanonicalType().lock();
}

ComplexType::ComplexType(std::shared_ptr<QualType> Element)
    : Type(Complex, (*Element)->isDependentType()),
      ElementType(Element)
{}

shared_ptr<QualType> PointerType::creator(std::shared_ptr<QualType> Pointee, std::shared_ptr<QualType> CanonicalPtr)
{
    auto ptr=make_shared<PointerType>(Pointee);
    ptr->setCanonicalType(CanonicalPtr);
    return ptr->getCanonicalType().lock();
}

PointerType::PointerType(std::shared_ptr<QualType> Pointee)
    :Type(Pointer, (*Pointee)->isDependentType()),
      PointeeType(Pointee)
{}

std::weak_ptr<QualType> PointerType::getPointeeType() const
{
	return PointeeType;
}

shared_ptr<QualType> BlockPointerType::creator(std::shared_ptr<QualType> Pointee,
                                           std::shared_ptr<QualType> CanonicalCls)
{
    auto ptr=make_shared<BlockPointerType>(Pointee);
    ptr->setCanonicalType(CanonicalCls);
    return ptr->getCanonicalType().lock();
}

BlockPointerType::BlockPointerType(std::shared_ptr<QualType> Pointee)
    : Type(BlockPointer, (*Pointee)->isDependentType()),
      PointeeType(Pointee)
{}

ReferenceType::ReferenceType(TypeClass tc, std::shared_ptr<QualType> Referencee)
    :Type(tc, (*Referencee)->isDependentType()),
      PointeeType(Referencee)
{}

shared_ptr<QualType> LValueReferenceType::creator(std::shared_ptr<QualType> Referencee, std::shared_ptr<QualType> CanonicalRef)
{
    auto ptr=make_shared<LValueReferenceType>(Referencee);
    ptr->setCanonicalType(CanonicalRef);
    return ptr->getCanonicalType().lock();
}

LValueReferenceType::LValueReferenceType(std::shared_ptr<QualType> Referencee)
    :ReferenceType(LValueReference, Referencee)
{}

shared_ptr<QualType> RValueReferenceType::creator(std::shared_ptr<QualType> Referencee, std::shared_ptr<QualType> CanonicalRef)
{
    auto ptr=make_shared<RValueReferenceType>(Referencee);
    ptr->setCanonicalType(CanonicalRef);
    return ptr->getCanonicalType().lock();
}

RValueReferenceType::RValueReferenceType(std::shared_ptr<QualType> Referencee)
    :ReferenceType(RValueReference, Referencee)
{}

shared_ptr<QualType> MemberPointerType::creator(std::shared_ptr<QualType> Pointee,
                                            const shared_ptr<Type> Cls,
                                            std::shared_ptr<QualType> CanonicalPtr)
{
    auto ptr=make_shared<MemberPointerType>(Pointee, Cls);
    ptr->setCanonicalType(CanonicalPtr);
    return ptr->getCanonicalType().lock();
}

MemberPointerType::MemberPointerType(std::shared_ptr<QualType> Pointee,
                                     const shared_ptr<Type>Cls)
    :Type(MemberPointer, Cls->isDependentType() || (*Pointee)->isDependentType()),
      PointeeType(Pointee), Class(Cls)
{}

ArrayType::ArrayType(TypeClass tc, std::shared_ptr<QualType> et,
                     ArraySizeModifier sm, unsigned tq)
    : Type(tc, (*et)->isDependentType() || tc == DependentSizedArray),
      ElementType(et), SizeModifier(sm), IndexTypeQuals(tq)
{}

ConstantArrayType::ConstantArrayType(std::shared_ptr<QualType> et, int size,
                                     ArraySizeModifier sm, unsigned tq)
    :ArrayType(ConstantArray, et, sm, tq),Size(size)
{}

shared_ptr<QualType> ConstantArrayType::creator(std::shared_ptr<QualType> et,
                                            std::shared_ptr<QualType> can,
                                            int size,
                                            ArrayType::ArraySizeModifier sm,
                                            unsigned tq)
{
    auto ptr=make_shared<ConstantArrayType>(et, size, sm, tq);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

ConstantArrayType::ConstantArrayType(TypeClass tc, std::shared_ptr<QualType> et,
                                     int size, ArraySizeModifier sm, unsigned tq)
    :ArrayType(tc, et, sm, tq), Size(size)
{}

shared_ptr<QualType> ConstantArrayWithExprType::creator(std::shared_ptr<QualType> et,
                                                    std::shared_ptr<QualType> can,
                                                    int size,
                                                    shared_ptr<Expr> e,
                                                    ArrayType::ArraySizeModifier sm,
                                                    unsigned tq)
{
    auto ptr=make_shared<ConstantArrayWithExprType>(et, size, e, sm, tq);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

ConstantArrayWithExprType::ConstantArrayWithExprType(std::shared_ptr<QualType> et,
                                                     int size,
                                                     std::shared_ptr<Expr>e,
                                                     ArraySizeModifier sm,
                                                     unsigned tq)
    :ConstantArrayType(ConstantArrayWithExpr, et, size, sm, tq),
      SizeExpr(e)
{}

shared_ptr<QualType> ConstantArrayWithoutExprType::creator(std::shared_ptr<QualType> et,
                                                       std::shared_ptr<QualType> can,
                                                       int size,
                                                       ArrayType::ArraySizeModifier sm,
                                                       unsigned tq)
{
    auto ptr=make_shared<ConstantArrayWithoutExprType>(et, size, sm, tq);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

ConstantArrayWithoutExprType::ConstantArrayWithoutExprType(std::shared_ptr<QualType> et,
                                                           int size,
                                                           ArraySizeModifier sm,
                                                           unsigned tq)
    :ConstantArrayType(ConstantArrayWithoutExpr, et, size, sm, tq)
{}

shared_ptr<QualType> IncompleteArrayType::creator(std::shared_ptr<QualType> et,
                                              std::shared_ptr<QualType> can,
                                              ArrayType::ArraySizeModifier sm,
                                              unsigned tq)
{
    auto ptr=make_shared<IncompleteArrayType>(et, sm, tq);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

IncompleteArrayType::IncompleteArrayType(std::shared_ptr<QualType> et,
                                         ArraySizeModifier sm, unsigned tq)
    :ArrayType(IncompleteArray, et, sm, tq)
{}

shared_ptr<QualType> VariableArrayType::creator(std::shared_ptr<QualType> et,
                                            std::shared_ptr<QualType> can,
                                            shared_ptr<Expr> e,
                                            ArrayType::ArraySizeModifier sm,
                                            unsigned tq)
{
    auto ptr=make_shared<VariableArrayType>(et, e, sm, tq);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

VariableArrayType::VariableArrayType(std::shared_ptr<QualType> et,
                                     std::shared_ptr<Expr>e,
                                     ArraySizeModifier sm, unsigned tq)
    :ArrayType(VariableArray, et, sm, tq),SizeExpr(e)
{}

shared_ptr<QualType> DependentSizedArrayType::creator(std::shared_ptr<QualType> et,
                                                  std::shared_ptr<QualType> can,
                                                  shared_ptr<Expr> e,
                                                  ArrayType::ArraySizeModifier sm,
                                                  unsigned tq)
{
    auto ptr=make_shared<DependentSizedArrayType>(et, e, sm, tq);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

DependentSizedArrayType::DependentSizedArrayType(std::shared_ptr<QualType> et,
                                                 shared_ptr<Expr>e,
                                                 ArraySizeModifier sm, unsigned tq)
    :ArrayType(DependentSizedArray, et, sm,tq), SizeExpr(e)
{}

std::shared_ptr<QualType> DependentSizedExtVectorType::creator(std::shared_ptr<QualType> ElementType,
                                                           std::shared_ptr<QualType> can,
                                                           std::shared_ptr<Expr> SizeExpr,
                                                           SourceLocation loc)
{
    auto ptr=std::make_shared<DependentSizedExtVectorType>(ElementType, SizeExpr, loc);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

DependentSizedExtVectorType::DependentSizedExtVectorType(std::shared_ptr<QualType> ElementType,
                                                         std::shared_ptr<Expr> SizeExpr,
                                                         SourceLocation loc)
    : Type (DependentSizedExtVector, true),
      SizeExpr(SizeExpr),
      ElementType(ElementType),
      loc(loc)
{}

std::shared_ptr<QualType> VectorType::creator(std::shared_ptr<QualType> vecType, unsigned nElements, std::shared_ptr<QualType> canonType)
{
    auto ptr=std::make_shared<VectorType>(vecType, nElements);
    ptr->setCanonicalType(canonType);
    return ptr->getCanonicalType().lock();
}

VectorType::VectorType(std::shared_ptr<QualType> vecType, unsigned nElements)
    : Type(Vector, (*vecType)->isDependentType()),
      ElementType(vecType),
      NumElements(nElements)
{}

VectorType::VectorType(Type::TypeClass tc, std::shared_ptr<QualType> vecType, unsigned nElements)
    : Type(tc, (*vecType)->isDependentType()), ElementType(vecType), NumElements(nElements)
{}

std::shared_ptr<QualType> ExtVectorType::creator(std::shared_ptr<QualType> vecType, unsigned nElements, std::shared_ptr<QualType> canonType)
{
    auto ptr=std::make_shared<ExtVectorType>(vecType, nElements);
    ptr->setCanonicalType(canonType);
    return ptr->getCanonicalType().lock();
}

ExtVectorType::ExtVectorType(std::shared_ptr<QualType> vecType, unsigned nElements)
    : VectorType(ExtVector, vecType, nElements) {}

FunctionType::FunctionType(Type::TypeClass tc,
                           std::shared_ptr<QualType> res,
                           bool SubclassInfo,
                           unsigned typeQuals,
                           bool Dependent,
                           bool noReturn)
    : Type(tc, Dependent),
      SubClassData(SubclassInfo), TypeQuals(typeQuals), NoReturn(noReturn),
      ResultType(res) {}

std::shared_ptr<QualType> FunctionNoProtoType::creator(std::shared_ptr<QualType> Result,
                                                   std::shared_ptr<QualType> Canonical,
                                                   bool NoReturn)
{
    auto ptr=std::make_shared<FunctionNoProtoType>(Result, NoReturn);
    ptr->setCanonicalType(Canonical);
    return ptr->getCanonicalType().lock();
}

FunctionNoProtoType::FunctionNoProtoType(std::shared_ptr<QualType> Result, bool NoReturn)
    : FunctionType(FunctionNoProto, Result, false, 0, false, NoReturn) {}

std::shared_ptr<QualType> FunctionProtoType::creator(std::shared_ptr<QualType> Result,
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
    return ptr->getCanonicalType().lock();
}

FunctionProtoType::FunctionProtoType(std::shared_ptr<QualType> Result,
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
                   ((*Result)->isDependentType() ||
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

std::shared_ptr<QualType> TypeOfExprType::creator(std::shared_ptr<Expr> E, std::shared_ptr<QualType> can)
{
    auto ptr=std::make_shared<TypeOfExprType>(E);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

TypeOfExprType::TypeOfExprType(std::shared_ptr<Expr> E)
    : Type(TypeOfExpr, E->isTypeDependent()), TOExpr(E)
{
}

DependentTypeOfExprType::DependentTypeOfExprType(std::shared_ptr<Expr> E)
    : TypeOfExprType(E){ }

std::shared_ptr<QualType> TypeOfType::creator(std::shared_ptr<QualType> T, QualType can)
{
    auto ptr=std::make_shared<TypeOfType>(T);
    //ptr->setCanonicalType(can.isNull()?QualType(ptr, 0):can);
    return ptr->getCanonicalType().lock();
}

TypeOfType::TypeOfType(std::shared_ptr<QualType> T)
    : Type(TypeOf, (*T)->isDependentType()), TOType(T) {
}

std::shared_ptr<QualType> DecltypeType::creator(std::shared_ptr<Expr> E, std::shared_ptr<QualType> underlyingType, std::shared_ptr<QualType> can)
{
    auto ptr=std::make_shared<DecltypeType>(E, underlyingType);
    ptr->setCanonicalType(can);
    return ptr->getCanonicalType().lock();
}

DecltypeType::DecltypeType(std::shared_ptr<Expr> E, std::shared_ptr<QualType> underlyingType)
    : Type(Decltype, E->isTypeDependent()), E(E),
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
