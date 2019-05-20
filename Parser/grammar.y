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
	@$.initialize(&driver.getOpenedFrom());
};

// Enable tracing and verbose errors (which may be wrong!)
%define parse.trace
%define parse.error verbose

// Parser needs to know about the driver:
%code
{
	#include "Parser/Driver.h"
	#include "OpenHelper/OpenHelper.h"
	#define yylex driver.getSacnner().yylex
	#define DRIVER yy::Parser::driver
}

%define api.token.prefix {TOK_}

%token  ELLIPSIS		"..."
%token  RIGHT_SHIFT_ASSIGN	">>="	LEFT_SHIFT_ASSIGN	"<<="
%token  LEFT_SHIFT_OP		"<<"	RIGHT_SHIFT_OP		">>"
%token  INC_OP			"++"	DEC_OP			"--"
%token  POST_INC			POST_DEC			PRE_INC				PRE_DEC
%token  POINT_OP		"->"
%token  LE_OP			"<="	GE_OP			">="	EQ_OP			"=="	NE_OP			"!="
%token  NOT			"!"	GREATER_THAN		">"	LESS_THAN		"<"
%token  AND_OP			"&&"	OR_OP			"||"
%token  MUL_ASSIGN		"*="	DIV_ASSIGN		"/="	MOD_ASSIGN		"%="
%token  ADD_ASSIGN		"+="	SUB_ASSIGN		"-="
%token  AND_ASSIGN		"&="	XOR_ASSIGN		"^="	OR_ASSIGN		"|="	ASSIGN_OP		"="
%token  DOUBLE_POUND		"##"	POUND			"#"
%token  OPEN_PARENTHESE		"("	CLOSE_PARENTHESE	")"	OPEN_BRACKET		"["	CLOSE_BRACKET		"]"
%token  OPEN_CURLY		"{"	CLOSE_CURLY		"}"
%token  OPEN_BRACKET_UNUSUAL	"<:"	CLOSE_BRACKET_UNUSUAL	":>"	OPEN_CURLY_UNUSUAL	"<%"	CLOSE_CURLY_UNUSUAL	"%>"
%token  DOT			"."
%token  START			"*"	PLUS			"+"	SUBTRACTION		"-"	DIVISION		"/"	MODULO	"%"
%token  BITWISE_AND		"&"	BITWISE_XOR		"^"	BITWISE_OR		"|"	BITWISE_NOT		"~"
%token  QUESTION_MARK		"?"	COLON			":"	SEMICOLON		";"	COMMA			","
%token  SIZEOF			"sizeof"

%token KEYWORD_FOR	"for"		KEYWORD_SWITCH	"switch"	KEYWORD_CASE	"case"
%token KEYWORD_CONTINUE	"continue"	KEYWORD_BREAK	"break"		KEYWORD_RETURN	"return"
%token KEYWORD_DO	"do"		KEYWORD_WHILE	"while"

%token TYPE_BOOL	"bool"		TYPE_CHAR	"char"		TYPE_SHORT	"short"
%token TYPE_INT		"int"		TYPE_LONG	"long"		TYPE_LLONG	"long long"
%token TYPE_FLOAT	"float"		TYPE_DOUBLE	"double"
%token TYPE_SIGNED	"signed"	TYPE_UNSIGNED	"unsigned"

%token <std::string>IDENTIFIER
%token <int>INTEGER_LITERAL	<double>FLOATING_LITERAL	<std::string>STRING_LITERAL	<char>CHARACTER_LITERAL

%token  EOF	0	"eof"

%type <int>unary_op
%type <int>assignment_op
%type <std::string>type_name
%type <int>block_stmt

%printer { yyoutput << $$; }<*>
%%

start: | start statement;

statement:
	null_stmt |
	compound_stmt |
	execution_statement |
	decl_stmt
	;
null_stmt:
	";"				{ DRIVER.makeNullStmt(@1); }
	;

block_stmt:			{ $$ = 0; }
	| block_stmt statement	{ $$ = $1 + 1; }
	;
compound_stmt:
	"{" block_stmt "}"	{ DRIVER.makeCompoundStmt($2, @1, @3); }
	;

execution_statement:
	control_statment |
	expr_stmt 
	;
