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
//#include "visitor.h"

// TODO: check for no returns anywhere!!, i.e. no return keyword in any rule!!

int yyerror(translation_unit_t ** /*expression*/, yyscan_t scanner, const char *msg) {
	fprintf(stderr,"At: %s,\n  line %d, column %d: Error:%s\n", yyget_text(scanner), yyget_lineno(scanner), yyget_column(scanner), msg); return 0;
	// Add error handling routine as needed
}

extern geom_t get_pos();
extern void set_scope(node_base* new_scope);

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

extern std::vector<terminal_t*>& get_token_vector();

//terminal_t* app_token() { return get_token_vector().back(); }

template<class T> T* alloc(T*& ptr_ref) { return ptr_ref = new T(); /* be C++03 conform */ }

token_t* t(int token_id) { return new token_t(get_pos(), token_id); }

extern void flag_symbol(const char* str, int type, const node_base* scope);
extern void leave_scope(const node_base* scope);
extern int type_of(const char* str);
extern bool recent_enum_flag;

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
	struct identifier_t* name;
	float _float;
	int _int;
	struct token_t* token;
	class node_base* node;

	struct type_specifier_t* type_specifier;
	struct specifier_qualifier_list_t* specifier_qualifier_list;
	struct expression_t *expression;
	struct declaration_specifiers_t* declaration_specifiers;
	struct declarator_t* declarator;
	struct declaration_list_t* declaration_list;
	struct compound_statement_t* compound_statement;
	struct external_declaration_t* external_declaration;
	struct function_definition_t* function_definition;
/*	storage_class_specifier_t* storage_class_specifier;
	alignment_specifier_t* alignment_specifier;
	function_specifier_t* function_specifier;*/
	struct pointer_t* pointer;
	struct direct_declarator_t* direct_declarator;
	struct struct_or_union_specifier_t* struct_or_union_specifier;
	struct enum_specifier_t* enum_specifier;
	struct declaration_t* declaration;
	struct labeled_statement_t* labeled_statement;
	struct expression_statement_t* expression_statement;
	struct jump_statement_t* jump_statement;
	struct selection_statement_t* selection_statement;
	struct iteration_statement_t* iteration_statement;
	struct init_declarator_t* init_declarator;
	struct init_declarator_list_t* init_declarator_list;
	struct initializer_t* initializer;
	struct initializer_list_t* initializer_list;
	identifier_t* identifier;
	struct statement_t* statement;
	struct block_item_t* block_item;
//	struct constant_t* constant;
	struct type_name_t* type_name;

	struct struct_declaration_t* struct_declaration;
	struct struct_declaration_list_t* struct_declaration_list;
	struct struct_declarator_list_t* struct_declarator_list;
	struct struct_declarator_t* struct_declarator;
	struct enumerator_list_t* enumerator_list;
	struct enumerator_t* enumerator;
	struct parameter_list_t* parameter_list;
	struct parameter_declaration_t* parameter_declaration;
	struct identifier_list_t* identifier_list;

	struct parameter_type_list_t* parameter_type_list;
	struct type_qualifier_list_t* type_qualifier_list;
	struct type_qualifier_t* type_qualifier;
	struct direct_abstract_declarator_t* direct_abstract_declarator;
	struct abstract_declarator_t* abstract_declarator;
	struct designator_list_t* designator_list;
	struct designator_t* designator;
	struct argument_expression_list_t* argument_expression_list;

	struct primary_expression_t* primary_expression;
	struct terminal_t* terminal;
	struct iconstant_t* iconstant;
	struct fconstant_t* fconstant;

	struct string_literal_t* string_literal;

	struct translation_unit_t* translation_unit;
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

%type <name> IDENTIFIER ENUMERATION_CONSTANT TYPEDEF_NAME

%type <string_literal> STRING_LITERAL
%type <token> I_CONSTANT F_CONSTANT

// FEATURE: wrong: translation_unit_t?
%type <translation_unit> translation_unit
%type <node> static_assert_declaration

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
%type <terminal> constant string

%type <block_item> block_item
 // TODO: _operator unused?
