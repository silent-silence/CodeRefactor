#include "compoundliteralexpr.h"

using std::shared_ptr;

CompoundLiteralExpr::CompoundLiteralExpr(QualType ty, shared_ptr<Expr> init, bool fileScope)
    : Expr(CompoundLiteralExprClass, ty), Init(init), FileScope(fileScope)
{}

CompoundLiteralExpr::CompoundLiteralExpr(Stmt::EmptyShell Empty)
    : Expr(CompoundLiteralExprClass, Empty)
{}
