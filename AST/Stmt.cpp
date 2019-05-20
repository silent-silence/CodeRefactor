#include "AST/Stmt.h"
#include "AST/Expr.h"
//line 22 61
Stmt::Stmt(Stmt::StmtClass SC)
    : sClass(SC), RefCount(1)
{
}

Stmt::Stmt(Stmt::StmtClass SC, Stmt::EmptyShell)
    : sClass(SC), RefCount(1)
{
}

DeclStmt::DeclStmt(SourceLocation startLoc, SourceLocation endLoc)
    : Stmt(DeclStmtClass), StartLoc(startLoc), EndLoc(endLoc) {}

DeclStmt::DeclStmt(EmptyShell Empty) : Stmt(DeclStmtClass, Empty) { }

NullStmt::NullStmt(SourceLocation L) : Stmt(NullStmtClass), SemiLoc(L) {}

NullStmt::NullStmt(EmptyShell Empty) : Stmt(NullStmtClass, Empty) { }

CompoundStmt::CompoundStmt(std::vector<std::shared_ptr<Stmt>> StmtStart, SourceLocation LB, SourceLocation RB)
    :Stmt (CompoundStmtClass),LBracLoc(LB), RBracLoc(RB)
{
}


CompoundStmt::CompoundStmt(EmptyShell Empty):Stmt(CompoundStmtClass, Empty)
{}

SwitchCase::SwitchCase(StmtClass SC)
    : Stmt(SC), NextSwitchCase(nullptr)
{}

CaseStmt:: CaseStmt(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs):SwitchCase(CaseStmtClass)
{
    SubExprs[SUBSTMT] = 0;
    SubExprs[LHS] = lhs;
    SubExprs[RHS] =rhs;
}
CaseStmt::CaseStmt(EmptyShell Empty)
    :SwitchCase (CaseStmtClass)
{}

DefaultStmt::DefaultStmt(std::shared_ptr<Stmt> substmt) :
    SwitchCase(DefaultStmtClass), SubStmt(substmt){}

DefaultStmt::DefaultStmt(EmptyShell)
    : SwitchCase(DefaultStmtClass)
{ }

LabelStmt::LabelStmt(std::shared_ptr<Stmt> substmt)
    :Stmt(LabelStmtClass),SubStmt(substmt)
{}

LabelStmt::LabelStmt(EmptyShell Empty)
    :Stmt (LabelStmtClass,Empty)
{}

SwitchStmt::SwitchStmt(std::shared_ptr<Expr> cond)
    :Stmt (SwitchStmtClass),FirstCase(0)
{
    SubExprs[COND]=cond;
    SubExprs[BODY]=NULL;
}

SwitchStmt::SwitchStmt(EmptyShell Empty)
    : Stmt(SwitchStmtClass, Empty)
{ }
WhileStmt::WhileStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> body, SourceLocation WL)
    : Stmt(WhileStmtClass)
{
    SubExprs[COND] = cond;
    SubExprs[BODY] = body;
    WhileLoc = WL;
}

WhileStmt::WhileStmt(Stmt::EmptyShell Empty)
    : Stmt(WhileStmtClass, Empty) { }

DoStmt::DoStmt(std::shared_ptr<Stmt> body,
               std::shared_ptr<Expr> cond,
               SourceLocation DL,
               SourceLocation WL,
               SourceLocation RP)
    : Stmt(DoStmtClass), DoLoc(DL), WhileLoc(WL), RParenLoc(RP) {
    SubExprs[COND] = cond;
    SubExprs[BODY] = body;
}

DoStmt::DoStmt(Stmt::EmptyShell Empty)
    : Stmt(DoStmtClass, Empty) { }

