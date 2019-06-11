#ifndef AST_EXPR_H
#define AST_EXPR_H

#include "AST/Type.h"
#include "AST/Stmt.h"
#include "Decl/Decl.h"

class Expr;
class Stmt;

class DeclRefExpr;
class PredefinedExpr;
class IntegerLiteral;
class CharacterLiteral;
class FloatingLiteral;
class ImaginaryLiteral;
class StringLiteral;//
class ParenExpr;
class UnaryOperator;
class SizeOfAlignOfExpr;
class ArraySubscriptExpr;
class CallExpr;
class MemberExpr;
class CompoundLiteralExpr;
class CastExpr;
class ImplicitCastExpr;
class ExplicitCastExpr;
class CStyleCastExpr;

class BinaryOperator;
class CompoundAssignOperator;
class ConditionalOperator;
class AddrLabelExpr;
class StmtExpr;
class TypesCompatibleExpr;
class ShuffleVectorExpr;
class ChooseExpr;
class GNUNullExpr;
class VAArgExpr;
class InitListExpr;
class DesignatedInitExpr;
class ImplicitValueInitExpr;
class ParenListExpr;
class ExtVectorElementExpr;
class BlockExpr;
class BlockDeclRefExpr;

/// Expr - This represents one expression.  Note that Expr's are subclasses of
/// Stmt.  This allows an expression to be transparently used any place a Stmt
/// is required.
///
class Expr : public Stmt
{
public:
    std::weak_ptr<QualType> getType() const;
    void setType(const std::shared_ptr<QualType> &value);/**/

    bool isTypeDependent() const;
    void setTypeDependent(bool value);

    bool isValueDependent() const;
    void setValueDependent(bool value);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<Expr>);
    virtual int EvaluateAsInt() const = 0;
protected:
    Expr(StmtClass SC, std::shared_ptr<QualType> T);
    Expr(StmtClass SC, std::shared_ptr<QualType> T, bool TD, bool VD);

    /// \brief Construct an empty expression.
    explicit Expr(StmtClass SC, EmptyShell);

    /// TypeDependent - Whether this expression is type-dependent
    bool TypeDependent : 1;
    /// ValueDependent - Whether this expression is value-dependent
    bool ValueDependent : 1;
private:
    std::shared_ptr<QualType> TR;
};

//===----------------------------------------------------------------------===//
// Primary Expressions.
//===----------------------------------------------------------------------===//

/// DeclRefExpr - [C99 6.5.1p2] - A reference to a declared variable, function,
/// enum, etc.
class DeclRefExpr : public Expr
{
public:
    DeclRefExpr(std::shared_ptr<NamedDecl> d, std::shared_ptr<QualType> t, SourceLocation l);
    DeclRefExpr(std::shared_ptr<NamedDecl> d, std::shared_ptr<QualType> t,
                SourceLocation l, bool TD, bool VD);
    explicit DeclRefExpr(EmptyShell Empty);

	virtual child_iterator child_begin();
	virtual child_iterator child_end();

	static bool classof(const std::weak_ptr<Stmt>T);
	static bool classof(const std::weak_ptr<DeclRefExpr>);

	std::weak_ptr<NamedDecl>getDecl();
	void setDecl(std::shared_ptr<NamedDecl>NewD);

	SourceLocation getLocation() const;
	void setLocation(SourceLocation L);

    virtual int EvaluateAsInt() const;
protected:
    DeclRefExpr(StmtClass SC, std::shared_ptr<NamedDecl> d,
                std::shared_ptr<QualType> t, SourceLocation l);
    DeclRefExpr(StmtClass SC, std::shared_ptr<NamedDecl> d,
                std::shared_ptr<QualType> t, SourceLocation l,
                bool TD, bool VD);
private:
    std::shared_ptr<NamedDecl> D;
    SourceLocation Loc;
};

/// PredefinedExpr - [C99 6.4.2.2] - A predefined identifier such as __func__.
class PredefinedExpr : public Expr
{
public:
    enum IdentType {
        Func,
        Function,
        PrettyFunction
    };
    PredefinedExpr(SourceLocation l, std::shared_ptr<QualType> type, IdentType IT);
    explicit PredefinedExpr(EmptyShell Empty);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<PredefinedExpr>);

    IdentType getIdentType() const;
    void setIdentType(IdentType IT);

    SourceLocation getLocation() const;
    void setLocation(SourceLocation L);

    virtual int EvaluateAsInt() const;
