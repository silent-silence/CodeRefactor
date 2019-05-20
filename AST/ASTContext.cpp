#include "ASTContext.h"
#include "AST/Expr.h"

using std::queue;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

std::weak_ptr<Stmt> ASTContext::getTop() const
{
	return queue.front();
}

void ASTContext::cleanQueue()
{
	while(!queue.empty())
		queue.pop();
}

shared_ptr<Stmt> ASTContext::createCompoundStmt(unsigned num, SourceLocation LB, SourceLocation RB)
{
    vector<shared_ptr<Stmt>> stmts;
    for(auto i = 0; i != num; ++i){
        stmts.push_back(pop_back());
    }
    return make_shared<CompoundStmt>(stmts, LB, RB);
}

shared_ptr<Stmt> ASTContext::createIfStmt(SourceLocation IL, SourceLocation EL)
{
    return make_shared<IfStmt>(IL, pop_back<Expr>(), pop_back(), EL, pop_back());
}

std::shared_ptr<Stmt> ASTContext::createIntegerLiteral(int val, QualType type)
{
    return make_shared<IntegerLiteral>(val, type);
}

std::shared_ptr<Stmt> ASTContext::createCharacterLiteral(unsigned val, bool iswide, QualType type)
{
    return make_shared<CharacterLiteral>(val, iswide, type);
}

shared_ptr<Stmt> ASTContext::createFloatingLiteral(const float &V, bool isexact, QualType type)
{
    return make_shared<FloatingLiteral>(V, isexact, type);
}

std::shared_ptr<Stmt> ASTContext::createStringLiteral(const char *StrData, unsigned ByteLength, bool Wide, QualType Ty, const SourceLocation Loc, unsigned NumStrs)
{
    return  StringLiteral::Create(StrData, ByteLength, Wide, Ty, Loc, NumStrs);
}

std::shared_ptr<Stmt> ASTContext::createStringLiteral(const char *StrData, unsigned ByteLength, bool Wide, QualType Ty, SourceLocation Loc)
{
    return StringLiteral::Create(StrData, ByteLength, Wide, Ty, Loc);
}

std::shared_ptr<Stmt> ASTContext::createUnaryOperator(UnaryOperator::Opcode opc, QualType type)
{
    return make_shared<UnaryOperator>(pop_back<Expr>(), opc, type);
}

shared_ptr<Stmt> ASTContext::createBinaryOperator(BinaryOperator::Opcode opc, QualType ResTy, SourceLocation opLoc)
{
    return make_shared<BinaryOperator>(pop_back<Expr>(), pop_back<Expr>(), opc, ResTy, opLoc);
}

std::shared_ptr<Stmt> ASTContext::createCompoundAssignOperator(BinaryOperator::Opcode opc, QualType ResType, QualType CompLHSType, QualType CompResultType, SourceLocation OpLoc)
{
    return make_shared<CompoundAssignOperator>(pop_back<Expr>(), pop_back<Expr>(), opc, ResType, CompLHSType, CompResultType, OpLoc);
}

std::shared_ptr<Stmt> ASTContext::createConditionalOperator(QualType t)
{
    return make_shared<ConditionalOperator>(pop_back<Expr>(), pop_back<Expr>(), pop_back<Expr>(), t);
}

std::shared_ptr<Stmt> ASTContext::createStmtExpr(QualType T, SourceLocation lp, SourceLocation rp)
{
    return make_shared<StmtExpr>(pop_back<CompoundStmt>(), T, lp, rp);
}

std::shared_ptr<Stmt> ASTContext::createSimpleStmt(Stmt::StmtClass type)
{
	return pop_back<Expr>();
}
/*queue<shared_ptr<Stmt>> ASTContext::queue()
{
    static std::queue<std::shared_ptr<Stmt>> m_queue;
    return m_queue;
}
*/