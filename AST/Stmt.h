#ifndef AST_STMT_H
#define AST_STMT_H

#include <array>
#include <memory>
#include <list>
#include "Basic/SourceLocation.h"
#include "StmtIterator.h"

class DeclGroupRef;

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
class CommentStmt;

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
    StmtClass getStmtClass() const;
    static bool classof(const std::shared_ptr<Stmt> );
    virtual ~Stmt(){}

    typedef StmtIterator child_iterator;
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
    DeclStmt(std::shared_ptr<DeclGroupRef> dg, SourceLocation startLoc, SourceLocation endLoc);
    explicit DeclStmt(EmptyShell Empty);

    SourceLocation getStartLoc() const;
    SourceLocation getEndLoc() const;

    void setStartLoc(SourceLocation L);
    void setEndLoc(SourceLocation L) ;

    std::weak_ptr<DeclGroupRef> getDeclGroup();

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<DeclStmt>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
	std::shared_ptr<DeclGroupRef> m_dg;
    SourceLocation StartLoc;
    SourceLocation EndLoc;
};

/// NullStmt - This is the null statement ";": C99 6.8.3p3.
class NullStmt : public Stmt
{
public:
    NullStmt(SourceLocation L);
    explicit NullStmt(EmptyShell Empty);

    SourceLocation getSemiLoc() const;
    void setSemiLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<NullStmt>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    SourceLocation SemiLoc;
};

class CompoundStmt : public Stmt
{
public:
    CompoundStmt(std::list<std::shared_ptr<Stmt>> StmtStart, SourceLocation LB, SourceLocation RB);
    explicit CompoundStmt(EmptyShell Empty);

    void setStmts(std::list<std::shared_ptr<Stmt>> Stmts);
    void addStmt(std::shared_ptr<Stmt> Stmt);
    unsigned size() const;

    SourceLocation getLBracLoc() const;
    SourceLocation getRBracLoc() const;

    void setLBracLoc(SourceLocation L);
    void setRBracLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<CompoundStmt>);

    typedef std::list<std::shared_ptr<Stmt>>::iterator body_iterator;

    body_iterator body_begin() { return Body.begin(); }
    body_iterator body_end() { return Body.end(); }

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    std::list<std::shared_ptr<Stmt>> Body;
    SourceLocation LBracLoc, RBracLoc;
};

class SwitchCase : public Stmt
{
public:
    std::weak_ptr<SwitchCase> getNextSwitchCase();

    void setNextSwitchCase(std::shared_ptr<SwitchCase> SC);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<SwitchCase>);
protected:
    SwitchCase(StmtClass SC);
    std::shared_ptr<SwitchCase> NextSwitchCase;
};

class CaseStmt : public SwitchCase
{
public:
    CaseStmt(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, SourceLocation caseLoc,SourceLocation ellipsisLoc, SourceLocation colonLoc);
    explicit CaseStmt(EmptyShell Empty);

    SourceLocation getCaseLoc() const;
    SourceLocation getEllipsisLoc() const;
    SourceLocation getColonLoc() const;

    void setCaseLoc(SourceLocation L);
    void setEllipsisLoc(SourceLocation L);
    void setColonLoc(SourceLocation L);

    std::weak_ptr<Expr>getLHS();
    std::weak_ptr<Expr>getRHS();
    std::weak_ptr<Stmt>getSubStmt();

    void setSubStmt(std::shared_ptr<Stmt>S);
    void setLHS(std::shared_ptr<Expr>Val);
    void setRHS(std::shared_ptr<Expr>Val);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<CaseStmt>);

    // Iterators
    virtual child_iterator child_begin();
    virtual child_iterator child_end();

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

    std::weak_ptr<Stmt>getSubStmt();
    SourceLocation getDefaultLoc() const;
    SourceLocation getColonLoc() const;

    void setSubStmt(std::shared_ptr<Stmt>S);
    void setDefaultLoc(SourceLocation L);
    void setColonLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<DefaultStmt>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();

private:
    std::shared_ptr<Stmt> SubStmt;
    SourceLocation DefaultLoc;
    SourceLocation ColonLoc;
};

class LabelStmt : public Stmt
{
public:
    LabelStmt(SourceLocation IL, std::shared_ptr<Stmt> substmt);
    explicit LabelStmt(EmptyShell Empty);

    SourceLocation getIdentLoc() const;
 //    IdentifierInfo *getID() const { return Label; }
    std::weak_ptr<Stmt>getSubStmt();



