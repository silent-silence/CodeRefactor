#include "AST/Stmt.h"
#include "AST/Expr.h"

using std::dynamic_pointer_cast;

//line 22 61
Stmt::Stmt(Stmt::StmtClass SC)
    : sClass(SC), RefCount(1)
{
}

Stmt::StmtClass Stmt::getStmtClass() const
{
    return static_cast<StmtClass>(sClass);
}

bool Stmt::classof(const std::shared_ptr<Stmt>)
{
    return true;
}

Stmt::Stmt(Stmt::StmtClass SC, Stmt::EmptyShell)
    : sClass(SC), RefCount(1)
{
}

DeclStmt::DeclStmt(std::shared_ptr<DeclGroupRef> dg, SourceLocation startLoc, SourceLocation endLoc)
    : Stmt(StmtClass::DeclStmtClass), m_dg{dg}, StartLoc(startLoc), EndLoc(endLoc) {}

DeclStmt::DeclStmt(EmptyShell Empty) : Stmt(StmtClass::DeclStmtClass, Empty) { }

SourceLocation DeclStmt::getStartLoc() const
{
    return StartLoc;
}

SourceLocation DeclStmt::getEndLoc() const
{
     return EndLoc;
}

void DeclStmt::setStartLoc(SourceLocation L)
{
    StartLoc = L;
}

void DeclStmt::setEndLoc(SourceLocation L)
{
    EndLoc = L;
}

std::weak_ptr<DeclGroupRef> DeclStmt::getDeclGroup()
{
	return m_dg;
}

bool DeclStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::DeclStmtClass;
}

bool DeclStmt::classof(const std::weak_ptr<DeclStmt>)
{
    return true;
}

Stmt::child_iterator DeclStmt::child_begin()
{
    return child_iterator(std::make_shared<EmptyIterator>());
}

Stmt::child_iterator DeclStmt::child_end()
{
    return child_iterator(std::make_shared<EmptyIterator>());
}

NullStmt::NullStmt(SourceLocation L) : Stmt(StmtClass::NullStmtClass), SemiLoc(L) {}

NullStmt::NullStmt(EmptyShell Empty) : Stmt(StmtClass::NullStmtClass, Empty) { }

SourceLocation NullStmt::getSemiLoc() const
{
    return SemiLoc;
}

void NullStmt::setSemiLoc(SourceLocation L)
{
    SemiLoc = L;
}

bool NullStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::NullStmtClass;
}

bool NullStmt::classof(const std::weak_ptr<NullStmt>)
{
    return true;
}

Stmt::child_iterator NullStmt::child_begin()
{
    return child_iterator(std::make_shared<EmptyIterator>());
}

Stmt::child_iterator NullStmt::child_end()
{
    return child_iterator(std::make_shared<EmptyIterator>());
}

CompoundStmt::CompoundStmt(std::list<std::shared_ptr<Stmt>> StmtStart,
                           SourceLocation LB, SourceLocation RB)
    :Stmt (StmtClass::CompoundStmtClass), Body{StmtStart}, LBracLoc(LB), RBracLoc(RB)
{}

CompoundStmt::CompoundStmt(EmptyShell Empty):Stmt(StmtClass::CompoundStmtClass, Empty)
{}

void CompoundStmt::setStmts(std::list<std::shared_ptr<Stmt> > Stmts)
{
    this->Body=Stmts;
}

void CompoundStmt::addStmt(std::shared_ptr<Stmt> Stmt)
{
    Body.push_back(Stmt);
}

unsigned CompoundStmt::size() const
{
    return static_cast<unsigned>(Body.size());
}

SourceLocation CompoundStmt::getLBracLoc() const
{
    return LBracLoc;
}

SourceLocation CompoundStmt::getRBracLoc() const
{
    return RBracLoc;
}

void CompoundStmt::setLBracLoc(SourceLocation L)
{
    LBracLoc = L;
}

