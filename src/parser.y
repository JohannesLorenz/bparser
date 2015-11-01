%{

/**
 * @file parser.y
 * To generate the parser run: "bison Parser.y"
 */

#include <algorithm>
#include <vector>
#include <iostream>
#include "node.h"
#include "parser.h"
#include "lexer.h"
#include "visitor.h"

// TODO: check for no returns anywhere!!

int yyerror(translation_unit_t ** /*expression*/, yyscan_t scanner, const char *msg) {
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

template<class T> T* alloc(T*& ptr_ref) { return ptr_ref = new T(); /* be C++03 conform */ }

token_t* t(int token_id) { return new token_t(get_pos(), token_id); }

void move_str(std::string& str, const char* s) {
	str = s; delete[] s;
	// TODO: remove function, write non-POD char* pointer , and function (std::string, char*-ptr) { string = ptr; delete ptr; }
}

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
%parse-param { translation_unit_t **expression }
%parse-param { yyscan_t scanner }

%union {
	char* name;
	float _float;
	int _int;
	token_t* token;
	node_base* node;
	op_t _operator;

	type_specifier_t* type_specifier;
	specifier_qualifier_list_t* specifier_qualifier_list;
	type_specifier_id _type_specifier_id;
	expression_t *expression;
	declaration_specifiers_t* declaration_specifiers;
	declarator_t* declarator;
	declaration_list_t* declaration_list;
	compound_statement_t* compound_statement;
	external_declaration_t* external_declaration;
	function_definition_t* function_definition;
	storage_class_specifier_t* storage_class_specifier;
	alignment_specifier_t* alignment_specifier;
	function_specifier_t* function_specifier;
	pointer_t* pointer;
	direct_declarator_t* direct_declarator;
	struct_or_union_specifier_t* struct_or_union_specifier;
	enum_specifier_t* enum_specifier;
	declaration_t* declaration;
	labeled_statement_t* labeled_statement;
	expression_statement_t* expression_statement;
	jump_statement_t* jump_statement;
	selection_statement_t* selection_statement;
	iteration_statement_t* iteration_statement;
	init_declarator_t* init_declarator;
	init_declarator_list_t* init_declarator_list;
	initializer_t* initializer;
	initializer_list_t* initializer_list;
	identifier_t* identifier;
	statement_t* statement;
	block_item_t* block_item;
	constant_t* constant;
	type_name_t* type_name;
	parameter_type_list_t* parameter_type_list;
	type_qualifier_list_t* type_qualifier_list;
	type_qualifier_t* type_qualifier;
	direct_abstract_declarator_t* direct_abstract_declarator;
	abstract_declarator_t* abstract_declarator;
	designator_list_t* designator_list;
	designator_t* designator;
	argument_expression_list_t* argument_expression_list;

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
%type <name> IDENTIFIER ENUMERATION_CONSTANT TYPEDEF_NAME string STRING_LITERAL enumeration_constant

%type <_int> I_CONSTANT struct_or_union ';' ':' CASE DEFAULT
	'{' '}' IF ELSE '(' ')' SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
	INC_OP DEC_OP SIZEOF ',' '*' '/' '%' '+' '-' LEFT_OP RIGHT_OP '<' '>'
	LE_OP GE_OP EQ_OP NE_OP '&' '^' '|' AND_OP OR_OP '=' '[' ']' '.' '?' PTR_OP
%type <_float> F_CONSTANT

// TODO: wrong: translation_unit?
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

%type <block_item> block_item

%type <type_name> type_name
%type <declaration> declaration
%type <statement> statement
//%type <identifier> identifier
%type <token> type_specifier_simple
%type <_operator> unary_operator assignment_operator
%type <declaration_specifiers> declaration_specifiers
%type <compound_statement> compound_statement block_item_list
%type <declarator > declarator
%type <declaration_list> declaration_list
%type <function_definition> function_definition
%type <external_declaration> external_declaration
%type <token> storage_class_specifier // TODO? token type?
%type <type_specifier> type_specifier
%type <token> alignment_specifier // TODO? token type?
%type <token> function_specifier // TODO? token type?
%type <pointer> pointer
%type <direct_declarator> direct_declarator
%type <struct_or_union_specifier> struct_or_union_specifier
%type <enum_specifier> enum_specifier
%type <iteration_statement> iteration_statement
%type <selection_statement> selection_statement
%type <expression_statement> expression_statement
%type <init_declarator> init_declarator
%type <init_declarator_list> init_declarator_list
%type <initializer> initializer
%type <initializer_list> initializer_list
%type <labeled_statement> labeled_statement
%type <jump_statement> jump_statement
%type <expression> conditional_expression constant_expression
%type <parameter_type_list> parameter_type_list
%type <type_qualifier_list> type_qualifier_list
%type <direct_abstract_declarator> direct_abstract_declarator
%type <abstract_declarator> abstract_declarator
%type <argument_expression_list> argument_expression_list
%type <specifier_qualifier_list> specifier_qualifier_list
%type <type_qualifier> type_qualifier

%type <designator_list> designator_list;
%type <designator> designator;

%type <token> CONST RESTRICT VOLATILE ATOMIC TYPEDEF EXTERN STATIC AUTO REGISTER
	VOID CHAR SHORT INT LONG FLOAT DOUBLE SIGNED UNSIGNED BOOL COMPLEX INLINE

%start translation_unit

%printer { fprintf (yyoutput, "constant %d", $$); } I_CONSTANT
// %printer { fprintf (yyoutput, "type %d", $$.expression->type); } <>
%printer { fprintf (yyoutput, "<...?>"); } <>


%%

primary_expression // parents: postfix_expression
	: IDENTIFIER { alloc($$); move_str($$->identifier, $1); $$->type = pt_id; }
	| constant { alloc($$); $$->constant = $1; $$->type = pt_constant; }
	| string { alloc($$); move_str($$->string, $1); $$->type = pt_string; }
	| '(' expression ')' { alloc($$); $$->lbrace = t($1); $$->expression = $2; $$->rbrace=t($3); $$->type = pt_expression; }
	| generic_selection { c11(); }
	;

constant // parents: primary expression
	: I_CONSTANT { alloc($$); $$->value.i = $1; $$->type = ct_int; }		/* includes character_constant */
	| F_CONSTANT { alloc($$); $$->value.f = $1; $$->type = ct_float; }
	| ENUMERATION_CONSTANT{ alloc($$); $$->enum_id = new identifier_t($1); $$->type = ct_enum; }	/* after it has been defined as such */
	;

enumeration_constant		/* before it has been defined as such */
	: IDENTIFIER { $$ = $1; }
	;

string
	: STRING_LITERAL { $$=$1; }
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

// FEATURE: base class: nary_expression_t?
postfix_expression
	: primary_expression { $$=$1; }
	| postfix_expression '[' expression ']' { array_access_expression_t* u; $$=alloc(u); u->c.fill($1, t($2), $3, t($4)); u->op_id = op_arr_acc; }
	| postfix_expression '(' ')' { function_call_expression_t* u; $$=alloc(u); u->c.fill($1, t($2), NULL, t($3)); u->op_id = op_func_call; }
	| postfix_expression '(' argument_expression_list ')' { function_call_expression_t* u; $$=alloc(u); u->c.fill($1, t($2), $3, t($4)); u->op_id = op_func_call; }
	| postfix_expression '.' IDENTIFIER { struct_access_expression_t* e; $$=alloc(e); e->c.fill($1, t($2), new identifier_t($3)); e->op_id = op_struct_access_ref; }
	| postfix_expression PTR_OP IDENTIFIER { struct_access_expression_t* e; $$=alloc(e); e->c.fill($1, t($2), new identifier_t($3)); e->op_id = op_struct_access_ptr; }
	| postfix_expression INC_OP { unary_expression_r* u; $$=alloc(u); u->c.fill($1, t($2)); u->op_id = op_inc_post; }
	| postfix_expression DEC_OP { unary_expression_r* u; $$=alloc(u); u->c.fill($1, t($2)); u->op_id = op_dec_post; }
	| '(' type_name ')' '{' initializer_list '}' { cast_postfix_expression_t* e; $$=alloc(e); e->c.fill(t($1), $2, t($3), t($4), $5, NULL, t($6)); e->op_id = op_cast_postfix; }
	| '(' type_name ')' '{' initializer_list ',' '}' { cast_postfix_expression_t* e; $$=alloc(e); e->c.fill(t($1), $2, t($3), t($4), $5, t($6), t($7)); e->op_id = op_cast_postfix; }
	;

// FEATURE: alternate list<token_t, assignment_exp>
argument_expression_list
	: assignment_expression { alloc($$); $$->c.fill(NULL, NULL, $1); }
	| argument_expression_list ',' assignment_expression { alloc($$); $$->c.fill($1, t($2), $3); }
	;

unary_expression
	: postfix_expression { $$=$1; }
	| INC_OP unary_expression { unary_expression_l* u; $$=alloc(u); u->c.fill(t($1), $2); u->op_id = op_inc_pre; }
	| DEC_OP unary_expression { unary_expression_l* u; $$=alloc(u); u->c.fill(t($1), $2); u->op_id = op_dec_pre; }
	| unary_operator cast_expression { unary_expression_l* u; $$=alloc(u); u->c.fill(t($1), $2); u->op_id = $1; }
	| SIZEOF unary_expression { not_yet(); }
	| SIZEOF '(' type_name ')' { sizeof_expression_t* e; alloc(e); e->c.set(t($1)).set(t($2)).set($3).set(t($4)); $$=e; }
	| ALIGNOF '(' type_name ')' { c11(); }
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
	| '(' type_name ')' cast_expression { cast_expression_t* c; ($$=alloc(c))->op_id = op_cast; c->c.fill(t($1), $2, t($3), $4); }
	;

multiplicative_expression
	: cast_expression { $$=$1; }
	| multiplicative_expression '*' cast_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_mult; e->c.fill($1, t($2), $3); }
	| multiplicative_expression '/' cast_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_div; e->c.fill($1, t($2), $3); }
	| multiplicative_expression '%' cast_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_mod; e->c.fill($1, t($2), $3); }
	;