%type <type_name> type_name
%type <declaration> declaration
%type <statement> statement
%type <identifier> enumeration_constant
%type <token> type_specifier_simple unary_operator assignment_operator
%type <declaration_specifiers> declaration_specifiers
%type <compound_statement> compound_statement block_item_list
%type <declarator> declarator
%type <declaration_list> declaration_list
%type <function_definition> function_definition
%type <external_declaration> external_declaration
%type <token> storage_class_specifier
%type <type_specifier> type_specifier
%type <token> alignment_specifier
%type <token> function_specifier
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

%type <struct_declaration_list> struct_declaration_list
%type <struct_declaration> struct_declaration
%type <struct_declarator_list> struct_declarator_list
%type <struct_declarator> struct_declarator
%type <enumerator_list> enumerator_list
%type <enumerator> enumerator
%type <parameter_list> parameter_list
%type <parameter_declaration> parameter_declaration
%type <identifier_list> identifier_list

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
	'(' ')' struct_or_union ';' ':' CASE DEFAULT
	'{' '}' IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
	INC_OP DEC_OP SIZEOF ',' '*' '/' '%' '+' '-' LEFT_OP RIGHT_OP '<' '>'
	LE_OP GE_OP EQ_OP NE_OP '&' '^' '|' AND_OP OR_OP '=' '[' ']' '.' '?' PTR_OP
	'~' '!' MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN
	LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
	STRUCT UNION ENUM ELLIPSIS

%start translation_unit

%printer { fprintf (yyoutput, "constant %d", $$); } I_CONSTANT
// %printer { fprintf (yyoutput, "type %d", $$.expression->type); } <>
%printer { fprintf (yyoutput, "<...?>"); } <>

%%

primary_expression // parents: postfix_expression
	: IDENTIFIER { if(type_of($1->raw.c_str())==lt_undefined) throw "identifier undefined"; else
		{ alloc($$); $$->c.fill(NULL, $1); } }
	| constant { alloc($$); $$->c.set($1); }
	| string { alloc($$); $$->c.set($1); }
	| '(' expression ')' { alloc($$); $$->c.fill(NULL, NULL, $1, $2, $3); }
	| generic_selection { c11(); }
	;

constant // parents: primary expression
	: I_CONSTANT { $$=$1; }		/* includes character_constant */
	| F_CONSTANT { $$=$1; }
	| ENUMERATION_CONSTANT{ not_yet(); }	/* after it has been defined as such */
	;

enumeration_constant		/* before it has been defined as such */
	: IDENTIFIER { $$ = $1; }
	;

string
	: STRING_LITERAL { $$=$1; /*c->c.set($1);*/ }
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
	| postfix_expression '[' expression ']' { array_access_expression_t* u; $$=alloc(u); u->c.fill($1, $2, $3, $4); }
	| postfix_expression '(' ')' { function_call_expression_t* u; $$=alloc(u); u->c.fill($1, $2, NULL, $3); }
	| postfix_expression '(' argument_expression_list ')' { function_call_expression_t* u; $$=alloc(u); u->c.fill($1, $2, $3, $4); }
	| postfix_expression '.' IDENTIFIER { struct_access_expression_t* e; $$=alloc(e); e->c.fill($1, $2, $3); } // FEATURE: bool if with pointer?
	| postfix_expression PTR_OP IDENTIFIER { struct_access_expression_t* e; $$=alloc(e); e->c.fill($1, $2, $3); }
	| postfix_expression INC_OP { unary_expression_r* u; $$=alloc(u); u->c.fill($1, $2); }
	| postfix_expression DEC_OP { unary_expression_r* u; $$=alloc(u); u->c.fill($1, $2); }
	| '(' type_name ')' '{' initializer_list '}' { cast_postfix_expression_t* e; $$=alloc(e); e->c.fill($1, $2, $3, $4, $5, NULL, $6); }
	| '(' type_name ')' '{' initializer_list ',' '}' { cast_postfix_expression_t* e; $$=alloc(e); e->c.fill($1, $2, $3, $4, $5, $6, $7); }
	;

// FEATURE: alternate list<token_t, assignment_exp>
argument_expression_list
	: assignment_expression { alloc($$); $$->c.fill(NULL, NULL, $1); }
	| argument_expression_list ',' assignment_expression { alloc($$); $$->c.fill($1, $2, $3); }
	;

