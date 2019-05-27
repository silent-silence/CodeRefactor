#ifndef AST_STMT_H
#define AST_STMT_H

#include <array>
#include <vector>
#include <memory>
#include "Basic/SourceLocation.h"

class Expr;
class Stmt;
class StringLiteral;

class DeclStmt;//
class NullStmt;
class CompoundStmt;
class SwitchCase;
class CaseStmt;
class DefaultStmt;
class LabelStmt;//
class IfStmt;
class SwitchStmt;

class WhileStmt;
class DoStmt;
class ForStmt;
class GotoStmt;
class IndirectGotoStmt;
class ContinueStmt;
class BreakStmt;
class ReturnStmt;
class AsmStmt;//GNU内联汇编语句扩展

class Iterator{
    std::shared_ptr<Stmt> *m_ptr;
public:
    Iterator():m_ptr(nullptr){}
    Iterator(std::shared_ptr<Stmt> *ptr)
        :m_ptr(ptr)
    {}
    Iterator& operator++() { ++m_ptr; return *this; }
    Iterator operator-(size_t i) { return m_ptr-i; }
    Iterator operator+(size_t i) { return m_ptr+i; }
    std::shared_ptr<Stmt> operator[](size_t idx){
        return m_ptr[idx];
    }

    signed operator-(const Iterator& R) const{
        return static_cast<signed>(m_ptr - R.m_ptr);
    }
    std::shared_ptr<Stmt> operator*() const{
        return *m_ptr;
    }
    std::shared_ptr<Stmt> operator->() const{
        return *m_ptr;
    }
    bool operator==(const Iterator& R) const { return m_ptr == R.m_ptr; }
    bool operator!=(const Iterator& R) const { return m_ptr != R.m_ptr; }
    bool operator>(const Iterator& R) const { return m_ptr > R.m_ptr; }
    bool operator>=(const Iterator& R) const { return m_ptr >= R.m_ptr; }
};

//Stmt - This represents one statement.
class Stmt
{
public:
    enum StmtClass {
        NoStmtClass = 0,
#define STMT(CLASS, PARENT) CLASS##Class,
#define FIRST_STMT(CLASS) firstStmtConstant = CLASS##Class,
#define LAST_STMT(CLASS) lastStmtConstant = CLASS##Class,
#define FIRST_EXPR(CLASS) firstExprConstant = CLASS##Class,
#define LAST_EXPR(CLASS) lastExprConstant = CLASS##Class
#include "StmtNodes.def"
    };
    struct EmptyShell {};
    Stmt(StmtClass SC);
    StmtClass getStmtClass() const { return static_cast<StmtClass>(sClass); }
    static bool classof(const std::shared_ptr<Stmt> ) { return true; }
    virtual ~Stmt(){}


    typedef Iterator child_iterator;
    virtual child_iterator child_begin() = 0;
    virtual child_iterator child_end()   = 0;
protected:
    explicit Stmt(StmtClass SC, EmptyShell);
private:
    const unsigned sClass : 8;//The statement class.
    unsigned RefCount : 24;//The reference count for this statement.
};

// DeclStmt - Adaptor class for mixing declarations with statements and expressions.
class DeclStmt : public Stmt
{
public:
    DeclStmt(SourceLocation startLoc, SourceLocation endLoc);
    explicit DeclStmt(EmptyShell Empty);
    virtual child_iterator child_begin(){return child_iterator();}
    virtual child_iterator child_end(){return child_iterator();}
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == DeclStmtClass;
    }
    static bool classof(const DeclStmt *) { return true; }
private:
    SourceLocation StartLoc;
    SourceLocation EndLoc;
};

/// NullStmt - This is the null statement ";": C99 6.8.3p3.
class NullStmt : public Stmt
{
public:
    NullStmt(SourceLocation L);
    explicit NullStmt(EmptyShell Empty);
    virtual child_iterator child_begin(){return child_iterator();}
    virtual child_iterator child_end(){return child_iterator();}
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == NullStmtClass;
    }
    static bool classof(const NullStmt *) { return true; }