additive_expression
	: multiplicative_expression { $$=$1; }
	| additive_expression '+' multiplicative_expression { binary_expression_t* e; $$=alloc(e); e; e->op_id = op_plus; e->c.fill($1, t($2), $3); }
	| additive_expression '-' multiplicative_expression { binary_expression_t* e; $$=alloc(e); e; e->op_id = op_minus; e->c.fill($1, t($2), $3); }
	;

shift_expression
	: additive_expression { $$=$1; }
	| shift_expression LEFT_OP additive_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_lshift; e->c.fill($1, t($2), $3); }
	| shift_expression RIGHT_OP additive_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_rshift; e->c.fill($1, t($2), $3); }
	;

relational_expression
	: shift_expression { $$=$1; }
	| relational_expression '<' shift_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_lt; e->c.fill($1, t($2), $3); }
	| relational_expression '>' shift_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_gt; e->c.fill($1, t($2), $3); }
	| relational_expression LE_OP shift_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_le; e->c.fill($1, t($2), $3); }
	| relational_expression GE_OP shift_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_ge; e->c.fill($1, t($2), $3); }
	;

equality_expression
	: relational_expression { $$=$1; }
	| equality_expression EQ_OP relational_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_eq; e->c.fill($1, t($2), $3); }
	| equality_expression NE_OP relational_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_ne; e->c.fill($1, t($2), $3); }
	;

