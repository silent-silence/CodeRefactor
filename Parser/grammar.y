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
	#define ADAPTER yy::Parser::driver.getAdapter()
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
%token KEYWORD_ELSE	"else"		KEYWORD_DEFAULT	"default"	KEYWORD_GOTO	"goto"

%token TYPE_BOOL	"bool"		TYPE_CHAR	"char"		TYPE_SHORT	"short"
%token TYPE_INT		"int"		TYPE_LONG	"long"		TYPE_VOID	"void"
%token TYPE_FLOAT	"float"		TYPE_DOUBLE	"double"
%token TYPE_SIGNED	"signed"	TYPE_UNSIGNED	"unsigned"

%token KEYWORD_STRUCT	"struct"	KEYWORD_UNION	"union"		KEYWORD_ENUM	"enum"

%token KEYWORD_TYPEDEF	"typedef"	KEYWORD_EXTERN	"extern"
%token KEYWORD_STATIC	"static"	KEYWORD_AUTO	"auto"
%token KEYWORD_REGISTER	"register"

%token KEYWORD_CONST	"const"		KEYWORD_VOLATILE	"volatile"

%token <std::string>IDENTIFIER	<std::string>TYPE_NAME
%token <int>INTEGER_LITERAL	<double>FLOATING_LITERAL	<std::string>STRING_LITERAL	<char>CHARACTER_LITERAL
%token <std::string>COMMENT

%token  EOF	0	"eof"

%type <int>unary_op
%type <int>assignment_op
%type <int>block_stmt
%type <int>argument_expression_list
%type <int>struct_declaration_list
%type <int>struct_declarator_list
%type <int>parameter_list
%type <int>parameter_type_list
%type <int>case_stmt
%type <int>init_declarator_list

%printer { yyoutput << $$; }<*>
%%

start
	:
	| start statement
	| start function_definition
	;


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
	| comment_stmt
	;
comment_stmt
	: COMMENT						{ ADAPTER.makeCommentStmt($1, @1); }
	;
null_stmt
	: ";"							{ ADAPTER.makeNullStmt(@1); }
	;
compound_stmt
	: "{" { ADAPTER.enterCompoundBlock(@1); } block_stmt "}"{ ADAPTER.makeCompoundStmt($3, @1, @4); }
	;
block_stmt
	:							{ $$ = 0; }
	| block_stmt statement					{ $$ = $1 + 1; }
	;
expr_stmt
	: expression ";" 					{ /* Do nothing */ }
	;
if_stmt
	: "if" "(" expression ")" statement			{ ADAPTER.makeIfStmt(@1); }
	| "if" "(" expression ")" statement "else" statement	{ ADAPTER.makeIfElseStmt(@1, @6); }
	;
for_stmt
	: "for" "(" for_init_stmt expr_or_null_stmt ")" statement		{ ADAPTER.makeForStmt(@1, @2, @5, false); }
	| "for" "(" for_init_stmt expr_or_null_stmt expression ")" statement	{ ADAPTER.makeForStmt(@1, @2, @6, true); }
	;
expr_or_null_stmt
	: expr_stmt
	| null_stmt
	;
for_init_stmt
	: expr_or_null_stmt
	| decl_stmt
	;
while_stmt
	: "while" "(" expression ")" statement			{ ADAPTER.makeWhileStmt(@1); }
	;
do_stmt
	: "do" statement "while" "(" expression ")"		{ ADAPTER.makeDoStmt(@1, @3, @4); }
	;
continue_stmt
	: "continue" ";"					{ ADAPTER.makeContinueStmt(@1); }
	;
break_stmt
	: "break" ";"						{ ADAPTER.makeBreakStmt(@1); }
	;
switch_stmt
	: "switch" "(" expression ")" statement			{ ADAPTER.makeSwitchStmt(@1); }
	;
case_stmt
	: "case" assignment_expression ":"			{ ADAPTER.makeCaseStmt(@1, @2, @3); }
	;
default_stmt
	: "default" ":"						{ ADAPTER.makeDefaultStmt(@1, @2); }
	;
label_stmt
	: IDENTIFIER ":" statement				{ ADAPTER.makeLabelStmt($1, @1); }
	;
goto_stmt
	: "goto" IDENTIFIER ";"					{ ADAPTER.makeGotoStmt($2, @1, @2); }
	;
