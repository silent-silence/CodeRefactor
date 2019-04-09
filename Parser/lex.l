%{
	#include "parser.hpp"
	#include "Parser/Scanner.h"
	#include <string.h>

	#define YY_NO_UNISTD_H
	
	#undef yywrap
	#define yywrap() 1

	static yy::location loc;
	#define YY_USER_ACTION  loc.columns(yyleng);

	#define YY_NO_UNISTD_H
%}

%option noyywrap nounput batch noinput

letter		[a-zA-Z]
digit		[0-9]
string		((\"{letter}+\")|(\"{digit}+\")|(\"({letter}|{digit})+\"))
float		(({digit}|[1-9]{digit})+\.{digit}*)
integer		({digit})+
identifier	([_a-zA-Z][_0-9a-zA-Z]*)

%%

%{
	loc.step();
%}

"int"			{ return yy::Parser::make_TYPE_INT(loc); }
"int *"			{ return yy::Parser::make_TYPE_INT_POINT(loc); }
"double"		{ return yy::Parser::make_TYPE_DOUBLE(loc); }
"float"			{ return yy::Parser::make_TYPE_FLOAT(loc); }
"char"			{ return yy::Parser::make_TYPE_CHAR(loc); }
"bool"			{ return yy::Parser::make_TYPE_BOOL(loc); }
"for"           { return yy::Parser::make_STATMENT_FOR(loc); }
"switch"        { return yy::Parser::make_STATMENT_SWITCH(loc); }
"case"          { return yy::Parser::make_CASE(loc); }

"continue"		{ return yy::Parser::make_KEYWORD_CONTINUE(loc); }
"break"			{ return yy::Parser::make_KEYWORK_BREAK(loc); }
"return"		{ return yy::Parser::make_KEYWORD_RETURN(loc); }

"sizeof"		{ return yy::Parser::make_SIZEOF(loc); }

{float}			{ return yy::Parser::make_FLOAT(atof(yytext), loc); }
{integer}		{ return yy::Parser::make_INTEGER(atoi(yytext), loc); }
{identifier}	{ return yy::Parser::make_IDENTIFIER(yytext, loc); }

"..."   		{ return yy::Parser::make_ELLIPSIS(loc); }
"<<="         	{ return yy::Parser::make_RIGHT_SHIFT_ASSIGN(loc); }
">>="          	{ return yy::Parser::make_LEFT_SHIFT_ASSIGN(loc); }

"->"        	{ return yy::Parser::make_POINT_OP(loc); }
"++"    		{ return yy::Parser::make_INC_OP(loc); }
"--"    		{ return yy::Parser::make_DEC_OP(loc); }
"<<"    		{ return yy::Parser::make_LEFT_SHIFT_OP(loc); }
">>"    		{ return yy::Parser::make_RIGHT_SHIFT_OP(loc); }
"<="    		{ return yy::Parser::make_LE_OP(loc); }
">="    		{ return yy::Parser::make_GE_OP(loc); }
"=="    		{ return yy::Parser::make_EQ_OP(loc); }
"!="    		{ return yy::Parser::make_NE_OP(loc); }
"&&"	    	{ return yy::Parser::make_AND_OP(loc); }
"||"	    	{ return yy::Parser::make_OR_OP(loc); }
"*="    		{ return yy::Parser::make_MUL_ASSIGN(loc); }
"/="		    { return yy::Parser::make_DIV_ASSIGN(loc); }
"%="	    	{ return yy::Parser::make_MOD_ASSIGN(loc); }
"+="    		{ return yy::Parser::make_ADD_ASSIGN(loc); }
"-="    		{ return yy::Parser::make_SUB_ASSIGN(loc); }
"&="    		{ return yy::Parser::make_AND_ASSIGN(loc); }
"^="    		{ return yy::Parser::make_XOR_ASSIGN(loc); }
"|="    		{ return yy::Parser::make_OR_ASSIGN(loc); }
"##"    		{ return yy::Parser::make_DOUBLE_POUND(loc); }
"<:"			{ return yy::Parser::make_OPEN_BRACKET_UNUSUAL(loc); }
":>"			{ return yy::Parser::make_CLOSE_BRACKET_UNUSUAL(loc); }
"<%"			{ return yy::Parser::make_OPEN_CURLY_UNUSUAL(loc); }
"%>"			{ return yy::Parser::make_CLOSE_CURLY_UNUSUAL(loc); }

"["				{ return yy::Parser::make_OPEN_BRACKET(loc); }
"]" 		 	{ return yy::Parser::make_CLOSE_BRACKET(loc); }
"("				{ return yy::Parser::make_OPEN_PARENTHESE(loc); }
")"				{ return yy::Parser::make_CLOSE_PARENTHESE(loc); }
"{"				{ return yy::Parser::make_OPEN_CURLY(loc); }
"}"				{ return yy::Parser::make_CLOSE_CURLY(loc); }
"."				{ return yy::Parser::make_DOT(loc); }
"*"				{ return yy::Parser::make_START(loc); }
"+"				{ return yy::Parser::make_PLUS(loc); }
"-"				{ return yy::Parser::make_SUBTRACTION(loc); }
"/"				{ return yy::Parser::make_DIVISION(loc); }
"%"				{ return yy::Parser::make_MODULO(loc); }
"!"				{ return yy::Parser::make_NOT(loc); }
">"				{ return yy::Parser::make_GREATER_THAN(loc); }
"<"				{ return yy::Parser::make_LESS_THAN(loc); }
"&"				{ return  yy::Parser::make_BITWISE_AND(loc); }
"^"				{ return  yy::Parser::make_BITWISE_AND(loc); }
"|"				{ return  yy::Parser::make_BITWISE_AND(loc); }
"~"				{ return yy::Parser::make_BITWISE_NOT(loc); }
"?"				{ return yy::Parser::make_QUESTION_MARK(loc); }
":"				{ return yy::Parser::make_COLON(loc); }
";"				{ return  yy::Parser::make_SEMICOLON(loc); }
"="				{ return  yy::Parser::make_ASSIGN_OP(loc); }
","				{ return  yy::Parser::make_COMMA(loc); }
"#"				{ return  yy::Parser::make_POUND(loc); }

<<EOF>>    { return yy::Parser::make_EOF(loc); }

%%

int yyFlexLexer::yylex() {
    std::cerr << "'int yyFlexLexer::yylex()' should never be called." << std::endl;
    exit(1);
}
