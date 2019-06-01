#ifndef REFACTOR_H
#define REFACTOR_H

#include <memory>
#include <vector>

class Stmt;
class OpenHelper;

class Refactor
{
public:
    Refactor(OpenHelper &openHelper);

    void refactor_MCIf(std::weak_ptr<Stmt> root);
private:

    void conversion_MCIf(std::weak_ptr<Stmt> parent, std::weak_ptr<Stmt> s);
    bool find_MCIf(std::weak_ptr<Stmt> s);
//    std::vector<std::weak_ptr<Stmt>> findSCM_IfStmt(std::weak_ptr<Stmt> s);
//    std::vector<std::weak_ptr<Stmt>> findForStmt(std::weak_ptr<Stmt> s);
//    std::vector<std::weak_ptr<Stmt>> findWhileStmt(std::weak_ptr<Stmt> s);

    template<typename T=Stmt>
    std::shared_ptr<T> TypeConversion(std::weak_ptr<Stmt> ptr){
        return std::move(std::dynamic_pointer_cast<T>(ptr.lock()));
    }

    OpenHelper &m_openHelper;
};

#endif // REFACTOR_H
