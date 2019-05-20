//Classes for representing expressions
//This file defines the Expr interface and subclasses
#ifndef AST_EXPR_H
#define AST_EXPR_H

#include "AST/Type.h"
#include "AST/Stmt.h"

class Expr;
class Stmt;

class DeclRefExpr;
class PredefinedExpr;
class IntegerLiteral;
class CharacterLiteral;
class FloatingLiteral;
class ImaginaryLiteral;
class StringLiteral;
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

//This represents one expression.  Note that Expr's are subclasses of  Stmt.  This allows an expression to be transparently used any place a Stmt
class Expr : public Stmt
{
public:
    QualType getType() const;
    void setType(const QualType &value);

    bool isTypeDependent() const;
    void setTypeDependent(bool value);

    bool isValueDependent() const;
    void setValueDependent(bool value);

protected:
    //Eventually, this constructor should go away and we should require every subclass to provide type/value-dependence information.
    Expr(StmtClass SC, QualType T);
    Expr(StmtClass SC, QualType T, bool TD, bool VD);

    //Construct an empty expression.
    explicit Expr(StmtClass SC, EmptyShell);
    //Whether this expression is type-dependent
    bool TypeDependent : 1;
    //Whether this expression is value-dependent
    bool ValueDependent : 1;
private:
    QualType TR;
};
//A reference to a declared variable, function,enum, etc.
class DeclRefExpr : public Expr
{
public:
    DeclRefExpr(QualType t);
    DeclRefExpr(QualType t, bool TD, bool VD);
    //Construct an empty declaration reference expression.
    explicit DeclRefExpr(EmptyShell Empty);
protected:
    DeclRefExpr(StmtClass SC, QualType t);
    DeclRefExpr(StmtClass SC, QualType t, bool TD, bool VD);
private:
    enum { COND, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

//A predefined identifier such as __func__.
class PredefinedExpr : public Expr
{
public:
    enum IdentType {
        Func,
        Function,
        PrettyFunction
    };
    PredefinedExpr(QualType type, IdentType IT);
    //Construct an empty predefined expression.
    explicit PredefinedExpr(EmptyShell Empty);
private:
    IdentType Type;
};

class IntegerLiteral : public Expr
{
public:
    IntegerLiteral(const int &V, QualType type);
    //Construct an empty integer literal.
    explicit IntegerLiteral(EmptyShell Empty);
private:
    int Value;
};

class CharacterLiteral : public Expr
{
public:
    // type should be IntTy
    CharacterLiteral(unsigned value, bool iswide, QualType type);
    //Construct an empty character literal.
    CharacterLiteral(EmptyShell Empty);
private:
    unsigned Value;
    bool IsWide;
};

class FloatingLiteral : public Expr
{
public:
    FloatingLiteral(const float &V, bool isexact, QualType Type);
    //Construct an empty floating-point literal.
    explicit FloatingLiteral(EmptyShell Empty);
private:
    float Value;
    bool IsExact : 1;
};

//We support imaginary integer and floating point literals,like "1.0i".
//We represent these as a wrapper around FloatingLiteral and Instances of this class always have a Complex type
// whose element type matches the subexpression.
/// IntegerLiteral classes.
class ImaginaryLiteral : public Expr
{
public:
    ImaginaryLiteral(std::shared_ptr<Expr> val, QualType Ty);
    // Build an empty imaginary literal.
    explicit ImaginaryLiteral(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Val;
};

//This represents a string literal expression
class StringLiteral : public Expr
{
public:
    static std::shared_ptr<StringLiteral> Create(const char *StrData,
                                                 unsigned ByteLength,
                                                 bool Wide,
                                                 QualType Ty,
                                                 unsigned NumStrs);
    static std::shared_ptr<StringLiteral> Create(const char *StrData,
                                                 unsigned ByteLength,
                                                 bool Wide,
                                                 QualType Ty);
private:
    StringLiteral(QualType Ty);

