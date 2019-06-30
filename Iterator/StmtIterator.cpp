#include "StmtIterator.h"

using std::list;
using std::shared_ptr;
using std::dynamic_pointer_cast;

/*StmtIterator::StmtIterator()
    :iterator_()
{}*/

StmtIterator::StmtIterator(std::shared_ptr<Iterator> &&iter)
		: iterator_{iter}
{}

/*StmtIterator::Iter &StmtIterator::operator-(size_t i)
{
    auto ptr=iterator_.lock();
    if(ptr == nullptr)
        return *this;
    return ptr->operator-(i);
}

StmtIterator::Iter &StmtIterator::operator+(size_t i)
{
    auto ptr=iterator_.lock();
    if(ptr == nullptr)
        return *this;
    return ptr->operator+(i);
}*/

StmtIterator::Iter &StmtIterator::operator++()
{
	auto ptr = iterator_.get();
	if (ptr == nullptr)
		return *this;
	return ptr->operator++();
}

StmtIterator::Iter &StmtIterator::operator--()
{
	auto ptr = iterator_.get();
	if (ptr == nullptr)
		return *this;
	return ptr->operator--();
}

/*Stmt* StmtIterator::operator[](size_t idx)
{
    auto ptr=iterator_.lock();
    if(ptr == nullptr)
        return nullptr;
    return ptr->operator[](idx);
}

signed StmtIterator::operator-(const Iter &R)
{
    auto r =dynamic_cast<const StmtIterator &>(R);
    auto ptr1=iterator_.lock();
    auto ptr2=r.iterator_.lock();

    if(ptr1 == nullptr || ptr2 == nullptr)
        return 0;
    return ptr1->operator-(*ptr2);
}*/

StmtIterator::Ret StmtIterator::operator*() const
{
	auto ptr = iterator_.get();
	if (ptr == nullptr)
		return nullptr;
	return ptr->operator*();
}

StmtIterator::Ret StmtIterator::operator->() const
{
	auto ptr = iterator_.get();
	if (ptr == nullptr)
		return nullptr;
	return ptr->operator->();
}

/*void StmtIterator::operator=(const Iter &R)
{
	auto ptr = iterator_.get();
	if (ptr == nullptr)
		return;
	ptr->operator=(R);
}*/

bool StmtIterator::operator==(const Iter &R) const
{
	auto ptr1 = iterator_.get();
	auto ptr2 = dynamic_cast<const StmtIterator &>(R).iterator_.get();

	if (ptr1 == nullptr && ptr2 == nullptr)
		return true;
	else if (ptr1 == nullptr || ptr2 == nullptr)
	{
		return false;
	}
	return ptr1->operator==(*ptr2);
}

bool StmtIterator::operator!=(const Iter &R) const
{
	auto ptr1 = iterator_.get();
	auto ptr2 = dynamic_cast<const StmtIterator &>(R).iterator_.get();

	if (ptr1 == nullptr && ptr2 == nullptr)
		return false;
	else if (ptr1 == nullptr || ptr2 == nullptr)
	{
		return true;
	}
	return ptr1->operator!=(*ptr2);
}

void StmtIterator::replaceNode(Iterator<Stmt>::Ret R)
{
	iterator_->replaceNode(R);
}

/*bool StmtIterator::operator>=(const Iter &R) const
{
    auto r =dynamic_cast<const StmtIterator &>(R);
    auto ptr1=iterator_.lock();
    auto ptr2=r.iterator_.lock();

    if(ptr1==nullptr)
        return true;
    else if(ptr2==nullptr){
        return false;
    }
    return ptr1->operator>=(*ptr2);
}

bool StmtIterator::operator>(const Iter &R) const
{
    auto r =dynamic_cast<const StmtIterator &>(R);
    auto ptr1=iterator_.lock();
    auto ptr2=r.iterator_.lock();

    if(ptr1==nullptr&&ptr2!=nullptr)
        return true;
    else if(ptr2==nullptr){
        return false;
    }
    return ptr1->operator>(*ptr2);
}*/

ListIterator::ListIterator(list<shared_ptr<Stmt>>::iterator iterator)
		: iterator_(iterator)
{}

ListIterator::Iter &ListIterator::operator++()
{
	++iterator_;
	return *this;
}

/*ListIterator::Iter &ListIterator::operator+(size_t i)
{
    ListIterator r(iterator_);
    while (i++) {
        ++r.iterator_;
    }
    return r;
}*/

/*Stmt* ListIterator::operator[](size_t idx)
{
    while (idx++) {
        ++iterator_;
    }
    return (*iterator_).get();
}

signed ListIterator::operator-(const Iter &R)
{
    signed t = 0;
    ListIterator r1(iterator_);
    ListIterator r2(dynamic_cast<const ListIterator &>(R).iterator_);
    if(r1.iterator_ == r2.iterator_)
        return t;
    return t;
}*/

