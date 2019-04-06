//
// Created by 17271 on 2019/4/2.
//

#ifndef PARSER_POSTFIXUNARYOPERATION_H
#define PARSER_POSTFIXUNARYOPERATION_H

#include "Operation.h"
#include <memory>
#include <string>

class PostfixUnaryOperation : public Operation {
public:
	enum PostfixUnary {
		Increment, 		// ++
		Decrement 		// --
	};
	PostfixUnaryOperation(std::shared_ptr<Expression> expression, PostfixUnary operatorType);
	~PostfixUnaryOperation() override = default;

	void print() override;

private:
	std::shared_ptr<Expression> m_expression;
	PostfixUnary m_operatorType;
};


#endif //PARSER_POSTFIXUNARYOPERATION_H
