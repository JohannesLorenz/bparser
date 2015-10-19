%{

/**
 * @file parser.y
 * To generate the parser run: "bison Parser.y"
 */

#include <vector>
#include <iostream>
#include "node.h"
#include "parser.h"
#include "lexer.h"
#include "visitor.h"

int yyerror(node_t **expression, yyscan_t scanner, const char *msg) {
	fprintf(stderr,"At: %s,\n  line %d, column %d: Error:%s\n", yyget_text(scanner), yyget_lineno(scanner), yyget_column(scanner), msg); return 0;
	// Add error handling routine as needed
}

extern geom_t get_pos();

/*template<class R, class Left>
R* init(Left* left) {
	R* r = new R(); r->push_back(left);
	return r;
}*/

template<class S, class Left>
void app_first(S& stor, Left* left) {
	 stor.push_front(left); 
}

template<class R, class S, class Left>
R* app_left(R* cur, S& stor, Left* left) {
	 return stor.push_front(left), cur; 
}

template<class R, class S, class Right>
R* app_right(R* cur, S& stor, Right* left) {
	 return stor.push_back(left), cur;
}

void c11() { throw "This C11 extension is not implemented yet."; }
void not_yet() { throw "This extension is to be done."; }

extern std::vector<token_t*>& get_token_vector();

token_t* app_token() { return get_token_vector().back(); }

template<class T> void alloc(T*& ptr_ref) { ptr_ref = new T(); }

token_t* t(int token_id) { return new token_t(get_pos(), token_id); }

%}

%code requires {

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

}

%output  "parser.c"
%defines "parser.h"
 
%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { node_t **expression }
%parse-param { yyscan_t scanner }

%union {
	int value;
	node_t* node;
	op_t _operator;
	type_specifier_id _type_specifier_id;
	expression_t *expression;
	declaration_specifiers_t* declaration_specifiers;
	declarator_t* declarator;
	declaration_list_t* declaration_list;
	compound_statement_t* compound_statement;
	external_declaration_t* external_declaration;
	function_definition_t* function_definition;
	storage_class_specifier_t* storage_class_specifier;
	type_specifier_t* type_specifier;
	type_qualifier_t* type_qualifier;
	alignment_specifier_t* alignment_specifier;
	function_specifier_t* function_specifier;
	pointer_t* pointer;
	direct_declarator_t* direct_declarator;
	struct_or_union_specifier_t* struct_or_union_specifier;
	enum_specifier_t* enum_specifier;
	conditional_expression_t* conditional_expression;
	declaration_t* declaration;
	labeled_statement_t* labeled_statement;
	expression_statement_t* expression_statement;
	jump_statement_t* jump_statement;
	selection_statement_t* selection_statement;
	iteration_statement_t* iteration_statement;
	init_declarator_list_t* init_declarator_list;
	identifier_t* identifier;
	statement_t* statement;
	block_item_t* block_item;
	block_item_list_t* block_item_list;
	constant_t* constant;

	primary_expression_t* primary_expression;
}

%token	IDENTIFIER I_CONSTANT F_CONSTANT STRING_LITERAL FUNC_NAME SIZEOF
%token	PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token	AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token	SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token	XOR_ASSIGN OR_ASSIGN
%token	TYPEDEF_NAME ENUMERATION_CONSTANT

%token	TYPEDEF EXTERN STATIC AUTO REGISTER INLINE
%token	CONST RESTRICT VOLATILE
%token	BOOL CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID
%token	COMPLEX IMAGINARY 
%token	STRUCT UNION ENUM ELLIPSIS

%token	CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token	ALIGNAS ALIGNOF ATOMIC GENERIC NORETURN STATIC_ASSERT THREAD_LOCAL

// TODO: identifier should not just return an int
%type <value> I_CONSTANT struct_or_union ';' ':' CASE DEFAULT '{' '}' IF ELSE '(' ')' SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN IDENTIFIER
%type <node> translation_unit
	static_assert_declaration

%type<constant> constant