return_stmt
	: "return" ";"						{ ADAPTER.makeReturnStmt(@1, false); }
	| "return" expression ";"				{ ADAPTER.makeReturnStmt(@1, true); }
	;


/* Declarations */
decl_stmt
	: declaration_specifiers ";"				{ ADAPTER.makeDeclStmt(@1, @2); }
	| declaration_specifiers init_declarator_list ";"	{ ADAPTER.makeDeclStmt(@1, @3); }
	;
declaration_specifiers
	: type_specifier					{ ADAPTER.makeType(); }
	| type_specifier declaration_specifiers
	| type_qualifier
        | type_qualifier declaration_specifiers
	;
init_declarator_list
	: init_declarator					{ $$ = 1; }
	| init_declarator_list "," init_declarator		{ $$ = $1 + 1; }
	;
init_declarator
	: declarator						{ ADAPTER.makeVariables(false); }
	| declarator "=" initializer				{ ADAPTER.makeVariables(true); }
	;
type_specifier
	: "typedef"						{ ADAPTER.addTypeSpecifier(YaccAdapter::TYPEDEF); }
	| "extern"						{ ADAPTER.addTypeSpecifier(YaccAdapter::EXTERN); }
	| "static"						{ ADAPTER.addTypeSpecifier(YaccAdapter::STATIC); }
	| "auto"						{ ADAPTER.addTypeSpecifier(YaccAdapter::AUTO); }
	| "register"						{ ADAPTER.addTypeSpecifier(YaccAdapter::REGISTER); }
	| "void"						{ ADAPTER.addTypeSpecifier(YaccAdapter::VOID); }
	| "char"						{ ADAPTER.addTypeSpecifier(YaccAdapter::CHAR); }
	| "short"						{ ADAPTER.addTypeSpecifier(YaccAdapter::SHORT); }
	| "int"							{ ADAPTER.addTypeSpecifier(YaccAdapter::INT); }
	| "long"						{ ADAPTER.addTypeSpecifier(YaccAdapter::LONG); }
	| "float"						{ ADAPTER.addTypeSpecifier(YaccAdapter::FLOAT); }
	| "double"						{ ADAPTER.addTypeSpecifier(YaccAdapter::DOUBLE); }
	| "signed"						{ ADAPTER.addTypeSpecifier(YaccAdapter::SIGNED); }
	| "unsigned"						{ ADAPTER.addTypeSpecifier(YaccAdapter::UNSIGNED); }
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME						{ ADAPTER.findTypeByName($1); }
	;
struct_or_union_specifier
	: struct_or_union IDENTIFIER "{" { ADAPTER.enterStructBlock(@3); } struct_declaration_list "}"
	| struct_or_union "{" { ADAPTER.enterStructBlock(@2); } struct_declaration_list "}"
	| struct_or_union IDENTIFIER
	;
struct_or_union
	: "struct"						{ ADAPTER.addTypeSpecifier(YaccAdapter::STRUCT); }
	| "union"						{ ADAPTER.addTypeSpecifier(YaccAdapter::UNION); }
	;
struct_declaration_list
	: struct_declaration					{ $$ = 1; }
	| struct_declaration_list struct_declaration		{ $$ = $1 + 1; }
	;
struct_declaration
	: struct_declaration_type struct_declarator_list ";"	{ ADAPTER.makeInStructDeclStmt(@1, @3); }
	;
struct_declaration_type
	: specifier_qualifier_list				{ ADAPTER.makeType(); }
	;
specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;
struct_declarator_list
	: struct_declarator					{ $$ = 1; }
	| struct_declarator_list "," struct_declarator		{ $$ = $1 + 1; }
	;
struct_declarator
	: declarator						{ ADAPTER.makeVariables(false); }
	| ":" assignment_expression
	| declarator ":" assignment_expression			{ ADAPTER.makeVariables(true); }
	;
enum_specifier
	: "enum" "{" enumerator_list "}"			{ ADAPTER.makeEnumContext(@1); ADAPTER.addTypeSpecifier(YaccAdapter::ENUM); }
	| "enum" IDENTIFIER "{" enumerator_list "}"		{ ADAPTER.makeEnumContext($2, @1); ADAPTER.addTypeSpecifier(YaccAdapter::ENUM); }
	| "enum" IDENTIFIER					{ ADAPTER.makeEnumContext($2, @1); ADAPTER.addTypeSpecifier(YaccAdapter::ENUM); }
	;
