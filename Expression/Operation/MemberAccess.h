//
// Created by 17271 on 2019/4/2.
//

#ifndef PARSER_MEMBERACCESS_H
#define PARSER_MEMBERACCESS_H

#include "Operation.h"
#include <memory>
#include <string>

class Identity;

class MemberAccess : public Operation {
public:

	enum MemberOperator {
		StructureDereference,	// ->
		StructureReference		// .
	};

	MemberAccess(std::shared_ptr<Expression> expression, MemberOperator accessOperator, std::shared_ptr<Identity> identity);
	~MemberAccess() override = default;

	std::string print() override;

private:
	std::shared_ptr<Expression> m_expression;
	MemberOperator m_accessOperator;
	std::shared_ptr<Identity> m_identity;
};


#endif //PARSER_MEMBERACCESS_H
