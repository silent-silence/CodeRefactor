#include "Refactor.h"
#include "Stmt.h"
#include "Expr.h"
#include "OpenHelper.h"
#include <exception>

using std::exception;

using std::list;
using std::weak_ptr;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

Refactor::Refactor()
{}

void Refactor::refactor_MCIf(std::weak_ptr<Stmt> root)
{
    if(root.lock()==nullptr)return;
    auto ptr = TypeConversion(root);

    for(auto i = ptr->child_begin(); i != ptr->child_end(); ++i){
        if((*i)==nullptr)break;
        if(is_MCIf(i)){
            conversion_MCIf(i);
        }
        refactor_MCIf(*i);
    }
}

void Refactor::refactor_SCMIf(std::weak_ptr<Stmt> root)
{
    if(root.lock()==nullptr)return;
    auto ptr = TypeConversion(root);

    for(auto i = ptr->child_begin(); i != ptr->child_end(); ++i){
        if((*i)==nullptr)break;
        refactor_SCMIf(*i);
        if(is_SCMIf(i)){
            conversion_SCMIf(i);
        }
    }
}

void Refactor::refactor_MIf(std::weak_ptr<Stmt> root)
{

}

void Refactor::refactor_Switch(std::weak_ptr<Stmt> root)
{
    if(root.lock()==nullptr)return;
    auto ptr=TypeConversion(root);
    for(auto i=ptr->child_begin();i !=ptr->child_end();++i){
        if((*i)==nullptr)break;
        if(SwitchStmt::classof(*i)){
            conversion_Switch(i);
        }
    }
}

void Refactor::refactor_For(std::weak_ptr<Stmt> root)
{
    if(root.lock()==nullptr)return;
    auto ptr=TypeConversion(root);
    for(auto i=ptr->child_begin();i !=ptr->child_end();++i){
        if((*i)==nullptr)break;
        if(ForStmt::classof(*i)){
            conversion_For(i);
        }
    }
}

void Refactor::refactor_While(std::weak_ptr<Stmt> root)
{
    if(root.lock()==nullptr)return;
    auto ptr=TypeConversion(root);
    for(auto i=ptr->child_begin();i !=ptr->child_end();++i){
        if((*i)==nullptr)break;
        if(WhileStmt::classof(*i)){
            conversion_While(i);
        }
    }
}

void Refactor::conversion_MCIf(StmtIterator iter)
{
    auto stmt = TypeConversion<IfStmt>(*iter);
    auto expr = TypeConversion<BinaryOperator>(stmt->getCond());

    shared_ptr<Expr> L_ptr=expr->getLHS().lock();
    while(ParenExpr::classof(L_ptr)){
        L_ptr = TypeConversion<ParenExpr>(L_ptr)->getSubExpr().lock();
    }
    shared_ptr<Expr> R_ptr=expr->getRHS().lock();
    while(ParenExpr::classof(R_ptr)){
        R_ptr = TypeConversion<ParenExpr>(R_ptr)->getSubExpr().lock();
    }
    shared_ptr<Stmt> ptr;
    shared_ptr<IfStmt> L_stmt;
    shared_ptr<IfStmt> R_stmt;
    switch (expr->getOpcode()) {
    case BinaryOperator::LAnd:
        R_stmt = make_shared<IfStmt>(SourceLocation(),
                                     R_ptr,
                                     stmt->getThen().lock(),
                                     stmt->getElseLoc(), stmt->getElse().lock());
        L_stmt = make_shared<IfStmt>(SourceLocation(),
                                     L_ptr,
                                     R_stmt,
                                     stmt->getElseLoc(), stmt->getElse().lock());
        if(L_stmt->getThen().lock()!=nullptr&&!CompoundStmt::classof(L_stmt->getThen())){
            list<shared_ptr<Stmt>> li({L_stmt->getThen().lock()});
            L_stmt->setThen(make_shared<CompoundStmt>(li, SourceLocation(), SourceLocation()));
        }
        ptr=L_stmt;
        break;
    case BinaryOperator::LOr:
        R_stmt = make_shared<IfStmt>(SourceLocation(), R_ptr, stmt->getThen().lock(),
                                     stmt->getElseLoc(), stmt->getElse().lock());
        L_stmt = make_shared<IfStmt>(SourceLocation(), L_ptr, stmt->getThen().lock(),
                                     SourceLocation(), R_stmt);
        ptr=L_stmt;
        break;
    default:break;
    }
    iter.replaceNode(ptr);
    if(is_MCIf(iter))
        conversion_MCIf(iter);
    refactor_MCIf(*iter);
}