private:
    IdentType Type;
    SourceLocation Loc;
};

class IntegerLiteral : public Expr
{
public:
    IntegerLiteral(const int &V, std::shared_ptr<QualType> type, SourceLocation l);
    explicit IntegerLiteral(EmptyShell Empty);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<IntegerLiteral>);

    const int &getValue() const;
    SourceLocation getLocation() const;
    void setValue(const int Val);
    void setLocation(SourceLocation Location);

    virtual int EvaluateAsInt() const;
private:
    int Value;
    SourceLocation Loc;
};

class CharacterLiteral : public Expr
{
public:
    CharacterLiteral(unsigned value, bool iswide,
                     std::shared_ptr<QualType> type, SourceLocation l);
    CharacterLiteral(EmptyShell Empty);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<CharacterLiteral>);

    SourceLocation getLoc() const;
    bool isWide() const;
    unsigned getValue() const;

    void setLocation(SourceLocation Location);
    void setWide(bool W);
    void setValue(unsigned Val);

    virtual int EvaluateAsInt() const;

private:
    unsigned Value;
    bool IsWide;
    SourceLocation Loc;
};

class FloatingLiteral : public Expr
{
public:
    FloatingLiteral(const float &V, bool isexact,
                    std::shared_ptr<QualType> Type, SourceLocation L);
    explicit FloatingLiteral(EmptyShell Empty);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<FloatingLiteral>);

    const float &getValue() const;
    void setValue(const float Val);

    bool isExact() const;
    void setExact(bool E);

    SourceLocation getLocation() const;
    void setLocation(SourceLocation L);

    virtual int EvaluateAsInt() const;
private:
    float Value;
    bool IsExact : 1;
    SourceLocation Loc;
};

/// ImaginaryLiteral - We support imaginary integer and floating point literals,
/// like "1.0i".  We represent these as a wrapper around FloatingLiteral and
/// IntegerLiteral classes.  Instances of this class always have a Complex type
/// whose element type matches the subexpression.
///
class ImaginaryLiteral : public Expr
{
public:
    ImaginaryLiteral(std::shared_ptr<Expr> val, std::shared_ptr<QualType> Ty);
    explicit ImaginaryLiteral(EmptyShell Empty);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<ImaginaryLiteral>);

    std::weak_ptr<Expr>getSubExpr();
    void setSubExpr(std::shared_ptr<Expr>E);

private:
    std::shared_ptr<Stmt> Val;
};

/// StringLiteral - This represents a string literal expression, e.g.
class StringLiteral : public Expr
{
public:
    static std::shared_ptr<StringLiteral> Create(
            const std::string StrData, unsigned ByteLength, bool Wide,
            std::shared_ptr<QualType> Ty, const SourceLocation Loc, unsigned NumStrs);
    static std::shared_ptr<StringLiteral> Create(
            const std::string StrData, unsigned ByteLength, bool Wide,
            std::shared_ptr<QualType> Ty, SourceLocation Loc);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<StringLiteral>);

    const std::string getStrData() const { return StrData; }
    unsigned getByteLength() const { return ByteLength; }

    void setStrData(const char *Str, unsigned Len);

    bool isWide() const { return IsWide; }
    void setWide(bool W) { IsWide = W; }
    unsigned getNumConcatenated() const { return NumConcatenated; }

    SourceLocation getTokloc() const { return TokLocs; }

    virtual int EvaluateAsInt() const;
private:
    StringLiteral(std::shared_ptr<QualType> Ty);

    std::string StrData;
    unsigned ByteLength;
    bool IsWide;
    unsigned NumConcatenated;
    SourceLocation TokLocs;
};

class ParenExpr : public Expr
{
public:
    ParenExpr(SourceLocation l, SourceLocation r, std::shared_ptr<Expr> val);
    explicit ParenExpr(EmptyShell Empty);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<ParenExpr>);

    std::weak_ptr<Expr>getSubExpr();
    void setSubExpr(std::shared_ptr<Expr>E);
    SourceLocation getLParen() const;
    void setLParen(SourceLocation Loc);

    SourceLocation getRParen() const;
    void setRParen(SourceLocation Loc);

    virtual int EvaluateAsInt() const;
