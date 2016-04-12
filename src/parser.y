%{
/*************************************************************************/
/* bparser - a bison-based, C99 parser                                   */
/* Copyright (C) 2015-2015                                               */
/* Johannes Lorenz (jlsf2013 @ sourceforge)                              */
/*                                                                       */
/* This program is free software; you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation; either version 3 of the License, or (at */
/* your option) any later version.                                       */
/* This program is distributed in the hope that it will be useful, but   */
/* WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      */
/* General Public License for more details.                              */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program; if not, write to the Free Software           */
/* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA  */
/*************************************************************************/

#include <algorithm>
#include <vector>
#include <iostream>
#include "node_fwd.h"
#include "node.h"
#include "parser.h"
#include "lexer.h"

extern void dump_lookup_table();

int yyerror(translation_unit_t ** /*expression*/, yyscan_t scanner, const char *msg) {
	dump_lookup_table();
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
R* app_left(R* cur, S& stor, Left* left) { // TODO: all 3 funcs: stor redundant! use .c
	 return stor.push_front(left), cur; 
}

template<class R, class S, class Right>
R* app_right(R* cur, S& stor, Right* left) {
	 return stor.push_back(left), cur;
}

void c11() { throw "This C11 extension is not implemented yet."; }

extern std::vector<terminal_t*>& get_token_vector();

//terminal_t* app_token() { return get_token_vector().back(); }

template<class T> T* alloc(T*& ptr_ref) { return ptr_ref = new T(); /* be C++03 conform */ }

token_t* t(int token_id) { return new token_t(get_pos(), token_id); }

extern void flag_symbol(const char* str, int type, const node_base* scope);
extern void leave_scope(const node_base* scope);
extern int type_of(const char* str);

template<class P>
struct parent_assigner
{
	P* parent;
public:
	parent_assigner(P* parent) : parent(parent) {}
	template<class C>
	void operator()(C* child) { if(child) child->parent = parent; }
	template<class C>
	void operator()(std::list<C>* children)
	{
		if(children)
		for(typename std::list<C>::iterator itr = children->begin();
			itr != children->end(); ++itr)
			if(*itr)
			 operator()(*itr);
	}
};

template<
	class Base, class C
	>
void setc(
	Base* b,
	const C&,
	const typename C::type& e0, // FEATURE: rename: e0, e1,...
	typename C::template ar<1>::g e1 = typename C::template ar<1>::c(),
	typename C::template ar<2>::g e2 = typename C::template ar<2>::c(),
	typename C::template ar<3>::g e3 = typename C::template ar<3>::c(),
	typename C::template ar<4>::g e4 = typename C::template ar<4>::c(),
	typename C::template ar<5>::g e5 = typename C::template ar<5>::c(),
	typename C::template ar<6>::g e6 = typename C::template ar<6>::c(),
	typename C::template ar<7>::g e7 = typename C::template ar<7>::c(),
	typename C::template ar<8>::g e8 = typename C::template ar<8>::c(),
	typename C::template ar<9>::g e9 = typename C::template ar<9>::c(),
	typename C::template ar<10>::g e10 = typename C::template ar<10>::c(),
	typename C::template ar<11>::g e11 = typename C::template ar<11>::c()
	) {
		b->c.fill(e0, e1, e2, e3, e4, e5, e6, e7,
		e8, e9, e10, e11);
		parent_assigner<Base> pa(b);
		foreach(b->c, pa);
}

%}

%code requires {

#include "node_fwd.h"
using namespace nodes;
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
	attr_name_t* attr_name;
	attribute_t* attribute;
	identifier_t* name;
	typedef_name_t* typedef_name;
	enumeration_constant_t* enumeration_constant;
	float _float;
	int _int;
	token_t* token;
	node_t* node;

	type_specifier_t* type_specifier;
	declaration_specifiers_t* specifier_qualifier_list;
	expression_t *expression;
	declaration_specifiers_t* declaration_specifiers;
	declarator_t* declarator;
	declaration_list_t* declaration_list;
	compound_statement_t* compound_statement;
	external_declaration_t* external_declaration;
	function_definition_t* function_definition;
/*	storage_class_specifier_t* storage_class_specifier;
	alignment_specifier_t* alignment_specifier;
	function_specifier_t* function_specifier;*/
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
//	constant_t* constant;
	type_name_t* type_name;

	struct_declaration_t* struct_declaration;
	struct_declaration_list_t* struct_declaration_list;
	struct_declarator_list_t* struct_declarator_list;
	struct_declarator_t* struct_declarator;
	enumerator_list_t* enumerator_list;
	enumerator_t* enumerator;
	parameter_list_t* parameter_list;
	parameter_declaration_t* parameter_declaration;
	identifier_list_t* identifier_list;

	parameter_type_list_t* parameter_type_list;
	type_qualifier_list_t* type_qualifier_list;
	type_qualifier_t* type_qualifier;
	direct_abstract_declarator_t* direct_abstract_declarator;
	abstract_declarator_t* abstract_declarator;
	designator_list_t* designator_list;
	designator_t* designator;
	argument_expression_list_t* argument_expression_list;

	primary_expression_t* primary_expression;
	noconst_terminal_t* terminal;
	iconstant_t* iconstant;
	fconstant_t* fconstant;

	string_literal_t* string_literal;

	translation_unit_t* translation_unit;
}

%token  COMMENT
%token	ATTR_NAME ATTRIBUTE
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

%type <name> IDENTIFIER
%type <enumeration_constant> ENUMERATION_CONSTANT
%type <typedef_name> TYPEDEF_NAME

%type <string_literal> STRING_LITERAL
%type <iconstant> I_CONSTANT
%type <fconstant> F_CONSTANT

// FEATURE: wrong: translation_unit_t?
%type <translation_unit> translation_unit translation_unit_nonempty
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
%type <type_name> type_name
%type <declaration> declaration
%type <statement> statement
%type <attr_name> ATTR_NAME
%type <attribute> attribute
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
	ATTRIBUTE

%start translation_unit

//%printer { fprintf (yyoutput, "constant %d", $$); } I_CONSTANT
// %printer { fprintf (yyoutput, "type %d", $$.expression->type); } <>
%printer { fprintf (yyoutput, "<...?>"); } <>

%%

primary_expression // parents: postfix_expression
	: IDENTIFIER { if(type_of($1->raw.c_str())==lt_undefined) throw "identifier undefined"; else
		{ alloc($$); setc($$, $$->c, NULL, $1); } }
	| constant { alloc($$); $$->c.set($1); }
	| string { alloc($$); $$->c.set($1); }
	| '(' expression ')' { alloc($$); setc($$, $$->c, NULL, NULL, $1, $2, $3); }
	| generic_selection { c11(); }
	;

constant // parents: primary expression
	: I_CONSTANT { $$=$1; }		/* includes character_constant */
	| F_CONSTANT { $$=$1; }
	| ENUMERATION_CONSTANT{ $$=$1; }	/* after it has been defined as such */
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
	| postfix_expression '[' expression ']' { array_access_expression_t* u; $$=alloc(u); setc(u, u->c, $1, $2, $3, $4); }
	| postfix_expression '(' ')' { function_call_expression_t* u; $$=alloc(u); setc(u, u->c, $1, $2, NULL, $3); }
	| postfix_expression '(' argument_expression_list ')' { function_call_expression_t* u; $$=alloc(u); setc(u, u->c, $1, $2, $3, $4); }
	| postfix_expression '.' IDENTIFIER { struct_access_expression_t* e; $$=alloc(e); setc(e, e->c, $1, $2, $3); } // FEATURE: bool if with pointer?
	| postfix_expression PTR_OP IDENTIFIER { struct_access_expression_t* e; $$=alloc(e); setc(e, e->c, $1, $2, $3); }
	| postfix_expression INC_OP { unary_expression_r* u; $$=alloc(u); setc(u, u->c, $1, $2); }
	| postfix_expression DEC_OP { unary_expression_r* u; $$=alloc(u); setc(u, u->c, $1, $2); }
	| '(' type_name ')' '{' initializer_list '}' { compound_literal_t* e; $$=alloc(e); setc(e, e->c, $1, $2, $3, $4, $5, NULL, $6); }
	| '(' type_name ')' '{' initializer_list ',' '}' { compound_literal_t* e; $$=alloc(e); setc(e, e->c, $1, $2, $3, $4, $5, $6, $7); }
	;

// FEATURE: alternate list<token_t, assignment_exp>
argument_expression_list
	: assignment_expression { alloc($$); setc($$, $$->c, NULL, NULL, $1); }
	| argument_expression_list ',' assignment_expression { alloc($$); setc($$, $$->c, $1, $2, $3); }
	;

unary_expression
	: postfix_expression { $$=$1; }
	| INC_OP unary_expression { unary_expression_l* u; $$=alloc(u); setc(u, u->c, $1, $2); }
	| DEC_OP unary_expression { unary_expression_l* u; $$=alloc(u); setc(u, u->c, $1, $2); }
	| unary_operator cast_expression { unary_expression_l* u; $$=alloc(u); setc(u, u->c, $1, $2); /*u->op_id = $1;*/ }
	| SIZEOF unary_expression { sizeof_expression_t* e; alloc(e); setc(e, e->c, $1, NULL, NULL, $2, NULL); $$=e; } // example: sizeof 255+1
	| SIZEOF '(' type_name ')' { sizeof_expression_t* e; alloc(e); setc(e, e->c, $1,$2,$3,NULL, $4); $$=e; }
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
	| '(' type_name ')' cast_expression { cast_expression_t* c; $$=alloc(c); setc(c, c->c, $1, $2, $3, $4); }
	;

multiplicative_expression
	: cast_expression { $$=$1; }
	| multiplicative_expression '*' cast_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_mult; setc(e, e->c, $1, $2, $3); }
	| multiplicative_expression '/' cast_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_div; setc(e, e->c, $1, $2, $3); }
	| multiplicative_expression '%' cast_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_mod; setc(e, e->c, $1, $2, $3); }
	;

