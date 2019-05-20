#ifndef ASTCONTEXT_H
#define ASTCONTEXT_H

#include <queue>
#include <memory>
#include <variant>
#include "AST/AST.hpp"

class ASTContext
{
public:
    template<auto... args>
    void createStmt(Stmt::StmtClass type)
    {
        std::shared_ptr<Stmt> stmt;
        switch(type){
        case Stmt::CompoundStmtClass:
            stmt=createCompoundStmt((args, ...));
            break;
        case Stmt::IfStmtClass:
            stmt=createIfStmt((args, ...));
            break;
        case Stmt::IntegerLiteralClass:
            stmt=createIntegerLiteral((args, ...));
            break;
        case Stmt::CharacterLiteralClass:
            stmt=createIntegerLiteral((args, ...));
            break;
        case Stmt::FloatingLiteralClass:
            stmt=createIntegerLiteral((args, ...));
            break;
        case Stmt::StringLiteralClass:
            stmt=createIntegerLiteral((args, ...));
            break;
        case Stmt::UnaryOperatorClass:
            stmt=createUnaryOperator((args, ...));
            break;
        case Stmt::BinaryOperatorClass:
            stmt=createBinaryOperator((args, ...));
            break;
        case Stmt::CompoundAssignOperatorClass:
            stmt=createCompoundAssignOperator((args, ...));
            break;
        case Stmt::ConditionalOperatorClass:
            stmt=createConditionalOperator((args, ...));
            break;
        case Stmt::StmtExprClass:
            stmt=createStmtExpr((args, ...));
            break;
        default:break;
        }
        queue().push(stmt);
    }

private:
    std::shared_ptr<Stmt> createCompoundStmt(unsigned num, SourceLocation LB, SourceLocation RB);
    std::shared_ptr<Stmt> createIfStmt(SourceLocation IL, SourceLocation EL = SourceLocation());
    std::shared_ptr<Stmt> createIntegerLiteral(int val, QualType type);
    std::shared_ptr<Stmt> createCharacterLiteral(unsigned val, bool iswide, QualType type);
    std::shared_ptr<Stmt> createFloatingLiteral(const float &V, bool isexact, QualType type);
    std::shared_ptr<Stmt> createStringLiteral(const char *StrData,
                                              unsigned ByteLength, bool Wide, QualType Ty,
                                              const SourceLocation Loc, unsigned NumStrs);
    std::shared_ptr<Stmt> createStringLiteral(const char *StrData,
                                              unsigned ByteLength,
                                              bool Wide, QualType Ty, SourceLocation Loc);
    std::shared_ptr<Stmt> createUnaryOperator(UnaryOperator::Opcode opc, QualType type);
    std::shared_ptr<Stmt> createBinaryOperator(BinaryOperator::Opcode opc, QualType ResTy, SourceLocation opLoc);
    std::shared_ptr<Stmt> createCompoundAssignOperator(CompoundAssignOperator::Opcode opc,
                                                       QualType ResType, QualType CompLHSType,
                                                       QualType CompResultType,
                                                       SourceLocation OpLoc);
    std::shared_ptr<Stmt> createConditionalOperator(QualType t);
    std::shared_ptr<Stmt> createStmtExpr(QualType T, SourceLocation lp, SourceLocation rp);

    std::queue<std::shared_ptr<Stmt>> queue();

    template<typename T=Stmt>
    std::shared_ptr<T> pop_back(){
        if(queue().empty())
            return nullptr;
        std::shared_ptr<Stmt> ptr=queue().front();
        queue().pop();
        return std::dynamic_pointer_cast<T>(ptr);
    }
};

#endif // ASTCONTEXT_H
