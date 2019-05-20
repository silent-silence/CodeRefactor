%skeleton "lalr1.cc"
%defines
%define api.parser.class { Parser }
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires
{
	#include <iostream>
	#include <string>
	#include <memory>

	class Driver;
}

%param { Driver &driver }

// Location tracking
%locations
%initial-action
{
    // Initialize the initial location.
    @$.begin.filename = @$.end.filename = &driver.getOpenedFrom();
};

// Enable tracing and verbose errors (which may be wrong!)
%define parse.trace
%define parse.error verbose

// Parser needs to know about the driver:
%code
{
	#include "Parser/Driver.h"
	#include "Opener/Opener.h"
	#define yylex driver.getSacnner().yylex
}

%define api.token.prefix {TOK_}

%token  ELLIPSIS                "..."
%token  RIGHT_SHIFT_ASSIGN      ">>="   LEFT_SHIFT_ASSIGN       "<<="
%token  LEFT_SHIFT_OP           "<<"    RIGHT_SHIFT_OP          ">>"
%token  INC_OP                  "++"    DEC_OP                  "--"
%token  POINT_OP                "->"
%token  LE_OP                   "<="    GE_OP                   ">="    EQ_OP               "=="    NE_OP               "!="
%token  NOT                     "!"     GREATER_THAN            ">"     LESS_THAN           "<"
%token  AND_OP                  "&&"    OR_OP                   "||"
%token  MUL_ASSIGN              "*="    DIV_ASSIGN              "/="    MOD_ASSIGN          "%="
%token  ADD_ASSIGN              "+="    SUB_ASSIGN              "-="
%token  AND_ASSIGN              "&="    XOR_ASSIGN              "^="    OR_ASSIGN           "|="    ASSIGN_OP           "="
%token  DOUBLE_POUND            "##"    POUND "#"
%token  OPEN_PARENTHESE         "("     CLOSE_PARENTHESE        ")"     OPEN_BRACKET        "["     CLOSE_BRACKET       "]"
%token  OPEN_CURLY              "{"     CLOSE_CURLY             "}"
%token  OPEN_BRACKET_UNUSUAL    "<:"    CLOSE_BRACKET_UNUSUAL   ":>"    OPEN_CURLY_UNUSUAL  "<%"    CLOSE_CURLY_UNUSUAL "%>"
%token  DOT                     "."
%token  START                   "*"     PLUS                    "+"     SUBTRACTION         "-"     DIVISION            "/"     MODULO "%"
%token  BITWISE_AND             "&"     BITWISE_XOR             "^"     BITWISE_OR          "|"     BITWISE_NOT         "~"
%token  QUESTION_MARK           "?"     COLON                   ":"     SEMICOLON           ";"     COMMA               ","
%token  SIZEOF                  "sizeof"

%token STATMENT_FOR         "for"       STATMENT_SWITCH "switch"    CASE            "case"
%token KEYWORD_CONTINUE     "continue"  KEYWORK_BREAK   "break"     KEYWORD_RETURN  "return"

%token TYPE_INT             "int"       TYPE_INT_POINT  "int *"
%token TYPE_DOUBLE          "double"	TYPE_FLOAT      "float"     TYPE_CHAR       "char"
%token TYPE_BOOL            "bool"

%token <std::string>IDENTIFIER  <int>INTEGER	<double>FLOAT

%token  EOF  0  "eof"

%type <int>unary_op
%type <int>assignment_op
%type <std::string>type_name

%printer { yyoutput << $$; }<*>
%%

start: | start statement;

statement: 
	null_statement | 
	compound_statement | 
	execution_statement | 
	declaration_statement
	;
null_statement:
	";"												{ yy::Parser::driver.getOpener().getOutputStream() << ";"; }
	;
compound_statement: 
	"{" statement "}" {}
	;
execution_statement:
	control_statment |
	expression_statement 
	;
declaration_statement:
	type_name IDENTIFIER ";" 						{ yy::Parser::driver.getOpener().getOutputStream() << $1 << " " << $2 << ";"; } |
	type_name IDENTIFIER "=" expression ";" 		{ yy::Parser::driver.getOpener().getOutputStream() << $1 << " " << $2 << "=" << $4->print() << ";"; } |
	type_name IDENTIFIER "[" expression "]" ";"		{ yy::Parser::driver.getOpener().getOutputStream() << $1 << " " << $2 << "[" << $4->print() << "];"; } |
	type_name IDENTIFIER "(" argument_list ")" ";" 	{ yy::Parser::driver.getOpener().getOutputStream() << $1 << " " << $2 << "(" << ");"; } |
	;
control_statment:
	if_statement |
	for_statement |
	while_statement |
	do_while_statement |
	continue_statement |
	break_statement |
	switch_statement |
	goto_statement |
	return_statement 
	;
