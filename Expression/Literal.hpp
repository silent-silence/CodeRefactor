#ifndef LITERAL_H
#define LITERAL_H

#include <iostream>
#include "Expression.h"

template<typename T>
class Literal : public Expression {
public:
	Literal(T i) : m_value{i}
	{}

	~Literal() override = default;

	void print() override {
		std::cout << "(Literal)" << m_value;
	}

private:
	T m_value;
};

#endif
