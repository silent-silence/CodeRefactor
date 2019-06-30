#ifndef ITERATOR_STMTITERATOR_H
#define ITERATOR_STMTITERATOR_H

#include <list>
#include <array>
#include "Iterator.hpp"

class Stmt;

//typedef Iterator<Stmt> IteratorStmt;

/// @brief A wrapper of stmt iterator,
/// child_begin()/child_end() should return this, instead of a pointer or a reference of super class Iterator
class StmtIterator : public Iterator<Stmt> {
public:
	//StmtIterator();
	StmtIterator(std::shared_ptr<Iterator> &&iter);
	~StmtIterator() override = default;

	Iter &operator++() override;
	Iter &operator--() override;

	/*Iter &operator-(size_t i) override;
	Iter &operator+(size_t i) override;*/
	//Stmt* operator[](size_t idx) override;
	//signed operator-(const Iter &R) override;

	void replaceNode(Ret R) override;

	Ret operator*() const override;
	Ret operator->() const override;

	bool operator==(const Iter &R) const override;
	bool operator!=(const Iter &R) const override;
	//bool operator>=(const Iter &R) const override;
	//bool operator>(const Iter &R) const override;
private:
	std::shared_ptr<Iterator> iterator_;
};

/// @brief Provide an adapter to std::list<>::iterator
class ListIterator : public Iterator<Stmt> {
public:
	ListIterator(std::list<std::shared_ptr<Stmt>>::iterator iterator);
	~ListIterator() override = default;

	void replaceNode(Ret R) override;

	Iter &operator++() override;
	Iter &operator--() override;

	/*Iter &operator-(size_t i) override;
	Iterator &operator+(size_t i) override;*/

	//Stmt* operator[](size_t idx) override;
	//signed operator-(const Iter &R) override;

	Ret operator*() const override;
	Ret operator->() const override;

	bool operator==(const Iter &R) const override;
	bool operator!=(const Iter &R) const override;
	//bool operator>=(const Iter &R) const override;
	//bool operator>(const Iter &R) const override;
private:
	std::list<std::shared_ptr<Stmt>>::iterator iterator_;
};

/// @brief Provide an adapter to std::shared_ptr<>
class PtrIterator : public Iterator<Stmt> {
public:
	enum class PtrPosition {
		begin,
		end
	};
	PtrIterator(PtrPosition position, std::shared_ptr<Stmt> &iterator);
	~PtrIterator() override = default;

	void replaceNode(Ret R) override;

	Iter &operator++() override;
	Iter &operator--() override;

	/*Iterator<Stmt> &operator-(size_t i) override;
	Iterator<Stmt> &operator+(size_t i) override;*/

	//Stmt* operator[](size_t idx) override;
	//signed operator-(const Iterator<Stmt> &R) override;

	Ret operator*() const override;
	Ret operator->() const override;

	bool operator==(const Iter &R) const override;
	bool operator!=(const Iter &R) const override;
	//bool operator>=(const Iter &R) const override;
	//bool operator>(const Iter &R) const override;
private:
	PtrPosition m_position;
	std::shared_ptr<Stmt> &iterator_;
};

/// @brief Provide an adapter to no member iterator
class EmptyIterator : public Iterator<Stmt> {
public:
	EmptyIterator() = default;
	~EmptyIterator() override = default;

	void replaceNode(Ret R) override;

	Iter &operator++() override;
	Iter &operator--() override;

	/*Iter &operator-(size_t i) override;
	Iterator &operator+(size_t i) override;*/

	//Stmt* operator[](size_t idx) override;
	//signed operator-(const Iter &R) override;

	Ret operator*() const override;
	Ret operator->() const override;

	bool operator==(const Iter &R) const override;
	bool operator!=(const Iter &R) const override;
	//bool operator>=(const Iter &R) const override;
	//bool operator>(const Iter &R) const override;
};

/// @brief Provide an adapter to std::array<>
template<typename T>
class ArrayIterator : public Iterator<Stmt> {
public:
	ArrayIterator(T iterator)
			: iterator_{iterator}
	{}
	~ArrayIterator() override = default;

	Iterator &operator++() override
	{
		++iterator_;
		return *this;
	}

	Iter &operator--() override
	{
		--iterator_;
		return *this;
	}
	/*Iter &operator - (size_t i) override{
		ArrayIterator r(iterator_);
		while (i--) {
			--r.iterator_;
		}
		return r;
	}
	Iter &operator + (size_t i) override{
		ArrayIterator r(iterator_);
		while (i++) {
			++r.iterator_;
		}
		return r;
	}*/
	/*Stmt* operator[](size_t idx) override{
		while (idx++) {
			++iterator_;
		}
		return *iterator_;
	}
	signed operator - (const Iter &R) override{
		signed t = 0;
		ArrayIterator r1(iterator_);
		ArrayIterator r2(dynamic_cast<const ArrayIterator &>(R).iterator_);
		if(r1.iterator_ == r2.iterator_)
			return t;

		return t;
	}*/
	Ret operator*() const override
	{
		return *iterator_;
	}

	Ret operator->() const override
	{
		return *iterator_;
	}

	void replaceNode(Ret R) override
	{
		*iterator_ = R;
	}

	bool operator==(const Iter &R) const override
	{
		auto r = dynamic_cast<const ArrayIterator &>(R);
		return iterator_ == r.iterator_;
	}

	bool operator!=(const Iter &R) const override
	{
		auto r = dynamic_cast<const ArrayIterator &>(R);
		return iterator_ != r.iterator_;
	}
	/*bool operator >=(const Iter &R) const override{
		auto r =dynamic_cast<const ArrayIterator &>(R);
		//return iterator_ >= r.iterator_;
	}
	bool operator >(const Iter &R) const override{
		auto r =dynamic_cast<const ArrayIterator &>(R);
		//return iterator_ > r.iterator_;
	}*/
private:
	T iterator_;
};

#endif // ITERATOR_STMTITERATOR_H
