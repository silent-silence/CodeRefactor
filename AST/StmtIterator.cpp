#include "StmtIterator.h"

using std::list;
using std::shared_ptr;

Iterator::~Iterator()
{}

StmtIterator::StmtIterator(shared_ptr<Iterator> iterator)
    : iterator_(iterator)
{}

Iterator &StmtIterator::operator++()
{
	if(iterator_ == nullptr)
		return *this;
    return iterator_->operator++();
}

Iterator &StmtIterator::operator+(size_t i)
{
	if(iterator_ == nullptr)
		return *this;
    return iterator_->operator+(i);
}

std::shared_ptr<Stmt> StmtIterator::operator[](size_t idx)
{
	if(iterator_ == nullptr)
		return nullptr;
    return iterator_->operator[](idx);
}

signed StmtIterator::operator-(const Iterator &R)
{
    auto r =dynamic_cast<const StmtIterator &>(R);
	if(iterator_ == nullptr || r.iterator_ == nullptr)
		return 0;
    return iterator_->operator-(*r.iterator_.get());
}

void StmtIterator::operator=(shared_ptr<Stmt> stmt)
{
	if(iterator_ == nullptr)
		return;
    iterator_->operator=(stmt);
}

shared_ptr<Stmt> StmtIterator::operator*() const
{
	if(iterator_ == nullptr)
		return nullptr;
    return iterator_->operator*();
}

shared_ptr<Stmt> StmtIterator::operator->() const
{
	if(iterator_ == nullptr)
		return nullptr;
    return iterator_->operator->();
}

bool StmtIterator::operator==(const Iterator &R) const
{
    auto r =dynamic_cast<const StmtIterator &>(R);
    if(iterator_==nullptr&&r.iterator_==nullptr)
        return true;
    else if(iterator_==nullptr||r.iterator_==nullptr){
        return false;
    }
    return iterator_->operator==(*r.iterator_);
}

bool StmtIterator::operator!=(const Iterator &R) const
{
    auto r =dynamic_cast<const StmtIterator &>(R);
    if(iterator_==nullptr&&r.iterator_==nullptr)
        return false;
    else if(iterator_==nullptr||r.iterator_==nullptr){
        return true;
    }
    return iterator_->operator!=(*r.iterator_);
}

bool StmtIterator::operator>=(const Iterator &R) const
{
    auto r =dynamic_cast<const StmtIterator &>(R);
    if(iterator_==nullptr)
        return true;
    else if(r.iterator_==nullptr){
        return false;
    }
    return iterator_->operator>=(*r.iterator_);
}

bool StmtIterator::operator>(const Iterator &R) const
{
    auto r =dynamic_cast<const StmtIterator &>(R);
    if(iterator_==nullptr&&r.iterator_!=nullptr)
        return true;
    else if(r.iterator_==nullptr){
        return false;
    }
    return iterator_->operator>(*r.iterator_);
}

Iterator &StmtIterator::operator-(size_t i)
{
    return iterator_->operator-(i);
}

ListIterator::ListIterator(list<shared_ptr<Stmt>>::iterator iterator)
    : iterator_(iterator)
{}

Iterator &ListIterator::operator++()
{
    ++iterator_;
    return *this;
}

Iterator &ListIterator::operator+(size_t i)
{
    ListIterator r(iterator_);
    while (i++) {
        ++r.iterator_;
    }
    return r;
}

shared_ptr<Stmt> ListIterator::operator[](size_t idx)
{
    while (idx++) {
        ++iterator_;
    }
    return *iterator_;
}

signed ListIterator::operator-(const Iterator &R)
{
    signed t = 0;
    ListIterator r1(iterator_);
    ListIterator r2(dynamic_cast<const ListIterator &>(R).iterator_);
    if(r1.iterator_ == r2.iterator_)
        return t;
    return t;
}

shared_ptr<Stmt> ListIterator::operator*() const
{
    return *iterator_;
}

shared_ptr<Stmt> ListIterator::operator->() const
{
    return *iterator_;
}

void ListIterator::operator=(shared_ptr<Stmt> stmt)
{
    *iterator_=stmt;
}

bool ListIterator::operator==(const Iterator &R) const
{
    auto r =dynamic_cast<const ListIterator &>(R);
    return iterator_ == r.iterator_;
}

bool ListIterator::operator!=(const Iterator &R) const
{
    auto r =dynamic_cast<const ListIterator &>(R);
    return iterator_ != r.iterator_;
}

bool ListIterator::operator>=(const Iterator &R) const
{
    auto r =dynamic_cast<const ListIterator &>(R);
    //return iterator_ >= r.iterator_;
    return false;
}

bool ListIterator::operator>(const Iterator &R) const
{
    auto r =dynamic_cast<const ListIterator &>(R);
    //return iterator_ > r.iterator_;
    return false;
}

Iterator &ListIterator::operator-(size_t i)
{
    ListIterator r(iterator_);
    while (i--) {
        --r.iterator_;
    }
    return r;
}

PtrIterator::PtrIterator(shared_ptr<Stmt> iterator)
    : iterator_{iterator}
{}

Iterator &PtrIterator::operator++()
{
    iterator_ = nullptr;
    return *this;
}

Iterator &PtrIterator::operator+(size_t i)
{
    PtrIterator r(iterator_);
    r.iterator_=nullptr;
    return r;
}

shared_ptr<Stmt> PtrIterator::operator[](size_t idx)
{
    if(iterator_ && idx == 0)
        return iterator_;
    else {
        return nullptr;
    }
}

signed PtrIterator::operator-(const Iterator &R)
{
    return 0;
}

shared_ptr<Stmt> PtrIterator::operator*() const
{
    return iterator_;
}

shared_ptr<Stmt> PtrIterator::operator->() const
{
    return iterator_;
}

void PtrIterator::operator=(shared_ptr<Stmt> stmt)
{
    iterator_=stmt;
}

bool PtrIterator::operator==(const Iterator &R) const
{
    auto r =dynamic_cast<const PtrIterator &>(R);
    return iterator_ == r.iterator_;
}

bool PtrIterator::operator!=(const Iterator &R) const
{
    auto r =dynamic_cast<const PtrIterator &>(R);
    return iterator_ != r.iterator_;
}

bool PtrIterator::operator>=(const Iterator &R) const
{
    return false;
}

bool PtrIterator::operator>(const Iterator &R) const
{
    return false;
}

Iterator &PtrIterator::operator-(size_t i)
{
    PtrIterator r(iterator_);
    r.iterator_=nullptr;
    return r;
}
