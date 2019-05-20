#include "AST/Type.h"
#include "AST/Expr.h"
#include "AST/ASTContext.h"

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

ExtQualType::ExtQualType(std::shared_ptr<Type> Base,QualType CanonicalPtr, unsigned AddrSpace,QualType::GCAttrTypes gcAttr)
    :Type(ExtQual, CanonicalPtr, Base->isDependentType()), BaseType(Base),
    AddressSpace(AddrSpace), GCAttrType(gcAttr)
{}

QualifierSet::QualifierSet()
    :Mask(0)
{}

BuiltinType::BuiltinType(Kind k)
    :Type (Builtin,QualType(),(k==Dependent)),TypeKind(k)
{}

FixedWidthIntType::FixedWidthIntType(unsigned W,bool S)
    :Type(FixedWidthInt,QualType(),false),Width(W),Signed(S)
{}

ComplexType::ComplexType(QualType Element, QualType CanonicalPtr)
    : Type(Complex, CanonicalPtr, Element->isDependentType()),
    ElementType(Element)
{}

PointerType::PointerType(QualType Pointee, QualType CanonicalPtr)
    :Type(Pointer, CanonicalPtr, Pointee->isDependentType()), PointeeType(Pointee)
{}

BlockPointerType::BlockPointerType(QualType Pointee, QualType CanonicalCls)
    : Type(BlockPointer, CanonicalCls, Pointee->isDependentType()),
    PointeeType(Pointee)
{}

ReferenceType::ReferenceType(TypeClass tc, QualType Referencee, QualType CanonicalRef)
    :Type(tc, CanonicalRef, Referencee->isDependentType()),
    PointeeType(Referencee)
{}

LValueReferenceType::LValueReferenceType(QualType Referencee, QualType CanonicalRef)
    :ReferenceType(LValueReference, Referencee, CanonicalRef)
{}

RValueReferenceType::RValueReferenceType(QualType Referencee, QualType CanonicalRef)
    :ReferenceType(RValueReference, Referencee, CanonicalRef)
{}

MemberPointerType::MemberPointerType(QualType Pointee, const std::shared_ptr<Type>Cls, QualType CanonicalPtr)
    :Type(MemberPointer, CanonicalPtr,                                                                                                      Cls->isDependentType() || Pointee->isDependentType()),
                                                                                                            PointeeType(Pointee), Class(Cls)
{}

ArrayType::ArrayType(TypeClass tc, QualType et, QualType can,
                              ArraySizeModifier sm, unsigned tq)
                      : Type(tc, can, et->isDependentType() || tc == DependentSizedArray),ElementType(et), SizeModifier(sm), IndexTypeQuals(tq)
{}

ConstantArrayType::ConstantArrayType(QualType et, QualType can,int size,
                                     ArraySizeModifier sm, unsigned tq)
    :ArrayType(ConstantArray, et, can, sm, tq),Size(size)
{}

ConstantArrayType::ConstantArrayType(TypeClass tc, QualType et, QualType can,
int size, ArraySizeModifier sm, unsigned tq)
    :ArrayType(tc, et, can, sm, tq), Size(size)
{}

ConstantArrayWithExprType::ConstantArrayWithExprType(QualType et, QualType can,
int size,std::shared_ptr<Expr>e,ArraySizeModifier sm, unsigned tq)
    :ConstantArrayType(ConstantArrayWithExpr, et, can, size, sm, tq),
    SizeExpr(e)
{}

ConstantArrayWithoutExprType::ConstantArrayWithoutExprType(QualType et, QualType can,int size,ArraySizeModifier sm, unsigned tq)
    :ConstantArrayType(ConstantArrayWithoutExpr, et, can, size, sm, tq)
{}

IncompleteArrayType::IncompleteArrayType(QualType et, QualType can,
                                         ArraySizeModifier sm, unsigned tq)
    :ArrayType(IncompleteArray, et, can, sm, tq)
{}

VariableArrayType::VariableArrayType(QualType et, QualType can, std::shared_ptr<Expr>e,ArraySizeModifier sm, unsigned tq)
    :ArrayType(VariableArray, et, can, sm, tq),SizeExpr(e)
{}

