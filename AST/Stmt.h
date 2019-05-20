// Classes for representing statements
//The LLVM Compiler Infrastructure
//This file defines the Stmt interface and subclasses.
#ifndef AST_STMT_H
#define AST_STMT_H

#include <array>
#include <vector>
#include <memory>
#include "Basic/SourceLocation.h"

class Expr;
class Stmt;
class StringLiteral;

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

// Adaptor class for mixing declarations with statements and expressions.
class DeclStmt : public Stmt
{
public:
    DeclStmt() : Stmt(DeclStmtClass){}
    explicit DeclStmt(EmptyShell Empty);//Build an empty declaration statement.
};

//This is the null statement
class NullStmt : public Stmt
{
public:
    NullStmt();
    explicit NullStmt(EmptyShell Empty);//Build an empty null statement.
};

//This represents a group of statements like { stmt stmt }.
class CompoundStmt : public Stmt
{
public:
    CompoundStmt(std::vector<std::shared_ptr<Stmt>> StmtStart, unsigned numStmts);
    explicit CompoundStmt(EmptyShell Empty);// \brief Build an empty compound statement.
private:
    std::vector<std::shared_ptr<Stmt>> Body;
    unsigned NumStmts;
};
// SwitchCase is the base class for CaseStmt and DefaultStmt
class SwitchCase : public Stmt
{
public:
    SwitchCase(StmtClass SC);
private:
    // A pointer to the following CaseStmt or DefaultStmt class,used by SwitchStmt.
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
    DefaultStmt(std::shared_ptr<Stmt> substmt);
    // \brief Build an empty default statement.
    explicit DefaultStmt(EmptyShell);
private:
    std::shared_ptr<Stmt> SubStmt;
};

class LabelStmt : public Stmt
{
public:
    LabelStmt(std::shared_ptr<Stmt> substmt);
    // \brief Build an empty label statement.
    explicit LabelStmt(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> SubStmt;
};

//This represents an if/then/else.
class IfStmt : public Stmt
{
public:
    IfStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> then, std::shared_ptr<Stmt> elsev = nullptr);
    //Build an empty if/then/else statement
    explicit IfStmt(EmptyShell Empty);
private:
    enum { COND, THEN, ELSE, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

//This represents a 'switch' stmt.
class SwitchStmt : public Stmt
{
public:
    SwitchStmt(std::shared_ptr<Expr> cond);
    explicit SwitchStmt(EmptyShell Empty);
private:
    enum { COND, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
     // This points to a linked list of case and default statements
    std::shared_ptr<SwitchCase> FirstCase;
};
// This represents a 'while' stmt.
class WhileStmt : public Stmt
{
public:
    WhileStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> body, SourceLocation WL);
    //Build an empty while statement.
    explicit WhileStmt(EmptyShell Empty);
private:
    enum { COND, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation WhileLoc;
};
//This represents a 'do/while' stmt.
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
    SourceLocation RParenLoc;// Location of final ')' in do stmt condition.

};

//This represents a 'for (init;cond;inc)' stmt.  Note that any of the init/cond/inc parts of the ForStmt will be null if they were not specified in the source.
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
    explicit ForStmt(EmptyShell Empty);// \brief Build an empty for statement.
private:
    enum { INIT, COND, INC, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation ForLoc;
    SourceLocation LParenLoc, RParenLoc;
};

//This represents a direct goto.
class GotoStmt : public Stmt
{
public:
    GotoStmt(std::shared_ptr<LabelStmt> label, SourceLocation GL, SourceLocation LL);
    // \brief Build an empty goto statement.
    explicit GotoStmt(EmptyShell Empty);
private:
    std::shared_ptr<LabelStmt> Label;
    SourceLocation GotoLoc;
    SourceLocation LabelLoc;
};
//This represents an indirect goto.
class IndirectGotoStmt : public Stmt
{
public:
    IndirectGotoStmt(SourceLocation gotoLoc, SourceLocation starLoc, std::shared_ptr<Expr> target);
    // \brief Build an empty indirect goto statement.
    explicit IndirectGotoStmt(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Target;
    SourceLocation GotoLoc;
    SourceLocation StarLoc;
};

//This represents a continue.
class ContinueStmt : public Stmt
{
public:
    ContinueStmt(SourceLocation CL);
    // \brief Build an empty continue statement.
    explicit ContinueStmt(EmptyShell Empty);
private:
    SourceLocation ContinueLoc;
};
//This represents a break.
class BreakStmt : public Stmt
{
public:
    BreakStmt(SourceLocation BL);
    // \brief Build an empty break statement.
    explicit BreakStmt(EmptyShell Empty);
private:
    SourceLocation BreakLoc;
};

//This represents a return: return or return 4;
//Note that GCC allows return with no argument in a function declared to return a value, and it allows returning a value in functions declared to return void.
//We explicitly model this in the AST, which means you can't depend on the return type of the function and the presence of an argument.
class ReturnStmt : public Stmt
{
public:
    ReturnStmt(SourceLocation RL, std::shared_ptr<Expr> E = nullptr);
    // \brief Build an empty return expression.
    explicit ReturnStmt(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> RetExpr;
    SourceLocation RetLoc;
};

//This represents a GNU inline-assembly statement extension.
class AsmStmt : public Stmt
{
public:
    AsmStmt(SourceLocation asmloc, bool issimple, bool isvolatile,
            unsigned numoutputs, unsigned numinputs,
            std::vector<std::string> names, std::vector<std::shared_ptr<StringLiteral> > constraints,
            std::vector<std::shared_ptr<Expr> > exprs, std::shared_ptr<StringLiteral> asmstr, unsigned numclobbers,
            std::vector<std::shared_ptr<StringLiteral> > clobbers, SourceLocation rparenloc);
    // \brief Build an empty inline-assembly statement.
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
