#ifndef ASTCONTEXT_H
#define ASTCONTEXT_H

#include <queue>
#include <memory>
#include <variant>
#include <stack>
#include "AST/AST.hpp"

class ASTContext
{
public:
    typedef std::variant<
    unsigned,
    unsigned long,
    int,
    double,
    float,
    bool,
    char *,
    SourceLocation,
    QualType,
    UnaryOperator::Opcode,
    BinaryOperator::Opcode,
    PredefinedExpr::IdentType,
    std::string
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

#ifdef ENV_TEST
    std::weak_ptr<Stmt> getTop() const;
    void cleanAST();
#endif

private:
	void createStmt(Stmt::StmtClass type, std::vector<var_t> &value);
	void createType(Type::TypeClass type, std::vector<var_t> value);

	void createDeclStmt(std::vector<var_t> &value);
	void createNullStmt(std::vector<var_t> &value);//done
	void createCompoundStmt(std::vector<var_t> &value);//done
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
	void createContinueStmt(std::vector<var_t> &value);//done
	void createBreakStmt(std::vector<var_t> &value);//done
	void createReturnStmt(std::vector<var_t> &value);//done

	void createDeclRefExpr(std::vector<var_t> &value);
	void createPredefinedExpr(std::vector<var_t> &value);
	void createIntegerLiteral(std::vector<var_t> &value);//done
	void createCharacterLiteral(std::vector<var_t> &value);
	void createFloatingLiteral(std::vector<var_t> &value);//done
	void createImaginaryLiteral(std::vector<var_t> &value);
	void createStringLiteral(std::vector<var_t> &value);//done
	void createParenExpr(std::vector<var_t> &value);//done
	void createUnaryOperator(std::vector<var_t> &value);//done
	void createSizeOfAlignOfExpr(std::vector<var_t> &value);//done
	void createArraySubscriptExpr(std::vector<var_t> &value);//done
	void createCallExpr(std::vector<var_t> &value);
	void createMemberExpr(std::vector<var_t> &value);
	void createCompoundLiteralExpr(std::vector<var_t> &value);
	void createImplicitCastExpr(std::vector<var_t> &value);
	void createCStyleCastExpr(std::vector<var_t> &value);//done
	void createBinaryOperator(std::vector<var_t> &value);//done
	void createCompoundAssignOperator(std::vector<var_t> &value);//done
	void createConditionalOperator(std::vector<var_t> &value);//done
	void createAddrLabelExpr(std::vector<var_t> &value);///gun extension
	void createStmtExpr(std::vector<var_t> &value);///gun extension
	void createTypesCompatibleExpr(std::vector<var_t> &value);///gun extension
	void createShuffleVectorExpr(std::vector<var_t> &value);///clang extension
	void createChooseExpr(std::vector<var_t> &value);///gun extension
	void createGNUNullExpr(std::vector<var_t> &value);///gun extension
	void createVAArgExpr(std::vector<var_t> &value);
	void createInitListExpr(std::vector<var_t> &value);
	void createParenListExpr(std::vector<var_t> &value);

	std::stack<std::shared_ptr<Stmt>> queue;
	template<typename T=Stmt>
	std::shared_ptr<T> pop_back(){
		if(queue.empty())
			throw std::range_error("Queue empty");
		std::shared_ptr<Stmt> ptr=queue.top();
		queue.pop();
		return std::dynamic_pointer_cast<T>(ptr);
	}
};

#endif // ASTCONTEXT_H