additive_expression
	: multiplicative_expression { $$=$1; }
	| additive_expression '+' multiplicative_expression { binary_expression_t* e; $$=alloc(e); e; e->op_id = op_plus; setc(e, e->c, $1, $2, $3); }
	| additive_expression '-' multiplicative_expression { binary_expression_t* e; $$=alloc(e); e; e->op_id = op_minus; setc(e, e->c, $1, $2, $3); }
	;

shift_expression
	: additive_expression { $$=$1; }
	| shift_expression LEFT_OP additive_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_lshift; setc(e, e->c, $1, $2, $3); }
	| shift_expression RIGHT_OP additive_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_rshift; setc(e, e->c, $1, $2, $3); }
	;

relational_expression
	: shift_expression { $$=$1; }
	| relational_expression '<' shift_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_lt; setc(e, e->c, $1, $2, $3); }
	| relational_expression '>' shift_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_gt; setc(e, e->c, $1, $2, $3); }
	| relational_expression LE_OP shift_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_le; setc(e, e->c, $1, $2, $3); }
	| relational_expression GE_OP shift_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_ge; setc(e, e->c, $1, $2, $3); }
	;

equality_expression
	: relational_expression { $$=$1; }
	| equality_expression EQ_OP relational_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_eq; setc(e, e->c, $1, $2, $3); }
	| equality_expression NE_OP relational_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_ne; setc(e, e->c, $1, $2, $3); }
	;