private:
    SourceLocation SemiLoc;
};

class CompoundStmt : public Stmt
{
public:
    CompoundStmt(std::vector<std::shared_ptr<Stmt>> StmtStart, SourceLocation LB, SourceLocation RB);
    explicit CompoundStmt(EmptyShell Empty);
    virtual child_iterator child_begin(){ return &Body[0]; }
    virtual child_iterator child_end(){ return &Body[Body.size()]; }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == CompoundStmtClass;
    }
    static bool classof(const CompoundStmt *) { return true; }

private:
    std::vector<std::shared_ptr<Stmt>> Body;
    SourceLocation LBracLoc, RBracLoc;
};

class SwitchCase : public Stmt
{
public:
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == CaseStmtClass ||
      T->getStmtClass() == DefaultStmtClass;
    }
    static bool classof(const SwitchCase *) { return true; }
protected:
    SwitchCase(StmtClass SC);
    std::shared_ptr<SwitchCase> NextSwitchCase;
};

class CaseStmt : public SwitchCase
{
public:
    // Iterators
    virtual child_iterator child_begin(){ return &SubExprs[0]; }
    virtual child_iterator child_end(){ return &SubExprs[END_EXPR]; }
    CaseStmt(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, SourceLocation caseLoc,
             SourceLocation ellipsisLoc, SourceLocation colonLoc);
    explicit CaseStmt(EmptyShell Empty);
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == CaseStmtClass;
    }
    static bool classof(const CaseStmt *) { return true; }

    std::shared_ptr<Stmt> getSubStmt() const { return SubExprs[SUBSTMT]; }
    std::shared_ptr<Stmt> getLHS() const { return SubExprs[LHS]; }
    std::shared_ptr<Stmt> getRHS() const { return SubExprs[RHS]; }

private:
    enum { SUBSTMT, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation CaseLoc;
    SourceLocation EllipsisLoc;
    SourceLocation ColonLoc;
};

class DefaultStmt : public SwitchCase
{
public:
    DefaultStmt(SourceLocation DL, SourceLocation CL, std::shared_ptr<Stmt> substmt);
    explicit DefaultStmt(EmptyShell);
    virtual child_iterator child_begin() { return &SubStmt; }
    virtual child_iterator child_end() { return &SubStmt+1; }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == DefaultStmtClass;
    }
    static bool classof(const DefaultStmt *) { return true; }
private:
    std::shared_ptr<Stmt> SubStmt;
    SourceLocation DefaultLoc;
    SourceLocation ColonLoc;
};

class LabelStmt : public Stmt
{
public:
    virtual child_iterator child_begin() { return &SubStmt; }
    virtual child_iterator child_end() { return &SubStmt+1; }
    LabelStmt(SourceLocation IL, std::shared_ptr<Stmt> substmt);
    explicit LabelStmt(EmptyShell Empty);
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == LabelStmtClass;
    }
    static bool classof(const LabelStmt *) { return true; }

private:
    std::shared_ptr<Stmt> SubStmt;
    SourceLocation IdentLoc;
    //IdentifierInfo *Label;
};

class IfStmt : public Stmt
{
public:
    IfStmt(SourceLocation IL, std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> then,
           SourceLocation EL = SourceLocation(), std::shared_ptr<Stmt> elsev= nullptr);
    explicit IfStmt(EmptyShell Empty);
    virtual child_iterator child_begin() { return &SubExprs[0]; }
    virtual child_iterator child_end() { return &SubExprs[0]+END_EXPR; }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == IfStmtClass;
    }
    static bool classof(const IfStmt *) { return true; }

    std::shared_ptr<Stmt> getCond() const { return SubExprs[COND]; }
	std::shared_ptr<Stmt> getThen() const { return SubExprs[THEN]; }
	std::shared_ptr<Stmt> getElse() const { return SubExprs[ELSE]; }

