#include "compoundassignoperator.h"

using std::shared_ptr;

CompoundAssignOperator::CompoundAssignOperator(shared_ptr<Expr> lhs,
                                               shared_ptr<Expr> rhs,
                                               BinaryOperator::Opcode opc,
                                               QualType ResType,
                                               QualType CompLHSType,
                                               QualType CompResultType)
    : BinaryOperator(lhs, rhs, opc, ResType, true),
      ComputationLHSType(CompLHSType),
      ComputationResultType(CompResultType)
{
}

CompoundAssignOperator::CompoundAssignOperator(Stmt::EmptyShell Empty)
    : BinaryOperator(CompoundAssignOperatorClass, Empty)
{}
