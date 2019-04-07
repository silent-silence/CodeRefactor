//
// Created by 17271 on 2019/4/2.
//

#ifndef PARSER_ARGUMENTLIST_H
#define PARSER_ARGUMENTLIST_H

#include "Expression.h"
#include <memory>

class ArgumentList : public Expression {
public:
	ArgumentList(std::shared_ptr<ArgumentList> argumentList, std::shared_ptr<Expression> expression);
	~ArgumentList() override = default;

	std::string print() override;

private:
	std::shared_ptr<ArgumentList> m_argumentList;
	std::shared_ptr<Expression> m_expression;
};


#endif //PARSER_ARGUMENTLIST_H