decl_stmt:
	type_name identifier_list ";" 			{ /*yy::Parser::driver.getOpener().getOutputStream() << $1 << " " << $2 << ";";*/ } |
	type_name IDENTIFIER "[" expression "]" ";"	{ /*yy::Parser::driver.getOpener().getOutputStream() << $1 << " " << $2 << "[" << $4->print() << "];";*/ } |
	type_name IDENTIFIER "(" argument_list ")" ";" 	{ /*yy::Parser::driver.getOpener().getOutputStream() << $1 << " " << $2 << "(" << ");";*/ } |
	;
identifier_list:
	identifier_initial				{ }
	| identifier_initial "," identifier_list	{ }
	;
identifier_initial:
	IDENTIFIER					{ }
	| IDENTIFIER "=" expression			{ }
	;

control_statment:
	if_stmt |
	for_stmt |
	while_stmt |
	do_stmt |
	continue_stmt |
	break_stmt |
	switch_stmt |
	goto_stmt |
	return_stmt 
	;
expr_stmt:
	expression ";" 					{ /*DRIVER.makeExprStmt(@2);*/ }
	;
argument_list: |
	argument_list "," type_name IDENTIFIER		{}
	;
if_stmt:
	;
for_stmt:
	;
while_stmt:
	;
do_stmt:
	;
continue_stmt:
	"continue" ";"					{ DRIVER.makeContinueStmt(@1); }
	;
break_stmt:
	"break" ";"					{ DRIVER.makeBreakStmt(@1); }
	;
switch_stmt:
	;
case_stmt:
	;
default_stmt:
	;
label_stmt:
	;
goto_stmt:
	;
return_stmt:
	"return" ";"					{ DRIVER.makeReturnStmt(@1, false); }
	| "return" expression ";"			{ DRIVER.makeReturnStmt(@1, true); }
	;


primary_expression: IDENTIFIER			{ /*$$ = ExpressionFactory::makeIdentity($1);*/ }
	| INTEGER_LITERAL			{ DRIVER.makeIntegerLiteral($1, @1); }
	| FLOATING_LITERAL			{ DRIVER.makeFloatingLiteral($1, @1); }
	| STRING_LITERAL			{ DRIVER.makeStringLiteral($1, @1); }
	| CHARACTER_LITERAL			{ DRIVER.makeCharactorLiteral($1, @1); }
	| "(" expression ")"			{ DRIVER.makeParenExpr(@1, @3); }
	;
expression: assignment_expression		{  }
	| expression "," assignment_expression	{ /*$$ = ExpressionFactory::makeCommaExpression($1, $3);*/ }
	;
assignment_expression: conditional_expression	{  }
	| unary_expression assignment_op assignment_expression	{ DRIVER.makeCompoundAssignOperator($2, @2); }
	;
unary_expression: postfix_expression		{  }
	| INC_OP unary_expression		{ DRIVER.makeUnaryOperator(yy::Parser::token::yytokentype::TOK_PRE_INC, @1); }
	| DEC_OP unary_expression		{ DRIVER.makeUnaryOperator(yy::Parser::token::yytokentype::TOK_PRE_DEC, @1); }
	| unary_op cast_expression		{ DRIVER.makeUnaryOperator($1, @1); }
	| SIZEOF unary_expression		{ /*DRIVER.makeSizeofExpr(@1, @2);*/ }
	| SIZEOF "(" type_name ")"		{ /*DRIVER.makeSizeofExpr(@1, @3, $3);*/ }
	;
postfix_expression: primary_expression				{  }
	| postfix_expression "[" expression "]"			{ /*$$ = ExpressionFactory::makeRandomAccessExpression($1, $3);*/ }
	| postfix_expression "(" ")"				{ /*$$ = ExpressionFactory::makeFunctionCallExpression($1);*/ }
	| postfix_expression "(" argument_expression_list ")"	{ /*$$ = ExpressionFactory::makeFunctionCallExpression($1, $3);*/ }
	| postfix_expression "." IDENTIFIER			{ DRIVER.makeBinaryOperator('.', @2); }
	| postfix_expression POINT_OP IDENTIFIER		{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_POINT_OP, @2); }
	| postfix_expression INC_OP				{ DRIVER.makeUnaryOperator(yy::Parser::token::yytokentype::TOK_POST_INC, @2); }
	| postfix_expression DEC_OP				{ DRIVER.makeUnaryOperator(yy::Parser::token::yytokentype::TOK_POST_DEC, @2); }
	;
unary_op: "&"	{ $$ = '&'; }
	| "*"	{ $$ = '*'; }
	| "+"	{ $$ = '+'; }
	| "-"	{ $$ = '-'; }
	| "~"	{ $$ = '~'; }
	| "!"	{ $$ = '!'; }
	;
