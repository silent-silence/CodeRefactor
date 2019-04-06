//
// Created by 17271 on 2019/4/2.
//

#ifndef PARSER_CASTEXPRESSION_H
#define PARSER_CASTEXPRESSION_H

#include "Operation.h"
#include <memory>
#include <string>

class CastExpression : public Operation {
public:
	CastExpression(std::string castType, std::shared_ptr<Expression> expression);
	~CastExpression() override = default;

	void print() override;

private:
	std::string m_castType;
	std::shared_ptr<Expression> m_expression;
};


#endif //PARSER_CASTEXPRESSION_H
