//
// Created by 17271 on 2019/5/26.
//

#ifndef CODEREFACTOR_PRINTER_H
#define CODEREFACTOR_PRINTER_H

#include <memory>
#include <sstream>

class OpenHelper;
class Stmt;
class QualType;
class DeclContext;
class Decl;

// TODO: Use decorator refactor Printer, returning vector of string,
// TODO: reformat strings in higher level(such as if stmt).
// TODO: for example: 'if stmt', it's sub statement just return a string of expr or stmt,
// TODO: if will add indent or '\n' in condition
namespace Printer {

	/// @brief Indent control
	static std::string indent();
	static int indentNum = 0;
	static const char indentCharacter = ' ';

#ifdef ENV_TEST
	void resetPrinter();
#endif

	class ASTPrinter;
	class TypePrinter;

	/// @brief Print type
	class TypePrinter {
	public:
		TypePrinter(OpenHelper &stream, ASTPrinter &printer);

		/// @brief Print type
		/// @example in 'int *a[1]', prefix is int, infix is *, postfix is [1]
		void printTypePrefix(std::shared_ptr<QualType> type);
		void printTypeInfix(std::shared_ptr<QualType> type);
		void printTypePostfix(std::shared_ptr<QualType> type);

	private:
		void recordPrefix(std::shared_ptr<QualType> type);
		void enumPrefix(std::shared_ptr<QualType> type);

		void arrayWithExprPostfix(std::shared_ptr<QualType> type);
		void arrayWithoutExprPostfix(std::shared_ptr<QualType> type);
		void functionProtoPostfix(std::shared_ptr<QualType> type);
		void functionNoProtoPostfix(std::shared_ptr<QualType> type);

		OpenHelper &m_openHelper;
		ASTPrinter &ast;
	};

	/// @brief Print AST
	class ASTPrinter {
	public:
		explicit ASTPrinter(OpenHelper &stream);

		/// @brief Print all child in root
		void printAST(std::shared_ptr<Stmt> root);

	private:
		/// @brief Print an expr as a stmt(add ';\n' at end)
		/// 		if s is a stmt type, just pass the parameter to print
		void formatExprAsStmt(std::shared_ptr<Stmt> s);

		/// @brief Print stmts
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
        void processCommentStmt(std::shared_ptr<Stmt> &s);

		/// @brief Print exprs
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
		TypePrinter m_typePrinter;
		bool stmtInOneLine;
		bool noIntent;

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
		explicit ContextPrinter(OpenHelper &stream);

		/// @brief Print all decls in context
		void printContext(std::shared_ptr<DeclContext> context);

	private:
		void printVar(std::shared_ptr<Decl> decl);
		void printTypedef(std::shared_ptr<Decl> decl);
		void printRecord(std::shared_ptr<Decl> decl);
		void printFunction(std::shared_ptr<Decl> decl);

		OpenHelper &m_openHelper;
		ASTPrinter astPrinter;
		TypePrinter typePrinter;
	};
}

#endif //CODEREFACTOR_PRINTER_H
