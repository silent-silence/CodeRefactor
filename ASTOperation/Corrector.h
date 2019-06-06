#ifndef CORRECTOR_H
#define CORRECTOR_H

#include <memory>

class Stmt;
class OpenHelper;

class Corrector
{
public:
    Corrector(OpenHelper &openHelper);

    void correct(std::weak_ptr<Stmt> root);
private:
//    void processNullStmt(std::weak_ptr<Stmt> s);
//    void processCompoundStmt(std::weak_ptr<Stmt> s);
//    void processCaseStmt(std::weak_ptr<Stmt> s);
//    void processDefaultStmt(std::weak_ptr<Stmt> s);
//    void processLabelStmt(std::weak_ptr<Stmt> s);
//    void processIfStmt(std::weak_ptr<Stmt> s);
//    void processSwitchStmt(std::weak_ptr<Stmt> s);
//    void processWhileStmt(std::weak_ptr<Stmt> s);
//    void processDoStmt(std::weak_ptr<Stmt> s);
//    void processForStmt(std::weak_ptr<Stmt> s);
//    void processGotoStmt(std::weak_ptr<Stmt> s);
//    void processIndirectGotoStmt(std::weak_ptr<Stmt> s);
//    void processContinueStmt(std::weak_ptr<Stmt> s);
//    void processBreakStmt(std::weak_ptr<Stmt> s);
//    void processReturnStmt(std::weak_ptr<Stmt> s);
//    //void processDeclStmt(std::weak_ptr<Stmt> s);

    void processDeclRefExpr(std::weak_ptr<Stmt> s);
    void processIntergerLiteral(std::weak_ptr<Stmt> s);
    void processFloatingLiteral(std::weak_ptr<Stmt> s);
    void processImaginaryLiteral(std::weak_ptr<Stmt> s);
    void processStringLiteral(std::weak_ptr<Stmt> s);
    void processCharacterLiteral(std::weak_ptr<Stmt> s);
    void processParenExpr(std::weak_ptr<Stmt> s);
    void processUnaryOperator(std::weak_ptr<Stmt> s);
    void processSizeOfAlignOfExpr(std::weak_ptr<Stmt> s);
    void processArraySubscriptExpr(std::weak_ptr<Stmt> s);
    void processCallExpr(std::weak_ptr<Stmt> s);
    void processBinaryOperator(std::weak_ptr<Stmt> s);
    void processCompoundAssignOperator(std::weak_ptr<Stmt> s);
    void processConditionalOperator(std::weak_ptr<Stmt> s);
    void processImplicitCastExpr(std::weak_ptr<Stmt> s);
    void processCStyleCastExpr(std::weak_ptr<Stmt> s);
    void processVAArgExpr(std::weak_ptr<Stmt> s);
    void processInitListExpr(std::weak_ptr<Stmt> s);
    void processParenListExpr(std::weak_ptr<Stmt> s);

    template<typename T=Stmt>
    std::shared_ptr<T> TypeConversion(std::weak_ptr<Stmt> ptr){
        if(ptr.lock() == nullptr)
            return nullptr;
        return std::dynamic_pointer_cast<T>(ptr.lock());
    }

    OpenHelper &m_openHelper;

};

#endif // CORRECTOR_H
