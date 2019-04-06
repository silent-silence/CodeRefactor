//
// Created by gaojian on 19-4-5.
//

#ifndef CODEREFACTOR_SCANNER_H
#define CODEREFACTOR_SCANNER_H

#undef yyFlexLexer
#include <FlexLexer.h>
#include "parser.hpp"

#ifndef YY_DECL
#define	YY_DECL		\
    yy::Parser::symbol_type	Scanner::yylex(Driver &driver)
#endif

class Driver;

class Scanner : public yyFlexLexer {
public:
	using yyFlexLexer::yyFlexLexer;
	//~Scanner() override = default;

	yy::Parser::symbol_type yylex(Driver &driver);
};

#endif //CODEREFACTOR_SCANNER_H