unary_expression
	: postfix_expression { $$=$1; }
	| INC_OP unary_expression { unary_expression_l* u; $$=alloc(u); u->c.fill($1, $2); }
	| DEC_OP unary_expression { unary_expression_l* u; $$=alloc(u); u->c.fill($1, $2); }
	| unary_operator cast_expression { unary_expression_l* u; $$=alloc(u); u->c.fill($1, $2); /*u->op_id = $1;*/ }
	| SIZEOF unary_expression { not_yet(); }
	| SIZEOF '(' type_name ')' { sizeof_expression_t* e; alloc(e); e->c.fill($1,$2,$3,$4); $$=e; }
	| ALIGNOF '(' type_name ')' { c11(); }
	;

unary_operator
	: '&' { $$=$1; /*op_addr;*/ }
	| '*' { $$=$1; /*op_ptr;*/ }
	| '+' { $$=$1; /*op_pos;*/ }
	| '-' { $$=$1; /*op_neg;*/ }
	| '~' { $$=$1; /*op_compl;*/ }
	| '!' { $$=$1; /*op_not;*/ }
	;

cast_expression
	: unary_expression { $$=$1; }
	| '(' type_name ')' cast_expression { cast_expression_t* c; $$=alloc(c); c->c.fill($1, $2, $3, $4); }
	;

multiplicative_expression
	: cast_expression { $$=$1; }
	| multiplicative_expression '*' cast_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_mult; e->c.fill($1, $2, $3); }
	| multiplicative_expression '/' cast_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_div; e->c.fill($1, $2, $3); }
	| multiplicative_expression '%' cast_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_mod; e->c.fill($1, $2, $3); }
	;

additive_expression
	: multiplicative_expression { $$=$1; }
	| additive_expression '+' multiplicative_expression { binary_expression_t* e; $$=alloc(e); e; e->op_id = op_plus; e->c.fill($1, $2, $3); }
	| additive_expression '-' multiplicative_expression { binary_expression_t* e; $$=alloc(e); e; e->op_id = op_minus; e->c.fill($1, $2, $3); }
	;

shift_expression
	: additive_expression { $$=$1; }
	| shift_expression LEFT_OP additive_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_lshift; e->c.fill($1, $2, $3); }
	| shift_expression RIGHT_OP additive_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_rshift; e->c.fill($1, $2, $3); }
	;

relational_expression
	: shift_expression { $$=$1; }
	| relational_expression '<' shift_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_lt; e->c.fill($1, $2, $3); }
	| relational_expression '>' shift_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_gt; e->c.fill($1, $2, $3); }
	| relational_expression LE_OP shift_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_le; e->c.fill($1, $2, $3); }
	| relational_expression GE_OP shift_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_ge; e->c.fill($1, $2, $3); }
	;

equality_expression
	: relational_expression { $$=$1; }
	| equality_expression EQ_OP relational_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_eq; e->c.fill($1, $2, $3); }
	| equality_expression NE_OP relational_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_ne; e->c.fill($1, $2, $3); }
	;

and_expression
	: equality_expression { $$=$1; }
	| and_expression '&' equality_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_band; e->c.fill($1, $2, $3); }
	;

exclusive_or_expression
	: and_expression { $$=$1; }
	| exclusive_or_expression '^' and_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_xor; e->c.fill($1, $2, $3); }
	;

inclusive_or_expression
	: exclusive_or_expression { $$=$1; }
	| inclusive_or_expression '|' exclusive_or_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_bor; e->c.fill($1, $2, $3); }
	;

logical_and_expression
	: inclusive_or_expression { $$=$1; }
	| logical_and_expression AND_OP inclusive_or_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_and; e->c.fill($1, $2, $3); }
	;

logical_or_expression
	: logical_and_expression { $$=$1; }
	| logical_or_expression OR_OP logical_and_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_or; e->c.fill($1, $2, $3); }
	;

conditional_expression
	: logical_or_expression { $$=$1; }
	| logical_or_expression '?' expression ':' conditional_expression { ternary_expression_t* e; $$=alloc(e); e->c.fill($1, $2, $3, $4, $5); }
	;