private:
    std::shared_ptr<Stmt> Val;
    SourceLocation L;
    SourceLocation R;
};

class UnaryOperator : public Expr
{
public:
    enum Opcode {
        PostInc, PostDec, // [C99 6.5.2.4] Postfix increment and decrement operators
        PreInc, PreDec,   // [C99 6.5.3.1] Prefix increment and decrement operators.
        AddrOf, Deref,    // [C99 6.5.3.2] Address and indirection operators.
        Plus, Minus,      // [C99 6.5.3.3] Unary arithmetic operators.
        Not, LNot,        // [C99 6.5.3.3] Unary arithmetic operators.
        Real, Imag,       // "__real expr"/"__imag expr" Extension.
        Extension,        // __extension__ marker.
        OffsetOf          // __builtin_offsetof
    };

    UnaryOperator(std::shared_ptr<Expr> input, Opcode opc,
                  std::shared_ptr<QualType> type, SourceLocation l);
    explicit UnaryOperator(EmptyShell Empty);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<UnaryOperator>);

    Opcode getOpcode() const;
    void setOpcode(Opcode O);

    std::weak_ptr<Expr>getSubExpr() const;
    void setSubExpr(std::shared_ptr<Expr>E);

    SourceLocation getOperatorLoc() const;
    void setOperatorLoc(SourceLocation L);

    virtual SourceLocation getExprLoc() const;

    virtual int EvaluateAsInt() const;
private:
    std::shared_ptr<Stmt> Val;
    Opcode Opc;
    SourceLocation Loc;
};

/// SizeOfAlignOfExpr - [C99 6.5.3.4] - This is for sizeof/alignof, both of
/// types and expressions.
class SizeOfAlignOfExpr : public Expr
{
public:
    SizeOfAlignOfExpr(bool issizeof, std::shared_ptr<QualType> T,
                      std::shared_ptr<QualType> resultType, SourceLocation op,
                      SourceLocation rp);

    SizeOfAlignOfExpr(bool issizeof, std::shared_ptr<Expr> E,
                      std::shared_ptr<QualType> resultType, SourceLocation op,
                      SourceLocation rp);

    explicit SizeOfAlignOfExpr(EmptyShell Empty);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<SizeOfAlignOfExpr>);

    bool isSizeOf() const;
    void setSizeof(bool S);
    bool isArgumentType() const;

    SourceLocation getOperatorLoc() const;
    void setOperatorLoc(SourceLocation L);

    std::weak_ptr<QualType> getArgumentType() const;
    std::weak_ptr<Expr> getArgumentExpr();
    const std::weak_ptr<Expr> getArgumentExpr() const;

    std::weak_ptr<QualType> getTypeOfArgument() const;
    void setArgument(std::shared_ptr<Expr> E);
    void setArgument(std::shared_ptr<QualType> T);

    SourceLocation getRParenLoc() const;
    void setRParenLoc(SourceLocation L);
    virtual int EvaluateAsInt() const{}

private:
    bool isSizeof : 1;  // true if sizeof, false if alignof.
    bool isType : 1;    // true if operand is a type, false if an expression
    std::shared_ptr<void> Ty;
    std::shared_ptr<Stmt> Ex;
    SourceLocation OpLoc, RParenLoc;
};

//===----------------------------------------------------------------------===//
// Postfix Operators.
//===----------------------------------------------------------------------===//

/// ArraySubscriptExpr - [C99 6.5.2.1] Array Subscripting.
class ArraySubscriptExpr : public Expr
{
public:
    ArraySubscriptExpr(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs,
                       std::shared_ptr<QualType> t, SourceLocation rbracketloc);
    explicit ArraySubscriptExpr(EmptyShell Shell);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<ArraySubscriptExpr>);

    std::weak_ptr<Expr>getLHS();
    const std::weak_ptr<Expr>getLHS() const;
    void setLHS(std::shared_ptr<Expr>E);

    std::weak_ptr<Expr>getRHS();
    const std::weak_ptr<Expr>getRHS() const;
    void setRHS(std::shared_ptr<Expr>E);

    SourceLocation getRBracketLoc() const;
    void setRBracketLoc(SourceLocation L);

    virtual int EvaluateAsInt() const;
private:
    enum { LHS, RHS, END_EXPR=2 };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation RBracketLoc;
};

