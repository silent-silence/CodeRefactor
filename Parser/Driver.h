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
    OpenHelper &getOpener();
    std::string &getOpenedFrom() const;

    /// @brief Make statements
	void makeCompoundStmt(yy::location &l, yy::location &r);
	void makeIfStmt(yy::location &l, yy::location &r);
	void makeStmtExpr(yy::location &l, yy::location &r);
	void makeSimpleStmt();

	/// @brief Make expressions
	void makeIntegerLiteral(int val);
	void makeCharactorLiteral(unsigned val);
	void makeFloatingLiteral(const float &val);
	void makeStringLiteral(std::string str, yy::location &location);
	void makeUnaryOperator(int opc);
	void makeBinaryOperator(int opc, yy::location &location);
	void makeCompoundAssignOperator(int opc, yy::location &location);
	void makeConditionalOperator();

private:
	/// @brief Start scan
	void scan_begin();

	SourceLocation toSourceLocation(yy::location &location);
	unsigned m_stmtNumInBlock;
	ASTContext &m_context;

	bool trace_parsing;
	bool trace_scanning;
	OpenHelper &m_opener;
	Scanner m_scanner;
};

#endif //CODEREFACTOR_DRIVER_H
