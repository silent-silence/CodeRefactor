#ifndef OPERATION_H
#define OPERATION_H

#include "../Expression.h"

class Operation : public Expression {
public:
	~Operation() override = 0;
	std::string print() override = 0;
};

#endif
