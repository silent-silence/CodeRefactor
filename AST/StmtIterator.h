#ifndef STMTITERATOR_H
#define STMTITERATOR_H

#include <list>
#include <memory>

class Stmt;
class StmtIterator;
class Iterator;
class ListIterator;
class PtrIterator;
template<typename T>class ArrayIterator;

/// @interface
/// @brief The interface of stmt iterator, use this to iterate stmt child
class Iterator
{
public:
    virtual Iterator &operator++() = 0;
    virtual Iterator &operator -(size_t i) = 0;
    virtual Iterator &operator +(size_t i) = 0;
    virtual std::shared_ptr<Stmt> operator[](size_t idx) = 0;
    virtual signed operator -(const Iterator &R) = 0;

    virtual std::shared_ptr<Stmt> operator*() const = 0;
    virtual std::shared_ptr<Stmt> operator->() const = 0;

    virtual void operator =(std::shared_ptr<Stmt> stmt) = 0;
    virtual bool operator ==(const Iterator &R) const = 0;
    virtual bool operator !=(const Iterator &R) const = 0;
    virtual bool operator >=(const Iterator &R) const = 0;
    virtual bool operator >(const Iterator &R) const = 0;

    virtual ~Iterator() = 0;
};

/// @brief A wrapper of stmt iterator,
/// child_begin()/child_end() should return this, instead of a pointer or a reference of super class Iterator
class StmtIterator : public Iterator
{
public:
    StmtIterator(std::shared_ptr<Iterator> iterator);

    Iterator &operator++() override;

    Iterator &operator-(size_t i) override;
    Iterator &operator+(size_t i) override;

    std::shared_ptr<Stmt> operator[](size_t idx) override;

    signed operator-(const Iterator &R) override;

    void operator=(std::shared_ptr<Stmt> stmt) override;

    std::shared_ptr<Stmt> operator*() const  override;
    std::shared_ptr<Stmt> operator->() const override;

    bool operator==(const Iterator &R) const override;
    bool operator!=(const Iterator &R) const override;
    bool operator>=(const Iterator &R) const override;
    bool operator>(const Iterator &R) const override;
private:
    std::shared_ptr<Iterator> iterator_;
};

/// @brief Provide an adapter to std::list<>::iterator
class ListIterator : public Iterator
{
public:
    ListIterator(std::list<std::shared_ptr<Stmt>>::iterator iterator);

    Iterator &operator++() override;

    Iterator &operator-(size_t i) override;
    Iterator &operator+(size_t i) override;

    std::shared_ptr<Stmt> operator[](size_t idx) override;
    signed operator-(const Iterator &R) override;

    std::shared_ptr<Stmt> operator*() const override;
    std::shared_ptr<Stmt> operator->() const override;

    void operator=(std::shared_ptr<Stmt> stmt) override;

    bool operator==(const Iterator &R) const override;
    bool operator!=(const Iterator &R) const override;
    bool operator>=(const Iterator &R) const override;
    bool operator>(const Iterator &R) const override;
private:
    std::list<std::shared_ptr<Stmt>>::iterator iterator_;
};

/// @brief Provide an adapter to std::shared_ptr<>
class PtrIterator : public Iterator
{
public:
    PtrIterator(std::shared_ptr<Stmt> iterator);

    Iterator &operator++() override;

    Iterator &operator-(size_t i) override;
    Iterator &operator+(size_t i) override;

    std::shared_ptr<Stmt> operator[](size_t idx) override;
    signed operator-(const Iterator &R) override;

    std::shared_ptr<Stmt> operator*() const override;
    std::shared_ptr<Stmt> operator->() const override;

    void operator=(std::shared_ptr<Stmt> stmt) override;

    bool operator==(const Iterator &R) const override;
    bool operator!=(const Iterator &R) const override;
    bool operator>=(const Iterator &R) const override;
    bool operator>(const Iterator &R) const override;
private:
    std::shared_ptr<Stmt> iterator_;
};

/// @brief Provide an adapter to std::array<>
template <typename T>
class ArrayIterator : public Iterator
{
public:
    ArrayIterator(T iterator)
        : iterator_{iterator}
    {}
    Iterator &operator++() override{
        ++iterator_;
        return *this;
    }
    Iterator &operator - (size_t i) override{
        ArrayIterator r(iterator_);
        while (i--) {
            --r.iterator_;
        }
        return r;
    }
    Iterator &operator + (size_t i) override{
        ArrayIterator r(iterator_);
        while (i++) {
            ++r.iterator_;
        }
        return r;
    }
    std::shared_ptr<Stmt> operator[](size_t idx) override{
        while (idx++) {
            ++iterator_;
        }
        return *iterator_;
    }
    signed operator - (const Iterator &R) override{
        signed t = 0;
        ArrayIterator r1(iterator_);
        ArrayIterator r2(dynamic_cast<const ArrayIterator &>(R).iterator_);
        if(r1.iterator_ == r2.iterator_)
            return t;

        return t;
    }
    std::shared_ptr<Stmt> operator*() const override{
        return *iterator_;
    }
    std::shared_ptr<Stmt> operator->() const override{
        return *iterator_;
    }
    void operator =(std::shared_ptr<Stmt> stmt) {
        *iterator_=stmt;
    }
    bool operator ==(const Iterator &R) const override{
        auto r =dynamic_cast<const ArrayIterator &>(R);
        return iterator_ == r.iterator_;
    }
    bool operator !=(const Iterator &R) const override{
        auto r =dynamic_cast<const ArrayIterator &>(R);
        return iterator_ != r.iterator_;
    }
    bool operator >=(const Iterator &R) const override{
        auto r =dynamic_cast<const ArrayIterator &>(R);
        //return iterator_ >= r.iterator_;
    }
    bool operator >(const Iterator &R) const override{
        auto r =dynamic_cast<const ArrayIterator &>(R);
        //return iterator_ > r.iterator_;
    }
private:
    T iterator_;
};

#endif // STMTITERATOR_H