void Refactor::conversion_SCMIf(StmtIterator iter)
{
    auto L_stmt = dynamic_pointer_cast<IfStmt>(*iter);
    shared_ptr<IfStmt> R_stmt;
    if(IfStmt::classof(L_stmt->getThen())){
        R_stmt = TypeConversion<IfStmt>(L_stmt->getThen());
    }
    else {
        auto stmt = TypeConversion<CompoundStmt>(L_stmt->getThen());
        R_stmt = TypeConversion<IfStmt>(*stmt->child_begin());
    }

    shared_ptr<Expr> L_ptr = make_shared<ParenExpr>(
                SourceLocation(),
                SourceLocation(),
                L_stmt->getCond().lock());
    shared_ptr<Expr> R_ptr = make_shared<ParenExpr>(
                SourceLocation(),
                SourceLocation(),
                R_stmt->getCond().lock());

    shared_ptr<Expr> cond = make_shared<BinaryOperator>(L_ptr, R_ptr,
                                                        BinaryOperator::LAnd,
                                                        nullptr, SourceLocation());

    auto ptr = make_shared<IfStmt>(L_stmt->getIfLoc(), cond, R_stmt->getThen().lock(),
                                   L_stmt->getElseLoc(), L_stmt->getElse().lock());
    iter.replaceNode(ptr);
}

void Refactor::conversion_MIf(StmtIterator iter)
{

}

void Refactor::conversion_Switch(StmtIterator iter)
{
    auto ptr=TypeConversion<SwitchStmt>(*iter);
    auto L_expr = TypeConversion<Expr>(ptr->getCond());
    auto case_stmt = TypeConversion<CaseStmt>(ptr->getSwitchCaseList());

    auto body = TypeConversion<CompoundStmt>(ptr->getBody());
    shared_ptr<IfStmt> stmt = nullptr;
    while (case_stmt != nullptr) {
        auto R_expr = TypeConversion<Expr>(case_stmt->getLHS());
        auto cond = make_shared<BinaryOperator>(L_expr, R_expr, BinaryOperator::EQ,
                                                nullptr, SourceLocation());

        std::list<shared_ptr<Stmt>> li;
        auto body_begin = body->body_begin();
        while(body_begin != body->body_end()){
            if(CaseStmt::classof(*body_begin)){
                auto i = TypeConversion<CaseStmt>(*body_begin);
                if(i->getCaseLoc()==case_stmt->getCaseLoc())
                    break;
            }
            ++body_begin;
        }

        while(body_begin != body->body_end()){
            if(CaseStmt::classof(*body_begin)){
                ++body_begin;
                continue;
            }

            if(BreakStmt::classof(*body_begin))
                break;
            li.push_back(*body_begin);
            ++body_begin;
        }

        auto then = make_shared<CompoundStmt>(li, SourceLocation(), SourceLocation());

        auto ifstmt = make_shared<IfStmt>(SourceLocation(), cond, then);
        if(stmt == nullptr)
            iter.replaceNode(ifstmt);
        else
            stmt->setElse(ifstmt);
        stmt = ifstmt;
        case_stmt = TypeConversion<CaseStmt>(case_stmt->getNextSwitchCase());
    }
}

