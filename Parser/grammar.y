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
	#include "Parser/YaccAdapter.h"
	#define yylex driver.getSacnner().yylex
	#define DRIVER yy::Parser::driver.getAdapter()
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
%token KEYWORD_DO	"do"		KEYWORD_WHILE	"while"		KEYWORD_IF	"if"
%token KEYWORD_ELSE	"else"		KEYWORD_DEFAULT	"default"

%token TYPE_BOOL	"bool"		TYPE_CHAR	"char"		TYPE_SHORT	"short"
%token TYPE_INT		"int"		TYPE_LONG	"long"		TYPE_VOID	"void"
%token TYPE_FLOAT	"float"		TYPE_DOUBLE	"double"
%token TYPE_SIGNED	"signed"	TYPE_UNSIGNED	"unsigned"

%token KEYWORD_STRUCT	"struct"	KEYWORD_UNION	"union"

%token KEYWORD_TYPEDEF	"typedef"	KEYWORD_EXTERN	"extern"
%token KEYWORD_STATIC	"static"	KEYWORD_AUTO	"auto"
%token KEYWORD_REGISTER	"register"

%token KEYWORD_CONST	"const"		KEYWORD_VOLATILE	"volatile"

%token <std::string>IDENTIFIER
%token <int>INTEGER_LITERAL	<double>FLOATING_LITERAL	<std::string>STRING_LITERAL	<char>CHARACTER_LITERAL

%token  EOF	0	"eof"

%type <int>unary_op
%type <int>assignment_op
%type <int>block_stmt
%type <int>argument_expression_list

%printer { yyoutput << $$; }<*>
%%

start: | start statement;


/* Statements */
statement
	: null_stmt
	| compound_stmt
	| expr_stmt
	| if_stmt
	| for_stmt
	| while_stmt
	| do_stmt
	| continue_stmt
	| break_stmt
	| switch_stmt
	| case_stmt
	| default_stmt
	| label_stmt
	| goto_stmt
	| return_stmt
	| decl_stmt
	;
null_stmt
	: ";"							{ DRIVER.makeNullStmt(@1); }
	;
compound_stmt
	: "{" block_stmt "}"					{ DRIVER.makeCompoundStmt($2, @1, @3); }
	;
block_stmt
	:							{ $$ = 0; }
	| block_stmt statement					{ $$ = $1 + 1; }
	;
expr_stmt
	: expression ";" 					{ /* Do nothing */ }
	;
if_stmt
	: "if" "(" expression ")" statement			{ /*DRIVER.makeIfStmt();*/ }
	| "if" "(" expression ")" statement "else" statement	{ /*DRIVER.makeIfStmt();*/ }
	;
for_stmt
	: "for" "(" expr_or_null_expr expr_or_null_expr ")" statement			{ /*DIRVER.makeForStmt();*/ }
	| "for" "(" expr_or_null_expr expr_or_null_expr expression ")" statement	{ /*DIRVER.makeForStmt();*/ }
	;
expr_or_null_expr
	: expr_stmt
	| null_stmt
	;
while_stmt
	: "while" "(" expression ")" statement			{ DRIVER.makeWhileStmt(@1); }
	;
do_stmt
	: "do" statement "while" "(" expression ")"		{ DRIVER.makeDoStmt(@1, @3, @4); }
	;
continue_stmt
	: "continue" ";"					{ DRIVER.makeContinueStmt(@1); }
	;
break_stmt
	: "break" ";"						{ DRIVER.makeBreakStmt(@1); }
	;
switch_stmt
	: "switch" "(" expression ")" statement			{ DRIVER.makeSwitchStmt(@1); }
	;
case_stmt
	: "case" integer_constant_expression ":" statement	{ DRIVER.makeCaseStmt(@1, @2, @3); }
	;
default_stmt
	: "default" ":" statement				{ DRIVER.makeDefaultStmt(@1, @2); }
	;
label_stmt
	: IDENTIFIER ":" statement				{ /*DRIVER.makeLabelStmt(@1, @2);*/ }
	;
goto_stmt
	: "goto" IDENTIFIER					{ /*DRIVER.makeGotoStmt(@1, @2);*/ }
	;
return_stmt
	: "return" ";"						{ DRIVER.makeReturnStmt(@1, false); }
	| "return" expression ";"				{ DRIVER.makeReturnStmt(@1, true); }
	;


/* Declarations */
decl_stmt
	: declaration_specifiers ";"				{ DRIVER.makeDeclStmt(@1, @2); }
	| declaration_specifiers init_declarator_list ";"	{ DRIVER.makeDeclStmt(@1, @3); }
	;
declaration_specifiers
	: type_specifier					{ DRIVER.makeBuiltinType(); }
	| type_specifier declaration_specifiers
	;
init_declarator_list
	: init_declarator
	| init_declarator_list "," init_declarator
	;
init_declarator
	: declarator
	| declarator "=" initializer
	;