void CompoundStmt::setRBracLoc(SourceLocation L)
{
    RBracLoc = L;
}

bool CompoundStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::CompoundStmtClass;
}

bool CompoundStmt::classof(const std::weak_ptr<CompoundStmt>)
{
    return true;
}

Stmt::child_iterator CompoundStmt::child_begin()
{
    return child_iterator(std::make_shared<ListIterator>(Body.begin()));
}

Stmt::child_iterator CompoundStmt::child_end()
{
    return child_iterator(std::make_shared<ListIterator>(Body.end()));
}

std::weak_ptr<SwitchCase> SwitchCase::getNextSwitchCase()
{
    return NextSwitchCase;
}

void SwitchCase::setNextSwitchCase(std::shared_ptr<SwitchCase> SC)
{
    NextSwitchCase = SC;
}

bool SwitchCase::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::CaseStmtClass ||
            T.lock()->getStmtClass() == StmtClass::DefaultStmtClass;
}

bool SwitchCase::classof(const std::weak_ptr<SwitchCase>)
{
     return true;
}

SwitchCase::SwitchCase(StmtClass SC)
    : Stmt(SC), NextSwitchCase(nullptr)
{}

CaseStmt::CaseStmt(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs,
                   SourceLocation caseLoc, SourceLocation ellipsisLoc,
                   SourceLocation colonLoc)
    : SwitchCase(StmtClass::CaseStmtClass)
{
    SubExprs[SUBSTMT] = nullptr;
    SubExprs[LHS] = lhs;
    SubExprs[RHS] = rhs;
    CaseLoc = caseLoc;
    EllipsisLoc = ellipsisLoc;
    ColonLoc = colonLoc;
}

CaseStmt::CaseStmt(EmptyShell Empty)
    :SwitchCase (StmtClass::CaseStmtClass)
{}

SourceLocation CaseStmt::getCaseLoc() const
{
    return CaseLoc;
}

SourceLocation CaseStmt::getEllipsisLoc() const
{
    return EllipsisLoc;
}

SourceLocation CaseStmt::getColonLoc() const
{
    return ColonLoc;
}

void CaseStmt::setCaseLoc(SourceLocation L)
{
    CaseLoc = L;
}

void CaseStmt::setEllipsisLoc(SourceLocation L)
{
    EllipsisLoc = L;
}

void CaseStmt::setColonLoc(SourceLocation L)
{
    ColonLoc = L;
}

std::weak_ptr<Expr> CaseStmt::getLHS()
{
    return std::dynamic_pointer_cast<Expr>(SubExprs[LHS]);
}

std::weak_ptr<Expr> CaseStmt::getRHS()
{
    return std::dynamic_pointer_cast<Expr>(SubExprs[RHS]);
}

std::weak_ptr<Stmt> CaseStmt::getSubStmt()
{
    return SubExprs[SUBSTMT];
}

void CaseStmt::setSubStmt(std::shared_ptr<Stmt> S)
{
    SubExprs[SUBSTMT] = S;
}

void CaseStmt::setLHS(std::shared_ptr<Expr> Val)
{
    SubExprs[LHS] = std::dynamic_pointer_cast<Stmt>(Val);
}

void CaseStmt::setRHS(std::shared_ptr<Expr> Val)
{
    SubExprs[RHS] = std::dynamic_pointer_cast<Stmt>(Val);
}

bool CaseStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::CaseStmtClass;
}

bool CaseStmt::classof(const std::weak_ptr<CaseStmt>)
{
    return true;
}

Stmt::child_iterator CaseStmt::child_begin()
{
    return child_iterator(std::make_shared<
                          ArrayIterator<
                          std::array<
                          std::shared_ptr<Stmt>,
                          END_EXPR>::iterator>>(SubExprs.begin()));
}

Stmt::child_iterator CaseStmt::child_end()
{
    return child_iterator(std::make_shared<
                          ArrayIterator<
                          std::array<
                          std::shared_ptr<Stmt>,
                          END_EXPR>::iterator>>(SubExprs.end()));
}

