//
// Created by 17271 on 2019/4/2.
//

#ifndef PARSER_FUNCTIONCALL_H
#define PARSER_FUNCTIONCALL_H

#include "Operation.h"
#include <memory>

class ArgumentList;

class FunctionCall : public Operation {
public:
	FunctionCall(std::shared_ptr<Expression> expression, std::shared_ptr<ArgumentList> argumentList);
	~FunctionCall() override = default;

	void print() override;

private:
	std::shared_ptr<Expression> m_expression;
	std::shared_ptr<ArgumentList> m_argumentList;
};


#endif //PARSER_FUNCTIONCALL_H
