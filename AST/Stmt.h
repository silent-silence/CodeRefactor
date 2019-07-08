#ifndef AST_STMT_H
#define AST_STMT_H

#include <array>
#include <memory>
#include <list>
#include "Basic/SourceLocation.h"
#include "Iterator/StmtIterator.h"

class DeclGroupRef;

class Expr;
class Stmt;
class Decl;
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

/// @brief This represents one statement, super class of all Stmt and Expr
class Stmt
{
public:
	/// @brief List stmt and expr kind
    enum class StmtClass : unsigned {
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
    const StmtClass sClass;//The statement class.
    unsigned RefCount : 24;//The reference count for this statement.
};

/// @brief This represent a stmt who declared a name
/// @example: 'int a;' is a DeclStmt
class DeclStmt : public Stmt
{
public:
    DeclStmt(std::shared_ptr<DeclGroupRef> dg, SourceLocation startLoc, SourceLocation endLoc);
    explicit DeclStmt(EmptyShell Empty);

    SourceLocation getStartLoc() const;
    SourceLocation getEndLoc() const;

    void setStartLoc(SourceLocation L);
    void setEndLoc(SourceLocation L) ;

    const std::weak_ptr<DeclGroupRef> getDeclGroup() const;
    std::weak_ptr<DeclGroupRef> getDeclGroup();
    void setDeclGroup(std::shared_ptr<DeclGroupRef> DGR);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<DeclStmt>);

    child_iterator child_begin() override;
    child_iterator child_end() override;
private:
	/// @brief Save names that declared in the stmt
	std::shared_ptr<DeclGroupRef> m_dg;
    SourceLocation StartLoc;
    SourceLocation EndLoc;
};

/// @brief This is the null statement ";"
class NullStmt : public Stmt
{
public:
    NullStmt(SourceLocation L);
    explicit NullStmt(EmptyShell Empty);

    SourceLocation getSemiLoc() const;
    void setSemiLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<NullStmt>);

    child_iterator child_begin() override;
    child_iterator child_end() override;
private:
    SourceLocation SemiLoc;
};

/// @brief This is a compound stmt
/// @example '{ 1+2; }' is a compound stmt
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

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<CompoundStmt>);

    typedef std::list<std::shared_ptr<Stmt>>::iterator body_iterator;

    body_iterator body_begin() { return Body.begin(); }
    body_iterator body_end() { return Body.end(); }

    child_iterator child_begin() override;
    child_iterator child_end() override;

private:
    std::list<std::shared_ptr<Stmt>> Body;
    SourceLocation LBracLoc, RBracLoc;
};

/// @brief The super class of switch/case stmt
class SwitchCase : public Stmt
{
public:
    const std::weak_ptr<SwitchCase> getNextSwitchCase() const;
    std::weak_ptr<SwitchCase> getNextSwitchCase();
    void setNextSwitchCase(std::shared_ptr<SwitchCase> SC);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<SwitchCase>);
protected:
    SwitchCase(StmtClass SC);
    std::shared_ptr<SwitchCase> NextSwitchCase;
};

/// @brief Represent a case stmt
/// @example 'case 1:'
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

    const std::weak_ptr<Expr>getLHS() const;
    const std::weak_ptr<Expr>getRHS() const;
    const std::weak_ptr<Stmt>getSubStmt() const;

    void setSubStmt(std::shared_ptr<Stmt>S);
    void setLHS(std::shared_ptr<Expr>Val);
    void setRHS(std::shared_ptr<Expr>Val);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<CaseStmt>);

    // Iterators
    child_iterator child_begin() override;
    child_iterator child_end() override;

private:
	/// @brief
	/// SUBSTMT: next stmt after case stmt
	/// LHS: condition
	/// RHS: GUN 'case 1...4' extension
    enum { SUBSTMT, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation CaseLoc;
    SourceLocation EllipsisLoc;
    SourceLocation ColonLoc;
};

/// @brief Represent default stmt
class DefaultStmt : public SwitchCase
{
public:
    DefaultStmt(SourceLocation DL, SourceLocation CL, std::shared_ptr<Stmt> substmt);
    explicit DefaultStmt(EmptyShell);

    std::weak_ptr<Stmt>getSubStmt();
    const std::weak_ptr<Stmt>getSubStmt() const;

    SourceLocation getDefaultLoc() const;
    SourceLocation getColonLoc() const;

    void setSubStmt(std::shared_ptr<Stmt>S);
    void setDefaultLoc(SourceLocation L);
    void setColonLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<DefaultStmt>);

    child_iterator child_begin() override;
    child_iterator child_end() override;

private:
	/// @brief Next stmt after default
    std::shared_ptr<Stmt> SubStmt;
    SourceLocation DefaultLoc;
    SourceLocation ColonLoc;
};

/// @brief Represent a switch stmt
class SwitchStmt : public Stmt
{
public:
	SwitchStmt(SourceLocation SL, std::shared_ptr<Expr> cond);
	explicit SwitchStmt(EmptyShell Empty);

    const std::weak_ptr<Expr>getCond() const;
    const std::weak_ptr<Stmt>getBody() const;
    const std::weak_ptr<SwitchCase>getSwitchCaseList() const;

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

	child_iterator child_begin() override;
	child_iterator child_end() override;
private:
	enum { COND, BODY, END_EXPR };
	std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
	std::shared_ptr<SwitchCase> FirstCase;
	SourceLocation SwitchLoc;
};