and_expression
	: equality_expression { $$=$1; }
	| and_expression '&' equality_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_band; setc(e, e->c, $1, $2, $3); }
	;

exclusive_or_expression
	: and_expression { $$=$1; }
	| exclusive_or_expression '^' and_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_xor; setc(e, e->c, $1, $2, $3); }
	;

inclusive_or_expression
	: exclusive_or_expression { $$=$1; }
	| inclusive_or_expression '|' exclusive_or_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_bor; setc(e, e->c, $1, $2, $3); }
	;

logical_and_expression
	: inclusive_or_expression { $$=$1; }
	| logical_and_expression AND_OP inclusive_or_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_and; setc(e, e->c, $1, $2, $3); }
	;

logical_or_expression
	: logical_and_expression { $$=$1; }
	| logical_or_expression OR_OP logical_and_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_or; setc(e, e->c, $1, $2, $3); }
	;

conditional_expression
	: logical_or_expression { $$=$1; }
	| logical_or_expression '?' expression ':' conditional_expression { ternary_expression_t* e; $$=alloc(e); setc(e, e->c, $1, $2, $3, $4, $5); }
	;

assignment_expression
	: conditional_expression { $$=$1; }
	| unary_expression assignment_operator assignment_expression { binary_expression_t* e; $$=alloc(e); /*e->op_id = op_asn*/; setc(e, e->c, $1, $2, $3); }
	;

