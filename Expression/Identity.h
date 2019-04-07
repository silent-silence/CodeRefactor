#ifndef IDENTITY_H
#define IDENTITY_H

#include "Expression.h"
#include <string>

class Identity : public Expression {
public:
	Identity(std::string id);
	~Identity() override = default;

	std::string print() override;

private:
	std::string m_id;
};

#endif