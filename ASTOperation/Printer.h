//
// Created by 17271 on 2019/5/26.
//

#ifndef CODEREFACTOR_PRINTER_H
#define CODEREFACTOR_PRINTER_H

#include <memory>
#include <sstream>

class OpenHelper;
class Stmt;
class Type;

class Printer {
public:
	Printer(OpenHelper &openHelper);

	void print(std::shared_ptr<Stmt> root);

#ifdef ENV_TEST
	void resetPrinter();
#endif

private:
	//void childIteration(std::shared_ptr<Stmt> &root);
	/// @brief Out put a expr as a stmt(add ';\n' at end)
	/// 		if s is a stmt type, just pass the parameter
	void formatExprAsStmt(std::shared_ptr<Stmt> s);
	// TODO: move this into Type
	std::string getTypeName(std::shared_ptr<Type> type) const;

	void processNullStmt(std::shared_ptr<Stmt> &s);
	void processCompoundStmt(std::shared_ptr<Stmt> &s);
	void processCaseStmt(std::shared_ptr<Stmt> &s);
	void processDefaultStmt(std::shared_ptr<Stmt> &s);
	void processLabelStmt(std::shared_ptr<Stmt> &s);
	void processIfStmt(std::shared_ptr<Stmt> &s, bool isElseCond = false);
	void processSwitchStmt(std::shared_ptr<Stmt> &s);
	void processWhileStmt(std::shared_ptr<Stmt> &s);
	void processDoStmt(std::shared_ptr<Stmt> &s);
	void processForStmt(std::shared_ptr<Stmt> &s);
	void processGotoStmt(std::shared_ptr<Stmt> &s);
	void processIndirectGotoStmt(std::shared_ptr<Stmt> &s);
	void processContinueStmt(std::shared_ptr<Stmt> &s);
	void processBreakStmt(std::shared_ptr<Stmt> &s);
	void processReturnStmt(std::shared_ptr<Stmt> &s);
	void processDeclStmt(std::shared_ptr<Stmt> &s);

	void processRefExpr(std::shared_ptr<Stmt> &s);
	void processIntergerLiteral(std::shared_ptr<Stmt> &s);
	void processFloatingLiteral(std::shared_ptr<Stmt> &s);
	void processImaginaryLiteral(std::shared_ptr<Stmt> &s);
	void processStringLiteral(std::shared_ptr<Stmt> &s);
	void processCharacterLiteral(std::shared_ptr<Stmt> &s);
	void processParenExpr(std::shared_ptr<Stmt> &s);
	void processUnaryOperator(std::shared_ptr<Stmt> &s);
	void processSizeOfAlignOfExpr(std::shared_ptr<Stmt> &s);
	void processArraySubscriptExpr(std::shared_ptr<Stmt> &s);
	void processCallExpr(std::shared_ptr<Stmt> &s);
	void processBinaryOperator(std::shared_ptr<Stmt> &s);
	void processCompoundAssignOperator(std::shared_ptr<Stmt> &s);
	void processConditionalOperator(std::shared_ptr<Stmt> &s);
	void processImplicitCastExpr(std::shared_ptr<Stmt> &s);
	void processCStyleCastExpr(std::shared_ptr<Stmt> &s);
	void processVAArgExpr(std::shared_ptr<Stmt> &s);
	void processInitListExpr(std::shared_ptr<Stmt> &s);
	void processParenListExpr(std::shared_ptr<Stmt> &s);

	OpenHelper &m_openHelper;
	bool stmtInOneLine;
	int m_tabNum;
	const char tabType = ' ';

	template<typename T>
	std::string toString(T num) {
		std::ostringstream ss;
		ss << num;
		return ss.str();
	}

};


#endif //CODEREFACTOR_PRINTER_H