DefaultStmt::DefaultStmt(SourceLocation DL, SourceLocation CL, std::shared_ptr<Stmt> substmt)
    :SwitchCase(StmtClass::DefaultStmtClass), SubStmt(substmt), DefaultLoc(DL), ColonLoc(CL)
{}

DefaultStmt::DefaultStmt(EmptyShell)
    : SwitchCase(StmtClass::DefaultStmtClass)
{ }

std::weak_ptr<Stmt> DefaultStmt::getSubStmt()
{
    return SubStmt;
}

SourceLocation DefaultStmt::getDefaultLoc() const
{
    return DefaultLoc;
}

SourceLocation DefaultStmt::getColonLoc() const
{
    return ColonLoc;
}

void DefaultStmt::setSubStmt(std::shared_ptr<Stmt> S)
{
    SubStmt = S;
}

void DefaultStmt::setDefaultLoc(SourceLocation L)
{
    DefaultLoc = L;
}

void DefaultStmt::setColonLoc(SourceLocation L)
{
    ColonLoc = L;
}

bool DefaultStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::DefaultStmtClass;
}

bool DefaultStmt::classof(const std::weak_ptr<DefaultStmt>)
{
    return true;
}

Stmt::child_iterator DefaultStmt::child_begin()
{
    return child_iterator(std::make_shared<PtrIterator>(PtrIterator::PtrPosition::begin, SubStmt));
}

Stmt::child_iterator DefaultStmt::child_end()
{
     return child_iterator(std::make_shared<PtrIterator>(PtrIterator::PtrPosition::end, SubStmt));
}

LabelStmt::LabelStmt(SourceLocation IL, std::shared_ptr<Stmt> substmt)
    : Stmt(StmtClass::LabelStmtClass),
      SubStmt(substmt), IdentLoc(IL) {}

LabelStmt::LabelStmt(EmptyShell Empty)
    :Stmt (StmtClass::LabelStmtClass,Empty)
{}

SourceLocation LabelStmt::getIdentLoc() const
{
    return IdentLoc;
}

std::weak_ptr<Stmt> LabelStmt::getSubStmt()
{
    return SubStmt;
}

void LabelStmt::setIdentLoc(SourceLocation L)
{
    IdentLoc = L;
}

void LabelStmt::setSubStmt(std::shared_ptr<Stmt> SS)
{
    SubStmt = SS;
}

bool LabelStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::LabelStmtClass;
}

bool LabelStmt::classof(const std::weak_ptr<LabelStmt>)
{
    return true;
}

Stmt::child_iterator LabelStmt::child_begin()
{
    return child_iterator(std::make_shared<PtrIterator>(PtrIterator::PtrPosition::begin, SubStmt));
}

Stmt::child_iterator LabelStmt::child_end()
{
   return child_iterator(std::make_shared<PtrIterator>(PtrIterator::PtrPosition::end, SubStmt));
}

SwitchStmt::SwitchStmt(SourceLocation SL, std::shared_ptr<Expr> cond)
    :Stmt (StmtClass::SwitchStmtClass),FirstCase(0), SwitchLoc{SL}
{
    SubExprs[COND]=cond;
    SubExprs[BODY]=nullptr;
}

SwitchStmt::SwitchStmt(EmptyShell Empty)
    : Stmt(StmtClass::SwitchStmtClass, Empty)
{ }

std::weak_ptr<Expr> SwitchStmt::getCond()
{
    return std::dynamic_pointer_cast<Expr>(SubExprs[COND]);
}

std::weak_ptr<Stmt> SwitchStmt::getBody()
{
    return SubExprs[BODY];
}

std::weak_ptr<SwitchCase> SwitchStmt::getSwitchCaseList()
{
    return FirstCase;
}

void SwitchStmt::setCond(std::shared_ptr<Expr> E)
{
    SubExprs[COND] =std::dynamic_pointer_cast<Expr>(E);
}