    const char *StrData;
    unsigned ByteLength;
    bool IsWide;
    unsigned NumConcatenated;
};

//This represents a parethesized expression.
//his AST node is only formed if full location information is requested.
class ParenExpr : public Expr
{
public:
    ParenExpr(std::shared_ptr<Expr> val);
     //Construct an empty parenthesized expression.
    explicit ParenExpr(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Val;
};

//This represents the unary-expression's (except sizeof and alignof), the postinc/postdec operators from postfix-expression, and various extensions.
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
    UnaryOperator(std::shared_ptr<Expr> input, Opcode opc, QualType type);
    // Build an empty unary operator.
    explicit UnaryOperator(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Val;
    Opcode Opc;
};

//This is for sizeof/alignof, both of types and expressions.
class SizeOfAlignOfExpr : public Expr
{
public:
    SizeOfAlignOfExpr(bool issizeof, QualType T, QualType resultType);
    SizeOfAlignOfExpr(bool issizeof, std::shared_ptr<Expr> E, QualType resultType);
    explicit SizeOfAlignOfExpr(EmptyShell Empty);
protected:
    bool isSizeof : 1;  // true if sizeof, false if alignof.
    bool isType : 1;    // true if operand is a type, false if an expression
private:
    std::shared_ptr<void> Ty;
    std::shared_ptr<Stmt> Ex;
};

//Array Subscripting
class ArraySubscriptExpr : public Expr
{
public:
    ArraySubscriptExpr(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, QualType t);
    //Create an empty array subscript expression.
    explicit ArraySubscriptExpr(EmptyShell Shell);
private:
    enum { LHS, RHS, END_EXPR=2 };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

//Represents a function call,CallExpr itself represents a normal function call
//while its subclasses may represent alternative syntax that (semantically)results in a function call
//a subclass for overloaded operator calls that use operator syntax
class CallExpr : public Expr
{
public:
    CallExpr(std::shared_ptr<Expr> fn, std::shared_ptr<Expr> *args, unsigned numargs, QualType t);
    CallExpr(StmtClass SC, EmptyShell Empty);
protected:
    CallExpr(StmtClass SC, std::shared_ptr<Expr> fn, std::shared_ptr<Expr> *args, unsigned numargs, QualType t);
private:
    enum { FN=0, ARGS_START=1 };
    //std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    unsigned NumArgs;
};

//Structure and Union Members.
class MemberExpr : public Expr
{
public:
    MemberExpr(std::shared_ptr<Expr> base, bool isarrow, QualType ty);
    explicit MemberExpr(EmptyShell Empty);
private:
    //the expression for the base pointer or structure references.
    std::shared_ptr<Stmt> Base;
    //True if this is "X->F", false if this is "X.F"
    bool IsArrow;
};

class CompoundLiteralExpr : public Expr
{
public:
    CompoundLiteralExpr(QualType ty, std::shared_ptr<Expr> init, bool fileScope);
    explicit CompoundLiteralExpr(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Init;
    bool FileScope;
};

//Base class for type casts, including both implicit casts (ImplicitCastExpr) and explicit casts that have some representation in the source code (ExplicitCastExpr's derived classes).
class CastExpr : public Expr
{
public:
    //the kind of cast this represents.
  enum CastKind {

    CK_Unknown,             //Unknown cast kind
    CK_BitCast,             //Used for reinterpret_cast.
    CK_NoOp,                //Used for const_cast.
    CK_DerivedToBase,       //Derived to base class casts.
    CK_Dynamic,             //Dynamic cast.
    CK_ToUnion,             //Cast to union
    CK_ArrayToPointerDecay  //Array to pointer decay
  };

  struct CastInfo {
    const CastKind Kind;

    CastInfo(CastKind Kind)
      : Kind(Kind) { }
  };
private:
  CastKind Kind;
  std::shared_ptr<Stmt> Op;
protected:
  CastExpr(StmtClass SC, QualType ty, const CastInfo &info, std::shared_ptr<Expr>op) ;
  //Construct an empty cast.
  CastExpr(StmtClass SC, EmptyShell Empty);

};

//Allows us to explicitly represent implicit type conversions, which have no direct representation in the original source code.
class ImplicitCastExpr : public CastExpr
{
    //Whether this cast produces an lvalue.
    bool LvalueCast;

  public:
    ImplicitCastExpr(QualType ty, const CastInfo &info, std::shared_ptr<Expr>op, bool Lvalue);
    //Construct an empty implicit cast.
    explicit ImplicitCastExpr(EmptyShell Shell);

};

//An explicit cast written in the source code.
// This class is effectively an abstract class, because it provides the basic representation of an explicitly-written cast without specifying which kind of cast (C cast, functional cast, static cast, etc.) was written;
//specific derived classes represent the particular style of cast and its location information.
class ExplicitCastExpr : public CastExpr
{
    //The type that this expression is casting to, as written in the source code.
    QualType TypeAsWritten;

  protected:
    ExplicitCastExpr(StmtClass SC, QualType exprTy, const CastInfo &info,
                  std::shared_ptr<Expr>op, QualType writtenTy)
;
    //Construct an empty explicit cast.
    ExplicitCastExpr(StmtClass SC, EmptyShell Shell);

};
//An explicit cast in C or a C-style cast in C++ , which uses the syntax (Type)expr.
class CStyleCastExpr : public ExplicitCastExpr
{
  public:
    CStyleCastExpr(QualType exprTy, CastKind kind, std::shared_ptr<Expr> op, QualType writtenTy);
    // Construct an empty C-style explicit cast.
    explicit CStyleCastExpr(EmptyShell Shell);

};

//A builtin binary operation expression
//This expression node kind describes a builtin binary operation
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
    BinaryOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, Opcode opc, QualType ResTy, SourceLocation opLoc);