assignment_expression
	: conditional_expression { $$=$1; }
	| unary_expression assignment_operator assignment_expression { binary_expression_t* e; $$=alloc(e); /*e->op_id = op_asn*/; e->c.fill($1, $2, $3); }
	;

assignment_operator
	: '=' { $$=$1; /* op_asn */ }
	| MUL_ASSIGN { $$=$1; /* op_asn_mul */ }
	| DIV_ASSIGN { $$=$1; /* op_asn_div */ }
	| MOD_ASSIGN { $$=$1; /* op_asn_mod */ }
	| ADD_ASSIGN { $$=$1; /* op_asn_add */ }
	| SUB_ASSIGN { $$=$1; /* op_asn_sub */ }
	| LEFT_ASSIGN { $$=$1; /* op_asn_left */ }
	| RIGHT_ASSIGN { $$=$1; /* op_asn_right */ }
	| AND_ASSIGN { $$=$1; /* op_asn_and */ }
	| XOR_ASSIGN { $$=$1; /* op_asn_xor */ }
	| OR_ASSIGN { $$=$1; /* op_asn_or */ }
	;

expression
	: assignment_expression { $$=$1; }
	| expression ',' assignment_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_asn; e->c.fill($1, $2, $3); }
	;

constant_expression
	: conditional_expression { $$=$1; }	/* with constraints */
	;

declaration
	: declaration_specifiers ';' { $$ = new declaration_t(); $$->c.fill($1, NULL, $2); }
	| declaration_specifiers init_declarator_list ';' { $$ = new declaration_t(); $$->c.fill($1, $2, $3); }
	| static_assert_declaration { c11(); }
	;

declaration_specifiers
	: storage_class_specifier declaration_specifiers { $$ = app_left($2, $2->c, $1); }
	| storage_class_specifier { alloc($$); app_left($$, $$->c, $1); }
	| type_specifier declaration_specifiers { $$ = app_left($2, $2->c, $1); }
	| type_specifier { alloc($$); app_left($$, $$->c, $1); } // TODO: -> better return type than node_t?
	| type_qualifier declaration_specifiers { $$ = app_left($2, $2->c, $1); }
	| type_qualifier { alloc($$); app_left($$, $$->c, $1); }
	| function_specifier declaration_specifiers { $$ = app_left($2, $2->c, $1); }
	| function_specifier { alloc($$); app_left($$, $$->c, $1); }
	| alignment_specifier declaration_specifiers { $$ = app_left($2, $2->c, $1); }
	| alignment_specifier { alloc($$); app_left($$, $$->c, $1); }
	;

init_declarator_list
	: init_declarator { alloc($$); $$->c.fill(NULL, NULL, $1); }
	| init_declarator_list ',' init_declarator { alloc($$); $$->c.fill($1, $2, $3); }
	;

init_declarator
	: declarator '=' initializer { alloc($$); $$->c.fill($1, $2, $3); }
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
	: type_specifier_simple { $$ = alloc($$); $$->c.set($1); }
	| atomic_type_specifier { c11(); }
	| struct_or_union_specifier { $$ = alloc($$); $$->c.set($1); }
	| enum_specifier { $$ = alloc($$); $$->c.set($1); }
	| TYPEDEF_NAME { $$ = alloc($$); $$->c.set($1); }	/* after it has been defined as such */
	;

struct_or_union_specifier
	: struct_or_union '{' struct_declaration_list '}' { alloc($$); $$->c.fill($1, NULL, $2, $3, $4); }
	| struct_or_union IDENTIFIER '{' struct_declaration_list '}' { alloc($$); $$->c.fill($1, $2, $3, $4, $5); }
	| struct_or_union IDENTIFIER { alloc($$); $$->c.fill($1, $2); }
	;

struct_or_union
	: STRUCT { $$ = $1; }
	| UNION { $$ = $1; }
	;

struct_declaration_list
	: struct_declaration { alloc($$); app_first($$->c, $1); }
	| struct_declaration_list struct_declaration { app_right($1, $1->c, $2); }
	;