/// CallExpr - Represents a function call (C99 6.5.2.2, C++ [expr.call]).
/// CallExpr itself represents a normal function call, e.g., "f(x, 2)",
/// while its subclasses may represent alternative syntax that (semantically)
/// results in a function call.
class CallExpr : public Expr
{
public:
    CallExpr(std::shared_ptr<Expr> fn, std::list<std::shared_ptr<Expr>> args,
             unsigned numargs, std::shared_ptr<QualType> t, SourceLocation rparenloc);
    CallExpr(StmtClass SC, EmptyShell Empty);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<CallExpr>);

    const std::weak_ptr<Expr>getCallee() const;
    std::weak_ptr<Expr>getCallee();
    void setCallee(std::shared_ptr<Expr>F);

    unsigned getNumArgs() const;
    void setNumArgs(unsigned NumArgs);
    SourceLocation getRParenLoc() const;
    void setRParenLoc(SourceLocation L);

    virtual int EvaluateAsInt() const;
protected:
    CallExpr(StmtClass SC, std::shared_ptr<Expr> fn,
             std::list<std::shared_ptr<Expr>> args, unsigned numargs,
             std::shared_ptr<QualType> t, SourceLocation rparenloc);
private:
    enum { FN=0, ARGS_START=1 };
    std::list<std::shared_ptr<Stmt>> SubExprs;
    unsigned NumArgs;
    SourceLocation RParenLoc;
};

/// MemberExpr - [C99 6.5.2.3] Structure and Union Members.  X->F and X.F.
///
class MemberExpr : public Expr
{
public:
    MemberExpr(std::shared_ptr<Expr> base, bool isarrow,
               std::shared_ptr<NamedDecl> memberdecl,
               SourceLocation l, std::shared_ptr<QualType> ty);
    explicit MemberExpr(EmptyShell Empty);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<MemberExpr>);

    void setBase(std::shared_ptr<Expr>E);
    std::weak_ptr<Expr>getBase() const;

    std::weak_ptr<NamedDecl>getMemberDecl() const;
    void setMemberDecl(std::shared_ptr<NamedDecl>D);

    bool isArrow() const;
    void setArrow(bool A);

    SourceLocation getMemberLoc() const;
    void setMemberLoc(SourceLocation L);
    virtual SourceLocation getExprLoc() const;
    virtual int EvaluateAsInt() const{}
private:
    std::shared_ptr<Stmt> Base;
    std::shared_ptr<NamedDecl> MemberDecl;
    SourceLocation MemberLoc;
    bool IsArrow;
};

/// CompoundLiteralExpr - [C99 6.5.2.5]
///
class CompoundLiteralExpr : public Expr
{
public:
    CompoundLiteralExpr(SourceLocation lparenloc, std::shared_ptr<QualType> ty,
                        std::shared_ptr<Expr> init, bool fileScope);
    explicit CompoundLiteralExpr(EmptyShell Empty);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<CompoundLiteralExpr>);

    const std::weak_ptr<Expr>getInitializer() const;
    std::weak_ptr<Expr>getInitializer();
    void setInitializer(std::shared_ptr<Expr>E);

    bool isFileScope() const;
    void setFileScope(bool FS);

    SourceLocation getLParenLoc() const;
    void setLParenLoc(SourceLocation L);
private:
    std::shared_ptr<Stmt> Init;
    bool FileScope;
    SourceLocation LParenLoc;
};

/// CastExpr - Base class for type casts, including both implicit
/// casts (ImplicitCastExpr) and explicit casts that have some
/// representation in the source code (ExplicitCastExpr's derived
/// classes).
class CastExpr : public Expr
{
public:
    enum CastKind {
        CK_Unknown,
        CK_BitCast,
        CK_NoOp,
        CK_DerivedToBase,
        CK_Dynamic,
        CK_ToUnion,
        CK_ArrayToPointerDecay
    };
    struct CastInfo {
        const CastKind Kind;
        CastInfo(CastKind Kind)
            : Kind(Kind) { }
    };
    virtual child_iterator child_begin();
    virtual child_iterator child_end();

	static bool classof(const std::weak_ptr<Stmt>T);
	static bool classof(const std::weak_ptr<CastExpr>);

	CastKind getCastKind() const;
	void setCastKind(CastKind K);