assignment_operator
	: '=' { $$=$1; /* op_asn */ } // FEATURE: remove those comments?
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
	| expression ',' assignment_expression { binary_expression_t* e; $$=alloc(e); e->op_id = op_asn; setc(e, e->c, $1, $2, $3); }
	;

constant_expression
	: conditional_expression { $$=$1; }	/* with constraints */
	;

declaration
	: declaration_specifiers ';' { $$ = new declaration_t(); setc($$, $$->c, $1, NULL, $2); } // e.g. forward declaration
	| declaration_specifiers init_declarator_list ';' { $$ = new declaration_t(); setc($$, $$->c, $1, $2, $3); }
	| static_assert_declaration { c11(); }
	;

declaration_specifiers
	: storage_class_specifier declaration_specifiers { $$ = app_left($2, $2->c, $1); }
	| storage_class_specifier { alloc($$); app_left($$, $$->c, $1); }
	| type_specifier declaration_specifiers { $$ = app_left($2, $2->c, $1); }
	| type_specifier { alloc($$); app_left($$, $$->c, $1); }
	| type_qualifier declaration_specifiers { $$ = app_left($2, $2->c, $1); }
	| type_qualifier { alloc($$); app_left($$, $$->c, $1); }
	| function_specifier declaration_specifiers { $$ = app_left($2, $2->c, $1); }
	| function_specifier { alloc($$); app_left($$, $$->c, $1); }
	| alignment_specifier declaration_specifiers { $$ = app_left($2, $2->c, $1); }
	| alignment_specifier { alloc($$); app_left($$, $$->c, $1); }
	;

init_declarator_list
	: init_declarator { alloc($$); setc($$, $$->c, NULL, NULL, $1); }
	| init_declarator_list ',' init_declarator { alloc($$); setc($$, $$->c, $1, $2, $3); }
	;

init_declarator
	: declarator '=' initializer { alloc($$); setc($$, $$->c, $1, $2, $3); }
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

attribute
	: ATTRIBUTE '(' '(' ATTR_NAME ')' ')' { $$ = alloc($$); $$->c.set($1); }
	;

struct_or_union_specifier
	: struct_or_union '{' struct_declaration_list '}' { alloc($$); setc($$, $$->c, $1, NULL, $2, $3, $4); }
	| struct_or_union IDENTIFIER '{' struct_declaration_list '}' { alloc($$); setc($$, $$->c, $1, $2, $3, $4, $5); }
	| struct_or_union '{' struct_declaration_list '}' attribute { alloc($$); setc($$, $$->c, $1, NULL, $2, $3, $4, $5); }
	| struct_or_union IDENTIFIER '{' struct_declaration_list '}' attribute { alloc($$); setc($$, $$->c, $1, $2, $3, $4, $5, $6); }
	| struct_or_union IDENTIFIER { alloc($$); setc($$, $$->c, $1, $2); }
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
	: specifier_qualifier_list ';' { alloc($$); setc($$, $$->c, $1, NULL, $2); } /* for anonymous struct/union */
	| specifier_qualifier_list struct_declarator_list ';' { alloc($$); setc($$, $$->c, $1, $2, $3); }
	| static_assert_declaration { c11(); }
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list { $$ = app_left($2, $2->c, $1); }
	| type_specifier { alloc($$); app_left($$, $$->c, $1); }
	| type_qualifier specifier_qualifier_list { $$ = app_left($2, $2->c, $1); }
	| type_qualifier { alloc($$); app_left($$, $$->c, $1); }
	;

struct_declarator_list
	: struct_declarator { alloc($$); setc($$, $$->c, NULL, NULL, $1); }
	| struct_declarator_list ',' struct_declarator { alloc($$); setc($$, $$->c, $1, $2, $3); }
	;