struct_declaration
	: specifier_qualifier_list ';' { alloc($$); $$->c.fill($1, NULL, $2); } /* for anonymous struct/union */
	| specifier_qualifier_list struct_declarator_list ';' { alloc($$); $$->c.fill($1, $2, $3); }
	| static_assert_declaration { c11(); }
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list { $$ = app_left($2, $2->c, $1); }
	| type_specifier { alloc($$); app_left($$, $$->c, $1); }
	| type_qualifier specifier_qualifier_list { $$ = app_left($2, $2->c, $1); }
	| type_qualifier { alloc($$); app_left($$, $$->c, $1); }
	;

struct_declarator_list
	: struct_declarator { alloc($$); $$->c.fill(NULL, NULL, $1); }
	| struct_declarator_list ',' struct_declarator { alloc($$); $$->c.fill($1, $2, $3); }
	;

struct_declarator
	: ':' constant_expression { alloc($$); $$->c.fill(NULL, $1, $2); }
	| declarator ':' constant_expression { alloc($$); $$->c.fill($1, $2, $3); }
	| declarator { alloc($$); $$->c.set($1); }
	;

enum_specifier
	: ENUM '{' enumerator_list '}' { alloc($$); $$->c.fill($1, NULL, $2, $3, NULL, $4); }
	| ENUM '{' enumerator_list ',' '}' { alloc($$); $$->c.fill($1, NULL, $2, $3, $4, $5); }
	| ENUM IDENTIFIER '{' enumerator_list '}' { alloc($$); $$->c.fill($1, $2, $3, $4, NULL, $5); }
	| ENUM IDENTIFIER '{' enumerator_list ',' '}' { alloc($$); $$->c.fill($1, $2, $3, $4, $5, $6); }
	| ENUM IDENTIFIER { alloc($$); $$->c.fill($1, $2); }
	;

enumerator_list
	: enumerator { alloc($$); $$->c.fill(NULL, NULL, $1); }
	| enumerator_list ',' enumerator { alloc($$); $$->c.fill($1, $2, $3); }
	;

enumerator	/* identifiers must be flagged as ENUMERATION_CONSTANT */
	: enumeration_constant '=' constant_expression { recent_enum_flag = true; alloc($$); $$->c.fill($1, $2, $3); }
	| enumeration_constant { recent_enum_flag = true; alloc($$); $$->c.set($1); }
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
	: IDENTIFIER { direct_declarator_id* d; $$ = alloc(d); d->c = $1; }
	| '(' declarator ')' { direct_declarator_decl* d; $$ = alloc(d); d->c.fill($1, $2, $3); }
	| direct_declarator '[' ']' { direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, $2, NULL, NULL, NULL, NULL, $3); }
	| direct_declarator '[' '*' ']' { direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, $2, NULL, NULL, NULL, $3, $4); }
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, $2, $3, $4, $5, NULL, $6); }
	| direct_declarator '[' STATIC assignment_expression ']' { c11(); }
	| direct_declarator '[' type_qualifier_list '*' ']' { direct_declarator_arr* d; $$ = alloc(d);
		// FEATURE: autoptr that converts to pointer in assignment to $$?
		d->c.fill($1, $2, NULL, $3, NULL, $4, $5); }
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, $2, $4, $3, $5, NULL, $6); }
	| direct_declarator '[' type_qualifier_list assignment_expression ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, $2, NULL, $3, $4, NULL, $5); }
	| direct_declarator '[' type_qualifier_list ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, $2, NULL, $3, NULL, NULL, $4); }
	| direct_declarator '[' assignment_expression ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		d->c.fill($1, $2, NULL, NULL, $3, NULL, $4); }
	| direct_declarator '(' parameter_type_list ')' {
		direct_declarator_func* d; $$ = alloc(d);
		d->c.fill($1, $2, $3, $4); }
	| direct_declarator '(' ')' {
		direct_declarator_func* d; $$ = alloc(d);
		d->c.fill($1, $2, NULL, $3); }
	| direct_declarator '(' identifier_list ')' { not_yet(); }
	;