    //Construct an empty binary operator.
    explicit BinaryOperator(EmptyShell Empty);
protected:
    BinaryOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, Opcode opc, QualType ResTy, SourceLocation oploc, bool dead);

    BinaryOperator(StmtClass SC, EmptyShell Empty);
private:
    enum { LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    Opcode Opc;
    SourceLocation OpLoc;
};

//For compound assignments, we keep track of the type the operation is performed in.
//Due to the semantics of these operators, the operands are promoted, the aritmetic performed, an implicit conversion back to the result type done, then the assignment takes place.
//This captures the intermediate type which the computation is done in.
class CompoundAssignOperator : public BinaryOperator
{
public:
    CompoundAssignOperator(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs, Opcode opc,
                           QualType ResType, QualType CompLHSType,
                           QualType CompResultType,
                           SourceLocation OpLoc);
    //Build an empty compound assignment operator expression.
    explicit CompoundAssignOperator(EmptyShell Empty);
private:
    QualType ComputationLHSType;
    QualType ComputationResultType;
};

//The ?: operator.  Note that LHS may be null when the GNU "missing LHS" extension is in use.
class ConditionalOperator : public Expr
{
public:
    ConditionalOperator(std::shared_ptr<Expr> cond, std::shared_ptr<Expr> lhs,
                        std::shared_ptr<Expr> rhs, QualType t);

    explicit ConditionalOperator(EmptyShell Empty);
private:
    enum { COND, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
};

//The GNU address of label extension, representing &&label.
class AddrLabelExpr : public Expr
{
public:
    AddrLabelExpr(SourceLocation AALoc, SourceLocation LLoc, std::shared_ptr<LabelStmt> L,
                  QualType t);

    explicit AddrLabelExpr(EmptyShell Empty);
private:
    SourceLocation AmpAmpLoc;
    SourceLocation LabelLoc;
    std::shared_ptr<LabelStmt> Label;
};
//This is the GNU Statement Expression extension: ({int X=4; X;}).
//The StmtExpr contains a single CompoundStmt node, which it evaluates and takes the value of the last subexpression.
class StmtExpr : public Expr
{
public:
    StmtExpr(std::shared_ptr<CompoundStmt> substmt, QualType T,
             SourceLocation lp, SourceLocation rp);
    //Build an empty statement expression.
    explicit StmtExpr(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> SubStmt;
    SourceLocation LParenLoc;
    SourceLocation RParenLoc;
};

//GNU builtin-in function __builtin_types_compatible_p.
class TypesCompatibleExpr : public Expr
{
public:
    TypesCompatibleExpr(QualType ReturnType, SourceLocation BLoc,
                        QualType t1, QualType t2, SourceLocation RP);
    explicit TypesCompatibleExpr(EmptyShell Empty);
private:
    QualType Type1;
    QualType Type2;
    SourceLocation BuiltinLoc;
    SourceLocation RParenLoc;
};

//clang-specific builtin-in function __builtin_shufflevector.
class ShuffleVectorExpr : public Expr
{
public:
    ShuffleVectorExpr(std::vector<std::shared_ptr<Expr>> args, unsigned nexpr,
                      QualType Type, SourceLocation BLoc,
                      SourceLocation RP);

    explicit ShuffleVectorExpr(EmptyShell Empty);
private:
    SourceLocation BuiltinLoc, RParenLoc;
    //the list of values passed to the __builtin_shufflevector function. The first two are vectors, and the rest are constant indices.  The number of values in this list is always 2+the number of indices in the vector type.
    std::vector<std::shared_ptr<Stmt>> SubExprs;
    unsigned NumExprs;
};

//GNU builtin-in function __builtin_choose_expr.
//the test expression must be a integer constant expression.
class ChooseExpr : public Expr
{
public:
    ChooseExpr(SourceLocation BLoc, std::shared_ptr<Expr> cond, std::shared_ptr<Expr> lhs,
               std::shared_ptr<Expr> rhs, QualType t,
               SourceLocation RP);
    //Build an empty __builtin_choose_expr.
    explicit ChooseExpr(EmptyShell Empty);
private:
    enum { COND, LHS, RHS, END_EXPR };
    std::array<std::shared_ptr<Stmt>, END_EXPR> SubExprs;
    SourceLocation BuiltinLoc;
    SourceLocation RParenLoc;
};

//Implements the GNU __null extension, which is a name for a null pointer constant that has integral type and is the same size and alignment as a pointer.
class GNUNullExpr : public Expr
{
public:
    GNUNullExpr(QualType Ty, SourceLocation Loc);
    //Build an empty GNU __null expression.
    explicit GNUNullExpr(EmptyShell Empty);
private:
    SourceLocation TokenLoc;
};

// used for the builtin function __builtin_va_start.
class VAArgExpr : public Expr
{
public:
    VAArgExpr(SourceLocation BLoc, std::shared_ptr<Expr> e, QualType t, SourceLocation RPLoc);
    //Create an empty __builtin_va_start expression.
    explicit VAArgExpr(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Val;
    SourceLocation BuiltinLoc;
    SourceLocation RParenLoc;
};

//describes an initializer list, which can be used to initialize objects of different types, including struct/class/union types, arrays, and vectors.
class InitListExpr : public Expr
{
public:
    InitListExpr(SourceLocation lbraceloc,
                 std::vector<std::shared_ptr<Expr>> initexprs, unsigned numinits,
                 SourceLocation rbraceloc);