DependentSizedArrayType::DependentSizedArrayType(QualType et, QualType can,std::shared_ptr<Expr>e, ArraySizeModifier sm, unsigned tq)
    :ArrayType(DependentSizedArray, et, can, sm,tq),SizeExpr((std::shared_ptr<Stmt>) e)\
{}

DependentSizedExtVectorType::DependentSizedExtVectorType(QualType ElementType, QualType can, std::shared_ptr<Expr> SizeExpr, SourceLocation loc)
    : Type (DependentSizedExtVector, can, true),
      SizeExpr(SizeExpr),
      ElementType(ElementType),
      loc(loc)
{}

VectorType::VectorType(QualType vecType, unsigned nElements, QualType canonType)
    : Type(Vector, canonType, vecType->isDependentType()),
      ElementType(vecType),
      NumElements(nElements)
{}

VectorType::VectorType(Type::TypeClass tc, QualType vecType, unsigned nElements, QualType canonType)
    : Type(tc, canonType, vecType->isDependentType()),
      ElementType(vecType),
      NumElements(nElements)
{}

ExtVectorType::ExtVectorType(QualType vecType, unsigned nElements, QualType canonType) :
    VectorType(ExtVector, vecType, nElements, canonType) {}

FunctionType::FunctionType(Type::TypeClass tc, QualType res, bool SubclassInfo, unsigned typeQuals, QualType Canonical, bool Dependent, bool noReturn)
    : Type(tc, Canonical, Dependent),
      SubClassData(SubclassInfo), TypeQuals(typeQuals), NoReturn(noReturn),
      ResultType(res) {}

FunctionNoProtoType::FunctionNoProtoType(QualType Result, QualType Canonical, bool NoReturn)
    : FunctionType(FunctionNoProto, Result, false, 0, Canonical, false, NoReturn) {}

FunctionProtoType::FunctionProtoType(QualType Result, const std::vector<QualType> ArgArray, bool isVariadic, unsigned typeQuals, bool hasExs, bool hasAnyExs, const QualType *ExArray, unsigned numExs, QualType Canonical, bool NoReturn)
    : FunctionType(FunctionProto, Result, isVariadic, typeQuals, Canonical,
                   (Result->isDependentType() ||
                    hasAnyDependentType(ArgArray)), NoReturn),
      NumArgs(static_cast<unsigned>(ArgArray.size())), NumExceptions(numExs), HasExceptionSpec(hasExs),
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

TypeOfExprType::TypeOfExprType(std::shared_ptr<Expr> E, QualType can)
    : Type(TypeOfExpr, can, E->isTypeDependent()), TOExpr(E)
{
}

DependentTypeOfExprType::DependentTypeOfExprType(std::shared_ptr<Expr> E)
    : TypeOfExprType(E){ }

TypeOfType::TypeOfType(QualType T, QualType can)
    : Type(TypeOf, can, T->isDependentType()), TOType(T) {
}

DecltypeType::DecltypeType(std::shared_ptr<Expr> E, QualType underlyingType, QualType can)
    : Type(Decltype, can, E->isTypeDependent()), E(E),
      UnderlyingType(underlyingType)
{
}

TemplateTypeParmType::TemplateTypeParmType(unsigned D, unsigned I, bool PP, QualType Canon)
    : Type(TemplateTypeParm, Canon, true),
      Depth(D), Index(I), ParameterPack(PP) { }

TemplateTypeParmType::TemplateTypeParmType(unsigned D, unsigned I, bool PP)
    : Type(TemplateTypeParm, QualType(shared_from_this(), 0), true),
      Depth(D), Index(I), ParameterPack(PP){ }

bool Type::isDependentType() const
{
    return Dependent;
}

Type::Type(Type::TypeClass tc, QualType Canonical, bool dependent)
    : CanonicalType(Canonical.isNull() ? QualType(shared_from_this(), 0) : Canonical),
      Dependent(dependent), TC(tc) {}

DependentDecltypeType::DependentDecltypeType(std::shared_ptr<Expr> E)
: DecltypeType(E, QualType())
{ }