ForStmt::ForStmt(std::shared_ptr<Stmt> Init, std::shared_ptr<Expr> Cond, std::shared_ptr<Expr> Inc, std::shared_ptr<Stmt> Body, SourceLocation FL, SourceLocation LP, SourceLocation RP)
    : Stmt(ForStmtClass)
{
    SubExprs[INIT] = Init;
    SubExprs[COND] = Cond;
    SubExprs[INC] = Inc;
    SubExprs[BODY] = Body;
    ForLoc = FL;
    LParenLoc = LP;
    RParenLoc = RP;
}

ForStmt::ForStmt(Stmt::EmptyShell Empty)
    : Stmt(ForStmtClass, Empty) { }

GotoStmt::GotoStmt(std::shared_ptr<LabelStmt> label, SourceLocation GL, SourceLocation LL)
    : Stmt(GotoStmtClass), Label(label), GotoLoc(GL), LabelLoc(LL) {}

GotoStmt::GotoStmt(Stmt::EmptyShell Empty)
    : Stmt(GotoStmtClass, Empty) { }

IndirectGotoStmt::IndirectGotoStmt(SourceLocation gotoLoc, SourceLocation starLoc, std::shared_ptr<Expr> target)
    : Stmt(IndirectGotoStmtClass), GotoLoc(gotoLoc), StarLoc(starLoc), Target(target)
{

}

IndirectGotoStmt::IndirectGotoStmt(Stmt::EmptyShell Empty)
    : Stmt(IndirectGotoStmtClass, Empty) { }

ContinueStmt::ContinueStmt(SourceLocation CL)
    : Stmt(ContinueStmtClass), ContinueLoc(CL) {}

ContinueStmt::ContinueStmt(Stmt::EmptyShell Empty)
    : Stmt(ContinueStmtClass, Empty) { }

BreakStmt::BreakStmt(SourceLocation BL)
    : Stmt(BreakStmtClass), BreakLoc(BL) {}

BreakStmt::BreakStmt(Stmt::EmptyShell Empty)
    : Stmt(BreakStmtClass, Empty) { }

ReturnStmt::ReturnStmt(SourceLocation RL, std::shared_ptr<Expr> E)
    : Stmt(ReturnStmtClass),
      RetExpr(E),
      RetLoc(RL) {}

ReturnStmt::ReturnStmt(Stmt::EmptyShell Empty)
    : Stmt(ReturnStmtClass, Empty) { }

AsmStmt::AsmStmt(SourceLocation asmloc, bool issimple, bool isvolatile,
                 unsigned numoutputs, unsigned numinputs,
                 std::vector<std::string> names, std::vector<std::shared_ptr<StringLiteral>> constraints,
                 std::vector<std::shared_ptr<Expr>> exprs, std::shared_ptr<StringLiteral> asmstr, unsigned numclobbers,
                 std::vector<std::shared_ptr<StringLiteral>> clobbers, SourceLocation rparenloc)
    : Stmt(AsmStmtClass), AsmLoc(asmloc), RParenLoc(rparenloc), AsmStr(asmstr),
      IsSimple(issimple), IsVolatile(isvolatile), NumOutputs(numoutputs), NumInputs(numinputs)
{
    for (unsigned i = 0, e = numinputs + numoutputs; i != e; i++) {
        Names[i]=names[i];
        Exprs[i]=exprs[i];
        Constraints[i]=constraints[i];
    }
    for (unsigned i = 0; i != numclobbers; i++)
        Clobbers[i]=clobbers[i];
}

AsmStmt::AsmStmt(Stmt::EmptyShell Empty)
    : Stmt(AsmStmtClass, Empty) { }

IfStmt::IfStmt(SourceLocation IL, std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> then, SourceLocation EL, std::shared_ptr<Stmt> elsev)
    : Stmt(IfStmtClass)
{
    SubExprs[COND] = cond;
    SubExprs[THEN] = then;
    SubExprs[ELSE] = elsev;
    IfLoc = IL;
    ElseLoc = EL;
}

IfStmt::IfStmt(Stmt::EmptyShell Empty)
    : Stmt(IfStmtClass, Empty)
{ }