and_expression
	: equality_expression { $$=$1; }
	| and_expression '&' equality_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_band; e->c.fill($1, t($2), $3); }
	;

exclusive_or_expression
	: and_expression { $$=$1; }
	| exclusive_or_expression '^' and_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_xor; e->c.fill($1, t($2), $3); }
	;

inclusive_or_expression
	: exclusive_or_expression { $$=$1; }
	| inclusive_or_expression '|' exclusive_or_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_bor; e->c.fill($1, t($2), $3); }
	;

logical_and_expression
	: inclusive_or_expression { $$=$1; }
	| logical_and_expression AND_OP inclusive_or_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_and; e->c.fill($1, t($2), $3); }
	;

logical_or_expression
	: logical_and_expression { $$=$1; }
	| logical_or_expression OR_OP logical_and_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_or; e->c.fill($1, t($2), $3); }
	;

conditional_expression
	: logical_or_expression { $$=$1; }
	| logical_or_expression '?' expression ':' conditional_expression { ternary_expression_t* e; $$=alloc(e); e->op_id = op_tern; e->c.fill($1, t($2), $3, t($4), $5); }
	;

assignment_expression
	: conditional_expression { $$=$1; }
	| unary_expression assignment_operator assignment_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_asn; e->c.fill($1, t($2), $3); }
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
	| expression ',' assignment_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_asn; e->c.fill($1, t($2), $3); }
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
	| storage_class_specifier { $$ = new declaration_specifiers_t; app_left($$, $$->specifiers, $1); } // TODO: alloc($$)?
	| type_specifier declaration_specifiers { $$ = app_left($2, $2->specifiers, $1); }
	| type_specifier { $$ = new declaration_specifiers_t; app_left($$, $$->specifiers, $1); } // TODO: -> ???
	| type_qualifier declaration_specifiers { $$ = app_left($2, $2->specifiers, $1); }
	| type_qualifier { $$ = new declaration_specifiers_t; app_left($$, $$->specifiers, $1); }
	| function_specifier declaration_specifiers { $$ = app_left($2, $2->specifiers, $1); }
	| function_specifier { $$ = new declaration_specifiers_t; app_left($$, $$->specifiers, $1); }
	| alignment_specifier declaration_specifiers { $$ = app_left($2, $2->specifiers, $1); }
	| alignment_specifier { $$ = new declaration_specifiers_t; app_left($$, $$->specifiers, $1); }
	;