void Refactor::conversion_For(StmtIterator iter)
{
    auto ptr=TypeConversion<ForStmt>(*iter);
    std::shared_ptr<Stmt> Inc=ptr->getInc().lock();

    auto stmt_body = TypeConversion(ptr->getBody());
    shared_ptr<CompoundStmt> body;
    if(CompoundStmt::classof(stmt_body)){
        body=TypeConversion<CompoundStmt>(stmt_body);
        body->addStmt(Inc);
    }else{
        std::list<std::shared_ptr<Stmt>> li({stmt_body, Inc});
        body=make_shared<CompoundStmt>(li,SourceLocation(),SourceLocation());
    }

    auto Cond=TypeConversion<Expr>(ptr->getCond());
    SourceLocation wl=ptr->getForLoc();
    auto whileStmt=make_shared<WhileStmt>(Cond,body,wl);

    shared_ptr<Stmt> stmt;
    if(NullStmt::classof(ptr->getInit().lock())){
        stmt = whileStmt;
    }
    else{
        std::list<std::shared_ptr<Stmt>> li({ptr->getInit().lock(), whileStmt});
        stmt = make_shared<CompoundStmt>(li, SourceLocation(), SourceLocation());
    }

    iter.replaceNode(stmt);
}

void Refactor::conversion_While(StmtIterator iter)
{
    auto ptr=TypeConversion<WhileStmt>(*iter);
    std::shared_ptr<Expr> Cond=ptr->getCond().lock();
    std::shared_ptr<Stmt> Body=ptr->getBody().lock();
    SourceLocation fl=ptr->getWhileLoc();
    std::shared_ptr<Stmt> forStmt=make_shared<ForStmt>(make_shared<NullStmt>(SourceLocation()),
                                                       Cond,nullptr,
                                                       Body,fl,
                                                       SourceLocation(),
                                                       SourceLocation());
    iter.replaceNode(forStmt);
}

///                 ->  if(expr)        |                   ->  if(expr)
/// if(expr&&a)     ->      if(a)       |   if(expr||a)     ->      stmt1
///     stmt1       ->          stmt1   |       stmt1       ->  else if(a)
/// else            ->      else        |   else            ->      stmt1
///     stmt2       ->          stmt2   |       stmt2       ->  else
///                 ->  else stmt2      |                   ->      stmt2
bool Refactor::is_MCIf(StmtIterator iter)
{
    if(!IfStmt::classof(*iter))
        return false;
    auto stmt = TypeConversion<IfStmt>(*iter);
    auto expr = TypeConversion<Expr>(stmt->getCond().lock());
    if(BinaryOperator::classof(expr)){
        auto i = TypeConversion<BinaryOperator>(expr);
        if((i->getOpcode()==BinaryOperator::LAnd)||
                (i->getOpcode()==BinaryOperator::LOr)){
            return true;
        }
    }
    return false;
}

bool Refactor::is_SCMIf(StmtIterator iter)
{
    if(!IfStmt::classof(*iter))
        return false;
    auto stmt = TypeConversion<IfStmt>(*iter);
    auto then = TypeConversion<Stmt>(stmt->getThen().lock());
    if(IfStmt::classof(then)){
        auto i = TypeConversion<IfStmt>(then);
        if(!i->getElse().lock())
            return true;
    }
    else if(CompoundStmt::classof(then)){
        auto compound = TypeConversion<CompoundStmt>(then);
        if(compound->size()==1&&IfStmt::classof(*compound->child_begin())){
            auto i = TypeConversion<IfStmt>(*compound->child_begin());
            if(!i->getElse().lock())
                return true;
        }
    }
    return false;
}

bool Refactor::is_MIf(StmtIterator iter)
{
    //    if(!IfStmt::classof(*iter))
    //        return false;
    //    auto stmt = TypeConversion<IfStmt>(*iter);
    return false;
}
