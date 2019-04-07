#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

class Expression {
public:
	virtual ~Expression() = 0;
	virtual std::string print() = 0;
};

#endif