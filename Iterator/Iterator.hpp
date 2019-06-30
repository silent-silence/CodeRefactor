#ifndef ITERATOR_ITERATOR_HPP
#define ITERATOR_ITERATOR_HPP

#include <memory>

/// @brief An interface of all iterator
/// @param Iter: type of iterator
/// @param Val: value of the iterator contains
template <typename T>
class Iterator
{
public:
	typedef Iterator<T> Iter;
	typedef std::shared_ptr<T> Ret;

    virtual Iter &operator++() = 0;
    virtual Iter &operator--() = 0;

    //virtual Iterator<T> &operator-(size_t i) = 0;
    //virtual Iterator<T> &operator+(size_t i) = 0;
    //virtual T* operator[](size_t idx) = 0;
	virtual void replaceNode(Ret R) = 0;

    //virtual signed operator-(const Iterator &R) = 0;
	virtual Ret operator*() const = 0;
	virtual Ret operator->() const = 0;

	//virtual void operator=(const Ret &R) = 0;
    virtual bool operator==(const Iter &R) const = 0;
    virtual bool operator!=(const Iter &R) const = 0;
    //virtual bool operator>=(const Iterator &R) const = 0;
    //virtual bool operator>(const Iterator &R) const = 0;

    virtual ~Iterator() = 0;
};

template <typename T>
Iterator<T>::~Iterator()
{}

#endif // ITERATOR_ITERATOR_HPP