init_declarator_list
	: init_declarator { alloc($$); $$->c.fill(NULL, NULL, $1); }
	| init_declarator_list ',' init_declarator { alloc($$); $$->c.fill($1, t($2), $3); }
	;

init_declarator
	: declarator '=' initializer { alloc($$); $$->c.fill($1, t($2), $3); }
	| declarator { alloc($$); $$->c.set($1); }
	;

storage_class_specifier
	: TYPEDEF { $$=$1; }	/* identifiers must be flagged as TYPEDEF_NAME */
	| EXTERN { $$=$1; }
	| STATIC { $$=$1; }
	| THREAD_LOCAL { c11(); }
	| AUTO { $$=$1; }
	| REGISTER { $$=$1; }
	;

type_specifier_simple
	: VOID { $$ = $1; }
	| CHAR { $$ = $1; }
	| SHORT { $$ = $1; }
	| INT { $$ = $1; }
	| LONG { $$ = $1; }
	| FLOAT { $$ = $1; }
	| DOUBLE { $$ = $1; }
	| SIGNED { $$ = $1; }
	| UNSIGNED { $$ = $1; }
	| BOOL { $$ = $1; }
	| COMPLEX { $$ = $1; }
	| IMAGINARY { c11(); }		/* non-mandated extension */
	;

type_specifier
	: type_specifier_simple { type_specifier_token* t; $$ = alloc(t); t->c = $1; }
	| atomic_type_specifier { c11(); }
	| struct_or_union_specifier { /*alloc($$); $$->c.set($1);*/ not_yet(); }
	| enum_specifier { /*alloc($$); $$->c.set($1);*/ not_yet() }
	| TYPEDEF_NAME { type_identifier* t; $$ = alloc(t); t->c = new identifier_t($1); }	/* after it has been defined as such */
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
	: type_specifier specifier_qualifier_list { $$ = app_left($2, $2->c, $1); }
	| type_specifier { alloc($$); app_left($$, $$->c, $1); }
	| type_qualifier specifier_qualifier_list { $$ = app_left($2, $2->c, $1); }
	| type_qualifier { alloc($$); app_left($$, $$->c, $1); }
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
	: CONST { alloc($$); $$->c = $1; }
	| RESTRICT { alloc($$); $$->c = $1; }
	| VOLATILE { alloc($$); $$->c = $1; }
	| ATOMIC { alloc($$); $$->c = $1; }
	;

