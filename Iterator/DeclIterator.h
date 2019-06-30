#ifndef DECLITERATOR_H
#define DECLITERATOR_H

#include <list>
#include "Iterator.hpp"

class Decl;

class DeclIterator : public Iterator<Decl>
{
public:
    DeclIterator(std::list<std::shared_ptr<Decl>>::iterator iterator);
    ~DeclIterator() override = default;

    Iter &operator++() override;
	Iter &operator--() override;

	/*Iterator &operator-(size_t i) override;
    Iterator &operator+(size_t i) override;*/

    /*Decl* operator[](size_t idx) override;
    signed operator-(const Iter &R) override;*/

	Ret operator*() const override;
	Ret operator->() const override;

	void replaceNode(Ret R);
    //void operator=(const Iter &R) override;

    bool operator==(const Iter &R) const override;
    bool operator!=(const Iter &R) const override;
    /*bool operator>=(const Iter &R) const override;
    bool operator>(const Iter &R) const override;*/

private:
    std::list<std::shared_ptr<Decl>>::iterator iterator_;
};

#endif // DECLITERATOR_H
