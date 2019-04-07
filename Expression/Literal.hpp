#ifndef LITERAL_H
#define LITERAL_H

#include <iostream>
#include <sstream>
#include "Expression.h"

template<typename T>
class Literal : public Expression {
public:
	Literal(T i) : m_value{i}
	{}

	~Literal() override = default;

	std::string print() override {
		std::ostringstream os;
		os << "(Literal)" << m_value;
		return os.str();
	}

private:
	T m_value;
};

#endif