void SwitchStmt::setBody(std::shared_ptr<Stmt> S)
{
    SubExprs[BODY] = S;
}

void SwitchStmt::setSwitchCaseList(std::shared_ptr<SwitchCase> SC)
{
    FirstCase = SC;
}

SourceLocation SwitchStmt::getSwitchLoc() const
{
    return SwitchLoc;
}

void SwitchStmt::setSwitchLoc(SourceLocation L)
{
    SwitchLoc = L;
}

void SwitchStmt::setBody(std::shared_ptr<Stmt> S, SourceLocation SL)
{
    SubExprs[BODY] = S;
    SwitchLoc = SL;
}

bool SwitchStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::SwitchStmtClass;
}

bool SwitchStmt::classof(const std::weak_ptr<SwitchStmt>)
{
    return true;
}

Stmt::child_iterator SwitchStmt::child_begin()
{
    return child_iterator(
                std::make_unique<
                ArrayIterator<
                std::array<
                std::shared_ptr<Stmt>,
                END_EXPR>::iterator>>(SubExprs.begin()));
}

Stmt::child_iterator SwitchStmt::child_end()
{
    return child_iterator(
                std::make_unique<
                ArrayIterator<
                std::array<
                std::shared_ptr<Stmt>,
                END_EXPR>::iterator>>(SubExprs.end()));
}
WhileStmt::WhileStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> body, SourceLocation WL)
    : Stmt(StmtClass::WhileStmtClass)
{
    SubExprs[COND] = cond;
    SubExprs[BODY] = body;
    WhileLoc = WL;
}

WhileStmt::WhileStmt(Stmt::EmptyShell Empty)
    : Stmt(StmtClass::WhileStmtClass, Empty) { }

std::weak_ptr<Expr> WhileStmt::getCond()
{
    return std::dynamic_pointer_cast<Expr>(SubExprs[COND]);
}

std::weak_ptr<Stmt> WhileStmt::getBody()
{
    return SubExprs[BODY];
}

void WhileStmt::setCond(std::shared_ptr<Expr> E)
{
    SubExprs[COND] = std::dynamic_pointer_cast<Stmt>(E);
}

void WhileStmt::setBody(std::shared_ptr<Stmt> S)
{
    SubExprs[BODY] = S;
}

SourceLocation WhileStmt::getWhileLoc() const
{
    return WhileLoc;
}

void WhileStmt::setWhileLoc(SourceLocation L)
{
    WhileLoc = L;
}

bool WhileStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::WhileStmtClass;
}

bool WhileStmt::classof(const std::weak_ptr<WhileStmt>)
{
    return true;
}

Stmt::child_iterator WhileStmt::child_begin()
{
    return child_iterator(
                std::make_unique<
                ArrayIterator<
                std::array<
                std::shared_ptr<Stmt>,
                END_EXPR>::iterator>>(SubExprs.begin()));
}

Stmt::child_iterator WhileStmt::child_end()
{
    return child_iterator(
                std::make_unique<
                ArrayIterator<
                std::array<
                std::shared_ptr<Stmt>,
                END_EXPR>::iterator>>(SubExprs.end()));
}

DoStmt::DoStmt(std::shared_ptr<Stmt> body,
               std::shared_ptr<Expr> cond,
               SourceLocation DL,
               SourceLocation WL,
               SourceLocation RP)
    : Stmt(StmtClass::DoStmtClass), DoLoc(DL), WhileLoc(WL), RParenLoc(RP) {
    SubExprs[COND] = cond;
    SubExprs[BODY] = body;
}

DoStmt::DoStmt(Stmt::EmptyShell Empty)
    : Stmt(StmtClass::DoStmtClass, Empty) { }

std::weak_ptr<Expr> DoStmt::getCond()
{
    return std::dynamic_pointer_cast<Expr>(SubExprs[COND]);
}

std::weak_ptr<Stmt> DoStmt::getBody()
{
    return SubExprs[BODY];
}