pointer // should actually be named: "pointers"
	: '*' type_qualifier_list pointer { alloc($$); $$->c.fill($1, $2, $3); }
	| '*' type_qualifier_list { alloc($$); $$->c.fill($1, $2, NULL); }
	| '*' pointer { alloc($$); $$->c.fill($1, NULL, $2); }
	| '*' { alloc($$); $$->c.set($1); }
	;

type_qualifier_list
	: type_qualifier { alloc($$); app_first($$->c, $1); }
	| type_qualifier_list type_qualifier { app_right($1, $1->c, $2); }
	;

parameter_type_list
	: parameter_list ',' ELLIPSIS { alloc($$); $$->c.fill($1, $2, $3); }
	| parameter_list { alloc($$); $$->c.set($1); }
	;

parameter_list
	: parameter_declaration { alloc($$); $$->c.fill(NULL, NULL, $1); }
	| parameter_list ',' parameter_declaration { alloc($$); $$->c.fill($1, $2, $3); }
	;

parameter_declaration
	: declaration_specifiers declarator { alloc($$); $$->c.fill($1, $2); }
	| declaration_specifiers abstract_declarator { alloc($$); $$->c.fill($1, NULL, $2); }
	| declaration_specifiers { alloc($$); $$->c.set($1); }
	;

identifier_list
	: IDENTIFIER { alloc($$); $$->c.fill(NULL, NULL, $1); }
	| identifier_list ',' IDENTIFIER { alloc($$); $$->c.fill($1, $2, $3); }
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
	: '(' abstract_declarator ')' { direct_abstract_declarator_decl* d; $$ = alloc(d); d->c.fill($1, $2, $3); }
	| '[' ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); d->c.fill( NULL, $1, NULL, NULL, $2); }
	| '[' '*' ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); d->c.fill( NULL, $1, NULL, $2, $3); }
	| '[' STATIC type_qualifier_list assignment_expression ']' { c11(); }
	| '[' STATIC assignment_expression ']' { c11(); }
	| '[' type_qualifier_list STATIC assignment_expression ']' { c11(); }
	| '[' type_qualifier_list assignment_expression ']' { c11(); }
	| '[' type_qualifier_list ']' { c11(); }
	| '[' assignment_expression ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); d->c.fill( NULL, $1, $2, NULL, $3); }
	| direct_abstract_declarator '[' ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); d->c.fill( $1, $2, NULL, NULL, $3); }
	| direct_abstract_declarator '[' '*' ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); d->c.fill( $1, $2, NULL, $3, $4); }
	| direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']' { c11(); }
	| direct_abstract_declarator '[' STATIC assignment_expression ']' { c11(); }
	| direct_abstract_declarator '[' type_qualifier_list assignment_expression ']' { c11(); }
	| direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']' { c11(); }
	| direct_abstract_declarator '[' type_qualifier_list ']' { c11(); }
	| direct_abstract_declarator '[' assignment_expression ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); d->c.fill( $1, $2, $3, NULL, $4); }
	| '(' ')' {
		direct_abstract_declarator_func* d; $$ = alloc(d);
		d->c.fill(NULL, $1, NULL, $2); }
	| '(' parameter_type_list ')' {
		direct_abstract_declarator_func* d; $$ = alloc(d);
		d->c.fill(NULL, $1, $2, $3); }
	| direct_abstract_declarator '(' ')' {
		direct_abstract_declarator_func* d; $$ = alloc(d);
		d->c.fill($1, $2, NULL, $3); }
	| direct_abstract_declarator '(' parameter_type_list ')' {
		direct_abstract_declarator_func* d; $$ = alloc(d);
		d->c.fill($1, $2, $3, $4); }
	;

initializer
	: '{' initializer_list '}' { alloc($$); $$->c.fill(NULL, $1, $2, NULL, $3); }
	| '{' initializer_list ',' '}'{ alloc($$); $$->c.fill(NULL, $1, $2, $3, $4); }
	| assignment_expression { alloc($$); $$->c.set($1); }
	;