function_specifier
	: INLINE { $$=$1; }
	| NORETURN { c11(); }
	;

alignment_specifier
	: ALIGNAS '(' type_name ')' { c11(); }
	| ALIGNAS '(' constant_expression ')' { c11(); }
	;

declarator
	: pointer direct_declarator { alloc($$); $$->c.fill($1, $2); }
	| direct_declarator { alloc($$); $$->c.get_next().set($1); }
	;

// NOTE: static etc in [] can only appear in function parameter declarations
// direct_declarator_id
// direct_declarator_decl
// direct_declarator_arr_expr
// direct_declarator_func

direct_declarator
	: IDENTIFIER { direct_declarator_id* d; $$ = alloc(d); d->value = new identifier_t($1); }
	| '(' declarator ')' { direct_declarator_decl* d; $$ = alloc(d); d->c.fill(t($1), $2, t($3)); }
	| direct_declarator '[' ']' { direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, t($2), NULL, NULL, NULL, NULL, t($3)); }
	| direct_declarator '[' '*' ']' { direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, t($2), NULL, NULL, NULL, t($3), t($4)); }
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, t($2), $3, $4, $5, NULL, t($6)); }
	| direct_declarator '[' STATIC assignment_expression ']' { c11(); }
	| direct_declarator '[' type_qualifier_list '*' ']' { direct_declarator_arr* d; $$ = alloc(d);
		// TODO: autoptr that converts to pointer in assignment to $$?
		d->c.fill($1, t($2), NULL, $3, NULL, t($4), t($5)); }
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, t($2), $4, $3, $5, NULL, t($6)); }
	| direct_declarator '[' type_qualifier_list assignment_expression ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, t($2), NULL, $3, $4, NULL, t($5)); }
	| direct_declarator '[' type_qualifier_list ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, t($2), NULL, $3, NULL, NULL, t($4)); }
	| direct_declarator '[' assignment_expression ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, t($2), NULL, NULL, $3, NULL, t($4)); }
	| direct_declarator '(' parameter_type_list ')' {
		direct_declarator_func* d; $$ = alloc(d);
		d->c.fill($1, t($2), $3, t($4)); }
	| direct_declarator '(' ')' {
		direct_declarator_func* d; $$ = alloc(d);
		d->c.fill($1, t($2), NULL, t($3)); }
	| direct_declarator '(' identifier_list ')' { not_yet(); }
	;

pointer // should actually be named: "pointers"
	: '*' type_qualifier_list pointer { alloc($$); $$->c.fill(t($1), $2, $3); }
	| '*' type_qualifier_list { alloc($$); $$->c.fill(t($1), $2, NULL); }
	| '*' pointer { alloc($$); $$->c.fill(t($1), NULL, $2); }
	| '*' { alloc($$); $$->c.set(t($1)); }
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
	: specifier_qualifier_list abstract_declarator { alloc($$); $$->c.fill($1, $2); }
	| specifier_qualifier_list { alloc($$); $$->c.set($1); }
	;

