//
// Created by 17271 on 2019/4/2.
//
#if 0
#ifndef PARSER_EXPRESSIONFACTORY_H
#define PARSER_EXPRESSIONFACTORY_H

#include "AST/Expr.h"
#include <memory>
#include <string>

class ExpressionFactory {
public:
	static std::shared_ptr<Expr> makeIdentity(std::string id);
	static std::shared_ptr<Expr> makeLiteral(int num);
	static std::shared_ptr<Expr> makeLiteral(double num);
	static std::shared_ptr<Expr> makeBinaryOperation(std::shared_ptr<Expr> firstExpression, int assignmentOpType, std::shared_ptr<Expr> secondExpression);
	static std::shared_ptr<Expr> makePrefixUnaryOperation(int unaryOpType, std::shared_ptr<Expr> expression);
	static std::shared_ptr<Expr> makeSizeofTypeOperation(std::string typeName);
	static std::shared_ptr<Expr> makeRandomAccessExpression(std::shared_ptr<Expr> expression, std::shared_ptr<Expr> accessPoint);
	static std::shared_ptr<Expr> makeFunctionCallExpression(std::shared_ptr<Expr> callable, std::shared_ptr<Expr> argumentList = nullptr);
	static std::shared_ptr<Expr> makeMemberAccessExpression(std::shared_ptr<Expr> object, int accessMethod, std::shared_ptr<Expr> member);
	static std::shared_ptr<Expr> makePostfixUnaryOperation(std::shared_ptr<Expr> expression, int unaryOp);
	static std::shared_ptr<Expr> makeCastExpression(std::string caseToType, std::shared_ptr<Expr> expression);
	static std::shared_ptr<Expr> makeTernaryOperation(std::shared_ptr<Expr> firstExpression, std::shared_ptr<Expr> secondExpression, std::shared_ptr<Expr> thirdExpression);
	static std::shared_ptr<Expr> makeCommaExpression(std::shared_ptr<Expr> firstExpression, std::shared_ptr<Expr> secondExpression);
};


#endif //PARSER_EXPRESSIONFACTORY_H

#endif