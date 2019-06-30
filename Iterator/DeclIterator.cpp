#include "DeclIterator.h"

using std::list;
using std::shared_ptr;

DeclIterator::DeclIterator(list<shared_ptr<Decl>>::iterator iterator)
    : iterator_(iterator)
{}

/*Decl* DeclIterator::operator[](size_t idx)
{
    while (idx++) {
        ++iterator_;
    }
    return (*iterator_).get();
}*/

DeclIterator::Iter &DeclIterator::operator++()
{
    ++iterator_;
    return *this;
}

DeclIterator::Iter &DeclIterator::operator--()
{
	--iterator_;
	return *this;
}

/*DeclIterator::Iter &DeclIterator::operator+(size_t i)
{
    DeclIterator r(iterator_);
    while (i++) {
        ++r.iterator_;
    }
    return r;
}*/

/*signed DeclIterator::operator-(const Iter &R)
{
    signed t = 0;
    auto r =dynamic_cast<const DeclIterator &>(R);
    DeclIterator r1(iterator_);
    DeclIterator r2(r.iterator_);
    if(r1.iterator_ == r2.iterator_)
        return t;
    return t;
}*/

DeclIterator::Ret DeclIterator::operator*() const
{
    return *iterator_;
}

DeclIterator::Ret DeclIterator::operator->() const
{
    return *iterator_;
}

/*void DeclIterator::operator=(const Iter &R)
{
    auto r =dynamic_cast<const DeclIterator &>(R);
    iterator_=r.iterator_;
}*/

bool DeclIterator::operator==(const Iter &R) const
{
    auto r =dynamic_cast<const DeclIterator &>(R);
    return iterator_ == r.iterator_;
}

bool DeclIterator::operator!=(const Iter &R) const
{
    auto r =dynamic_cast<const DeclIterator &>(R);
    return iterator_ != r.iterator_;
}

void DeclIterator::replaceNode(Iterator<Decl>::Ret R)
{
	// TODO: this is not part of decl iterator
}

/*bool DeclIterator::operator>=(const Iter &R) const
{
    auto r =dynamic_cast<const DeclIterator &>(R);
    //return iterator_ >= r.iterator_;
    return false;
}

bool DeclIterator::operator>(const Iter &R) const
{
    auto r =dynamic_cast<const DeclIterator &>(R);
    //return iterator_ > r.iterator_;
    return false;
}*/

/*Iterator<Decl> &DeclIterator::operator-(size_t i)
{
    DeclIterator r(iterator_);
    while (i--) {
        --r.iterator_;
    }
    return r;
}*/