/// @brief Represent a labeled stmt
class LabelStmt : public Stmt
{
public:
    LabelStmt(SourceLocation IL, std::shared_ptr<Stmt> substmt, std::weak_ptr<Decl> label);
    explicit LabelStmt(EmptyShell Empty);

    SourceLocation getIdentLoc() const;
    std::weak_ptr<Decl> getID() const { return Label; }
    std::weak_ptr<Stmt>getSubStmt();

    void setIdentLoc(SourceLocation L);
//    void setID(IdentifierInfo *II) { Label = II; }
    void setSubStmt(std::shared_ptr<Stmt> SS);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<LabelStmt>);


    child_iterator child_begin() override;
    child_iterator child_end() override;
private:
    std::shared_ptr<Stmt> SubStmt;
    SourceLocation IdentLoc;
    std::weak_ptr<Decl> Label;
};

/// @brief Represent a if stmt
class IfStmt : public Stmt
{
public:
    IfStmt(SourceLocation IL, std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> then,
           SourceLocation EL = SourceLocation(), std::shared_ptr<Stmt> elsev= nullptr);
    explicit IfStmt(EmptyShell Empty);

    const std::weak_ptr<Expr>getCond() const;
    const std::weak_ptr<Stmt>getThen() const;
    const std::weak_ptr<Stmt>getElse() const;

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

    child_iterator child_begin() override;
    child_iterator child_end() override;
private:
    enum { COND, THEN, ELSE, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation IfLoc;
    SourceLocation ElseLoc;
};

/// @brief Represent a while stmt
class WhileStmt : public Stmt
{
public:
    WhileStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> body, SourceLocation WL);
    explicit WhileStmt(EmptyShell Empty);

    const std::weak_ptr<Expr> getCond() const;
    const std::weak_ptr<Stmt> getBody() const;

    std::weak_ptr<Expr> getCond();
    std::weak_ptr<Stmt> getBody();

    void setCond(std::shared_ptr<Expr> E);
    void setBody(std::shared_ptr<Stmt> S);

    SourceLocation getWhileLoc() const;
    void setWhileLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<WhileStmt>);

    child_iterator child_begin() override;
    child_iterator child_end() override;
private:
    enum { COND, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation WhileLoc;
};

/// @brief Represent a do-while stmt
class DoStmt : public Stmt
{
public:
    DoStmt(std::shared_ptr<Stmt> body,
           std::shared_ptr<Expr> cond,
           SourceLocation DL,
           SourceLocation WL,
           SourceLocation RP);
    explicit DoStmt(EmptyShell Empty);

    const std::weak_ptr<Expr> getCond() const;
    const std::weak_ptr<Stmt> getBody() const;

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

    child_iterator child_begin() override;
    child_iterator child_end() override;
private:
    enum { COND, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation DoLoc;
    SourceLocation WhileLoc;
    SourceLocation RParenLoc;
};

/// @brief Represent a for stmt
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

    const std::weak_ptr<Stmt> getInit() const;
    const std::weak_ptr<Stmt> getCond() const;
    const std::weak_ptr<Expr> getInc() const;
    const std::weak_ptr<Stmt> getBody() const;

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

    child_iterator child_begin() override;
    child_iterator child_end() override;
private:
    enum { INIT, COND, INC, BODY, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation ForLoc;
    SourceLocation LParenLoc, RParenLoc;
};

/// @brief Represent a goto stmt
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

    child_iterator child_begin() override;
    child_iterator child_end() override;
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
    const std::weak_ptr<Expr> getTarget() const;
    void setTarget(std::shared_ptr<Expr> E);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<IndirectGotoStmt>);

    child_iterator child_begin() override;
    child_iterator child_end() override;
private:
    SourceLocation GotoLoc;
    SourceLocation StarLoc;
    std::shared_ptr<Stmt> Target;
};

/// @brief Represent a continue stmt
class ContinueStmt : public Stmt
{
public:
    ContinueStmt(SourceLocation CL);
    explicit ContinueStmt(EmptyShell Empty);

    SourceLocation getContinueLoc() const;
    void setContinueLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<ContinueStmt>);

    child_iterator child_begin() override;
    child_iterator child_end() override;
private:
    SourceLocation ContinueLoc;
};

/// @brief Represent a break stmt
class BreakStmt : public Stmt
{
public:
    BreakStmt(SourceLocation BL);
    explicit BreakStmt(EmptyShell Empty);

    SourceLocation getBreakLoc() const;
    void setBreakLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<BreakStmt>);

    child_iterator child_begin() override;
    child_iterator child_end() override;
private:
    SourceLocation BreakLoc;
};

/// @brief Represent a return stmt
class ReturnStmt : public Stmt
{
public:
    ReturnStmt(SourceLocation RL, std::shared_ptr<Expr> E = nullptr);
    explicit ReturnStmt(EmptyShell Empty);

    const std::weak_ptr<Expr> getRetValue() const;
    std::weak_ptr<Expr> getRetValue();
    void setRetValue(std::shared_ptr<Expr> E);

    SourceLocation getReturnLoc() const;
    void setReturnLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<ReturnStmt>);

    child_iterator child_begin() override;
    child_iterator child_end() override;
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

/// @brief Represent a comment
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

	child_iterator child_begin() override;
    child_iterator child_end() override;

private:
    SourceLocation CommentLoc;
    std::string Comment;
};

#endif // AST_STMT_H