%type<expression> postfix_expression
	unary_expression
	cast_expression
	multiplicative_expression
	additive_expression
	shift_expression
	relational_expression
	equality_expression
	and_expression
	exclusive_or_expression
	inclusive_or_expression
	logical_and_expression
	logical_or_expression
	assignment_expression

%type <expression> expression

%type <primary_expression> primary_expression

%type <block_item_list>	block_item_list
%type <block_item> block_item

%type <declaration> declaration
%type <statement> statement
//%type <identifier> identifier
%type <_type_specifier_id> type_specifier_simple
%type <_operator> unary_operator assignment_operator
%type <declaration_specifiers> declaration_specifiers
%type <compound_statement> compound_statement
%type <declarator > declarator
%type <declaration_list> declaration_list
%type <function_definition> function_definition
%type <external_declaration> external_declaration
%type <storage_class_specifier> storage_class_specifier
%type <type_specifier> type_specifier
%type <type_qualifier> type_qualifier
%type <alignment_specifier> alignment_specifier
%type <function_specifier> function_specifier
%type <pointer> pointer
%type <direct_declarator> direct_declarator
%type <struct_or_union_specifier> struct_or_union_specifier
%type <enum_specifier> enum_specifier
%type <iteration_statement> iteration_statement
%type <selection_statement> selection_statement
%type <expression_statement> expression_statement
%type <init_declarator_list> init_declarator_list
%type <labeled_statement> labeled_statement
%type <jump_statement> jump_statement
%type <conditional_expression> conditional_expression constant_expression

%start translation_unit

%printer { fprintf (yyoutput, "constant %d", $$); } I_CONSTANT
// %printer { fprintf (yyoutput, "type %d", $$.expression->type); } <>
%printer { fprintf (yyoutput, "<...?>"); } <>


%%

primary_expression
	: IDENTIFIER
	| constant { alloc($$); $$->constant = $1; }
	| string
	| '(' expression ')'
	| generic_selection { c11(); }
	;

constant
	: I_CONSTANT { alloc($$); $$->iconstant = $1; }		/* includes character_constant */
	| F_CONSTANT
	| ENUMERATION_CONSTANT	/* after it has been defined as such */
	;

enumeration_constant		/* before it has been defined as such */
	: IDENTIFIER
	;

string
	: STRING_LITERAL
	| FUNC_NAME { c11(); }
	;

generic_selection
	: GENERIC '(' assignment_expression ',' generic_assoc_list ')'
	;

generic_assoc_list
	: generic_association
	| generic_assoc_list ',' generic_association
	;

generic_association
	: type_name ':' assignment_expression
	| DEFAULT ':' assignment_expression
	;

postfix_expression
	: primary_expression { $$=$1; }
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	| '(' type_name ')' '{' initializer_list '}'
	| '(' type_name ')' '{' initializer_list ',' '}'
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

unary_expression
	: postfix_expression { $$=$1; }
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator cast_expression
	| SIZEOF unary_expression
	| SIZEOF '(' type_name ')'
	| ALIGNOF '(' type_name ')'
	;

unary_operator
	: '&' { $$=op_addr; }
	| '*' { $$=op_ptr; }
	| '+' { $$=op_pos; }
	| '-' { $$=op_neg; }
	| '~' { $$=op_compl; }
	| '!' { $$=op_not; }
	;

cast_expression
	: unary_expression { $$=$1; }
	| '(' type_name ')' cast_expression
	;

multiplicative_expression
	: cast_expression { $$=$1; }
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression { $$=$1; }
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

shift_expression
	: additive_expression { $$=$1; }
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression { $$=$1; }
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression { $$=$1; }
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression { $$=$1; }
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression { $$=$1; }
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression { $$=$1; }
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression { $$=$1; }
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression { $$=$1; }
	| unary_expression assignment_operator assignment_expression
	;