void DoStmt::setCond(std::shared_ptr<Expr> E)
{
    SubExprs[COND] = std::dynamic_pointer_cast<Stmt>(E);
}

void DoStmt::setBody(std::shared_ptr<Stmt> S)
{
    SubExprs[BODY] = S;
}

SourceLocation DoStmt::getDoLoc() const
{
    return DoLoc;
}

SourceLocation DoStmt::getWhileLoc() const
{
    return WhileLoc;
}

SourceLocation DoStmt::getRParenLoc() const
{
    return RParenLoc;
}

void DoStmt::setDoLoc(SourceLocation L)
{
    DoLoc = L;
}

void DoStmt::setWhileLoc(SourceLocation L)
{
    WhileLoc = L;
}

void DoStmt::setRParenLoc(SourceLocation L)
{
    RParenLoc = L;
}

bool DoStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::DoStmtClass;
}

bool DoStmt::classof(const std::weak_ptr<DoStmt>)
{
    return true;
}

Stmt::child_iterator DoStmt::child_begin()
{
    return child_iterator(
                std::make_unique<
                ArrayIterator<
                std::array<
                std::shared_ptr<Stmt>,
                END_EXPR>::iterator>>(SubExprs.begin()));
}

Stmt::child_iterator DoStmt::child_end()
{
    return child_iterator(
                std::make_unique<
                ArrayIterator<
                std::array<
                std::shared_ptr<Stmt>,
                END_EXPR>::iterator>>(SubExprs.end()));
}

ForStmt::ForStmt(std::shared_ptr<Stmt> Init, std::shared_ptr<Stmt> Cond, std::shared_ptr<Expr> Inc, std::shared_ptr<Stmt> Body, SourceLocation FL, SourceLocation LP, SourceLocation RP)
    : Stmt(StmtClass::ForStmtClass)
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
    : Stmt(StmtClass::ForStmtClass, Empty) { }

std::weak_ptr<Stmt> ForStmt::getInit()
{
    return SubExprs[INIT];
}

std::weak_ptr<Stmt> ForStmt::getCond()
{
    return std::dynamic_pointer_cast<Stmt>(SubExprs[COND]);
}

std::weak_ptr<Expr> ForStmt::getInc()
{
    return std::dynamic_pointer_cast<Expr>(SubExprs[INC]);
}

std::weak_ptr<Stmt> ForStmt::getBody()
{
    return SubExprs[BODY];
}

void ForStmt::setInit(std::shared_ptr<Stmt> S)
{
    SubExprs[INIT] = S;
}

void ForStmt::setCond(std::shared_ptr<Expr> E)
{
    SubExprs[COND] = std::dynamic_pointer_cast<Stmt>(E);
}

void ForStmt::setInc(std::shared_ptr<Expr> E)
{
    SubExprs[INC] = std::dynamic_pointer_cast<Stmt>(E);
}

void ForStmt::setBody(std::shared_ptr<Stmt> S)
{
    SubExprs[BODY] = S;
}

SourceLocation ForStmt::getForLoc() const
{
    return ForLoc;
}

SourceLocation ForStmt::getLParenLoc() const
{
    return LParenLoc;
}

SourceLocation ForStmt::getRParenLoc() const
{
    return RParenLoc;
}

void ForStmt::setForLoc(SourceLocation L)
{
    ForLoc = L;
}

void ForStmt::setLParenLoc(SourceLocation L)
{
    LParenLoc = L;
}

void ForStmt::setRParenLoc(SourceLocation L)
{
    RParenLoc = L;
}

bool ForStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::ForStmtClass;
}

bool ForStmt::classof(const std::weak_ptr<ForStmt>)
{
    return true;
}

Stmt::child_iterator ForStmt::child_begin()
{
    return child_iterator(
                std::make_unique<
                ArrayIterator<
                std::array<
                std::shared_ptr<Stmt>,
                END_EXPR>::iterator>>(SubExprs.begin()));
}