private:
    enum { COND, THEN, ELSE, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation IfLoc;
    SourceLocation ElseLoc;
};

class SwitchStmt : public Stmt
{
public:
    SwitchStmt(SourceLocation SL, std::shared_ptr<Expr> cond);
    explicit SwitchStmt(EmptyShell Empty);
    virtual child_iterator child_begin(){return &SubExprs[0];}
    virtual child_iterator child_end(){ return &SubExprs[0]+END_EXPR; }
    static bool classof(const Stmt *T) {
      return T->getStmtClass() == SwitchStmtClass;
    }
    static bool classof(const SwitchStmt *) { return true; }

private:
    enum { COND, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    std::shared_ptr<SwitchCase> FirstCase;
    SourceLocation SwitchLoc;
};

class WhileStmt : public Stmt
{
public:
    WhileStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> body, SourceLocation WL);
    explicit WhileStmt(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
        return T->getStmtClass() == WhileStmtClass;
    }
    static bool classof(const std::shared_ptr<WhileStmt>) { return true; }
    virtual child_iterator child_begin(){ return &SubExprs[0];}
    virtual child_iterator child_end(){return &SubExprs[0]+END_EXPR;}

    std::shared_ptr<Stmt> getCond() const { return SubExprs[COND]; }
    std::shared_ptr<Stmt> getBody() const { return SubExprs[BODY]; }

private:
    enum { COND, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation WhileLoc;
};

class DoStmt : public Stmt
{
public:
    DoStmt(std::shared_ptr<Stmt> body,
           std::shared_ptr<Expr> cond,
           SourceLocation DL,
           SourceLocation WL,
           SourceLocation RP);
    explicit DoStmt(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
        return T->getStmtClass() == DoStmtClass;
    }
    static bool classof(const std::shared_ptr<DoStmt>) { return true; }
    virtual child_iterator child_begin(){ return &SubExprs[0]; }
    virtual child_iterator child_end(){ return &SubExprs[0]+END_EXPR; }

    std::shared_ptr<Stmt> getCond() const { return SubExprs[COND]; }
    std::shared_ptr<Stmt> getBody() const { return SubExprs[BODY]; }

private:
    enum { COND, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation DoLoc;
    SourceLocation WhileLoc;
    SourceLocation RParenLoc;
};

class ForStmt : public Stmt
{
public:
    ForStmt(std::shared_ptr<Stmt> Init,
            std::shared_ptr<Stmt> Cond,
            std::shared_ptr<Expr> Inc,
            std::shared_ptr<Stmt> Body,
            SourceLocation FL,
            SourceLocation LP,
            SourceLocation RP);
    explicit ForStmt(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
        return T->getStmtClass() == ForStmtClass;
    }
    static bool classof(const std::shared_ptr<ForStmt>) { return true; }
    virtual child_iterator child_begin(){ return &SubExprs[0]; }
    virtual child_iterator child_end() { return &SubExprs[0]+END_EXPR; }

    std::shared_ptr<Stmt> getInit() const { return SubExprs[INIT]; }
	std::shared_ptr<Stmt> getCond() const { return SubExprs[COND]; }
	std::shared_ptr<Stmt> getInc() const { return SubExprs[INC]; }
	std::shared_ptr<Stmt> getBody() const { return SubExprs[BODY]; }

private:
    enum { INIT, COND, INC, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation ForLoc;
    SourceLocation LParenLoc, RParenLoc;
};

class GotoStmt : public Stmt
{
public:
    GotoStmt(std::shared_ptr<LabelStmt> label, SourceLocation GL, SourceLocation LL);
    explicit GotoStmt(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
        return T->getStmtClass() == GotoStmtClass;
    }
    static bool classof(const std::shared_ptr<GotoStmt>) { return true; }
    virtual child_iterator child_begin(){ return child_iterator(); }
    virtual child_iterator child_end(){ return child_iterator(); }
private:
    std::shared_ptr<LabelStmt> Label;
    SourceLocation GotoLoc;
    SourceLocation LabelLoc;
};

class IndirectGotoStmt : public Stmt
{
public:
    IndirectGotoStmt(SourceLocation gotoLoc, SourceLocation starLoc, std::shared_ptr<Expr> target);