type_specifier
	: "typedef"			{ DRIVER.addTypeSpecifier(YaccAdapter::STORAGE, YaccAdapter::TYPEDEF); }
	| "extern"			{ DRIVER.addTypeSpecifier(YaccAdapter::STORAGE, YaccAdapter::EXTERN); }
	| "static"			{ DRIVER.addTypeSpecifier(YaccAdapter::STORAGE, YaccAdapter::STATIC); }
	| "auto"			{ DRIVER.addTypeSpecifier(YaccAdapter::STORAGE, YaccAdapter::AUTO); }
	| "register"			{ DRIVER.addTypeSpecifier(YaccAdapter::STORAGE, YaccAdapter::REGISTER); }
	| "void"			{ DRIVER.addTypeSpecifier(YaccAdapter::VOID); }
	| "char"			{ DRIVER.addTypeSpecifier(YaccAdapter::CHAR); }
	| "short"			{ DRIVER.addTypeSpecifier(YaccAdapter::SHORT); }
	| "int"				{ DRIVER.addTypeSpecifier(YaccAdapter::INT); }
	| "long"			{ DRIVER.addTypeSpecifier(YaccAdapter::LONG); }
	| "float"			{ DRIVER.addTypeSpecifier(YaccAdapter::FLOAT); }
	| "double"			{ DRIVER.addTypeSpecifier(YaccAdapter::DOUBLE); }
	| "signed"			{ DRIVER.addTypeSpecifier(YaccAdapter::SIGNED); }
	| "unsigned"			{ DRIVER.addTypeSpecifier(YaccAdapter::UNSIGNED); }
	| struct_or_union_specifier
	| enum_specifier
	//| TYPE_NAME
	;
struct_or_union_specifier
	: struct_or_union IDENTIFIER "{" struct_declaration_list "}"
	| struct_or_union "{" struct_declaration_list "}"
	| struct_or_union IDENTIFIER
	;
struct_or_union
	: "struct"			{ DRIVER.addTypeSpecifier(YaccAdapter::STRUCT); }
	| "union"			{ DRIVER.addTypeSpecifier(YaccAdapter::UNION); }
	;
struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;
struct_declaration
	: specifier_qualifier_list struct_declarator_list ";"
	;
specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;
struct_declarator_list
	: struct_declarator
	| struct_declarator_list "," struct_declarator
	;
struct_declarator
	: declarator
	| ":" integer_constant_expression
	| declarator ":" integer_constant_expression
	;
enum_specifier
	: "enum" "{" enumerator_list "}"
	| "enum" IDENTIFIER "{" enumerator_list "}"
	| "enum" IDENTIFIER
	;
enumerator_list
	: enumerator
	| enumerator_list "," enumerator
	;
enumerator
	: IDENTIFIER
	| IDENTIFIER "=" integer_constant_expression
	;
type_qualifier
	: "const"
	| "volatile"
	;
declarator
	: pointer direct_declarator
	| direct_declarator
	;
direct_declarator
	: IDENTIFIER							{ DRIVER.makeVariable($1, @1); }
	| "(" declarator ")"
	| direct_declarator "[" integer_constant_expression "]"
	| direct_declarator "[" "]"
	| direct_declarator "(" parameter_type_list ")"
	| direct_declarator "(" identifier_list ")"
	| direct_declarator "(" ")"
	;
pointer
	: "*"
	| "*" type_qualifier_list
	| "*" pointer
	| "*" type_qualifier_list pointer
	;
type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;
parameter_type_list
	: parameter_list
	| parameter_list "," ELLIPSIS
	;
parameter_list
	: parameter_declaration
	| parameter_list "," parameter_declaration
	;
parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;
identifier_list
	: IDENTIFIER
	| identifier_list "," IDENTIFIER
	;
type_name
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;
abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;
direct_abstract_declarator
	: "(" abstract_declarator ")"
	| "[" "]"
	| "[" integer_constant_expression "]"
	| direct_abstract_declarator "[" "]"
	| direct_abstract_declarator "[" integer_constant_expression "]"
	| "(" ")"
	| "(" parameter_type_list ")"
	| direct_abstract_declarator "(" ")"
	| direct_abstract_declarator "(" parameter_type_list ")"
	;
initializer
	: assignment_expression
	| "{" initializer_list "}"
	| "{" initializer_list "," "}"
	;
initializer_list
	: initializer
	| initializer_list "," initializer
	;

/* Expressions */
primary_expression
	: IDENTIFIER				{ /*$$ = ExpressionFactory::makeIdentity($1);*/ }
	| FLOATING_LITERAL			{ DRIVER.makeFloatingLiteral($1, @1); }
	| STRING_LITERAL			{ DRIVER.makeStringLiteral($1, @1); }
	| integer_constant_expression		{ }
	| "(" expression ")"			{ DRIVER.makeParenExpr(@1, @3); }
	;
integer_constant_expression
	: INTEGER_LITERAL			{ DRIVER.makeIntegerLiteral($1, @1); }
	| CHARACTER_LITERAL			{ DRIVER.makeCharacterLiteral($1, @1); }
	;
expression
	: assignment_expression			{  }
	| expression "," assignment_expression	{ DRIVER.makeBinaryOperator(',', @2); }
	;