enumerator_list
	: enumerator
	| enumerator_list "," enumerator
	;
enumerator
	: IDENTIFIER						{ ADAPTER.makeEnumConstant($1, @1, false); }
	| IDENTIFIER "=" assignment_expression			{ ADAPTER.makeEnumConstant($1, @1, true); }
	;
type_qualifier
	: "const"						{ ADAPTER.addTypeSpecifier(YaccAdapter::CONST); }
	| "volatile"						{ ADAPTER.addTypeSpecifier(YaccAdapter::VOLATILE); }
	;
declarator
	: store_decl_type_first pointer direct_declarator
	| store_decl_type_first direct_declarator
	;
direct_declarator
	: IDENTIFIER										{ ADAPTER.storeVariable($1, @1); }
	| "(" declarator ")"
	| direct_declarator "[" assignment_expression "]"					{ ADAPTER.makeConstantArrayType(); }
	| direct_declarator "[" "]"								{ ADAPTER.makeIncompleteArrayType(); }
	| direct_declarator "(" { ADAPTER.enterFunctionParamDecl(); } parameter_type_list ")"	{ ADAPTER.makeFunctionProtoType($4); }
	| direct_declarator "(" identifier_list ")"
	| direct_declarator "(" ")"								{ ADAPTER.makeFunctionNoProtoType(); }
	;
pointer
	: "*"							{ ADAPTER.makePointerType(); }
	| "*" type_qualifier_list				{ ADAPTER.makePointerType(); }
	| "*" pointer						{ ADAPTER.makePointerType(); }
	| "*" type_qualifier_list pointer			{ ADAPTER.makePointerType(); }
	;
store_decl_type_first
	:							{ ADAPTER.storeDeclType(); }
	;
type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;
parameter_type_list
	: parameter_list					{ $$ = $1; }
	| parameter_list "," ELLIPSIS				{ $$ = $1 + 1; }
	;
parameter_list
	: parameter_declaration					{ $$ = 1; }
	| parameter_list "," parameter_declaration		{ $$ = $1 + 1; }
	;
parameter_declaration
	: declaration_specifiers declarator			{ ADAPTER.makeFunParam(); }
	| declaration_specifiers abstract_declarator		{ ADAPTER.makeFunParam(); }
	| declaration_specifiers				{ ADAPTER.makeUnnamedFunParam(@1); }
	;
identifier_list
	: IDENTIFIER
	| identifier_list "," IDENTIFIER
	;
type_name
	: specifier_qualifier_list				{ ADAPTER.makeType(); }
	| specifier_qualifier_list abstract_declarator
	;
abstract_declarator
	: store_decl_type_first pointer
	| store_decl_type_first direct_abstract_declarator
	| store_decl_type_first pointer direct_abstract_declarator
	;
direct_abstract_declarator
	: "(" abstract_declarator ")"
	| "[" "]"
	| "[" assignment_expression "]"
	| direct_abstract_declarator "[" "]"
	| direct_abstract_declarator "[" assignment_expression "]"
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


function_definition
	: declaration_specifiers declarator { ADAPTER.enterFunctionBlock(); } compound_stmt	{ ADAPTER.makeFunctionDefinition(); }
	;


/* Expressions */
primary_expression
	: IDENTIFIER				{ ADAPTER.makeDeclRefExpr($1, @1); }
	| FLOATING_LITERAL			{ ADAPTER.makeFloatingLiteral($1, @1); }
	| STRING_LITERAL			{ ADAPTER.makeStringLiteral($1, @1); }
	| INTEGER_LITERAL			{ ADAPTER.makeIntegerLiteral($1, @1); }
	| CHARACTER_LITERAL			{ ADAPTER.makeCharacterLiteral($1, @1); }
	| "(" expression ")"			{ ADAPTER.makeParenExpr(@1, @3); }
	;
expression
	: assignment_expression			{  }
	| expression "," assignment_expression	{ ADAPTER.makeBinaryOperator(',', @2); }
	;
assignment_expression
	: conditional_expression				{  }
	| unary_expression assignment_op assignment_expression	{ ADAPTER.makeCompoundAssignOperator($2, @2); }
	;
