#ifndef REFACTOR_H
#define REFACTOR_H

#include <memory>
#include <vector>

class Stmt;
class StmtIterator;
class OpenHelper;

class Refactor
{
public:
    Refactor(OpenHelper &openHelper);

    void refactor_MCIf(std::weak_ptr<Stmt> root);
    void refactor_SCMIf(std::weak_ptr<Stmt> root);

    void refactor_MIf(std::weak_ptr<Stmt> root);
    void refactor_Switch(std::weak_ptr<Stmt> root);

    void refactor_For(std::weak_ptr<Stmt> root);
    void refactor_While(std::weak_ptr<Stmt> root);
private:
    void conversion_MCIf(StmtIterator iter);
    void conversion_SCMIf(StmtIterator iter);

    void conversion_MIf(StmtIterator iter);
    void conversion_Switch(StmtIterator iter);

    void conversion_For(StmtIterator iter);
    void conversion_While(StmtIterator iter);

    bool is_MCIf(StmtIterator iter);
    bool is_SCMIf(StmtIterator iter);

    bool is_MIf(StmtIterator iter);

    template<typename T=Stmt>
    std::shared_ptr<T> TypeConversion(std::weak_ptr<Stmt> ptr){
        if(ptr.lock() == nullptr)
            return nullptr;
        return std::move(std::dynamic_pointer_cast<T>(ptr.lock()));
    }

    OpenHelper &m_openHelper;
};

#endif // REFACTOR_H
