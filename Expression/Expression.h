#ifndef EXPRESSION_H
#define EXPRESSION_H

class Expression {
public:
	virtual ~Expression() = 0;
	virtual void print() = 0;
};

#endif