expression_statement:
	expression ";" 									{ yy::Parser::driver.getOpener().getOutputStream() << $1->print(); }
	;
argument_list: |
	argument_list "," type_name IDENTIFIER			{}
	;
if_statement:
	;
for_statement:
	;
while_statement:
	;
do_while_statement:
	;
continue_statement:
	"continue" ";"
	;
break_statement:
	"break" ";"
	;
switch_statement:
	;
goto_statement:
	;
return_statement:
	"return" expression ";"
	;


primary_expression: IDENTIFIER				{ $$ = ExpressionFactory::makeIdentity($1); }
	| INTEGER								{ $$ = ExpressionFactory::makeLiteral($1); }
	| FLOAT									{ $$ = ExpressionFactory::makeLiteral($1); }
	| "(" expression ")"					{ $$ = $2; }
	;
expression: assignment_expression			{ $$ = $1; }
	| expression "," assignment_expression	{ $$ = ExpressionFactory::makeCommaExpression($1, $3); }
	;
assignment_expression: conditional_expression				{ $$ = $1; }
	| unary_expression assignment_op assignment_expression	{ $$ = ExpressionFactory::makeBinaryOperation($1, $2, $3); }
	;
unary_expression: postfix_expression	{ $$ = $1; }
	| INC_OP unary_expression			{ $$ = ExpressionFactory::makePrefixUnaryOperation(yy::Parser::token::yytokentype::TOK_INC_OP, $2); }
	| DEC_OP unary_expression			{ $$ = ExpressionFactory::makePrefixUnaryOperation(yy::Parser::token::yytokentype::TOK_DEC_OP, $2); }
	| unary_op cast_expression			{ $$ = ExpressionFactory::makePrefixUnaryOperation($1, $2); }
	| SIZEOF unary_expression			{ $$ = ExpressionFactory::makePrefixUnaryOperation(yy::Parser::token::yytokentype::TOK_SIZEOF, $2); }
	| SIZEOF "(" type_name ")"			{ $$ = ExpressionFactory::makeSizeofTypeOperation($3); }
	;
postfix_expression: primary_expression						{ $$ = $1; }
	| postfix_expression "[" expression "]"					{ $$ = ExpressionFactory::makeRandomAccessExpression($1, $3); }
	| postfix_expression "(" ")"							{ $$ = ExpressionFactory::makeFunctionCallExpression($1); }
	| postfix_expression "(" argument_expression_list ")"	{ $$ = ExpressionFactory::makeFunctionCallExpression($1, $3); }
	| postfix_expression "." IDENTIFIER						{ $$ = ExpressionFactory::makeMemberAccessExpression($1, '.', ExpressionFactory::makeIdentity($3)); }
	| postfix_expression POINT_OP IDENTIFIER				{ $$ = ExpressionFactory::makeMemberAccessExpression($1, yy::Parser::token::yytokentype::TOK_POINT_OP, ExpressionFactory::makeIdentity($3)); }
	| postfix_expression INC_OP								{ $$ = ExpressionFactory::makePostfixUnaryOperation($1, yy::Parser::token::yytokentype::TOK_INC_OP); }
	| postfix_expression DEC_OP								{ $$ = ExpressionFactory::makePostfixUnaryOperation($1, yy::Parser::token::yytokentype::TOK_DEC_OP); }
	;
unary_op: "&"	{ $$ = '&'; }
	| "*"		{ $$ = '*'; }
	| "+"		{ $$ = '+'; }
	| "-"		{ $$ = '-'; }
	| "~"		{ $$ = '~'; }
	| "!"		{ $$ = '!'; }
	;
cast_expression: unary_expression							{ $$ = $1; }
	| "(" type_name ")" cast_expression						{ $$ = ExpressionFactory::makeCastExpression($2, $4); }
	;
argument_expression_list: assignment_expression				{ $$ = $1; }
	| argument_expression_list "," assignment_expression	{ $$ = ExpressionFactory::makeCommaExpression($1, $3); }
	;
assignment_op: "="	{ $$ = '='; }
	| "*="			{ $$ = yy::Parser::token::yytokentype::TOK_MUL_ASSIGN; }
	| "/="			{ $$ = yy::Parser::token::yytokentype::TOK_DIV_ASSIGN; }
	| "%="			{ $$ = yy::Parser::token::yytokentype::TOK_MOD_ASSIGN; }
	| "+="			{ $$ = yy::Parser::token::yytokentype::TOK_ADD_ASSIGN; }
	| "-="			{ $$ = yy::Parser::token::yytokentype::TOK_SUB_ASSIGN; }
	| "<<="		    { $$ = yy::Parser::token::yytokentype::TOK_LEFT_SHIFT_ASSIGN; }
	| ">>="         { $$ = yy::Parser::token::yytokentype::TOK_RIGHT_SHIFT_ASSIGN; }
	| "&="			{ $$ = yy::Parser::token::yytokentype::TOK_AND_ASSIGN; }
	| "^="			{ $$ = yy::Parser::token::yytokentype::TOK_XOR_ASSIGN; }
	| "|="			{ $$ = yy::Parser::token::yytokentype::TOK_OR_ASSIGN; }
	;
