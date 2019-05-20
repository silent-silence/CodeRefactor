#ifndef ASTCONTEXT_H
#define ASTCONTEXT_H

#include <queue>
#include <memory>
#include <variant>
#include "AST/AST.hpp"

class ASTContext
{
public:
    typedef std::variant<
    unsigned,
    int,
    double,
    float,
    bool,
    char *,
    SourceLocation,
    QualType,
    UnaryOperator::Opcode,
    BinaryOperator::Opcode,
    PredefinedExpr::IdentType
    > var_t;
    template<auto type, typename... Args>
    void create(Args... args){
        std::vector<var_t> value;
        (value.push_back(var_t{args}), ...);
        if(typeid (type)==typeid (Stmt::StmtClass)){
            createStmt(static_cast<Stmt::StmtClass>(type), value);
        }
        else if(typeid (type)==typeid (Type::TypeClass)){
            createType(static_cast<Type::TypeClass>(type), value);
        }
        else {
            throw std::string("Unable to identify identifier!");
        }
    }
private:
    void createStmt(Stmt::StmtClass type, std::vector<var_t> &value);
    void createType(Type::TypeClass type, std::vector<var_t> value);

    void createDeclStmt(std::vector<var_t> &value);
    void createNullStmt(std::vector<var_t> &value);
    void createCompoundStmt(std::vector<var_t> &value);
    void createCaseStmt(std::vector<var_t> &value);
    void createDefaultStmt(std::vector<var_t> &value);
    void createLabelStmt(std::vector<var_t> &value);
    void createIfStmt(std::vector<var_t> &value);
    void createSwitchStmt(std::vector<var_t> &value);
    void createWhileStmt(std::vector<var_t> &value);
    void createDoStmt(std::vector<var_t> &value);
    void createForStmt(std::vector<var_t> &value);
    void createGotoStmt(std::vector<var_t> &value);
    void createIndirectGotoStmt(std::vector<var_t> &value);
    void createContinueStmt(std::vector<var_t> &value);
    void createBreakStmt(std::vector<var_t> &value);
    void createReturnStmt(std::vector<var_t> &value);

    void createDeclRefExpr(std::vector<var_t> &value);
    void createPredefinedExpr(std::vector<var_t> &value);
    void createIntegerLiteral(std::vector<var_t> &value);
    void createCharacterLiteral(std::vector<var_t> &value);
    void createFloatingLiteral(std::vector<var_t> &value);
    void createImaginaryLiteral(std::vector<var_t> &value);
    void createStringLiteral(std::vector<var_t> &value);
    void createParenExpr(std::vector<var_t> &value);
    void createUnaryOperator(std::vector<var_t> &value);
    void createSizeOfAlignOfExpr(std::vector<var_t> &value);
    void createArraySubscriptExpr(std::vector<var_t> &value);
    void createCallExpr(std::vector<var_t> &value);
    void createMemberExpr(std::vector<var_t> &value);
    void createCompoundLiteralExpr(std::vector<var_t> &value);
    void createImplicitCastExpr(std::vector<var_t> &value);
    void createCStyleCastExpr(std::vector<var_t> &value);
    void createBinaryOperator(std::vector<var_t> &value);
    void createCompoundAssignOperator(std::vector<var_t> &value);
    void createConditionalOperator(std::vector<var_t> &value);
    void createAddrLabelExpr(std::vector<var_t> &value);
    void createStmtExpr(std::vector<var_t> &value);
    void createTypesCompatibleExpr(std::vector<var_t> &value);
    void createShuffleVectorExpr(std::vector<var_t> &value);
    void createChooseExpr(std::vector<var_t> &value);
    void createGNUNullExpr(std::vector<var_t> &value);
    void createVAArgExpr(std::vector<var_t> &value);
    void createInitListExpr(std::vector<var_t> &value);
    void createParenListExpr(std::vector<var_t> &value);

    void createExtQualType(std::vector<var_t> &value);
    void createVectorType(std::vector<var_t> &value);
    void createExtVectorType(std::vector<var_t> &value);

    void createFunctionNoProtoType(std::vector<var_t> &value);
    void createFunctionProtoType(std::vector<var_t> &value);
    void createTypeOfExprType(std::vector<var_t> &value);
    void createDependentTypeOfExprType(std::vector<var_t> &value);
    void createTypeOfType(std::vector<var_t> &value);
    void createDecltypeType(std::vector<var_t> &value);
    void createDependentDecltypeType(std::vector<var_t> &value);

    std::queue<std::shared_ptr<Stmt>> queue_stmt;
    std::queue<std::shared_ptr<Type>> queue_type;
    template<typename T=Stmt>
    std::shared_ptr<T> pop_stmt(){
        if(queue_stmt.empty())
            return nullptr;
        std::shared_ptr<Stmt> ptr=queue_stmt.front();
        queue_stmt.pop();
        return std::dynamic_pointer_cast<T>(ptr);
    }
    template<typename T=Type>
    std::shared_ptr<T> pop_type(){
        if(queue_type.empty())
            return nullptr;
        std::shared_ptr<Type> ptr=queue_type.front();
        queue_type.pop();
        return std::dynamic_pointer_cast<T>(ptr);
    }
};

#endif // ASTCONTEXT_H
