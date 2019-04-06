//
// Created by 17271 on 2019/4/2.
//

#ifndef PARSER_EXPRESSIONFACTORY_H
#define PARSER_EXPRESSIONFACTORY_H

#include "../Expression/Expression.h"
#include <memory>
#include <string>

class ExpressionFactory {
public:
	static std::shared_ptr<Expression> makeIdentity(std::string id);
	static std::shared_ptr<Expression> makeLiteral(int num);
	static std::shared_ptr<Expression> makeLiteral(double num);
	static std::shared_ptr<Expression> makeBinaryOperation(std::shared_ptr<Expression> firstExpression, int assignmentOpType, std::shared_ptr<Expression> secondExpression);
	static std::shared_ptr<Expression> makePrefixUnaryOperation(int unaryOpType, std::shared_ptr<Expression> expression);
	static std::shared_ptr<Expression> makeSizeofTypeOperation(std::string typeName);
	static std::shared_ptr<Expression> makeRandomAccessExpression(std::shared_ptr<Expression> expression, std::shared_ptr<Expression> accessPoint);
	static std::shared_ptr<Expression> makeFunctionCallExpression(std::shared_ptr<Expression> callable, std::shared_ptr<Expression> argumentList = nullptr);
	static std::shared_ptr<Expression> makeMemberAccessExpression(std::shared_ptr<Expression> object, int accessMethod, std::shared_ptr<Expression> member);
	static std::shared_ptr<Expression> makePostfixUnaryOperation(std::shared_ptr<Expression> expression, int unaryOp);
	static std::shared_ptr<Expression> makeCastExpression(std::string caseToType, std::shared_ptr<Expression> expression);
	static std::shared_ptr<Expression> makeTernaryOperation(std::shared_ptr<Expression> firstExpression, std::shared_ptr<Expression> secondExpression, std::shared_ptr<Expression> thirdExpression);
	static std::shared_ptr<Expression> makeCommaExpression(std::shared_ptr<Expression> firstExpression, std::shared_ptr<Expression> secondExpression);
};


#endif //PARSER_EXPRESSIONFACTORY_H
