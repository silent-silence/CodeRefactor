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
    virtual ~Stmt(){}
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
private:
    SourceLocation SemiLoc;
};

class CompoundStmt : public Stmt
{
public:
    CompoundStmt(std::vector<std::shared_ptr<Stmt>> StmtStart, SourceLocation LB, SourceLocation RB);
    explicit CompoundStmt(EmptyShell Empty);
private:
    std::vector<std::shared_ptr<Stmt>> Body;
    SourceLocation LBracLoc, RBracLoc;
};

class SwitchCase : public Stmt
{
public:

protected:
    SwitchCase(StmtClass SC);
    std::shared_ptr<SwitchCase> NextSwitchCase;
};

class CaseStmt : public SwitchCase
{
public:
    CaseStmt(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, SourceLocation caseLoc,
             SourceLocation ellipsisLoc, SourceLocation colonLoc);
    explicit CaseStmt(EmptyShell Empty);
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
private:
    enum { COND, THEN, ELSE, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation IfLoc;
    SourceLocation ElseLoc;
};

class SwitchStmt : public Stmt
{
public:
    SwitchStmt(std::shared_ptr<Expr> cond);
    explicit SwitchStmt(EmptyShell Empty);
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
            std::shared_ptr<Expr> Cond,
            std::shared_ptr<Expr> Inc,
            std::shared_ptr<Stmt> Body,
            SourceLocation FL,
            SourceLocation LP,
            SourceLocation RP);
    explicit ForStmt(EmptyShell Empty);
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
private:
    SourceLocation ContinueLoc;
};

class BreakStmt : public Stmt
{
public:
    BreakStmt(SourceLocation BL);
    explicit BreakStmt(EmptyShell Empty);
private:
    SourceLocation BreakLoc;
};

class ReturnStmt : public Stmt
{
public:
    ReturnStmt(SourceLocation RL, std::shared_ptr<Expr> E = nullptr);

    explicit ReturnStmt(EmptyShell Empty);
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
