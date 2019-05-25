#include "unaryoperator.h"

using std::shared_ptr;

UnaryOperator::UnaryOperator(shared_ptr<Expr> input, UnaryOperator::Opcode opc, QualType type)
    : Expr(UnaryOperatorClass, type,
           input->isTypeDependent() && opc != OffsetOf,
           input->isValueDependent()),
      Val(input), Opc(opc)
{}

UnaryOperator::UnaryOperator(Stmt::EmptyShell Empty)
    : Expr(UnaryOperatorClass, Empty), Opc(AddrOf)
{ }