abstract_declarator
	: pointer direct_abstract_declarator { alloc($$); $$->c.fill($1, $2); }
	| pointer { alloc($$); $$->c.set($1); }
	| direct_abstract_declarator { alloc($$); $$->c.fill(NULL, $1); }
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' { direct_abstract_declarator_decl* d; $$ = alloc(d); d->c.fill(t($1), $2, t($3)); }
	| '[' ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); d->c.fill( NULL, t($1), NULL, NULL, t($2)); }
	| '[' '*' ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); d->c.fill( NULL, t($1), NULL, t($2), t($3)); }
	| '[' STATIC type_qualifier_list assignment_expression ']' { c11(); }
	| '[' STATIC assignment_expression ']' { c11(); }
	| '[' type_qualifier_list STATIC assignment_expression ']' { c11(); }
	| '[' type_qualifier_list assignment_expression ']' { c11(); }
	| '[' type_qualifier_list ']' { c11(); }
	| '[' assignment_expression ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); d->c.fill( NULL, t($1), $2, NULL, t($3)); }
	| direct_abstract_declarator '[' ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); d->c.fill( $1, t($2), NULL, NULL, t($3)); }
	| direct_abstract_declarator '[' '*' ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); d->c.fill( $1, t($2), NULL, t($3), t($4)); }
	| direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']' { c11(); }
	| direct_abstract_declarator '[' STATIC assignment_expression ']' { c11(); }
	| direct_abstract_declarator '[' type_qualifier_list assignment_expression ']' { c11(); }
	| direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']' { c11(); }
	| direct_abstract_declarator '[' type_qualifier_list ']' { c11(); }
	| direct_abstract_declarator '[' assignment_expression ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); d->c.fill( $1, t($2), $3, NULL, t($4)); }
	| '(' ')' {
		direct_abstract_declarator_func* d; $$ = alloc(d);
		d->c.fill(NULL, t($1), NULL, t($2)); }
	| '(' parameter_type_list ')' {
		direct_abstract_declarator_func* d; $$ = alloc(d);
		d->c.fill(NULL, t($1), $2, t($3)); }
	| direct_abstract_declarator '(' ')' {
		direct_abstract_declarator_func* d; $$ = alloc(d);
		d->c.fill($1, t($2), NULL, t($3)); }
	| direct_abstract_declarator '(' parameter_type_list ')' {
		direct_abstract_declarator_func* d; $$ = alloc(d);
		d->c.fill($1, t($2), $3, t($4)); }
	;

initializer
	: '{' initializer_list '}' { alloc($$); $$->c.fill(NULL, t($1), $2, NULL, t($3)); }
	| '{' initializer_list ',' '}'{ alloc($$); $$->c.fill(NULL, t($1), $2, t($3), t($4)); }
	| assignment_expression { alloc($$); $$->c.set($1); }
	;

initializer_list
	: designator_list '=' initializer { alloc($$); $$->c.fill(NULL, NULL, $1, t($2), $3); }
	| initializer { alloc($$); $$->c.fill(NULL, NULL, NULL, NULL, $1); }
	| initializer_list ',' designator_list '=' initializer { alloc($$); $$->c.fill($1, t($2), $3, t($4), $5); }
	| initializer_list ',' initializer { alloc($$); $$->c.fill($1, t($2), NULL, NULL, $3); }
	;

// FEATURE: -> removed this, put eq sign into initializer_list? is this ok?
//designation
//	: designator_list '='
//	;

designator_list
	: designator { alloc($$); app_right($$, $$->c, $1); }
	| designator_list designator { $$ = app_right($1, $1->c, $2); }
	;

designator
	: '[' constant_expression ']' { designator_constant_expr* d; $$ = alloc(d); d->c.fill(t($1), $2, t($3)); }
	| '.' IDENTIFIER { designator_id* d; $$ = alloc(d); d->c.fill(t($1), new identifier_t($2)); }
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
	: IDENTIFIER ':' statement { alloc($$); $$->identifier = new identifier_t($1); $$->colon = t($2); $$->statement = $3; }
	| CASE constant_expression ':' statement { alloc($$); $$->case_token = t($1); $$->expression = $2; $$->colon = t($3); $$->statement = $4; }
	| DEFAULT ':' statement { alloc($$); $$->default_token = t($1); $$->colon = t($2); $$->statement = $3; }
	;

compound_statement
	: '{' '}' { alloc($$); $$->lbrack = t($1); $$->rbrack = t($2); }
	| '{'  block_item_list '}'  { /* extend $2 by brackets */ $$=$2; $$->lbrack = t($1); $$->rbrack=t($3); }
	;

block_item_list
	: block_item { alloc($$); app_first($$->block_items, $1); }
	| block_item_list block_item { app_right($$, $$->block_items, $2); }
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
	| translation_unit external_declaration { dynamic_cast<translation_unit_t*>(*expression)->v.push_back($2); /* append_t<external_declaration_t> a($2); (*expression)->accept(a); (*expression)->append($2);*/ }
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