Stmt::child_iterator ForStmt::child_end()
{
    return child_iterator(
                std::make_unique<
                ArrayIterator<
                std::array<
                std::shared_ptr<Stmt>,
                END_EXPR>::iterator>>(SubExprs.end()));
}

GotoStmt::GotoStmt(std::shared_ptr<LabelStmt> label, SourceLocation GL, SourceLocation LL)
    : Stmt(StmtClass::GotoStmtClass), Label(label), GotoLoc(GL), LabelLoc(LL) {}

GotoStmt::GotoStmt(Stmt::EmptyShell Empty)
    : Stmt(StmtClass::GotoStmtClass, Empty) { }

std::weak_ptr<LabelStmt> GotoStmt::getLabel() const
{
    return Label;
}

void GotoStmt::setLabel(std::shared_ptr<LabelStmt> S)
{
    Label = S;
}

SourceLocation GotoStmt::getGotoLoc() const
{
    return GotoLoc;
}

SourceLocation GotoStmt::getLabelLoc() const
{
    return LabelLoc;
}

void GotoStmt::setGotoLoc(SourceLocation L)
{
    GotoLoc = L;
}

void GotoStmt::setLabelLoc(SourceLocation L)
{
    LabelLoc = L;
}

bool GotoStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::GotoStmtClass;
}

bool GotoStmt::classof(const std::weak_ptr<GotoStmt>)
{
    return true;
}

Stmt::child_iterator GotoStmt::child_begin()
{
    return child_iterator(std::make_shared<EmptyIterator>());
}

Stmt::child_iterator GotoStmt::child_end()
{
  return child_iterator(std::make_shared<EmptyIterator>());
}

IndirectGotoStmt::IndirectGotoStmt(SourceLocation gotoLoc, SourceLocation starLoc, std::shared_ptr<Expr> target)
    : Stmt(StmtClass::IndirectGotoStmtClass), GotoLoc(gotoLoc), StarLoc(starLoc), Target(target)
{

}

IndirectGotoStmt::IndirectGotoStmt(Stmt::EmptyShell Empty)
    : Stmt(StmtClass::IndirectGotoStmtClass, Empty) { }

SourceLocation IndirectGotoStmt::getGotoLoc() const
{
    return GotoLoc;
}

SourceLocation IndirectGotoStmt::getStarLoc() const
{
    return StarLoc;
}

void IndirectGotoStmt::setGotoLoc(SourceLocation L)
{
    GotoLoc = L;
}

void IndirectGotoStmt::setStarLoc(SourceLocation L)
{
    StarLoc = L;
}

void IndirectGotoStmt::setTarget(std::shared_ptr<Expr> E)
{
    Target = std::dynamic_pointer_cast<Stmt>(E);
}

bool IndirectGotoStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::IndirectGotoStmtClass;
}

bool IndirectGotoStmt::classof(const std::weak_ptr<IndirectGotoStmt>)
{
    return true;
}

Stmt::child_iterator IndirectGotoStmt::child_begin()
{
    return child_iterator(std::make_shared<PtrIterator>(PtrIterator::PtrPosition::begin, Target));
}

Stmt::child_iterator IndirectGotoStmt::child_end()
{
    return child_iterator(std::make_shared<PtrIterator>(PtrIterator::PtrPosition::end, Target));
}

ContinueStmt::ContinueStmt(SourceLocation CL)
    : Stmt(StmtClass::ContinueStmtClass), ContinueLoc(CL) {}

ContinueStmt::ContinueStmt(Stmt::EmptyShell Empty)
    : Stmt(StmtClass::ContinueStmtClass, Empty) { }

SourceLocation ContinueStmt::getContinueLoc() const
{
    return ContinueLoc;
}

void ContinueStmt::setContinueLoc(SourceLocation L)
{
    ContinueLoc = L;
}

bool ContinueStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::ContinueStmtClass;
}

