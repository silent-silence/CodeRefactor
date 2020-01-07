%{
	#include "parser.hpp"
    #include "Scanner.h"
    #include "Driver.h"
	#include <string.h>

	#define YY_NO_UNISTD_H
	
	#undef yywrap
	#define yywrap() 1

	static yy::location loc;
	#define YY_USER_ACTION  loc.columns(yyleng);
%}

%option noyywrap nounput batch noinput

letter		[a-zA-Z]
digit		[0-9]
string		({letter}?\"(\\.|[^\\"])*\")
float		(({digit}|[1-9]{digit})+\.{digit}*)
integer		({digit})+
identifier	([_a-zA-Z][_0-9a-zA-Z]*)
character   (\'.\')
escape_character (\'\\[0ntvabrf]\')

single_comment      (\/\/.*?)
multi_comment       (\/\*(.|\n)*?\*\/)
include_comment     ("#include\<"+({letter}|{letter}\.{letter})+"\>")

%%

%{
	loc.step();
%}

"void"          { return yy::Parser::make_TYPE_VOID(loc); }
"bool"			{ return yy::Parser::make_TYPE_BOOL(loc); }
"char"			{ return yy::Parser::make_TYPE_CHAR(loc); }
"short"         { return yy::Parser::make_TYPE_SHORT(loc); }
"int"			{ return yy::Parser::make_TYPE_INT(loc); }
"long"          { return yy::Parser::make_TYPE_LONG(loc); }
"float"			{ return yy::Parser::make_TYPE_FLOAT(loc); }
"double"		{ return yy::Parser::make_TYPE_DOUBLE(loc); }
"signed"        { return yy::Parser::make_TYPE_SIGNED(loc); }
"unsigned"      { return yy::Parser::make_TYPE_UNSIGNED(loc); }

"struct"        { return yy::Parser::make_KEYWORD_STRUCT(loc); }
"union"         { return yy::Parser::make_KEYWORD_UNION(loc); }
"enum"          { return yy::Parser::make_KEYWORD_ENUM(loc); }

"for"           { return yy::Parser::make_KEYWORD_FOR(loc); }
"switch"        { return yy::Parser::make_KEYWORD_SWITCH(loc); }
"case"          { return yy::Parser::make_KEYWORD_CASE(loc); }
"continue"		{ return yy::Parser::make_KEYWORD_CONTINUE(loc); }
"break"			{ return yy::Parser::make_KEYWORD_BREAK(loc); }
"return"		{ return yy::Parser::make_KEYWORD_RETURN(loc); }
"do"            { return yy::Parser::make_KEYWORD_DO(loc); }
"while"         { return yy::Parser::make_KEYWORD_WHILE(loc); }
"if"            { return yy::Parser::make_KEYWORD_IF(loc); }
"else"          { return yy::Parser::make_KEYWORD_ELSE(loc); }
"default"       { return yy::Parser::make_KEYWORD_DEFAULT(loc); }
"goto"          { return yy::Parser::make_KEYWORD_GOTO(loc); }

"sizeof"		{ return yy::Parser::make_SIZEOF(loc); }

"typedef"       { return yy::Parser::make_KEYWORD_TYPEDEF(loc); }
"extern"        { return yy::Parser::make_KEYWORD_EXTERN(loc); }
"static"        { return yy::Parser::make_KEYWORD_STATIC(loc); }
"auto"          { return yy::Parser::make_KEYWORD_AUTO(loc); }
"register"      { return yy::Parser::make_KEYWORD_REGISTER(loc); }

"const"         { return yy::Parser::make_KEYWORD_CONST(loc); }
"volatile"      { return yy::Parser::make_KEYWORD_VOLATILE(loc); }

{float}			{ return yy::Parser::make_FLOATING_LITERAL(atof(yytext), loc); }
{integer}		{ return yy::Parser::make_INTEGER_LITERAL(atoi(yytext), loc); }
{identifier}	{ return m_driver.identifier_or_type_name(yytext, loc); }

{include_comment} |
{single_comment} |
{multi_comment}  { return yy::Parser::make_COMMENT(yytext, loc);}

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
"&"				{ return yy::Parser::make_BITWISE_AND(loc); }
"^"				{ return yy::Parser::make_BITWISE_XOR(loc); }
"|"				{ return yy::Parser::make_BITWISE_OR(loc); }
"~"				{ return yy::Parser::make_BITWISE_NOT(loc); }
"?"				{ return yy::Parser::make_QUESTION_MARK(loc); }
":"				{ return yy::Parser::make_COLON(loc); }
";"				{ return yy::Parser::make_SEMICOLON(loc); }
"="				{ return yy::Parser::make_ASSIGN_OP(loc); }
","				{ return yy::Parser::make_COMMA(loc); }
"#"				{ return yy::Parser::make_POUND(loc); }

"\n"            { loc.lines(1); }

{string}        { return yy::Parser::make_STRING_LITERAL(yytext, loc); }
{character}     { return yy::Parser::make_CHARACTER_LITERAL(*(yytext + 1), loc); }
{escape_character} { switch(*(yytext+2)){
                        case '0': return yy::Parser::make_CHARACTER_LITERAL('\0', loc);
                        case 't': return yy::Parser::make_CHARACTER_LITERAL('\t', loc);
                        case 'n': return yy::Parser::make_CHARACTER_LITERAL('\n', loc);
                        case 'a': return yy::Parser::make_CHARACTER_LITERAL('\a', loc);
                        case 'b': return yy::Parser::make_CHARACTER_LITERAL('\b', loc);
                        case 'f': return yy::Parser::make_CHARACTER_LITERAL('\f', loc);
                        case 'r': return yy::Parser::make_CHARACTER_LITERAL('\r', loc);
                        case 'v': return yy::Parser::make_CHARACTER_LITERAL('\v', loc);
                    }
                    }

<<EOF>>    { return yy::Parser::make_EOF(loc); }

%%

int yyFlexLexer::yylex() {
    std::cerr << "'int yyFlexLexer::yylex()' should never be called." << std::endl;
    exit(1);
}