	std::weak_ptr<Expr>getSubExpr();
	void setSubExpr(std::shared_ptr<Expr>E);

protected:
    CastExpr(StmtClass SC, std::shared_ptr<QualType> ty, const CastInfo &info, std::shared_ptr<Expr>op) ;
    CastExpr(StmtClass SC, EmptyShell Empty);

private:
    CastKind Kind;
    std::shared_ptr<Stmt> Op;
};

class ImplicitCastExpr : public CastExpr
{
public:
    ImplicitCastExpr(std::shared_ptr<QualType> ty, const CastInfo &info,
                     std::shared_ptr<Expr>op, bool Lvalue);
    explicit ImplicitCastExpr(EmptyShell Shell);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<ImplicitCastExpr>);

    bool isLvalueCast() const;
    void setLvalueCast(bool Lvalue);
private:
    bool LvalueCast;
};

/// ExplicitCastExpr - An explicit cast written in the source
/// code.
///
class ExplicitCastExpr : public CastExpr
{
protected:
    ExplicitCastExpr(StmtClass SC, std::shared_ptr<QualType> exprTy, const CastInfo &info,
                     std::shared_ptr<Expr>op, std::shared_ptr<QualType> writtenTy);
    ExplicitCastExpr(StmtClass SC, EmptyShell Shell);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::shared_ptr<ExplicitCastExpr>) { return true; }

    std::weak_ptr<QualType> getTypeAsWritten() const;
    void setTypeAsWritten(std::shared_ptr<QualType> T);
private:
    std::shared_ptr<QualType> TypeAsWritten;
};

/// CStyleCastExpr - An explicit cast in C (C99 6.5.4) or a C-style
/// cast in C++ (C++ [expr.cast]), which uses the syntax
/// (Type)expr. For example: @c (int)f.
class CStyleCastExpr : public ExplicitCastExpr
{
public:
    CStyleCastExpr(std::shared_ptr<QualType> exprTy, CastKind kind,
                   std::shared_ptr<Expr> op, std::shared_ptr<QualType> writtenTy,
                   SourceLocation l, SourceLocation r);
    explicit CStyleCastExpr(EmptyShell Shell);

    static bool classof(const std::weak_ptr<Stmt>T);
    static bool classof(const std::weak_ptr<CStyleCastExpr>);

    SourceLocation getLParenLoc() const;
    void setLParenLoc(SourceLocation L);

    SourceLocation getRParenLoc() const;
    void setRParenLoc(SourceLocation L);
    virtual int EvaluateAsInt() const{}
private:
    SourceLocation LPLoc;
    SourceLocation RPLoc;
};

/// \brief A builtin binary operation expression such as "x + y" or "x <= y".
///
class BinaryOperator : public Expr
{
public:
    enum Opcode {
        // Operators listed in order of precedence.
        PtrMemD, PtrMemI, // [C++ 5.5] Pointer-to-member operators.
        Mul, Div, Rem,    // [C99 6.5.5] Multiplicative operators.
        Add, Sub,         // [C99 6.5.6] Additive operators.
        Shl, Shr,         // [C99 6.5.7] Bitwise shift operators.
        LT, GT, LE, GE,   // [C99 6.5.8] Relational operators.
        EQ, NE,           // [C99 6.5.9] Equality operators.
        And,              // [C99 6.5.10] Bitwise AND operator.
        Xor,              // [C99 6.5.11] Bitwise XOR operator.
        Or,               // [C99 6.5.12] Bitwise OR operator.
        LAnd,             // [C99 6.5.13] Logical AND operator.
        LOr,              // [C99 6.5.14] Logical OR operator.
        Assign, MulAssign,// [C99 6.5.16] Assignment operators.
        DivAssign, RemAssign,
        AddAssign, SubAssign,
        ShlAssign, ShrAssign,
        AndAssign, XorAssign,
        OrAssign,
        Comma             // [C99 6.5.17] Comma operator.
    };
    BinaryOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs,
                   Opcode opc, std::shared_ptr<QualType> ResTy, SourceLocation opLoc);
    explicit BinaryOperator(EmptyShell Empty);

    SourceLocation getOperatorLoc() const;
    void setOperatorLoc(SourceLocation L);

    Opcode getOpcode() const;
    void setOpcode(Opcode O);

    std::weak_ptr<Expr> getLHS() const;
    void setLHS(std::shared_ptr<Expr> E);
    std::weak_ptr<Expr> getRHS() const;
    void setRHS(std::shared_ptr<Expr> E);

    virtual int EvaluateAsInt() const;

    static bool classof(const std::weak_ptr<Stmt> S);
    static bool classof(const std::weak_ptr<BinaryOperator>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
protected:
    BinaryOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, Opcode opc,
                   std::shared_ptr<QualType> ResTy, SourceLocation oploc, bool dead);

    BinaryOperator(StmtClass SC, EmptyShell Empty);
