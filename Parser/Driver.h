//
// Created by gaojian on 19-4-6.
//

#ifndef CODEREFACTOR_DRIVER_H
#define CODEREFACTOR_DRIVER_H

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "parser.hpp"
#include "Scanner.h"

class OpenHelper;
class SourceLocation;
class ASTContext;

/// @brief To drive bison and flex
class Driver
{
public:
    Driver(OpenHelper &opener, ASTContext &context);
    ~Driver() = default;

    int parse ();

    /// @brief Called on an error occurred while parsing
    void error (const yy::location& l, const std::string& m);
    void error (const std::string& m);

    Scanner &getSacnner();
    OpenHelper &getOpenHelper();
    std::string &getOpenedFrom() const;

    // TODO: move these making methods away from driver
    /// @brief Make statements
	void makeCompoundStmt(unsigned stmtNumInBlock, yy::location &l, yy::location &r);
	void makeNullStmt(yy::location &l);
	void makeContinueStmt(yy::location &l);
	void makeBreakStmt(yy::location &l);
	void makeReturnStmt(yy::location &l, bool haveExpr = false);
	void makeIfStmt();
	void makeForStmt();
	void makeWhileStmt(yy::location &l);
	void makeDoStmt(yy::location &d, yy::location &w, yy::location &l);
	void makeSwitchStmt(yy::location &l);
	void makeCaseStmt(yy::location &l, yy::location &r, yy::location &c);
	void makeDefaultStmt(yy::location &d, yy::location &e);
	void makeLabelStmt(yy::location &l, yy::location &r);
	void makeGotoStmt(yy::location &l, yy::location &r);

	/// @brief Make expressions
	//void makeStmtExpr(yy::location &l, yy::location &r);
	void makeIntegerLiteral(int val, yy::location &l);
	void makeCharacterLiteral(unsigned val, yy::location &l);
	void makeFloatingLiteral(const float &val, yy::location &l);
	void makeStringLiteral(std::string str, yy::location &location);
	void makeUnaryOperator(int opc, yy::location &l);
	void makeBinaryOperator(int opc, yy::location &location);
	void makeCompoundAssignOperator(int opc, yy::location &location);
	void makeConditionalOperator();
	void makeParenExpr(yy::location &l, yy::location &r);
	void makeSizeofExpr(yy::location &l, yy::location &r, bool isSizeofType);
	void makeArraySubscripExpr(yy::location &l);
	void makeCallExpr(unsigned parameterNum, yy::location &l);
	void makeMemberExpr(int opc, yy::location &l);
	void makeCStyleCastExpr(yy::location &l, yy::location &r);

private:
	/// @brief Start scan
	void scan_begin();

	SourceLocation toSourceLocation(yy::location &location);
	ASTContext &m_context;

	bool trace_parsing;
	bool trace_scanning;
	OpenHelper &m_opener;
	Scanner m_scanner;
};

#endif //CODEREFACTOR_DRIVER_H
