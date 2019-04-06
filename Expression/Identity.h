#ifndef IDENTITY_H
#define IDENTITY_H

#include "Expression.h"
#include <string>

class Identity : public Expression {
public:
	Identity(std::string id);
	~Identity() override = default;

	void print() override;

private:
	std::string m_id;
};

#endif