struct_declarator
	: ':' constant_expression { alloc($$); setc($$, $$->c, NULL, $1, $2); setc($$, $$->c, NULL, $1, $2); }
	| declarator ':' constant_expression { alloc($$); setc($$, $$->c, $1, $2, $3); }
	| declarator { alloc($$); $$->c.set($1); }
	;

enum_specifier
	: ENUM '{' enumerator_list '}' { alloc($$); setc($$, $$->c, $1, NULL, $2, $3, NULL, $4); }
	| ENUM '{' enumerator_list ',' '}' { alloc($$); setc($$, $$->c, $1, NULL, $2, $3, $4, $5); }
	| ENUM IDENTIFIER '{' enumerator_list '}' { alloc($$); setc($$, $$->c, $1, $2, $3, $4, NULL, $5); }
	| ENUM IDENTIFIER '{' enumerator_list ',' '}' { alloc($$); setc($$, $$->c, $1, $2, $3, $4, $5, $6); }
	| ENUM IDENTIFIER { alloc($$); setc($$, $$->c, $1, $2); }
	;

enumerator_list
	: enumerator { alloc($$); setc($$, $$->c, NULL, NULL, $1); }
	| enumerator_list ',' enumerator { alloc($$); setc($$, $$->c, $1, $2, $3); }
	;

enumerator	/* identifiers must be flagged as ENUMERATION_CONSTANT */
	: enumeration_constant '=' constant_expression { alloc($$); setc($$, $$->c, $1, $2, $3); }
	| enumeration_constant { alloc($$); $$->c.set($1); }
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
	: pointer direct_declarator { alloc($$); setc($$, $$->c, $1, $2); }
	| direct_declarator { alloc($$); $$->c.get_next().set($1); }
	;

// NOTE: static etc in [] can only appear in function parameter declarations

direct_declarator
	: IDENTIFIER { direct_declarator_id* d; $$ = alloc(d); d->c = $1; }
	| '(' declarator ')' { direct_declarator_decl* d; $$ = alloc(d); setc(d, d->c, $1, $2, $3); }
	| direct_declarator '[' ']' { direct_declarator_arr* d; $$ = alloc(d);
		setc(d, d->c, $1, $2, NULL, NULL, NULL, NULL, $3); }
	| direct_declarator '[' '*' ']' { direct_declarator_arr* d; $$ = alloc(d);
		setc(d, d->c, $1, $2, NULL, NULL, NULL, $3, $4); }
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		setc(d, d->c, $1, $2, $3, $4, $5, NULL, $6); }
	| direct_declarator '[' STATIC assignment_expression ']' { c11(); }
	| direct_declarator '[' type_qualifier_list '*' ']' { direct_declarator_arr* d; $$ = alloc(d);
		// FEATURE: autoptr that converts to pointer in assignment to $$?
		setc(d, d->c, $1, $2, NULL, $3, NULL, $4, $5); }
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		setc(d, d->c, $1, $2, $4, $3, $5, NULL, $6); }
	| direct_declarator '[' type_qualifier_list assignment_expression ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		setc(d, d->c, $1, $2, NULL, $3, $4, NULL, $5); }
	| direct_declarator '[' type_qualifier_list ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		setc(d, d->c, $1, $2, NULL, $3, NULL, NULL, $4); }
	| direct_declarator '[' assignment_expression ']' {
		direct_declarator_arr* d; $$ = alloc(d);
		setc(d, d->c, $1, $2, NULL, NULL, $3, NULL, $4); }
	| direct_declarator '(' parameter_type_list ')' {
		direct_declarator_func* d; $$ = alloc(d);
		setc(d, d->c, $1, $2, $3, $4); }
	| direct_declarator '(' ')' {
		direct_declarator_func* d; $$ = alloc(d);
		setc(d, d->c, $1, $2, NULL, $3); }
	| direct_declarator '(' identifier_list ')' {
		direct_declarator_idlist* d; $$ = alloc(d);
		setc(d, d->c, $1, $2, $3, $4); } // old-style C function declarator
	;