bool ContinueStmt::classof(const std::weak_ptr<ContinueStmt>)
{
    return true;
}

Stmt::child_iterator ContinueStmt::child_begin()
{
    return child_iterator(std::make_shared<EmptyIterator>());
}

Stmt::child_iterator ContinueStmt::child_end()
{
    return child_iterator(std::make_shared<EmptyIterator>());
}

BreakStmt::BreakStmt(SourceLocation BL)
    : Stmt(StmtClass::BreakStmtClass), BreakLoc(BL) {}

BreakStmt::BreakStmt(Stmt::EmptyShell Empty)
    : Stmt(StmtClass::BreakStmtClass, Empty) { }

SourceLocation BreakStmt::getBreakLoc() const
{
    return BreakLoc;
}

void BreakStmt::setBreakLoc(SourceLocation L)
{
    BreakLoc = L;
}

bool BreakStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::BreakStmtClass;
}

bool BreakStmt::classof(const std::weak_ptr<BreakStmt>)
{
    return true;
}

Stmt::child_iterator BreakStmt::child_begin()
{
    return child_iterator(std::make_shared<EmptyIterator>());
}

Stmt::child_iterator BreakStmt::child_end()
{
  return child_iterator(std::make_shared<EmptyIterator>());
}

ReturnStmt::ReturnStmt(SourceLocation RL, std::shared_ptr<Expr> E)
    : Stmt(StmtClass::ReturnStmtClass),
      RetExpr(E),
      RetLoc(RL) {}

ReturnStmt::ReturnStmt(Stmt::EmptyShell Empty)
    : Stmt(StmtClass::ReturnStmtClass, Empty) { }

std::weak_ptr<Expr> ReturnStmt::getRetValue()
{
	return dynamic_pointer_cast<Expr>(RetExpr);
}

void ReturnStmt::setRetValue(std::shared_ptr<Expr> E)
{
    RetExpr = std::dynamic_pointer_cast<Stmt>(E);
}

SourceLocation ReturnStmt::getReturnLoc() const
{
    return RetLoc;
}

void ReturnStmt::setReturnLoc(SourceLocation L)
{
    RetLoc = L;
}

bool ReturnStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::ReturnStmtClass;
}

bool ReturnStmt::classof(const std::weak_ptr<ReturnStmt>)
{
    return true;
}

Stmt::child_iterator ReturnStmt::child_begin()
{
    return child_iterator(std::make_shared<PtrIterator>(PtrIterator::PtrPosition::begin, RetExpr));
}

Stmt::child_iterator ReturnStmt::child_end()
{
    return child_iterator(std::make_shared<PtrIterator>(PtrIterator::PtrPosition::end, RetExpr));
}

AsmStmt::AsmStmt(SourceLocation asmloc, bool issimple, bool isvolatile,
                 unsigned numoutputs, unsigned numinputs,
                 std::list<std::string> names, std::list<std::shared_ptr<StringLiteral>> constraints,
                 std::list<std::shared_ptr<Expr>> exprs, std::shared_ptr<StringLiteral> asmstr, unsigned numclobbers,
                 std::list<std::shared_ptr<StringLiteral>> clobbers, SourceLocation rparenloc)
    : Stmt(StmtClass::AsmStmtClass), AsmLoc(asmloc), RParenLoc(rparenloc), AsmStr(asmstr),
      IsSimple(issimple), IsVolatile(isvolatile), NumOutputs(numoutputs), NumInputs(numinputs)
{
//    for (unsigned i = 0, e = numinputs + numoutputs; i != e; i++) {
//        Names[i]=names[i];
//        Exprs[i]=exprs[i];
//        Constraints[i]=constraints[i];
//    }
//    for (unsigned i = 0; i != numclobbers; i++)
//        Clobbers[i]=clobbers[i];
}

SourceLocation AsmStmt::getAsmLoc() const
{
    return AsmLoc;
}

SourceLocation AsmStmt::getRParenLoc() const
{
    return RParenLoc;
}