private:
    enum { LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    Opcode Opc;
    SourceLocation OpLoc;
};

/// CompoundAssignOperator - For compound assignments (e.g. +=), we keep
/// track of the type the operation is performed in.
class CompoundAssignOperator : public BinaryOperator
{
public:
    CompoundAssignOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, Opcode opc,
                           std::shared_ptr<QualType> ResType, std::shared_ptr<QualType> CompLHSType,
                           std::shared_ptr<QualType> CompResultType,
                           SourceLocation OpLoc);
    explicit CompoundAssignOperator(EmptyShell Empty);

    std::weak_ptr<QualType> getComputationLHSType() const;
    void setComputationLHSType(std::shared_ptr<QualType> T);

    std::weak_ptr<QualType> getComputationResultType() const;
    void setComputationResultType(std::shared_ptr<QualType> T);

    static bool classof(const std::weak_ptr<CompoundAssignOperator>);
    static bool classof(const std::weak_ptr<Stmt> S);
private:
    std::shared_ptr<QualType> ComputationLHSType;
    std::shared_ptr<QualType> ComputationResultType;
};

/// ConditionalOperator - The ?: operator.  Note that LHS may be null when the
/// GNU "missing LHS" extension is in use.
///
class ConditionalOperator : public Expr
{
public:
    ConditionalOperator(std::shared_ptr<Expr> cond, std::shared_ptr<Expr> lhs,
                        std::shared_ptr<Expr> rhs, std::shared_ptr<QualType> t);
    explicit ConditionalOperator(EmptyShell Empty);

    std::weak_ptr<Expr> getCond() const;
    void setCond(std::shared_ptr<Expr> E);

    std::weak_ptr<Expr> getTrueExpr() const;
    std::weak_ptr<Expr> getFalseExpr() const;

    std::weak_ptr<Expr> getLHS() const;
    void setLHS(std::shared_ptr<Expr> E);

    std::weak_ptr<Expr> getRHS() const;
    void setRHS(std::shared_ptr<Expr> E);