ListIterator::Ret ListIterator::operator*() const
{
	return *iterator_;
}

ListIterator::Ret ListIterator::operator->() const
{
	return *iterator_;
}

/*void ListIterator::operator=(const Iter &R)
{
	auto r = dynamic_cast<const ListIterator &>(R);
	iterator_ = r.iterator_;
}*/

bool ListIterator::operator==(const Iter &R) const
{
	auto r = dynamic_cast<const ListIterator &>(R);
	return iterator_ == r.iterator_;
}

bool ListIterator::operator!=(const Iter &R) const
{
	auto r = dynamic_cast<const ListIterator &>(R);
	return iterator_ != r.iterator_;
}

ListIterator::Iter &ListIterator::operator--()
{
	--iterator_;
	return *this;
}

void ListIterator::replaceNode(Iterator<Stmt>::Ret R)
{
	*iterator_ = R;
}

/*bool ListIterator::operator>=(const Iter &R) const
{
    auto r =dynamic_cast<const ListIterator &>(R);
    //return iterator_ >= r.iterator_;
    return false;
}

bool ListIterator::operator>(const Iter &R) const
{
    auto r =dynamic_cast<const ListIterator &>(R);
    //return iterator_ > r.iterator_;
    return false;
}*/

/*Iterator<Stmt> &ListIterator::operator-(size_t i)
{
    ListIterator r(iterator_);
    while (i--) {
        --r.iterator_;
    }
    return r;
}*/

PtrIterator::PtrIterator(PtrPosition position, std::shared_ptr<Stmt> &iterator)
		: m_position{position}, iterator_(iterator)
{}

/*PtrIterator::Iter &PtrIterator::operator+(size_t i)
{
    PtrIterator r(iterator_);
    r.iterator_.lock()=nullptr;
    return r;
}*/

PtrIterator::Iter &PtrIterator::operator++()
{
	if(m_position == PtrPosition::begin)
		m_position = PtrPosition::end;
	return *this;
}

PtrIterator::Iter &PtrIterator::operator--()
{
	if(m_position == PtrPosition::end)
		m_position = PtrPosition::begin;
	return *this;
}

/*Stmt* PtrIterator::operator[](size_t idx)
{
    auto ptr=iterator_.lock();
    if(ptr && idx == 0)
        return ptr.get();
    else {
        return nullptr;
    }
}

signed PtrIterator::operator-(const Iterator &R)
{
    return 0;
}*/

PtrIterator::Ret PtrIterator::operator*() const
{
	return iterator_;
}

PtrIterator::Ret PtrIterator::operator->() const
{
	return iterator_;
}

/*void PtrIterator::operator=(const Iter &R)
{
	auto r = dynamic_cast<const PtrIterator &>(R);
	auto ptr1 = iterator_;
	auto ptr2 = r.iterator_;
	ptr1 = ptr2;
}*/

void PtrIterator::replaceNode(Ret R)
{
	iterator_.swap(R);
}

bool PtrIterator::operator==(const Iter &R) const
{
	auto r = dynamic_cast<const PtrIterator &>(R);
	return iterator_ == r.iterator_ && m_position == r.m_position;
}

bool PtrIterator::operator!=(const Iter &R) const
{
	auto r = dynamic_cast<const PtrIterator &>(R);
	return iterator_ != r.iterator_ || m_position != r.m_position;
}

/*bool PtrIterator::operator>=(const Iter &R) const
{
    return false;
}

bool PtrIterator::operator>(const Iter &R) const
{
    return false;
}*/

/*PtrIterator::Iter &PtrIterator::operator-(size_t i)
{
    PtrIterator r(iterator_);
    r.iterator_.lock()=nullptr;
    return r;
}*/

Iterator<Stmt>::Iter &EmptyIterator::operator++()
{
	return *this;
}

Iterator<Stmt>::Ret EmptyIterator::operator*() const
{
	return nullptr;
}

Iterator<Stmt>::Ret EmptyIterator::operator->() const
{
	return nullptr;
}

/*void EmptyIterator::operator=(const Iterator<Stmt>::Iter &R)
{
	// do nothing
}*/

bool EmptyIterator::operator==(const Iterator<Stmt>::Iter &R) const
{
	return false;
}

bool EmptyIterator::operator!=(const Iterator<Stmt>::Iter &R) const
{
	return false;
}

Iterator<Stmt>::Iter &EmptyIterator::operator--()
{
	return *this;
}

void EmptyIterator::replaceNode(Iterator<Stmt>::Ret R)
{
	// do nothing
}