pointer
	: '*' type_qualifier_list pointer { alloc($$); setc($$, $$->c, $1, $2, $3); }
	| '*' type_qualifier_list { alloc($$); setc($$, $$->c, $1, $2, NULL); }
	| '*' pointer { alloc($$); setc($$, $$->c, $1, NULL, $2); }
	| '*' { alloc($$); $$->c.set($1); }
	;

type_qualifier_list
	: type_qualifier { alloc($$); app_first($$->c, $1); }
	| type_qualifier_list type_qualifier { app_right($1, $1->c, $2); }
	;

parameter_type_list
	: parameter_list ',' ELLIPSIS { alloc($$); setc($$, $$->c, $1, $2, $3); }
	| parameter_list { alloc($$); $$->c.set($1); }
	;

parameter_list
	: parameter_declaration { alloc($$); setc($$, $$->c, NULL, NULL, $1); }
	| parameter_list ',' parameter_declaration { alloc($$); setc($$, $$->c, $1, $2, $3); }
	;

parameter_declaration
	: declaration_specifiers declarator { alloc($$); setc($$, $$->c, $1, $2); }
	| declaration_specifiers abstract_declarator { alloc($$); setc($$, $$->c, $1, NULL, $2); }
	| declaration_specifiers { alloc($$); $$->c.set($1); }
	;

identifier_list
	: IDENTIFIER { alloc($$); setc($$, $$->c, NULL, NULL, $1); }
	| identifier_list ',' IDENTIFIER { alloc($$); setc($$, $$->c, $1, $2, $3); }
	;

type_name
	: specifier_qualifier_list abstract_declarator { alloc($$); setc($$, $$->c, $1, $2); }
	| specifier_qualifier_list { alloc($$); $$->c.set($1); }
	;

abstract_declarator
	: pointer direct_abstract_declarator { alloc($$); setc($$, $$->c, $1, $2); }
	| pointer { alloc($$); $$->c.set($1); }
	| direct_abstract_declarator { alloc($$); setc($$, $$->c, NULL, $1); }
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' { direct_abstract_declarator_decl* d; $$ = alloc(d); setc(d, d->c, $1, $2, $3); }
	| '[' ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); setc(d, d->c,  NULL, $1, NULL, NULL, $2); }
	| '[' '*' ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); setc(d, d->c,  NULL, $1, NULL, $2, $3); }
	| '[' STATIC type_qualifier_list assignment_expression ']' { c11(); }
	| '[' STATIC assignment_expression ']' { c11(); }
	| '[' type_qualifier_list STATIC assignment_expression ']' { c11(); }
	| '[' type_qualifier_list assignment_expression ']' { c11(); }
	| '[' type_qualifier_list ']' { c11(); }
	| '[' assignment_expression ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); setc(d, d->c,  NULL, $1, $2, NULL, $3); }
	| direct_abstract_declarator '[' ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); setc(d, d->c,  $1, $2, NULL, NULL, $3); }
	| direct_abstract_declarator '[' '*' ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); setc(d, d->c,  $1, $2, NULL, $3, $4); }
	| direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']' { c11(); }
	| direct_abstract_declarator '[' STATIC assignment_expression ']' { c11(); }
	| direct_abstract_declarator '[' type_qualifier_list assignment_expression ']' { c11(); }
	| direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']' { c11(); }
	| direct_abstract_declarator '[' type_qualifier_list ']' { c11(); }
	| direct_abstract_declarator '[' assignment_expression ']' { direct_abstract_declarator_arr* d; $$ = alloc(d); setc(d, d->c,  $1, $2, $3, NULL, $4); }
	| '(' ')' {
		direct_abstract_declarator_func* d; $$ = alloc(d);
		setc(d, d->c, NULL, $1, NULL, $2); }
	| '(' parameter_type_list ')' {
		direct_abstract_declarator_func* d; $$ = alloc(d);
		setc(d, d->c, NULL, $1, $2, $3); }
	| direct_abstract_declarator '(' ')' {
		direct_abstract_declarator_func* d; $$ = alloc(d);
		setc(d, d->c, $1, $2, NULL, $3); }
	| direct_abstract_declarator '(' parameter_type_list ')' {
		direct_abstract_declarator_func* d; $$ = alloc(d);
		setc(d, d->c, $1, $2, $3, $4); }
	;

