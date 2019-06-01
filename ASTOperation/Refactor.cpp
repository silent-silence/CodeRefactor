#include "Refactor.h"
#include "AST/Stmt.h"
#include "AST/Expr.h"
#include "OpenHelper/OpenHelper.h"
#include <exception>
#include <list>

using std::exception;

using std::weak_ptr;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::list;

Refactor::Refactor(OpenHelper &openHelper)
    : m_openHelper{openHelper}
{}

void Refactor::refactor_MCIf(std::weak_ptr<Stmt> root)
{
	if(root.lock() == nullptr)
		return;
    auto ptr = TypeConversion(root);
    for(auto i = ptr->child_begin(); i != ptr->child_end(); ++i){
    	if(*i == nullptr)
    		continue;
        refactor_MCIf(*i);
        if(IfStmt::classof(*i)&&find_MCIf(*i)){
            conversion_MCIf(ptr, *i);
        }
    }
}

void Refactor::conversion_MCIf(weak_ptr<Stmt> parent, weak_ptr<Stmt> s)
{
    auto ptr = make_shared<CompoundStmt>(Stmt::EmptyShell());
    ptr->setLBracLoc(SourceLocation());
    ptr->setRBracLoc(SourceLocation());

    list<shared_ptr<Stmt>> vec;

    auto stmt = dynamic_pointer_cast<IfStmt>(s.lock());
    auto expr = dynamic_pointer_cast<BinaryOperator>(stmt->getCond().lock());

    shared_ptr<Expr> L_ptr=expr->getLHS().lock();
    while(ParenExpr::classof(L_ptr)){
        L_ptr = dynamic_pointer_cast<ParenExpr>(L_ptr)->getSubExpr().lock();
    }
    shared_ptr<Expr> R_ptr=expr->getRHS().lock();
    while(ParenExpr::classof(R_ptr)){
        R_ptr = dynamic_pointer_cast<ParenExpr>(R_ptr)->getSubExpr().lock();
    }

    shared_ptr<IfStmt> ptr1;
    shared_ptr<IfStmt> ptr2;
    switch (expr->getOpcode()) {
    case BinaryOperator::LAnd:
        ptr1 = make_shared<IfStmt>(SourceLocation(), R_ptr,
                                   stmt->getThen().lock());
        ptr2 = make_shared<IfStmt>(SourceLocation(), L_ptr, ptr1,
                                   SourceLocation(), stmt->getElse().lock());
        vec.push_back(ptr2);
        break;
    case BinaryOperator::LOr:
        ptr1 = make_shared<IfStmt>(SourceLocation(), L_ptr,
                                   stmt->getThen().lock());
        ptr2 = make_shared<IfStmt>(SourceLocation(), R_ptr, stmt->getThen().lock(),
                                   SourceLocation(), stmt->getElse().lock());
        vec.push_back(ptr1);
        vec.push_back(ptr2);
        break;
    default:break;
    }
    ptr->setStmts(vec);
    refactor_MCIf(ptr);
    auto p = parent.lock();
    for(auto i = p->child_begin(); i != p->child_end(); ++i){
        if(IfStmt::classof(*i)){
            auto n = dynamic_pointer_cast<IfStmt>(*i);
            if(n->getIfLoc()==stmt->getIfLoc()){
				i = ptr;
				break;
            }
        }
    }
}


///                 ->  if(expr)        |                   ->  if(expr)
/// if(expr&&a)     ->      if(a)       |   if(expr||a)     ->      stmt1
///     stmt1       ->          stmt1   |       stmt1       ->  if(a)
/// else            ->  else{           |   else            ->      stmt1
///     stmt2       ->      stmt2       |       stmt2       ->  else
///                 ->  }               |                   ->      stmt2
bool Refactor::find_MCIf(std::weak_ptr<Stmt> s)
{
    auto stmt = TypeConversion<IfStmt>(s);
    auto expr = TypeConversion<Expr>(stmt->getCond().lock());
    if(BinaryOperator::classof(expr)){
        auto i = dynamic_pointer_cast<BinaryOperator>(expr);
        if((i->getOpcode()==BinaryOperator::LAnd)||
                (i->getOpcode()==BinaryOperator::LOr)){
            return true;
        }
    }
    return false;
}