    explicit IndirectGotoStmt(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
        return T->getStmtClass() == IndirectGotoStmtClass;
    }
    static bool classof(const std::shared_ptr<IndirectGotoStmt>) { return true; }
    virtual child_iterator child_begin(){ return &Target; }
    virtual child_iterator child_end(){ return &Target+1; }
private:
    SourceLocation GotoLoc;
    SourceLocation StarLoc;
    std::shared_ptr<Stmt> Target;
};

class ContinueStmt : public Stmt
{
public:
    ContinueStmt(SourceLocation CL);

    explicit ContinueStmt(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
        return T->getStmtClass() == ContinueStmtClass;
    }
    static bool classof(const std::shared_ptr<ContinueStmt>) { return true; }
    virtual child_iterator child_begin(){ return child_iterator(); }
    virtual child_iterator child_end(){ return child_iterator(); }
private:
    SourceLocation ContinueLoc;
};

class BreakStmt : public Stmt
{
public:
    BreakStmt(SourceLocation BL);
    explicit BreakStmt(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
        return T->getStmtClass() == BreakStmtClass;
    }
    static bool classof(const std::shared_ptr<BreakStmt>) { return true; }
    virtual child_iterator child_begin(){ return child_iterator(); }
    virtual child_iterator child_end(){ return child_iterator(); }
private:
    SourceLocation BreakLoc;
};

class ReturnStmt : public Stmt
{
public:
    ReturnStmt(SourceLocation RL, std::shared_ptr<Expr> E = nullptr);

    explicit ReturnStmt(EmptyShell Empty);
    static bool classof(const std::shared_ptr<Stmt> T) {
        return T->getStmtClass() == ReturnStmtClass;
    }
    static bool classof(const std::shared_ptr<ReturnStmt>) { return true; }
    virtual child_iterator child_begin(){ return &RetExpr; }
    virtual child_iterator child_end() { return RetExpr ? &RetExpr+1 : &RetExpr; }

    std::shared_ptr<Expr> getRetValue() const { return std::dynamic_pointer_cast<Expr>(RetExpr); }

private:
    std::shared_ptr<Stmt> RetExpr;
    SourceLocation RetLoc;
};

class AsmStmt : public Stmt
{
public:
    AsmStmt(SourceLocation asmloc, bool issimple, bool isvolatile,
            unsigned numoutputs, unsigned numinputs,
            std::vector<std::string> names, std::vector<std::shared_ptr<StringLiteral> > constraints,
            std::vector<std::shared_ptr<Expr> > exprs, std::shared_ptr<StringLiteral> asmstr, unsigned numclobbers,
            std::vector<std::shared_ptr<StringLiteral> > clobbers, SourceLocation rparenloc);

    static bool classof(const std::shared_ptr<Stmt> T) {return T->getStmtClass() == AsmStmtClass;}
    static bool classof(const std::shared_ptr<AsmStmt>) { return true; }
    explicit AsmStmt(EmptyShell Empty);
private:
    SourceLocation AsmLoc, RParenLoc;
    std::shared_ptr<StringLiteral> AsmStr;

    bool IsSimple;
    bool IsVolatile;

    unsigned NumOutputs;
    unsigned NumInputs;

    std::array<std::string, 4> Names;
    std::array<std::shared_ptr<StringLiteral>, 4> Constraints;
    std::array<std::shared_ptr<Stmt>, 4> Exprs;
    std::array<std::shared_ptr<StringLiteral>, 4> Clobbers;
};

#endif // AST_STMT_H