initializer_list
	: designator_list '=' initializer { alloc($$); $$->c.fill(NULL, NULL, $1, $2, $3); }
	| initializer { alloc($$); $$->c.fill(NULL, NULL, NULL, NULL, $1); }
	| initializer_list ',' designator_list '=' initializer { alloc($$); $$->c.fill($1, $2, $3, $4, $5); }
	| initializer_list ',' initializer { alloc($$); $$->c.fill($1, $2, NULL, NULL, $3); }
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
	: '[' constant_expression ']' { designator_constant_expr* d; $$ = alloc(d); d->c.fill($1, $2, $3); }
	| '.' IDENTIFIER { designator_id* d; $$ = alloc(d); d->c.fill($1, $2); }
	;

static_assert_declaration
	: STATIC_ASSERT '(' constant_expression ',' STRING_LITERAL ')' ';' { c11(); }
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
	: IDENTIFIER ':' statement { alloc($$); $$->c.fill(NULL, $1, NULL, $2, $3); }
	| CASE constant_expression ':' statement { alloc($$); $$->c.fill($1, NULL, $2, $3, $4); }
	| DEFAULT ':' statement { alloc($$); $$->c.fill($1, NULL, NULL, $2, $3); }
	;

compound_statement
	: '{' '}' { alloc($$); $$->c.fill($1, NULL, $2); }
	| '{' block_item_list '}'  { /* extends $2 by brackets */ $$=$2; $$->c.set($1).get_next().set($3); }
	;

block_item_list // returns compound_statement_t
	: block_item { alloc($$); alloc($$->c.get_next().value); /* <- allocates virtual list node */ app_first(*$$->c.get_next().value, $1); }
	| block_item_list block_item { app_right($$, *$$->c.get_next().value, $2); }
	;

block_item
	: declaration { $$=$1; }
	| statement { $$=$1; }
	;

expression_statement
	: ';' { alloc($$); $$->c.fill(NULL, $1); }
	| expression ';' { alloc($$); $$->c.fill($1, $2); }
	;

selection_statement
	: IF '(' expression ')' statement ELSE statement { alloc($$); $$->c.fill($1, $2, $3, $4, $5, $6, $7); }
	| IF '(' expression ')' statement { alloc($$); $$->c.fill($1, $2, $3, $4, $5); }
	| SWITCH '(' expression ')' statement { alloc($$); $$->c.fill($1, $2, $3, $4, $5); }
	;

iteration_statement
	: WHILE '(' expression ')' statement { alloc($$); $$->c.get_next().set($1).set($2).set($3).get_next().get_next().get_next().get_next().set($4).set($5); }
	| DO statement WHILE '(' expression ')' ';' { alloc($$); $$->c.set($1).set($3).set($4).set($5).get_next().get_next().get_next().get_next().set($6).set($2); }
	| FOR '(' expression_statement expression_statement ')' statement { alloc($$); $$->c.get_next().set($1).set($2).get_next().set($3).get_next().set($4).get_next().set($5).set($6); }
	| FOR '(' expression_statement expression_statement expression ')' statement { alloc($$); $$->c.get_next().set($1).set($2).get_next().set($3).get_next().set($4).set($5).set($6).set($7); }
	| FOR '(' declaration expression_statement ')' statement { alloc($$); $$->c.get_next().set($1).set($2).get_next().get_next().set($3).set($4).get_next().set($5).set($6); }
	| FOR '(' declaration expression_statement expression ')' statement { alloc($$); $$->c.get_next().set($1).set($2).get_next().get_next().set($3).set($4).set($5).set($6).set($7); }
	;	

jump_statement
	: GOTO IDENTIFIER ';' { alloc($$); $$->c.fill($1, $2, NULL, $3); }
	| CONTINUE ';' { alloc($$); $$->c.fill($1, NULL, NULL, $2); }
	| BREAK ';' { alloc($$); $$->c.fill($1, NULL, NULL, $2); }
	| RETURN ';' { alloc($$); $$->c.fill($1, NULL, NULL, $2); }
	| RETURN expression ';' { alloc($$); $$->c.fill($1, NULL, $2, $3); }
	;

translation_unit
	: external_declaration { alloc(*expression); dynamic_cast<translation_unit_t*>(*expression)->c.push_back($1); }
	| translation_unit external_declaration { (*expression)->c.push_back($2); }
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