    explicit InitListExpr(EmptyShell Empty);
private:
    std::vector<std::shared_ptr<Stmt>> InitExprs;
    SourceLocation LBraceLoc;
    SourceLocation RBraceLoc;

    std::shared_ptr<InitListExpr> SyntacticForm;

    //FieldDecl *UnionFieldInit;

    //Whether this initializer list originally had a GNU array-range designator in it. This is a temporary marker used by CodeGen.
    bool HadArrayRangeDesignator;
};

//A designated initializer expression contains one or more designators (which can be field designators, array designators, or GNU array-range designators) followed by an expression that initializes the field or element(s) that the designators refer to.
class DesignatedInitExpr : public Expr
{
public:
    // Forward declaration of the Designator class.
    class Designator;

private:

    // The location of the '=' or ':' prior to the actual initializer expression.
    SourceLocation EqualOrColonLoc;

    // Whether this designated initializer used the GNU deprecated syntax rather than the C99 '=' syntax.
    bool GNUSyntax : 1;

    // The number of designators in this initializer expression.
    unsigned NumDesignators : 15;

    //The designators in this designated initialization expression.
    std::shared_ptr<Designator> Designators;

    //The number of subexpressions of this initializer expression, which contains both the initializer and any additional expressions used by array and array-range designators.
    unsigned NumSubExprs : 16;


    //    DesignatedInitExpr(QualType Ty, unsigned NumDesignators,
    //                       const Designator *Designators,
    //                       SourceLocation EqualOrColonLoc, bool GNUSyntax,
    //                       Expr **IndexExprs, unsigned NumIndexExprs,
    //                       Expr *Init);

    //    explicit DesignatedInitExpr(unsigned NumSubExprs)
    //        : Expr(DesignatedInitExprClass, EmptyShell()),
    //          NumDesignators(0), Designators(0), NumSubExprs(NumSubExprs) { }

};

//Represents an implicitly-generated value initialization of an object of a given type.
// Implicit value initializations occur within semantic initializer list expressions (InitListExpr) as placeholders for subobject initializations not explicitly specified by the user.

class ImplicitValueInitExpr : public Expr
{
public:
    explicit ImplicitValueInitExpr(QualType ty);

    //Construct an empty implicit value initialization.
    explicit ImplicitValueInitExpr(EmptyShell Empty);
};

class ParenListExpr : public Expr
{
public:
    ParenListExpr(SourceLocation lparenloc, std::vector<std::shared_ptr<Expr>> exprs,
                  unsigned numexprs, SourceLocation rparenloc);
private:
    std::vector<std::shared_ptr<Stmt>> Exprs;
    unsigned NumExprs;
    SourceLocation LParenLoc;
    SourceLocation RParenLoc;
};

//This represents access to specific elements of a vector, and may occur on the left hand side or right hand side.
// Note that the base may have either vector or pointer to vector type, just like a struct field reference.
class ExtVectorElementExpr : public Expr
{
public:
    //    ExtVectorElementExpr(QualType ty, std::shared_ptr<Expr> base, IdentifierInfo &accessor,
    //                           SourceLocation loc)
    //        : Expr(ExtVectorElementExprClass, ty),
    //          Base(base), Accessor(&accessor), AccessorLoc(loc) {}

    //Build an empty vector element expression.
    explicit ExtVectorElementExpr(EmptyShell Empty);
private:
    std::shared_ptr<Stmt> Base;
    //      IdentifierInfo *Accessor;
    SourceLocation AccessorLoc;
};

//Adaptor class for mixing a BlockDecl with expressions.
class BlockExpr : public Expr
{

};

//A reference to a declared variable, function,enum, etc.
class BlockDeclRefExpr : public Expr
{

};

#endif // AST_EXPR_H