conditional_expression: logical_or_expression							{ $$ = $1; }
	| logical_or_expression "?" expression ":" conditional_expression	{ $$ = ExpressionFactory::makeTernaryOperation($1, $3, $5); }
	;
logical_or_expression: logical_and_expression				{ $$ = $1; }
	| logical_or_expression "||" logical_and_expression	    { $$ = ExpressionFactory::makeBinaryOperation($1, yy::Parser::token::yytokentype::TOK_OR_OP, $3); }
	;
logical_and_expression: inclusive_or_expression				{ $$ = $1; }
	| logical_and_expression "&&" inclusive_or_expression	{ $$ = ExpressionFactory::makeBinaryOperation($1, yy::Parser::token::yytokentype::TOK_AND_OP, $3); }
	;
inclusive_or_expression: exclusive_or_expression			{ $$ = $1; }
	| inclusive_or_expression "|" exclusive_or_expression	{ $$ = ExpressionFactory::makeBinaryOperation($1, '|', $3); }
	;
exclusive_or_expression: and_expression						{ $$ = $1; }
	| exclusive_or_expression "^" and_expression			{ $$ = ExpressionFactory::makeBinaryOperation($1, '^', $3); }
	;
and_expression: equality_expression							{ $$ = $1; }
	| and_expression "&" equality_expression				{ $$ = ExpressionFactory::makeBinaryOperation($1, '&', $3); }
	;
equality_expression: relational_expression					{ $$ = $1; }
	| equality_expression "==" relational_expression		{ $$ = ExpressionFactory::makeBinaryOperation($1, yy::Parser::token::yytokentype::TOK_EQ_OP, $3); }
	| equality_expression "!=" relational_expression		{ $$ = ExpressionFactory::makeBinaryOperation($1, yy::Parser::token::yytokentype::TOK_NE_OP, $3); }
	;
relational_expression: shift_expression						{ $$ = $1; }
	| relational_expression "<" shift_expression			{ $$ = ExpressionFactory::makeBinaryOperation($1, '<', $3); }
	| relational_expression ">" shift_expression			{ $$ = ExpressionFactory::makeBinaryOperation($1, '>', $3); }
	| relational_expression "<=" shift_expression			{ $$ = ExpressionFactory::makeBinaryOperation($1, yy::Parser::token::yytokentype::TOK_LE_OP, $3); }
	| relational_expression ">=" shift_expression			{ $$ = ExpressionFactory::makeBinaryOperation($1, yy::Parser::token::yytokentype::TOK_GE_OP, $3); }
	;
shift_expression: additive_expression						{ $$ = $1; }
	| shift_expression "<<" additive_expression	            { $$ = ExpressionFactory::makeBinaryOperation($1, yy::Parser::token::yytokentype::TOK_LEFT_SHIFT_OP, $3); }
	| shift_expression ">>" additive_expression	            { $$ = ExpressionFactory::makeBinaryOperation($1, yy::Parser::token::yytokentype::TOK_RIGHT_SHIFT_OP, $3); }
	;
additive_expression: multiplicative_expression				{ $$ = $1; }
	| additive_expression "+" multiplicative_expression		{ $$ = ExpressionFactory::makeBinaryOperation($1, '+', $3); }
	| additive_expression "-" multiplicative_expression		{ $$ = ExpressionFactory::makeBinaryOperation($1, '-', $3); }
	;
multiplicative_expression: cast_expression					{ $$ = $1; }
	| multiplicative_expression "*" cast_expression			{ $$ = ExpressionFactory::makeBinaryOperation($1, '*', $3); }
	| multiplicative_expression "/" cast_expression			{ $$ = ExpressionFactory::makeBinaryOperation($1, '/', $3); }
	| multiplicative_expression "%" cast_expression			{ $$ = ExpressionFactory::makeBinaryOperation($1, '%', $3); }
	;


type_name: "int"	{ $$ = "int"; }
	| "int *"		{ $$ = "int *"; }
	| "double"		{ $$ = "double"; }
	| "float"		{ $$ = "float"; }
	| "char"		{ $$ = "char"; }
	| "bool"		{ $$ = "bool"; }
	;
%%

void yy::Parser::error(const location_type& l,  const std::string& m)
{
    driver.error(l, m);
}
