#include "binaryoperator.h"

using std::shared_ptr;

BinaryOperator::BinaryOperator(shared_ptr<Expr> lhs,
                               shared_ptr<Expr> rhs,
                               BinaryOperator::Opcode opc,
                               QualType ResTy)
    : Expr(BinaryOperatorClass,
           ResTy,
           lhs->isTypeDependent() || rhs->isTypeDependent(),
           lhs->isValueDependent() || rhs->isValueDependent()), Opc(opc)
{
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
}

BinaryOperator::BinaryOperator(Stmt::EmptyShell Empty)
    : Expr(BinaryOperatorClass, Empty), Opc(Comma)
{}

BinaryOperator::BinaryOperator(shared_ptr<Expr> lhs,
                               shared_ptr<Expr> rhs,
                               BinaryOperator::Opcode opc,
                               QualType ResTy,
                               bool dead)
    : Expr(CompoundAssignOperatorClass, ResTy), Opc(opc) {
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
}

BinaryOperator::BinaryOperator(Stmt::StmtClass SC, Stmt::EmptyShell Empty)
    : Expr(SC, Empty), Opc(MulAssign)
{}