assignment_expression
	: conditional_expression				{  }
	| unary_expression assignment_op assignment_expression	{ DRIVER.makeCompoundAssignOperator($2, @2); }
	;
unary_expression
	: postfix_expression			{  }
	| "++" unary_expression			{ DRIVER.makeUnaryOperator(yy::Parser::token::yytokentype::TOK_PRE_INC, @1); }
	| "--" unary_expression			{ DRIVER.makeUnaryOperator(yy::Parser::token::yytokentype::TOK_PRE_DEC, @1); }
	| unary_op cast_expression		{ DRIVER.makeUnaryOperator($1, @1); }
	| "sizeof" "(" type_name ")"		{ DRIVER.makeSizeofExpr(@1, @3, true); }
	| "sizeof" unary_expression		{ DRIVER.makeSizeofExpr(@1, @2, false); }
	;
postfix_expression
	: primary_expression					{  }
	| postfix_expression "[" expression "]"			{ DRIVER.makeArraySubscripExpr(@2); }
	| postfix_expression "(" argument_expression_list ")"	{ DRIVER.makeCallExpr($3, @2); }
	| postfix_expression "." IDENTIFIER			{ DRIVER.makeMemberExpr('.', @2); }
	| postfix_expression "->" IDENTIFIER			{ DRIVER.makeMemberExpr(yy::Parser::token::yytokentype::TOK_POINT_OP, @2); }
	| postfix_expression "++"				{ DRIVER.makeUnaryOperator(yy::Parser::token::yytokentype::TOK_POST_INC, @2); }
	| postfix_expression "--"				{ DRIVER.makeUnaryOperator(yy::Parser::token::yytokentype::TOK_POST_DEC, @2); }
	;
unary_op
	: "&"	{ $$ = '&'; }
	| "*"	{ $$ = '*'; }
	| "+"	{ $$ = '+'; }
	| "-"	{ $$ = '-'; }
	| "~"	{ $$ = '~'; }
	| "!"	{ $$ = '!'; }
	;
cast_expression
	: unary_expression					{  }
	| "(" type_name ")" cast_expression			{ DRIVER.makeCStyleCastExpr(@1, @3); }
	;
argument_expression_list
	:							{ $$ = 0; }
	| assignment_expression					{ $$ = 1; }
	| argument_expression_list "," assignment_expression	{ $$ = $1 + 1; }
	;
assignment_op
	: "="		{ $$ = '='; }
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
conditional_expression
	: logical_or_expression							{  }
	| logical_or_expression "?" expression ":" conditional_expression	{ DRIVER.makeConditionalOperator(); }
	;
logical_or_expression
	: logical_and_expression				{  }
	| logical_or_expression "||" logical_and_expression	{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_OR_OP, @2); }
	;
logical_and_expression
	: inclusive_or_expression				{  }
	| logical_and_expression "&&" inclusive_or_expression	{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_AND_OP, @2); }
	;
inclusive_or_expression
	: exclusive_or_expression				{  }
	| inclusive_or_expression "|" exclusive_or_expression	{ DRIVER.makeBinaryOperator('|', @2); }
	;
exclusive_or_expression
	: and_expression					{  }
	| exclusive_or_expression "^" and_expression		{ DRIVER.makeBinaryOperator('^', @2); }
	;
and_expression
	: equality_expression					{  }
	| and_expression "&" equality_expression		{ DRIVER.makeBinaryOperator('&', @2); }
	;
equality_expression
	: relational_expression					{  }
	| equality_expression "==" relational_expression	{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_EQ_OP, @2); }
	| equality_expression "!=" relational_expression	{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_NE_OP, @2); }
	;
relational_expression
	: shift_expression					{  }
	| relational_expression "<" shift_expression		{ DRIVER.makeBinaryOperator('<', @2); }
	| relational_expression ">" shift_expression		{ DRIVER.makeBinaryOperator('>', @2); }
	| relational_expression "<=" shift_expression		{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_LE_OP, @2); }
	| relational_expression ">=" shift_expression		{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_GE_OP, @2); }
	;
shift_expression
	: additive_expression					{  }
	| shift_expression "<<" additive_expression		{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_LEFT_SHIFT_OP, @2); }
	| shift_expression ">>" additive_expression		{ DRIVER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_RIGHT_SHIFT_OP, @2); }
	;
additive_expression
	: multiplicative_expression				{  }
	| additive_expression "+" multiplicative_expression	{ DRIVER.makeBinaryOperator('+', @2); }
	| additive_expression "-" multiplicative_expression	{ DRIVER.makeBinaryOperator('-', @2); }
	;
multiplicative_expression
	: cast_expression					{  }
	| multiplicative_expression "*" cast_expression		{ DRIVER.makeBinaryOperator('*', @2); }
	| multiplicative_expression "/" cast_expression		{ DRIVER.makeBinaryOperator('/', @2); }
	| multiplicative_expression "%" cast_expression		{ DRIVER.makeBinaryOperator('%', @2); }
	;

%%

void yy::Parser::error(const location_type& l,  const std::string& m)
{
    driver.error(l, m);
}