assignment_operator
	: '=' { $$=op_asn; }
	| MUL_ASSIGN { $$=op_asn_mul; }
	| DIV_ASSIGN { $$=op_asn_div; }
	| MOD_ASSIGN { $$=op_asn_mod; }
	| ADD_ASSIGN { $$=op_asn_add; }
	| SUB_ASSIGN { $$=op_asn_sub; }
	| LEFT_ASSIGN { $$=op_asn_left; }
	| RIGHT_ASSIGN { $$=op_asn_right; }
	| AND_ASSIGN { $$=op_asn_and; }
	| XOR_ASSIGN { $$=op_asn_xor; }
	| OR_ASSIGN { $$=op_asn_or; }
	;

expression
	: assignment_expression { $$=$1; }
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression { $$=$1; }	/* with constraints */
	;

declaration
	: declaration_specifiers ';' { $$ = new declaration_t(); $$->declaration_specifiers = $1; $$->semicolon = t($2); }
	| declaration_specifiers init_declarator_list ';' { $$ = new declaration_t(); $$->declaration_specifiers = $1; $$->init_declarator_list = $2; $$->semicolon = t($3); }
	| static_assert_declaration { c11(); }
	;

declaration_specifiers
	: storage_class_specifier declaration_specifiers { $$ = app_left($2, $2->specifiers, $1); }
	| storage_class_specifier { $$ = new declaration_specifiers_t; app_left($$, $$->specifiers, $1); }
	| type_specifier declaration_specifiers { $$ = app_left($2, $2->specifiers, $1); }
	| type_specifier { $$ = new declaration_specifiers_t; app_left($$, $$->specifiers, $1); }
	| type_qualifier declaration_specifiers { $$ = app_left($2, $2->specifiers, $1); }
	| type_qualifier { $$ = new declaration_specifiers_t; app_left($$, $$->specifiers, $1); }
	| function_specifier declaration_specifiers { $$ = app_left($2, $2->specifiers, $1); }
	| function_specifier { $$ = new declaration_specifiers_t; app_left($$, $$->specifiers, $1); }
	| alignment_specifier declaration_specifiers { $$ = app_left($2, $2->specifiers, $1); }
	| alignment_specifier { $$ = new declaration_specifiers_t; app_left($$, $$->specifiers, $1); }
	;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator '=' initializer
	| declarator
	;

storage_class_specifier
	: TYPEDEF	/* identifiers must be flagged as TYPEDEF_NAME */
	| EXTERN
	| STATIC
	| THREAD_LOCAL { c11(); }
	| AUTO
	| REGISTER
	;

type_specifier_simple
	: VOID { $$ = t_void; }
	| CHAR { $$ = t_char; }
	| SHORT { $$ = t_short; }
	| INT { $$ = t_int; }
	| LONG { $$ = t_long; }
	| FLOAT { $$ = t_float; }
	| DOUBLE { $$ = t_double; }
	| SIGNED { $$ = t_signed; }
	| UNSIGNED { $$ = t_unsigned; }
	| BOOL { $$ = t_bool; }
	| COMPLEX { $$ = t_complex; }
	| IMAGINARY { c11(); }		/* non-mandated extension */
	;

type_specifier
	: type_specifier_simple { $$ = new type_specifier_simple_t(get_pos(), $1); }
	| atomic_type_specifier { c11(); }
	| struct_or_union_specifier { $$ = $1; }
	| enum_specifier { $$ = $1; }
	| TYPEDEF_NAME { not_yet(); }		/* after it has been defined as such */
	;

struct_or_union_specifier
	: struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	;

struct_or_union
	: STRUCT { $$ = t_struct; }
	| UNION { $$ = t_union; }
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list ';'	/* for anonymous struct/union */
	| specifier_qualifier_list struct_declarator_list ';'
	| static_assert_declaration { c11(); }
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: ':' constant_expression
	| declarator ':' constant_expression
	| declarator
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM '{' enumerator_list ',' '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list ',' '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator	/* identifiers must be flagged as ENUMERATION_CONSTANT */
	: enumeration_constant '=' constant_expression
	| enumeration_constant
	;

