#ifndef TERNARY_OPERATOR_H
#define TERNARY_OPERATOR_H

#include "Operation.h"
#include <memory>

class TernaryOperation : public Operation {
public:
	TernaryOperation(std::shared_ptr<Expression> firstExpression, std::shared_ptr<Expression> secondExpression, std::shared_ptr<Expression> thirdExpression);
	~TernaryOperation() override = default;

	void print() override;

private:
	std::shared_ptr<Expression> m_firstExpression;
	std::shared_ptr<Expression> m_secondExpression;
	std::shared_ptr<Expression> m_thirdExpression;
};

#endif