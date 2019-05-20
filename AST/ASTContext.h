#ifndef ASTCONTEXT_H
#define ASTCONTEXT_H

#include <queue>
#include <memory>
#include <variant>
#include "AST/AST.hpp"

class ASTContext
{
public:
    template<typename... Args>
    void createStmt(Stmt::StmtClass type, Args... args){
        typedef std::variant<
                unsigned,
                long unsigned,
                int,
                double,
                float,
                bool,
                char *,
                SourceLocation,
                QualType,
                UnaryOperator::Opcode,
                BinaryOperator::Opcode
                > VarType;
        std::vector<VarType> value;
        (value.push_back(VarType{args}), ...);
        std::shared_ptr<Stmt> stmt;
        switch (type) {
        case Stmt::CompoundStmtClass:
            stmt=createCompoundStmt(
                        std::get<unsigned>(value[0]),
                    std::get<SourceLocation>(value[1]),
                    std::get<SourceLocation>(value[2]));
            break;
        case Stmt::IfStmtClass:
            stmt=createIfStmt(
                        std::get<SourceLocation>(value[0]),
                    std::get<SourceLocation>(value[1]));
            break;
        case Stmt::IntegerLiteralClass:
            stmt=createIntegerLiteral(
                        std::get<int>(value[0]),
                    std::get<QualType>(value[1]));
            break;
        case Stmt::CharacterLiteralClass:
            stmt=createCharacterLiteral(
                        std::get<unsigned>(value[0]),
                    std::get<bool>(value[1]),
                    std::get<QualType>(value[2]));
            break;
        case Stmt::FloatingLiteralClass:
            stmt=createFloatingLiteral(
                        std::get<float>(value[0]),
                    std::get<bool>(value[1]),
                    std::get<QualType>(value[2]));
            break;
        case Stmt::StringLiteralClass:
            if(value.size()==5)
                stmt=createStringLiteral(
                            std::get<char *>(value[0]),
                        std::get<long unsigned>(value[1]),
                        std::get<bool>(value[2]),
                        std::get<QualType>(value[3]),
                        std::get<SourceLocation>(value[4]));
            else
                stmt=createStringLiteral(
                            std::get<char *>(value[0]),
                        std::get<unsigned>(value[1]),
                        std::get<bool>(value[2]),
                        std::get<QualType>(value[3]),
                        std::get<SourceLocation>(value[4]),
                        std::get<unsigned>(value[5]));
            break;
        case Stmt::UnaryOperatorClass:
            stmt=createUnaryOperator(
                        std::get<UnaryOperator::Opcode>(value[0]),
                    std::get<QualType>(value[1]));
            break;
        case Stmt::BinaryOperatorClass:
            stmt=createBinaryOperator(
                        std::get<BinaryOperator::Opcode>(value[0]),
                    std::get<QualType>(value[1]),
                    std::get<SourceLocation>(value[2]));
            break;
        case Stmt::CompoundAssignOperatorClass:
            stmt=createCompoundAssignOperator(
                        std::get<CompoundAssignOperator::Opcode>(value[0]),
                    std::get<QualType>(value[1]),
                    std::get<QualType>(value[2]),
                    std::get<QualType>(value[3]),
                    std::get<SourceLocation>(value[4]));
            break;
        case Stmt::ConditionalOperatorClass:
            stmt=createConditionalOperator(
                        std::get<QualType>(value[0]));
            break;
        case Stmt::StmtExprClass:
            stmt=createStmtExpr(
                        std::get<QualType>(value[0]),
                    std::get<SourceLocation>(value[1]),
                    std::get<SourceLocation>(value[2]));
            break;
        case Stmt::ExprClass:
        	stmt=createSimpleStmt(type);
        	break;
        default:break;
        }
        queue.push(stmt);
    }

    std::weak_ptr<Stmt> getTop() const;
    void cleanQueue();

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
    std::shared_ptr<Stmt> createSimpleStmt(Stmt::StmtClass type);

    std::queue<std::shared_ptr<Stmt>> queue;

    template<typename T=Stmt>
    std::shared_ptr<T> pop_back(){
        if(queue.empty())
            return nullptr;
        std::shared_ptr<Stmt> ptr=queue.front();
        queue.pop();
        return std::dynamic_pointer_cast<T>(ptr);
    }
};

#endif // ASTCONTEXT_H