    void setIdentLoc(SourceLocation L);
//    void setID(IdentifierInfo *II) { Label = II; }
    void setSubStmt(std::shared_ptr<Stmt> SS);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<LabelStmt>);


    virtual child_iterator child_begin();
    virtual child_iterator child_end();
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

    std::weak_ptr<Expr>getCond();
    std::weak_ptr<Stmt>getThen();
    std::weak_ptr<Stmt>getElse();

    void setCond(std::shared_ptr<Expr>E);
    void setThen(std::shared_ptr<Stmt>S);
    void setElse(std::shared_ptr<Stmt>S);

    SourceLocation getIfLoc() const;
    SourceLocation getElseLoc() const;

    void setIfLoc(SourceLocation L);
    void setElseLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<IfStmt>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
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

    std::weak_ptr<Expr>getCond();
    std::weak_ptr<Stmt>getBody();
    std::weak_ptr<SwitchCase>getSwitchCaseList();

    void setCond(std::shared_ptr<Expr>E);
    void setBody(std::shared_ptr<Stmt>S);
    void setSwitchCaseList(std::shared_ptr<SwitchCase>SC);

    SourceLocation getSwitchLoc() const ;

    void setSwitchLoc(SourceLocation L);
    void setBody(std::shared_ptr<Stmt>S, SourceLocation SL);


    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<SwitchStmt>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
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

    std::weak_ptr<Expr> getCond();
    std::weak_ptr<Stmt> getBody();

    void setCond(std::shared_ptr<Expr> E);
    void setBody(std::shared_ptr<Stmt> S);

    SourceLocation getWhileLoc() const;
    void setWhileLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<WhileStmt>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
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

    std::weak_ptr<Expr> getCond();
	std::weak_ptr<Stmt> getBody();

    void setCond(std::shared_ptr<Expr> E);
    void setBody(std::shared_ptr<Stmt> S);

    SourceLocation getDoLoc() const;
    SourceLocation getWhileLoc() const;
    SourceLocation getRParenLoc() const;

    void setDoLoc(SourceLocation L);
    void setWhileLoc(SourceLocation L);
    void setRParenLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<DoStmt>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
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

    std::weak_ptr<Stmt> getInit();
    std::weak_ptr<Stmt> getCond();
    std::weak_ptr<Expr> getInc();
    std::weak_ptr<Stmt> getBody();

    void setInit(std::shared_ptr<Stmt> S);
    void setCond(std::shared_ptr<Expr> E);
    void setInc(std::shared_ptr<Expr> E);
    void setBody(std::shared_ptr<Stmt> S);

    SourceLocation getForLoc() const;
    SourceLocation getLParenLoc() const;
    SourceLocation getRParenLoc() const;

    void setForLoc(SourceLocation L);
    void setLParenLoc(SourceLocation L);
    void setRParenLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<ForStmt>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
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

    std::weak_ptr<LabelStmt> getLabel() const;
    void setLabel(std::shared_ptr<LabelStmt> S);

    SourceLocation getGotoLoc() const;
    SourceLocation getLabelLoc() const;

    void setGotoLoc(SourceLocation L);
    void setLabelLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<GotoStmt>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
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

    SourceLocation getGotoLoc() const;
    SourceLocation getStarLoc() const;

    void setGotoLoc(SourceLocation L);
    void setStarLoc(SourceLocation L);

    std::weak_ptr<Expr> getTarget();
    void setTarget(std::shared_ptr<Expr> E);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<IndirectGotoStmt>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
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

    SourceLocation getContinueLoc() const;
    void setContinueLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<ContinueStmt>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    SourceLocation ContinueLoc;
};

class BreakStmt : public Stmt
{
public:
    BreakStmt(SourceLocation BL);
    explicit BreakStmt(EmptyShell Empty);

    SourceLocation getBreakLoc() const;
    void setBreakLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<BreakStmt>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    SourceLocation BreakLoc;
};

class ReturnStmt : public Stmt
{
public:
    ReturnStmt(SourceLocation RL, std::shared_ptr<Expr> E = nullptr);
    explicit ReturnStmt(EmptyShell Empty);

    std::weak_ptr<Expr> getRetValue();
    void setRetValue(std::shared_ptr<Expr> E);

    SourceLocation getReturnLoc() const;
    void setReturnLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<ReturnStmt>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    std::shared_ptr<Stmt> RetExpr;
    SourceLocation RetLoc;
};

class AsmStmt : public Stmt
{
public:
    AsmStmt(SourceLocation asmloc, bool issimple, bool isvolatile,
            unsigned numoutputs, unsigned numinputs,
            std::list<std::string> names, std::list<std::shared_ptr<StringLiteral> > constraints,
            std::list<std::shared_ptr<Expr> > exprs, std::shared_ptr<StringLiteral> asmstr, unsigned numclobbers,
            std::list<std::shared_ptr<StringLiteral> > clobbers, SourceLocation rparenloc);
    explicit AsmStmt(EmptyShell Empty);

    SourceLocation getAsmLoc() const;
    SourceLocation getRParenLoc() const;

    void setAsmLoc(SourceLocation L);
    void setRParenLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<AsmStmt>);


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

class CommentStmt : public Stmt
{
public:
    CommentStmt(std::string comment, SourceLocation SL);
    explicit CommentStmt(EmptyShell Empty);

    SourceLocation getCommentLoc() const;

    void setCommetLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<CommentStmt>);

    std::string getComment() const;

    virtual child_iterator child_begin(){}
    virtual child_iterator child_end(){}

private:
    SourceLocation CommentLoc;
    std::string Comment;
};

#endif // AST_STMT_H
