//
// Created by 17271 on 2019/4/2.
//

#ifndef PARSER_COMMAEXPRESSION_H
#define PARSER_COMMAEXPRESSION_H

#include "Operation.h"
#include <memory>

class CommaExpression : public Operation {
public:
	CommaExpression(std::shared_ptr<Expression> firstExpression, std::shared_ptr<Expression> secondExpression);
	~CommaExpression() override = default;

	void print() override;

private:
	std::shared_ptr<Expression> m_preExpression;
	std::shared_ptr<Expression> m_lastExpression;
};


#endif //PARSER_COMMAEXPRESSION_H
