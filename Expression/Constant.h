#ifndef CONSTANT_H
#define CONSTANT_H

#include "Expression.h"
#include <string>

class Constant : public Expression {
public:
	Constant(std::string name);
	~Constant() override = default;

	std::string print() override;

private:
	std::string m_name;
};

#endif