unary_expression
	: postfix_expression			{  }
	| "++" unary_expression			{ ADAPTER.makeUnaryOperator(yy::Parser::token::yytokentype::TOK_PRE_INC, @1); }
	| "--" unary_expression			{ ADAPTER.makeUnaryOperator(yy::Parser::token::yytokentype::TOK_PRE_DEC, @1); }
	| unary_op cast_expression		{ ADAPTER.makeUnaryOperator($1, @1); }
	| "sizeof" "(" type_name ")"		{ ADAPTER.makeSizeofExpr(@1, @3, true); }
	| "sizeof" unary_expression		{ ADAPTER.makeSizeofExpr(@1, @2, false); }
	;
postfix_expression
	: primary_expression					{  }
	| postfix_expression "[" expression "]"			{ ADAPTER.makeArraySubscripExpr(@2); }
	| postfix_expression "(" argument_expression_list ")"	{ ADAPTER.makeCallExpr($3, @2); }
	| postfix_expression "." IDENTIFIER			{ ADAPTER.makeMemberExpr('.', @2, $3); }
	| postfix_expression "->" IDENTIFIER			{ ADAPTER.makeMemberExpr(yy::Parser::token::yytokentype::TOK_POINT_OP, @2, $3); }
	| postfix_expression "++"				{ ADAPTER.makeUnaryOperator(yy::Parser::token::yytokentype::TOK_POST_INC, @2); }
	| postfix_expression "--"				{ ADAPTER.makeUnaryOperator(yy::Parser::token::yytokentype::TOK_POST_DEC, @2); }
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
	| "(" type_name ")" cast_expression			{ ADAPTER.makeCStyleCastExpr(@1, @3); }
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
	| logical_or_expression "?" expression ":" conditional_expression	{ ADAPTER.makeConditionalOperator(); }
	;
logical_or_expression
	: logical_and_expression				{  }
	| logical_or_expression "||" logical_and_expression	{ ADAPTER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_OR_OP, @2); }
	;
logical_and_expression
	: inclusive_or_expression				{  }
	| logical_and_expression "&&" inclusive_or_expression	{ ADAPTER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_AND_OP, @2); }
	;
inclusive_or_expression
	: exclusive_or_expression				{  }
	| inclusive_or_expression "|" exclusive_or_expression	{ ADAPTER.makeBinaryOperator('|', @2); }
	;
exclusive_or_expression
	: and_expression					{  }
	| exclusive_or_expression "^" and_expression		{ ADAPTER.makeBinaryOperator('^', @2); }
	;
and_expression
	: equality_expression					{  }
	| and_expression "&" equality_expression		{ ADAPTER.makeBinaryOperator('&', @2); }
	;
equality_expression
	: relational_expression					{  }
	| equality_expression "==" relational_expression	{ ADAPTER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_EQ_OP, @2); }
	| equality_expression "!=" relational_expression	{ ADAPTER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_NE_OP, @2); }
	;
relational_expression
	: shift_expression					{  }
	| relational_expression "<" shift_expression		{ ADAPTER.makeBinaryOperator('<', @2); }
	| relational_expression ">" shift_expression		{ ADAPTER.makeBinaryOperator('>', @2); }
	| relational_expression "<=" shift_expression		{ ADAPTER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_LE_OP, @2); }
	| relational_expression ">=" shift_expression		{ ADAPTER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_GE_OP, @2); }
	;
shift_expression
	: additive_expression					{  }
	| shift_expression "<<" additive_expression		{ ADAPTER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_LEFT_SHIFT_OP, @2); }
	| shift_expression ">>" additive_expression		{ ADAPTER.makeBinaryOperator(yy::Parser::token::yytokentype::TOK_RIGHT_SHIFT_OP, @2); }
	;
additive_expression
	: multiplicative_expression				{  }
	| additive_expression "+" multiplicative_expression	{ ADAPTER.makeBinaryOperator('+', @2); }
	| additive_expression "-" multiplicative_expression	{ ADAPTER.makeBinaryOperator('-', @2); }
	;
multiplicative_expression
	: cast_expression					{  }
	| multiplicative_expression "*" cast_expression		{ ADAPTER.makeBinaryOperator('*', @2); }
	| multiplicative_expression "/" cast_expression		{ ADAPTER.makeBinaryOperator('/', @2); }
	| multiplicative_expression "%" cast_expression		{ ADAPTER.makeBinaryOperator('%', @2); }
	;
%%

void yy::Parser::error(const location_type& l, const std::string& m)
{
    driver.error(l, m);
}