atomic_type_specifier
	: ATOMIC '(' type_name ')'
	;

type_qualifier
	: CONST
	| RESTRICT
	| VOLATILE
	| ATOMIC { c11(); }
	;

function_specifier
	: INLINE
	| NORETURN { c11(); }
	;

alignment_specifier
	: ALIGNAS '(' type_name ')' { c11(); }
	| ALIGNAS '(' constant_expression ')' { c11(); }
	;

declarator
	: pointer direct_declarator { $$ = new declarator_t; $$->pointer = $1; $$->direct_declarator = $2; }
	| direct_declarator { $$ = new declarator_t; $$->direct_declarator = $1; }
	;

direct_declarator
	: IDENTIFIER
	| '(' declarator ')' { $$->bracktype = 0; }
	| direct_declarator '[' ']'
	| direct_declarator '[' '*' ']'
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	| direct_declarator '[' STATIC assignment_expression ']'
	| direct_declarator '[' type_qualifier_list '*' ']'
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	| direct_declarator '[' type_qualifier_list assignment_expression ']'
	| direct_declarator '[' type_qualifier_list ']'
	| direct_declarator '[' assignment_expression ']'
	| direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' ')'
	| direct_declarator '(' identifier_list ')'
	;

pointer // should actually be named: "pointers"
	: '*' type_qualifier_list pointer
	| '*' type_qualifier_list
	| '*' pointer
	| '*'
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;


parameter_type_list
	: parameter_list ',' ELLIPSIS
	| parameter_list
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list abstract_declarator
	| specifier_qualifier_list
	;

abstract_declarator
	: pointer direct_abstract_declarator
	| pointer
	| direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' '*' ']'
	| '[' STATIC type_qualifier_list assignment_expression ']'
	| '[' STATIC assignment_expression ']'
	| '[' type_qualifier_list STATIC assignment_expression ']'
	| '[' type_qualifier_list assignment_expression ']'
	| '[' type_qualifier_list ']'
	| '[' assignment_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' '*' ']'
	| direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	| direct_abstract_declarator '[' STATIC assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list ']'
	| direct_abstract_declarator '[' assignment_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

initializer
	: '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	| assignment_expression
	;

initializer_list
	: designation initializer
	| initializer
	| initializer_list ',' designation initializer
	| initializer_list ',' initializer
	;

designation
	: designator_list '='
	;

designator_list
	: designator
	| designator_list designator
	;

designator
	: '[' constant_expression ']'
	| '.' IDENTIFIER
	;

static_assert_declaration
	: STATIC_ASSERT '(' constant_expression ',' STRING_LITERAL ')' ';'
	;

statement
	: labeled_statement { $$=$1; }
	| compound_statement { $$=$1; }
	| expression_statement { $$=$1; }
	| selection_statement { $$=$1; }
	| iteration_statement { $$=$1; }
	| jump_statement { $$=$1; }
	;



labeled_statement
	: IDENTIFIER ':' statement { alloc($$); $$->identifier = t($1); $$->colon = t($2); $$->statement = $3; }
	| CASE constant_expression ':' statement { alloc($$); $$->case_token = t($1); $$->expression = $2; $$->colon = t($3); $$->statement = $4; }
	| DEFAULT ':' statement { alloc($$); $$->default_token = t($1); $$->colon = t($2); $$->statement = $3; }
	;

// TODO: alloc($$) -> template<class T> alloc(T*& ptr_ref) { ptr_ref = new T*(); }
compound_statement
	: '{' '}' { alloc($$); $$->lbrack = t($1); $$->rbrack = t($2); }
	| '{'  block_item_list '}'  { alloc($$); $$->lbrack = t($1); $$->block_item_list=$2; $$->rbrack=t($3); }
	;

block_item_list
	: block_item { alloc($$); app_first($$->items, $1); }
	| block_item_list block_item { app_right($$, $$->items, $2); }
	;

block_item
	: declaration { $$=$1; }
	| statement { $$=$1; }
	;