cast_expression: unary_expression				{  }
	| "(" type_name ")" cast_expression			{ /*$$ = ExpressionFactory::makeCastExpression($2, $4);*/ }
	;
argument_expression_list: assignment_expression			{  }
	| argument_expression_list "," assignment_expression	{ /*$$ = ExpressionFactory::makeCommaExpression($1, $3);*/ }
	;
assignment_op: "="	{ $$ = '='; }
	| "*="		{ $$ = yy::Parser::token::yytokentype::TOK_MUL_ASSIGN; }
	| "/="		{ $$ = yy::Parser::token::yytokentype::TOK_DIV_ASSIGN; }
	| "%="		{ $$ = yy::Parser::token::yytokentype::TOK_MOD_ASSIGN; }
	| "+="		{ $$ = yy::Parser::token::yytokentype::TOK_ADD_ASSIGN; }
	| "-="		{ $$ = yy::Parser::token::yytokentype::TOK_SUB_ASSIGN; }
	| "<<="		{ $$ = yy::Parser::token::yytokentype::TOK_LEFT_SHIFT_ASSIGN; }
	| ">>="         { $$ = yy::Parser::token::yytokentype::TOK_RIGHT_SHIFT_ASSIGN; }
	| "&="		{ $$ = yy::Parser::token::yytokentype::TOK_AND_ASSIGN; }
	| "^="		{ $$ = yy::Parser::token::yytokentype::TOK_XOR_ASSIGN; }
	| "|="		{ $$ = yy::Parser::token::yytokentype::TOK_OR_ASSIGN; }
	;
conditional_expression: logical_or_expression					{  }
	| logical_or_expression "?" expression ":" conditional_expression	{ DRIVER.makeConditionalOperator(); }
	;
logical_or_expression: logical_and_expression			{  }
	| logical_or_expression "||" logical_and_expression	{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_OR_OP, @2); }
	;
logical_and_expression: inclusive_or_expression			{  }
	| logical_and_expression "&&" inclusive_or_expression	{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_AND_OP, @2); }
	;
inclusive_or_expression: exclusive_or_expression		{  }
	| inclusive_or_expression "|" exclusive_or_expression	{ DRIVER.makeBinaryOperator('|', @2); }
	;
exclusive_or_expression: and_expression				{  }
	| exclusive_or_expression "^" and_expression		{ DRIVER.makeBinaryOperator('^', @2); }
	;
and_expression: equality_expression				{  }
	| and_expression "&" equality_expression		{ DRIVER.makeBinaryOperator('&', @2); }
	;
equality_expression: relational_expression			{  }
	| equality_expression "==" relational_expression	{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_EQ_OP, @2); }
	| equality_expression "!=" relational_expression	{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_NE_OP, @2); }
	;
relational_expression: shift_expression				{  }
	| relational_expression "<" shift_expression		{ DRIVER.makeBinaryOperator('<', @2); }
	| relational_expression ">" shift_expression		{ DRIVER.makeBinaryOperator('>', @2); }
	| relational_expression "<=" shift_expression		{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_LE_OP, @2); }
	| relational_expression ">=" shift_expression		{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_GE_OP, @2); }
	;
shift_expression: additive_expression				{  }
	| shift_expression "<<" additive_expression		{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_LEFT_SHIFT_OP, @2); }
	| shift_expression ">>" additive_expression		{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_RIGHT_SHIFT_OP, @2); }
	;
additive_expression: multiplicative_expression			{  }
	| additive_expression "+" multiplicative_expression	{ DRIVER.makeBinaryOperator('+', @2); }
	| additive_expression "-" multiplicative_expression	{ DRIVER.makeBinaryOperator('-', @2); }
	;
multiplicative_expression: cast_expression			{  }
	| multiplicative_expression "*" cast_expression		{ DRIVER.makeBinaryOperator('*', @2); }
	| multiplicative_expression "/" cast_expression		{ DRIVER.makeBinaryOperator('/', @2); }
	| multiplicative_expression "%" cast_expression		{ DRIVER.makeBinaryOperator('%', @2); }
	;

type_name: "int"	{ $$ = "int"; }
	| "double"	{ $$ = "double"; }
	| "float"	{ $$ = "float"; }
	| "char"	{ $$ = "char"; }
	| "bool"	{ $$ = "bool"; }
	;
%%

void yy::Parser::error(const location_type& l,  const std::string& m)
{
    driver.error(l, m);
}
