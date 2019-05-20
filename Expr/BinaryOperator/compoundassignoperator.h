#ifndef COMPOUNDASSIGNOPERATOR_H
#define COMPOUNDASSIGNOPERATOR_H

#include "binaryoperator.h"

class CompoundAssignOperator : public BinaryOperator
{
public:
    CompoundAssignOperator(std::shared_ptr<Expr> lhs,
                           std::shared_ptr<Expr> rhs,
                           Opcode opc,
                           QualType ResType,
                           QualType CompLHSType,
                           QualType CompResultType);
    explicit CompoundAssignOperator(EmptyShell Empty);
private:
    QualType ComputationLHSType;
    QualType ComputationResultType;
};

#endif // COMPOUNDASSIGNOPERATOR_H