expression_statement
	: ';' { alloc($$); $$->semicolon = t($1); }
	| expression ';' { alloc($$); $$->expression = $1; $$->semicolon = t($2); }
	;

selection_statement
	: IF '(' expression ')' statement ELSE statement { alloc($$); $$->if_token = t($1); $$->lbrace = t($2); $$->expression = $3; $$->rbrace = t($4); $$->statement = $5; $$->else_token=t($6); $$->else_statement = $7; }
	| IF '(' expression ')' statement { alloc($$); $$->if_token = t($1); $$->lbrace = t($2); $$->expression = $3; $$->rbrace = t($4); $$->statement = $5; }
	| SWITCH '(' expression ')' statement { alloc($$); $$->switch_token = t($1); $$->lbrace = t($2); $$->expression = $3; $$->rbrace = t($4); $$->statement = $5; }
	;

iteration_statement
	: WHILE '(' expression ')' statement { alloc($$); $$->while_token = t($1); $$->lbrace = t($2); $$->while_condition = $3; $$->rbrace = t($4); $$->statement = $5; }
	| DO statement WHILE '(' expression ')' ';' { alloc($$); $$->do_token = t($1); $$->statement = $2; $$->while_token = t($3); $$->lbrace = t($4); $$->while_condition = $5; $$->rbrace = t($6); $$->semicolon = t($7); }
	| FOR '(' expression_statement expression_statement ')' statement { alloc($$); $$->for_token = t($1); $$->lbrace = t($2); $$->for_init_statement = $3; $$->for_condition = $4; $$->rbrace = t($5); $$->statement = $6; }
	| FOR '(' expression_statement expression_statement expression ')' statement { alloc($$); $$->for_token = t($1); $$->lbrace = t($2); $$->for_init_statement = $3; $$->for_condition = $4; $$->for_expression = $5; $$->rbrace = t($6); $$->statement = $7; }
	| FOR '(' declaration expression_statement ')' statement { alloc($$); $$->for_token = t($1); $$->lbrace = t($2); $$->for_init_declaration = $3; $$->for_condition = $4; $$->rbrace = t($5); $$->statement = $6; }
	| FOR '(' declaration expression_statement expression ')' statement { alloc($$); $$->for_token = t($1); $$->lbrace = t($2); $$->for_init_declaration = $3; $$->for_condition = $4; $$->for_expression = $5; $$->rbrace = t($6); $$->statement = $7; }
	;	

jump_statement
	: GOTO IDENTIFIER ';' { alloc($$); $$->keyword = t($1); $$->goto_identifier = NULL /*TODO*/; $$->semicolon = t($3); }
	| CONTINUE ';' { alloc($$); $$->keyword = t($1); $$->semicolon = t($2); }
	| BREAK ';' { alloc($$); $$->keyword = t($1); $$->semicolon = t($2); }
	| RETURN ';' { alloc($$); $$->keyword = t($1); $$->semicolon = t($2); }
	| RETURN expression ';' { alloc($$); $$->keyword = t($1); $$->expression = $2; $$->semicolon = t($3); }
	;

translation_unit
	: external_declaration { *expression = new translation_unit_t(); dynamic_cast<translation_unit_t*>(*expression)->v.push_back($1); }
	| translation_unit external_declaration { dynamic_cast<translation_unit_t*>(*expression)->v.push_back($2); /* append_t<external_declaration_t> a($2); (*expression)->accept(a); (*expression)->append($2);*/ } // TODO: this case hopefully never happens?
	;

external_declaration // TODO: this class is not needed (->virtual)
	: function_definition { $$ = new external_declaration_t($1); }
	| declaration { $$ = new external_declaration_t($1); }
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement { $$ = new function_definition_t($1, $2, $3, $4); }
	| declaration_specifiers declarator compound_statement { $$ = new function_definition_t($1, $2, NULL, $3); }
	;

declaration_list
	: declaration { alloc($$); app_first($$->declarations, $1); }
	| declaration_list declaration { app_right($1, $1->declarations, $2); }
	;

%%