void AsmStmt::setAsmLoc(SourceLocation L)
{
    AsmLoc = L;
}

void AsmStmt::setRParenLoc(SourceLocation L)
{
    RParenLoc = L;
}

bool AsmStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::AsmStmtClass;
}

bool AsmStmt::classof(const std::weak_ptr<AsmStmt>)
{
     return true;
}

AsmStmt::AsmStmt(Stmt::EmptyShell Empty)
    : Stmt(StmtClass::AsmStmtClass, Empty) { }

IfStmt::IfStmt(SourceLocation IL, std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> then, SourceLocation EL, std::shared_ptr<Stmt> elsev)
    : Stmt(StmtClass::IfStmtClass)
{
    SubExprs[COND] = cond;
    SubExprs[THEN] = then;
    SubExprs[ELSE] = elsev;
    IfLoc = IL;
    ElseLoc = EL;
}

IfStmt::IfStmt(Stmt::EmptyShell Empty)
    : Stmt(StmtClass::IfStmtClass, Empty)
{ }

std::weak_ptr<Expr> IfStmt::getCond()
{
    return std::dynamic_pointer_cast<Expr>(SubExprs[COND]);
}

std::weak_ptr<Stmt> IfStmt::getThen()
{
    return SubExprs[THEN];
}

std::weak_ptr<Stmt> IfStmt::getElse()
{
    return SubExprs[ELSE];
}

void IfStmt::setCond(std::shared_ptr<Expr> E)
{
    SubExprs[COND] =std::dynamic_pointer_cast<Stmt>(E);
}

void IfStmt::setThen(std::shared_ptr<Stmt> S)
{
    SubExprs[THEN] = S;
}

void IfStmt::setElse(std::shared_ptr<Stmt> S)
{
    SubExprs[ELSE] = S;
}

SourceLocation IfStmt::getIfLoc() const
{
    return IfLoc;
}

SourceLocation IfStmt::getElseLoc() const
{
    return ElseLoc;
}

void IfStmt::setIfLoc(SourceLocation L)
{
    IfLoc = L;
}

void IfStmt::setElseLoc(SourceLocation L)
{
    ElseLoc = L;
}

bool IfStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::IfStmtClass;
}

bool IfStmt::classof(const std::weak_ptr<IfStmt>)
{
    return true;
}

Stmt::child_iterator IfStmt::child_begin()
{
    return child_iterator(
                std::make_unique<
                ArrayIterator<
                std::array<
                std::shared_ptr<Stmt>,
                END_EXPR>::iterator>>(SubExprs.begin()));
}

Stmt::child_iterator IfStmt::child_end()
{
    return child_iterator(
                std::make_unique<
                ArrayIterator<
                std::array<
                std::shared_ptr<Stmt>,
                END_EXPR>::iterator>>(SubExprs.end()));
}

CommentStmt::CommentStmt(std::string comment, SourceLocation SL)
    :Stmt(StmtClass::CommentStmtClass), Comment{comment}, CommentLoc{SL}
{}

CommentStmt::CommentStmt(Stmt::EmptyShell Empty)
    :Stmt(StmtClass::CommentStmtClass, Empty)
{}

SourceLocation CommentStmt::getCommentLoc() const
{
    return CommentLoc;
}

void CommentStmt::setCommetLoc(SourceLocation L)
{
    CommentLoc = L;
}

bool CommentStmt::classof(const std::weak_ptr<Stmt> T)
{
    return T.lock()->getStmtClass() == StmtClass::CommentStmtClass;
}

bool CommentStmt::classof(const std::weak_ptr<CommentStmt>)
{
    return true;
}

std::string CommentStmt::getComment() const
{
    return Comment;
}

Stmt::child_iterator CommentStmt::child_end()
{
	return child_iterator(std::make_shared<EmptyIterator>());
}

Stmt::child_iterator CommentStmt::child_begin()
{
	return child_iterator(std::make_shared<EmptyIterator>());
}