    virtual int EvaluateAsInt() const;

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<ConditionalOperator>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    enum { COND, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

/// AddrLabelExpr - The GNU address of label extension, representing &&label.
class AddrLabelExpr : public Expr
{
public:
    AddrLabelExpr(SourceLocation AALoc, SourceLocation LLoc,
                  std::shared_ptr<LabelStmt> L, std::shared_ptr<QualType> t);
    explicit AddrLabelExpr(EmptyShell Empty);

    SourceLocation getAmpAmpLoc() const;
    void setAmpAmpLoc(SourceLocation L);
    SourceLocation getLabelLoc() const;
    void setLabelLoc(SourceLocation L);

    std::weak_ptr<LabelStmt> getLabel() const;
    void setLabel(std::shared_ptr<LabelStmt> S);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<AddrLabelExpr>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    SourceLocation AmpAmpLoc;
    SourceLocation LabelLoc;
    std::shared_ptr<LabelStmt> Label;
};

/// StmtExpr - This is the GNU Statement Expression extension: ({int X=4; X;}).
class StmtExpr : public Expr
{
public:
    StmtExpr(std::shared_ptr<CompoundStmt> substmt, std::shared_ptr<QualType> T,
             SourceLocation lp, SourceLocation rp);
    explicit StmtExpr(EmptyShell Empty);

    std::weak_ptr<CompoundStmt> getSubStmt();
    const std::weak_ptr<CompoundStmt> getSubStmt() const;
    void setSubStmt(std::shared_ptr<CompoundStmt> S);

    SourceLocation getLParenLoc() const;
    void setLParenLoc(SourceLocation L);
    SourceLocation getRParenLoc() const;
    void setRParenLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<StmtExpr>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    std::shared_ptr<Stmt> SubStmt;
    SourceLocation LParenLoc;
    SourceLocation RParenLoc;
};

/// TypesCompatibleExpr - GNU builtin-in function
class TypesCompatibleExpr : public Expr
{
public:
    TypesCompatibleExpr(std::shared_ptr<QualType> ReturnType, SourceLocation BLoc,
                        std::shared_ptr<QualType> t1, std::shared_ptr<QualType> t2, SourceLocation RP);
    explicit TypesCompatibleExpr(EmptyShell Empty);

    std::weak_ptr<QualType> getArgType1() const;
    void setArgType1(std::shared_ptr<QualType> T);
    std::weak_ptr<QualType> getArgType2() const;
    void setArgType2(std::shared_ptr<QualType> T);

    SourceLocation getBuiltinLoc() const;
    void setBuiltinLoc(SourceLocation L);

    SourceLocation getRParenLoc() const;
    void setRParenLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<TypesCompatibleExpr>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    std::shared_ptr<QualType> Type1;
    std::shared_ptr<QualType> Type2;
    SourceLocation BuiltinLoc;
    SourceLocation RParenLoc;
};

/// ShuffleVectorExpr - clang-specific builtin-in function
class ShuffleVectorExpr : public Expr
{
public:
    ShuffleVectorExpr(std::list<std::shared_ptr<Expr>> args, unsigned nexpr,
                      std::shared_ptr<QualType> Type, SourceLocation BLoc,
                      SourceLocation RP);
    explicit ShuffleVectorExpr(EmptyShell Empty);

    unsigned getNumSubExprs() const { return NumExprs; }

    std::weak_ptr<Expr> getExpr(unsigned Index);
    const std::weak_ptr<Expr> getExpr(unsigned Index) const;

    SourceLocation getBuiltinLoc() const;
    void setBuiltinLoc(SourceLocation L);

    SourceLocation getRParenLoc() const;
    void setRParenLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<ShuffleVectorExpr>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    SourceLocation BuiltinLoc, RParenLoc;
    std::list<std::shared_ptr<Stmt>> SubExprs;
    unsigned NumExprs;
};

/// ChooseExpr - GNU builtin-in function __builtin_choose_expr.
class ChooseExpr : public Expr
{
public:
    ChooseExpr(SourceLocation BLoc, std::shared_ptr<Expr> cond, std::shared_ptr<Expr> lhs,
               std::shared_ptr<Expr> rhs, std::shared_ptr<QualType> t,
               SourceLocation RP);
    explicit ChooseExpr(EmptyShell Empty);

    std::weak_ptr<Expr> getCond() const;
    void setCond(std::shared_ptr<Expr> E);
    std::weak_ptr<Expr> getLHS() const;
    void setLHS(std::shared_ptr<Expr> E);
    std::weak_ptr<Expr> getRHS() const;
    void setRHS(std::shared_ptr<Expr> E);

    SourceLocation getBuiltinLoc() const;
    void setBuiltinLoc(SourceLocation L);

    SourceLocation getRParenLoc() const;
    void setRParenLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<ChooseExpr>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    enum { COND, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation BuiltinLoc;
    SourceLocation RParenLoc;
};

/// GNUNullExpr - Implements the GNU __null extension, which is a name
/// for a null pointer constant that has integral type (e.g., int or
/// long) and is the same size and alignment as a pointer.
class GNUNullExpr : public Expr
{
public:
    GNUNullExpr(std::shared_ptr<QualType> Ty, SourceLocation Loc);
    explicit GNUNullExpr(EmptyShell Empty);

    SourceLocation getTokenLocation() const;
    void setTokenLocation(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<GNUNullExpr>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    SourceLocation TokenLoc;
};

/// VAArgExpr, used for the builtin function __builtin_va_start.
class VAArgExpr : public Expr
{
public:
    VAArgExpr(SourceLocation BLoc, std::shared_ptr<Expr> e,
              std::shared_ptr<QualType> t, SourceLocation RPLoc);
    explicit VAArgExpr(EmptyShell Empty);

    const std::weak_ptr<Expr> getSubExpr() const;
    std::weak_ptr<Expr> getSubExpr();
    void setSubExpr(std::shared_ptr<Expr> E);

    SourceLocation getBuiltinLoc() const;
    void setBuiltinLoc(SourceLocation L);

    SourceLocation getRParenLoc() const;
    void setRParenLoc(SourceLocation L);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<VAArgExpr>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    std::shared_ptr<Stmt> Val;
    SourceLocation BuiltinLoc;
    SourceLocation RParenLoc;
};

/// struct foo x = { 1, { 2, 3 } };
class InitListExpr : public Expr
{
public:
    InitListExpr(SourceLocation lbraceloc, std::list<std::shared_ptr<Expr>> initexprs,
                 unsigned numinits, SourceLocation rbraceloc);
    explicit InitListExpr(EmptyShell Empty);

    SourceLocation getLBraceLoc() const;
    void setLBraceLoc(SourceLocation Loc);
    SourceLocation getRBraceLoc() const;
    void setRBraceLoc(SourceLocation Loc);

    std::weak_ptr<InitListExpr> getSyntacticForm() const;
    void setSyntacticForm(std::shared_ptr<InitListExpr> Init);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<InitListExpr>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    std::list<std::shared_ptr<Stmt>> InitExprs;
    SourceLocation LBraceLoc;
    SourceLocation RBraceLoc;

    std::shared_ptr<InitListExpr> SyntacticForm;

    //FieldDecl *UnionFieldInit;

    bool HadArrayRangeDesignator;
};

/// struct point {
///   double x;
///   double y;
/// };
/// struct point ptarray[10] = { [2].y = 1.0, [2].x = 2.0, [0].x = 1.0 };
class DesignatedInitExpr : public Expr
{
public:
    class Designator;
private:
    SourceLocation EqualOrColonLoc;
    bool GNUSyntax : 1;
    unsigned NumDesignators : 15;
    std::shared_ptr<Designator> Designators;
    unsigned NumSubExprs : 16;
    static bool classof(const std::shared_ptr<Stmt> T) {
        return T->getStmtClass() == StmtClass::DesignatedInitExprClass;
    }
    static bool classof(const std::shared_ptr<DesignatedInitExpr>) { return true; }


    //    DesignatedInitExpr(QualType Ty, unsigned NumDesignators,
    //                       const Designator *Designators,
    //                       SourceLocation EqualOrColonLoc, bool GNUSyntax,
    //                       Expr **IndexExprs, unsigned NumIndexExprs,
    //                       Expr *Init);

    //    explicit DesignatedInitExpr(unsigned NumSubExprs)
    //        : Expr(DesignatedInitExprClass, EmptyShell()),
    //          NumDesignators(0), Designators(0), NumSubExprs(NumSubExprs) { }
    //    Stmt::child_iterator DesignatedInitExpr::child_begin() {
    //      char* Ptr = static_cast<char*>(static_cast<void *>(this));
    //      Ptr += sizeof(DesignatedInitExpr);
    //      return reinterpret_cast<Stmt**>(reinterpret_cast<void**>(Ptr));
    //    }
    //    Stmt::child_iterator DesignatedInitExpr::child_end() {
    //      return child_iterator(&*child_begin() + NumSubExprs);
    //    }

};

/// \brief Represents an implicitly-generated value initialization of
/// an object of a given type.
class ImplicitValueInitExpr : public Expr
{
public:
    explicit ImplicitValueInitExpr(std::shared_ptr<QualType> ty);
    explicit ImplicitValueInitExpr(EmptyShell Empty);

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<ImplicitValueInitExpr>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
};

class ParenListExpr : public Expr
{
public:
    ParenListExpr(SourceLocation lparenloc, std::list<std::shared_ptr<Expr>> exprs,
                  unsigned numexprs, SourceLocation rparenloc);

    const std::weak_ptr<Expr> getExpr(unsigned Init) const;

    std::weak_ptr<Expr> getExpr(unsigned Init);

    SourceLocation getLParenLoc() const;
    SourceLocation getRParenLoc() const;

    static bool classof(const std::weak_ptr<Stmt> T);
    static bool classof(const std::weak_ptr<ParenListExpr>);

    virtual child_iterator child_begin();
    virtual child_iterator child_end();
private:
    std::list<std::shared_ptr<Stmt>> Exprs;
    unsigned NumExprs;
    SourceLocation LParenLoc;
    SourceLocation RParenLoc;
};

#endif // AST_EXPR_H
