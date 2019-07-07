//
// Created by 17271 on 2019/5/26.
//

#ifndef CODEREFACTOR_PRINTER_H
#define CODEREFACTOR_PRINTER_H

#include <memory>
#include <sstream>
#include <vector>

class OpenHelper;
class Stmt;
class QualType;
class DeclContext;
class Decl;

class Printer;

/// @brief Print type
class TypePrinter {
public:
	explicit TypePrinter(Printer &p);

	/// @brief Print type
	/// @example in 'int *a[1]', prefix is 'int *', infix is '*', postfix is '[1]'
	std::string printTypePrefix(std::shared_ptr<QualType> type);
	std::string printTypeInfix(std::shared_ptr<QualType> type);
	std::string printTypePostfix(std::shared_ptr<QualType> type);

private:
	std::string qualifierPrefix(std::shared_ptr<QualType> type);

	std::vector<std::string> builtinPrefix(std::shared_ptr<QualType> type);
	std::vector<std::string> pointerPrefix(std::shared_ptr<QualType> type);
	std::vector<std::string> arrayPrefix(std::shared_ptr<QualType> type);
	std::vector<std::string> functionPrefix(std::shared_ptr<QualType> type);
	std::vector<std::string> typedefPrefix(std::shared_ptr<QualType> type);
	std::vector<std::string> recordPrefix(std::shared_ptr<QualType> type);
	std::vector<std::string> enumPrefix(std::shared_ptr<QualType> type);

	std::string formatBuiltinPrefix(std::vector<std::string> str);
	std::string formatPointerPrefix(std::vector<std::string> str);
	std::string formatArrayPrefix(std::vector<std::string> str);
	std::string formatFunctionPrefix(std::vector<std::string> str);
	std::string formatTypedefPrefix(std::vector<std::string> str);
	std::string formatRecodePrefix(std::vector<std::string> str);
	std::string formatEnumPrefix(std::vector<std::string> str);

	std::vector<std::string> arrayWithExprPostfix(std::shared_ptr<QualType> type);
	std::vector<std::string> arrayWithoutExprPostfix(std::shared_ptr<QualType> type);
	std::vector<std::string> functionProtoPostfix(std::shared_ptr<QualType> type);
	std::vector<std::string> functionNoProtoPostfix(std::shared_ptr<QualType> type);

	std::string formatArrayWithExprPostfix(std::vector<std::string> str);
	std::string formatArrayWithoutExprPostfix(std::vector<std::string> str);
	std::string formatFunctionProtoPostfix(std::vector<std::string> str);
	std::string formatFunctionNoProtoPostfix(std::vector<std::string> str);

	Printer &printer;
};

/// @brief Print AST
class ASTPrinter {
public:
	explicit ASTPrinter(Printer &p);

	/// @brief Print all child in root
	std::string printAST(std::shared_ptr<Stmt> root);

private:
	/// @brief Print an expr as a stmt(add ';\n' at end)
	/// 		if s is a stmt type, just pass the parameter to print
	std::string formatExprAsStmt(std::string str, std::shared_ptr<Stmt> s);

	/// @brief Print stmts
	std::string processNullStmt(std::shared_ptr<Stmt> &s);
	std::string processCaseStmt(std::shared_ptr<Stmt> &s);
	std::string processDefaultStmt(std::shared_ptr<Stmt> &s);
	std::string processLabelStmt(std::shared_ptr<Stmt> &s);
	std::string processGotoStmt(std::shared_ptr<Stmt> &s);
	std::string processIndirectGotoStmt(std::shared_ptr<Stmt> &s);
	std::string processContinueStmt(std::shared_ptr<Stmt> &s);
	std::string processBreakStmt(std::shared_ptr<Stmt> &s);
	std::string processReturnStmt(std::shared_ptr<Stmt> &s);
	std::string processDeclStmt(std::shared_ptr<Stmt> &s);
	std::string processCommentStmt(std::shared_ptr<Stmt> &s);
	/// @note Only blocks can add '\n' for statements in it's block(NOT itself)
	std::string processCompoundStmt(std::shared_ptr<Stmt> &s);
	std::string processWhileStmt(std::shared_ptr<Stmt> &s);
	std::string processDoStmt(std::shared_ptr<Stmt> &s);
	std::string processForStmt(std::shared_ptr<Stmt> &s);
	std::string processIfStmt(std::shared_ptr<Stmt> &s);
	std::string processSwitchStmt(std::shared_ptr<Stmt> &s);


	/// @brief Print exprs
	std::string processRefExpr(std::shared_ptr<Stmt> &s);
	std::string processIntergerLiteral(std::shared_ptr<Stmt> &s);
	std::string processFloatingLiteral(std::shared_ptr<Stmt> &s);
	std::string processImaginaryLiteral(std::shared_ptr<Stmt> &s);
	std::string processStringLiteral(std::shared_ptr<Stmt> &s);
	std::string processCharacterLiteral(std::shared_ptr<Stmt> &s);
	std::string processParenExpr(std::shared_ptr<Stmt> &s);
	std::string processUnaryOperator(std::shared_ptr<Stmt> &s);
	std::string processSizeOfAlignOfExpr(std::shared_ptr<Stmt> &s);
	std::string processArraySubscriptExpr(std::shared_ptr<Stmt> &s);
	std::string processCallExpr(std::shared_ptr<Stmt> &s);
	std::string processBinaryOperator(std::shared_ptr<Stmt> &s);
	std::string processCompoundAssignOperator(std::shared_ptr<Stmt> &s);
	std::string processConditionalOperator(std::shared_ptr<Stmt> &s);
	std::string processImplicitCastExpr(std::shared_ptr<Stmt> &s);
	std::string processCStyleCastExpr(std::shared_ptr<Stmt> &s);
	std::string processVAArgExpr(std::shared_ptr<Stmt> &s);
	std::string processInitListExpr(std::shared_ptr<Stmt> &s);
	std::string processParenListExpr(std::shared_ptr<Stmt> &s);
	std::string processMemberExpr(std::shared_ptr<Stmt> &s);

	Printer &printer;

	template<typename T>
	std::string toString(T num)
	{
		std::ostringstream ss;
		ss << num;
		return ss.str();
	}
};

/// @brief Print top level decl context
class ContextPrinter {
public:
	explicit ContextPrinter(Printer &p);

	/// @brief Print all decls in context
	std::string printContext(std::shared_ptr<DeclContext> context);

	std::string printDecl(std::shared_ptr<Decl> decl);

private:
	std::string printVar(std::shared_ptr<Decl> decl);
	std::string printTypedef(std::shared_ptr<Decl> decl);
	std::string printRecord(std::shared_ptr<Decl> decl);
	std::string printFunction(std::shared_ptr<Decl> decl);
	std::string printComment(std::shared_ptr<Decl> decl);

	Printer &printer;
};

class Printer {

	friend class TypePrinter;
	friend class ASTPrinter;
	friend class ContextPrinter;

public:
	explicit Printer(OpenHelper &openHelper);

	/// @brief Print decl context or ast
	void print(std::shared_ptr<DeclContext> context);
	void print(std::shared_ptr<Stmt> root);

	/// @brief Indent control
	std::string oneIndentLessIndent();
	std::string indent();
	void forwardIndent();
	void backwardIndent();

#ifdef ENV_TEST
	void resetPrinter();
#endif

private:
	OpenHelper &m_openHelper;

	TypePrinter typePrinter;
	ASTPrinter astPrinter;
	ContextPrinter contextPrinter;

	int indentNum;
	const char indentCharacter = ' ';
};

#endif //CODEREFACTOR_PRINTER_H
