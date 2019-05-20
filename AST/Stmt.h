#ifndef AST_STMT_H
#define AST_STMT_H

#include <array>
#include <vector>
#include <memory>

class Expr;
class Stmt;
class DeclStmt;
class NullStmt;
class CompoundStmt;
class SwitchCase;
class CaseStmt;
class DefaultStmt;
class LabelStmt;
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
class AsmStmt;

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
    explicit Stmt(StmtClass SC, EmptyShell);
protected:
private:
    const unsigned sClass : 8;//The statement class.
    unsigned RefCount : 24;//The reference count for this statement.
};

class DeclStmt : public Stmt
{
public:
    DeclStmt() : Stmt(DeclStmtClass){}
    explicit DeclStmt(EmptyShell Empty) : Stmt(DeclStmtClass, Empty) { }
};

class NullStmt : public Stmt
{
public:
    NullStmt();
    explicit NullStmt(EmptyShell Empty);
};

class CompoundStmt : public Stmt
{
public:
    CompoundStmt(std::vector<std::shared_ptr<Stmt>> StmtStart, unsigned numStmts);
    explicit CompoundStmt(EmptyShell Empty);
private:
    std::vector<std::shared_ptr<Stmt>> Body;
    unsigned NumStmts;
};

class SwitchCase : public Stmt
{
public:
    SwitchCase(StmtClass SC) : Stmt(SC), NextSwitchCase(nullptr) {}
private:
    std::shared_ptr<SwitchCase> NextSwitchCase;
};

class CaseStmt : public SwitchCase
{
public:
    CaseStmt(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs);
    explicit CaseStmt(EmptyShell Empty);
private:
    enum { SUBSTMT, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

class DefaultStmt : public SwitchCase
{
public:
    DefaultStmt(std::shared_ptr<Stmt> substmt) :
        SwitchCase(DefaultStmtClass), SubStmt(substmt){}

      explicit DefaultStmt(EmptyShell) : SwitchCase(DefaultStmtClass) { }
private:
    std::shared_ptr<Stmt> SubStmt;
};

class LabelStmt : public Stmt
{
public:
    LabelStmt(std::shared_ptr<Stmt> substmt);
    explicit LabelStmt(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> SubStmt;
};

class IfStmt : public Stmt
{
public:
    IfStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> then, std::shared_ptr<Stmt> elsev = nullptr);

    explicit IfStmt(EmptyShell Empty);
private:
    enum { COND, THEN, ELSE, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
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
};

class WhileStmt : public Stmt
{
public:
    WhileStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> body);
    explicit WhileStmt(EmptyShell Empty);
private:
    enum { COND, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

class DoStmt : public Stmt
{
public:
    DoStmt(std::shared_ptr<Stmt> body, std::shared_ptr<Expr> cond);
    explicit DoStmt(EmptyShell Empty);
private:
    enum { COND, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

class ForStmt : public Stmt
{
public:
    ForStmt(std::shared_ptr<Stmt> Init, std::shared_ptr<Expr> Cond, std::shared_ptr<Expr> Inc, std::shared_ptr<Stmt> Body);
    explicit ForStmt(EmptyShell Empty);
private:
    enum { INIT, COND, INC, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

class GotoStmt : public Stmt
{
public:
    GotoStmt(LabelStmt *label);
    explicit GotoStmt(EmptyShell Empty);
private:
    std::shared_ptr<LabelStmt> Label;
};

class IndirectGotoStmt : public Stmt
{
public:
    IndirectGotoStmt(std::shared_ptr<Expr> target);

    explicit IndirectGotoStmt(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Target;
};

class ContinueStmt : public Stmt
{
public:
    ContinueStmt() : Stmt(ContinueStmtClass){}
    explicit ContinueStmt(EmptyShell Empty) : Stmt(ContinueStmtClass, Empty) { }
};

class BreakStmt : public Stmt
{
public:
    BreakStmt() : Stmt(BreakStmtClass){}
    explicit BreakStmt(EmptyShell Empty) : Stmt(BreakStmtClass, Empty) { }
};

class ReturnStmt : public Stmt
{
public:
    ReturnStmt(std::shared_ptr<Expr> E = 0);
    explicit ReturnStmt(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> RetExpr;
};

class AsmStmt : public Stmt
{

};

#endif // AST_STMT_H