initializer
	: '{' '}' { alloc($$); setc($$, $$->c, NULL, $1, NULL, NULL, $2); }
	| '{' initializer_list '}' { alloc($$); setc($$, $$->c, NULL, $1, $2, NULL, $3); }
	| '{' initializer_list ',' '}'{ alloc($$); setc($$, $$->c, NULL, $1, $2, $3, $4); }
	| assignment_expression { alloc($$); $$->c.set($1); }
	;

initializer_list
	: designator_list '=' initializer { alloc($$); setc($$, $$->c, NULL, NULL, $1, $2, $3); }
	| initializer { alloc($$); setc($$, $$->c, NULL, NULL, NULL, NULL, $1); }
	| initializer_list ',' designator_list '=' initializer { alloc($$); setc($$, $$->c, $1, $2, $3, $4, $5); }
	| initializer_list ',' initializer { alloc($$); setc($$, $$->c, $1, $2, NULL, NULL, $3); }
	;

// FEATURE: -> removed this, put eq sign into initializer_list? is this ok?
//designation
//	: designator_list '='
//	;

designator_list // e.g. 2D arrays: int a[1][1] = { [0][0] = 0 };
	: designator { alloc($$); app_right($$, $$->c, $1); }
	| designator_list designator { $$ = app_right($1, $1->c, $2); }
	;

designator
	: '[' constant_expression ']' { designator_constant_expr* d; $$ = alloc(d); setc(d, d->c, $1, $2, $3); }
	| '.' IDENTIFIER { designator_id* d; $$ = alloc(d); setc(d, d->c, $1, $2); }
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
	: IDENTIFIER ':' statement { alloc($$); setc($$, $$->c, NULL, $1, NULL, $2, $3); }
	| CASE constant_expression ':' statement { alloc($$); setc($$, $$->c, $1, NULL, $2, $3, $4); }
	| DEFAULT ':' statement { alloc($$); setc($$, $$->c, $1, NULL, NULL, $2, $3); }
	;

compound_statement
	: '{' '}' { alloc($$); setc($$, $$->c, $1, NULL, $2); }
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
	: ';' { alloc($$); setc($$, $$->c, NULL, $1); }
	| expression ';' { alloc($$); setc($$, $$->c, $1, $2); }
	;

selection_statement
	: IF '(' expression ')' statement ELSE statement { alloc($$); setc($$, $$->c, $1, $2, $3, $4, $5, $6, $7); }
	| IF '(' expression ')' statement { alloc($$); setc($$, $$->c, $1, $2, $3, $4, $5); }
	| SWITCH '(' expression ')' statement { alloc($$); setc($$, $$->c, $1, $2, $3, $4, $5); }
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
	: GOTO IDENTIFIER ';' { alloc($$); setc($$, $$->c, $1, $2, NULL, $3); }
	| CONTINUE ';' { alloc($$); setc($$, $$->c, $1, NULL, NULL, $2); }
	| BREAK ';' { alloc($$); setc($$, $$->c, $1, NULL, NULL, $2); }
	| RETURN ';' { alloc($$); setc($$, $$->c, $1, NULL, NULL, $2); }
	| RETURN expression ';' { alloc($$); setc($$, $$->c, $1, NULL, $2, $3); }
	;

translation_unit_nonempty // TODO: dynamic cast useless?
	: external_declaration { alloc($$); $$->c.push_back($1); }
	| translation_unit_nonempty external_declaration { $$->c.push_back($2); }
	;

translation_unit
	: { alloc(*expression); }
	| translation_unit_nonempty { *expression = $1; }
	;

external_declaration
	: function_definition { $$ = new external_declaration_t($1); }
	| declaration { $$ = new external_declaration_t($1); }
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement { $$ = new function_definition_t($1, $2, $3, $4); }
	| declaration_specifiers declarator compound_statement { $$ = new function_definition_t($1, $2, NULL, $3); }
	;

declaration_list
	: declaration { alloc($$); app_first($$->c, $1); }
	| declaration_list declaration { app_right($1, $1->c, $2); }
	;

%%
