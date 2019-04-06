//
// Created by 17271 on 2019/4/2.
//

#ifndef PARSER_RANDOMACCESS_H
#define PARSER_RANDOMACCESS_H

#include "Operation.h"
#include <memory>

class RandomAccess : public Operation {
public:
	RandomAccess(std::shared_ptr<Expression> expression, std::shared_ptr<Expression> accessPoint);
	~RandomAccess() override = default;

	void print() override;

private:
	std::shared_ptr<Expression> m_expression;
	std::shared_ptr<Expression> m_accessPoint;
};


#endif //PARSER_RANDOMACCESS_H
