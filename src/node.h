/**
 * @file node.h
 * definition of a node of the resulting AST
 */
#ifndef NODE_H
#define NODE_H

#include <cstddef>
#include <list>
#include <string>
#include "tuple03.h"
#include "geom.h"

// TODO: include and *use* node_fwd.h

// TODO: not here?
enum lookup_type
{
	lt_enumeration,
	lt_typedef_name,
	lt_identifier,
	lt_undefined
};

void init_parser(); //!< see node.cpp

class node_base
{
public:
	typedef std::size_t id_t;

	span_t span; // TODO: public
	virtual void accept(class visitor_t& v) = 0;

	node_base() {}
	node_base(const geom_t& geom) : span(geom, geom_t()) {}

	virtual ~node_base() {}
};

template<class ParType = node_base>
class node_t : public node_base
{
public:
	null_type c; //!< base case, overwrite this

	ParType* parent;
	node_t() {}
	node_t(const geom_t& geom) : node_base(geom) {}

	virtual ~node_t() {}
};

template<class First, class Next = null_type>
class node_ch : public node_t<>
{
	ptn<First, Next> c;
public:
	node_ch() {}
	node_ch(const geom_t& geom) : node_t<>(geom) {}

	virtual ~node_ch() {}
	void accept_children(visitor_t& v); // TODO
};

struct terminal_t : public node_t<> { // TODO: class
	int value;
	virtual std::size_t length() const = 0;
	virtual std::size_t newlines() const = 0;
public:
	terminal_t(const geom_t& geom, int value) : node_t<>(geom), value(value) {}
};

struct token_t : public terminal_t
{
public:
	token_t(const geom_t& pos, int value) : terminal_t(pos, value) {}
	virtual void accept(class visitor_t& v);
	std::size_t length() const;
	std::size_t newlines() const;
};

struct noconst_terminal_t : public terminal_t
{
	std::string raw;
	noconst_terminal_t(const geom_t& geom, int value, const char* raw) :
		terminal_t(geom, value),
		raw(raw) {}
};

struct noconst_1line_terminal_t : public noconst_terminal_t
{
	std::size_t length() const;
	std::size_t newlines() const;
public:
	noconst_1line_terminal_t(const geom_t& geom, int value,
		const char* raw) :
		noconst_terminal_t(geom, value, raw) {}
};

struct identifier_t : public noconst_1line_terminal_t
{
	virtual void accept(class visitor_t& v);
	identifier_t(const char* name, geom_t geom);
};

struct typedef_name_t : public noconst_1line_terminal_t
{
	virtual void accept(class visitor_t& v);
	typedef_name_t(const char* name, geom_t geom);
};

struct enumeration_constant_t : public noconst_1line_terminal_t
{
	virtual void accept(class visitor_t& v);
	enumeration_constant_t(const char* name, geom_t geom);
};

struct string_literal_t : public noconst_terminal_t
{
	std::size_t _length, _newlines;
	std::size_t length() const;
	std::size_t newlines() const;
public:
	virtual void accept(class visitor_t& v);
	string_literal_t(const char* value, geom_t geom);
};

//typedef ch<token_t> tok;
/*
struct number_t : public node_t
{
	int value;
public:
	number_t(int value) : value(value) {}
	virtual void accept(class visitor_t& v);
};*/

enum op_type
{
	op_multiply,
	op_add
};

/*struct expression_t : public node_t
{
	node_t *first;
	node_t *second;
	node_t *third;
	op_type type;

	void accept_children(class visitor_t& v);
public:
	expression_t(op_type type, node_t *n1, node_t* n2 = NULL, node_t* n3 = NULL) :
		first(n1), second(n2), third(n3) {}
	virtual void accept(class visitor_t& v);
};*/

struct declaration_specifier_type : public node_t<> {
	declaration_specifier_type() {}
	declaration_specifier_type(const geom_t& geom) : node_t<>(geom) {}
};

#if 0
struct storage_class_specifier_t : public declaration_specifier_type {
	virtual void accept(class visitor_t& v);
};
#endif

#if 0
enum type_specifier_id
{
	t_void,
	t_char,
	t_short,
	t_long,
	t_int,
	t_float,
	t_double,
	t_signed,
	t_unsigned,
	t_bool,
	t_complex
};

enum struct_or_union_id
{
	t_struct,
	t_union
};

enum type_qualifier_id
{
	t_const,
	t_restrict,
	t_volatile,
	t_atomic
};
#endif

enum unary_op_t
{
	op_addr,
	op_ptr,
	op_pos,
	op_neg,
	op_compl,
	op_not,
	op_inc_post,
	op_dec_post,
	op_inc_pre,
	op_dec_pre,
};

enum binary_op_t
{
	op_asn,
	op_asn_mul,
	op_asn_div,
	op_asn_mod,
	op_asn_add,
	op_asn_sub,
	op_asn_left,
	op_asn_right,
	op_asn_and,
	op_asn_xor,
	op_asn_or,

	op_or,
	op_and,
	op_bor,
	op_band,
	op_xor,
	op_lshift,
	op_rshift,

	op_lt,
	op_gt,
	op_le,
	op_ge,
	op_eq,
	op_ne,

	op_plus,
	op_minus,
	op_mult,
	op_div,
	op_mod,

	op_com
};

typedef ptn<token_t> end_token;

struct expression_t : public node_t<> {
//	op_t op_id; // TODO: reliable? redundant?
//	virtual void accept(class visitor_t& v);
};

struct unary_expression_r : public expression_t
{
	unary_op_t op_id;
	ptn<	node_base, // FEATURE: more exactly?
			end_token > c;

	virtual void accept(class visitor_t& v);
};

struct unary_expression_l : public expression_t
{
	unary_op_t op_id;
	ptn<	token_t,
		ptn<	node_base> > c;

	virtual void accept(class visitor_t& v);
};

struct binary_expression_t : public expression_t
{
	binary_op_t op_id;
	ptn<	node_base,
		ptn<	token_t,
			ptn<	node_base> > > c;
	virtual void accept(class visitor_t& v);
};

struct ternary_expression_t : public expression_t
{
	void accept_children(visitor_t& v);
	virtual void accept(class visitor_t& v);

	ptn<	node_base,
		ptn<	token_t,
			ptn<	node_base,
				ptn<	token_t,
					ptn<	node_base > > > > > c;

//	expression_t(op_t op, token_t* op_token, token_t* op_token_2, node_t *n1, node_t* n2 = NULL, node_t* n3 = NULL) :
//		op(op), op_token(op_token), op_token_2(op_token_2), n1(n1), n2(n2), n3(n3) {}
};

struct abstract_declarator_t : public node_t<>
{
	ptn<	struct pointer_t,
		ptn<	struct direct_abstract_declarator_t > > c;
	void accept(class visitor_t& v);
};

struct type_specifier_t : public declaration_specifier_type
{
	ptn<	node_base> c; // FEATURE: more granular?
	virtual void accept(class visitor_t& v);
};

struct type_specifier_complex_t : public type_specifier_t {
	//virtual void accept(class visitor_t& v);
};

struct struct_or_union_specifier_t : public type_specifier_complex_t
{
	bool is_union_type;

	enum access_t {
		keyword,
			identifier,
				lbrack,
					declaration_list,
						rbrack
	};

	ptn<	token_t,
		ptn<	identifier_t,
			ptn<	token_t,
				ptn<	struct struct_declaration_list_t,
					end_token
		> > > > c;

	virtual void accept(class visitor_t& v);
};

struct struct_declaration_list_t : public node_t<> // FEATURE: when alt list
{
	std::list<struct struct_declaration_t*> c;
	void accept(class visitor_t& v);
};

struct struct_declaration_t : public node_t<struct_declaration_list_t>
{
	ptn<	struct specifier_qualifier_list_t,
		ptn<	struct struct_declarator_list_t,
				end_token > > c;
	void accept(class visitor_t& v);
};

struct specifier_qualifier_list_t : public node_t<>
{
	std::list<declaration_specifier_type*> c;
	void accept(class visitor_t& v);
};

struct struct_declarator_list_t : public node_t<> // FEATURE: when alt list
{
	ptn<	struct struct_declarator_list_t,
		ptn<	token_t, // :
			ptn<	struct struct_declarator_t
			> > > c;
	void accept(class visitor_t& v);
};

struct struct_declarator_t : public node_t<struct_declarator_list_t>
{
	ptn<	struct declarator_t,
		ptn<	token_t, // :
			ptn<	expression_t
		> > > c;
	void accept(class visitor_t& v);
};

struct enum_specifier_t : public type_specifier_complex_t
{
	enum access_t {
		enum_keyword,
			identifier,
				lbrack,
					enum_list,
						comma,
							rbrack
	};

	ptn<	token_t,
		ptn<	identifier_t,
			ptn<	token_t,
				ptn<	struct enumerator_list_t,
					ptn<	token_t,
							end_token
		> > > > > c;
	void accept(class visitor_t& v);
};

struct enumerator_list_t : public node_t<> // FEATURE: when alt list
{
	ptn<	enumerator_list_t,
		ptn<	token_t,
			ptn<	struct enumerator_t > > > c;
	void accept(class visitor_t& v);
};

struct enumerator_t : public node_t<enumerator_list_t>
{
	ptn<	identifier_t, // TODO
		ptn<	token_t, // =, optional
			ptn<	expression_t // optional
		> > > c;
	void accept(class visitor_t& v);
};

struct parameter_type_list_t : public node_t<> // FEATURE: declar. or abstr. declar.
{
	ptn<	struct parameter_list_t,
		ptn<	token_t,
				end_token > > c;
	void accept(class visitor_t& v);
};

struct parameter_list_t : public node_t<> // TODO: when alt list
{
	ptn<	parameter_list_t,
		ptn<	token_t,
			ptn<	struct parameter_declaration_t
		> > > c;
	void accept(class visitor_t& v);
};

struct parameter_declaration_t : public node_t<parameter_list_t>
{
	ptn<	struct declaration_specifiers_t,
		ptn<	declarator_t, // TODO: declarator base, and remove below
			ptn<	abstract_declarator_t
		> > > c;
	void accept(class visitor_t& v);
};

struct identifier_list_t : public node_t<> // TODO: when alt list
{
	ptn<	identifier_list_t,
		ptn<	token_t,
			ptn<	identifier_t
		> > > c;
	void accept(class visitor_t& v);
};

struct type_name_t : public node_t<>
{
	ptn<	specifier_qualifier_list_t,
		ptn<	abstract_declarator_t > > c;
	void accept(class visitor_t& v);
};

#if 0
template<class Next>
struct to
{
	typedef ptn<token_t, Next> type;
};
#endif

struct sizeof_expression_t : public expression_t
{
	void accept(class visitor_t& v);
	/*tok sizeof_token, lbrace, rbrace;
	ch<type_name_t> type_name;*/
	//to< ptn< type_name_t, end_token > >::type c;
	ptn<	token_t,
		ptn <	token_t,
			ptn <	type_name_t,
				ptn<	expression_t,
						end_token > > > > c;
};

enum constant_type
{
	ct_int,
	ct_float,
	ct_enum
};

#if 0
struct constant_t : public node_t<struct primary_expression>
{
	virtual void accept(class visitor_t& v);
	constant_type type;
	union {
		int i;
		float f;
	} value;
	ptn<identifier_t> c; //!< enumeration id
};
#endif

enum primary_type
{
	pt_expression,
	pt_constant,
	pt_id,
	pt_string
};

// strings, ints, floats
// TODO: keep strings of original values? e.g. ...f, ...lf ?
template<class T>
struct constant_t : public terminal_t
{
	std::string value;
//	ptn<token_t> c;
//	constant_t() {}
	constant_t(const char* value, geom_t geom) : terminal_t(geom, 0/*TODO*/), value(value) {}

	virtual void accept(class visitor_t& v);
};

struct iconstant_t : public noconst_1line_terminal_t
{
	int suf_type; // TODO
	iconstant_t(const char* value, geom_t geom) : // TODO: value
		noconst_1line_terminal_t(geom, 0, value) {}

	virtual void accept(class visitor_t& v);
};

struct fconstant_t : public noconst_1line_terminal_t
{
	fconstant_t(const char* value, geom_t geom) : // TODO: value
		noconst_1line_terminal_t(geom, 0, value) {}

	virtual void accept(class visitor_t& v);
};

struct primary_expression_t : public expression_t {
	ptn<	terminal_t,	// TODO: constant_base
		//ptn<	constant_t<float>,
			ptn<	struct identifier_t,
				ptn<	token_t,
					ptn<	expression_t,
							end_token
		> > > > c;
	
	virtual void accept(class visitor_t& v);
};
// TODO: currently no difference between enum_constant and identifier
#if 0
struct primary_identifier_t : public node_t
{
	ptn<identifier_t> c;
	virtual void accept(class visitor_t& v);
};

struct primary_expression_expression_t : public node_t // TODO: put this all into primary_expression_t?
{
	ptn<	token_t,	// left brace
		ptn<	expression_t,
				end_token
			> > c;
	virtual void accept(class visitor_t& v);
};
#endif
#if 0
struct primary_expression_t : public expression_t
{
	virtual void accept(class visitor_t& v);
	primary_type type;
	
/*	tok lbrace, rbrace;
	ch<constant_t> constant;
	identifier_t* identifier;
	identifier_t* string;
	expression_t* expression;*/
	ptn<	identifier_t,	// identifier
		ptn<	identifier_t,	// string literal
			ptn<	constant_t,
				ptn<	token_t,	// left brace
					ptn<	expression_t,
							end_token
			> > > > > c;
};
#endif

struct array_access_expression_t : public expression_t
{
	ptn<	expression_t,
		ptn<	token_t,
			ptn<	expression_t,
					end_token > > > c;
	virtual void accept(class visitor_t& v);
};

struct argument_expression_list_t : public node_t<>
{
	ptn<	argument_expression_list_t,
		ptn<	token_t,
			ptn<	expression_t> > > c;

	virtual void accept(class visitor_t& v);
};

struct function_call_expression_t : public expression_t
{
	ptn<	expression_t,
		ptn<	token_t,
			ptn<	argument_expression_list_t,
					end_token > > > c;
	virtual void accept(class visitor_t& v);
};
// TODO: base for postfix expressions?
// TODO: no op_id here, does this make sense?
struct struct_access_expression_t : public expression_t
{
	ptn<	expression_t,
		ptn<	token_t,	// . or ->
			ptn<	identifier_t > > > c;
	virtual void accept(class visitor_t& v);
};

// TODO: what is this??
struct cast_postfix_expression_t : public expression_t
{
	ptn<	token_t,
		ptn<	type_name_t,
			ptn<	token_t,
				ptn<	token_t,
					ptn<	struct initializer_list_t,
						ptn<	token_t,
								end_token
		> > > > > > c;
	virtual void accept(class visitor_t& v);
};

struct cast_expression_t : public expression_t
{
	ptn<	token_t,
		ptn<	type_name_t,
			ptn<	token_t,
				ptn<	expression_t
					> > > > c;
	virtual void accept(class visitor_t& v);
};

/*struct type_specifier_token : public type_specifier_t
{
	ptn<token_t> c;
	virtual void accept(class visitor_t& v);
};

struct type_identifier : public type_specifier_t
{
	ptn<identifier_t> c;
	virtual void accept(class visitor_t& v);
};*/

#if 0
template<class IdType, class Base = node_t>
struct token_base : public Base
{
	IdType id;
	virtual void accept(class visitor_t& v);
	token_base(geom_t pos, const IdType& id) : Base(pos), id(id) {}
};

typedef token_base<type_specifier_id, type_specifier_t> type_specifier_simple_t;
#endif

/*struct type_specifier_simple_t : public token_base
{
	type_specifier_id id;
	virtual void accept(class visitor_t& v);
};*/

/*struct type_specifier_t : public declaration_specifier_type
{
	ch<type_specifier_simple_t> simple;
	ch<type_specifier_complex_t> complex;
	virtual void accept(class visitor_t& v);
};*/

struct type_qualifier_t : public declaration_specifier_type
{
	ptn<token_t> c;
	virtual void accept(class visitor_t& v);
};

#if 0
struct function_specifier_t : public declaration_specifier_type {
	
	virtual void accept(class visitor_t& v);
};
// FEATURE: needed?
struct alignment_specifier_t : public declaration_specifier_type {
	virtual void accept(class visitor_t& v);
};
#endif

struct declaration_list_t : public node_t<struct function_definition_t>
{
	virtual void accept(class visitor_t& v);
	std::list<struct declaration_t*> declarations;
};


struct block_item_t : public node_t<struct compound_statement_t> {
	virtual void accept(class visitor_t& v);
};

struct statement_t : public block_item_t {};

struct conditional_expression_t : public expression_t
{
	virtual void accept(class visitor_t& v);
};

struct labeled_statement_t : public statement_t
{
	enum type_t {
		case_label,
		default_label,
		jump_label
	};

	type_t type;

	enum access {
		keyword,
			identifier,
				case_expr,
					colon,
						statement
	};

	ptn<	token_t, // keyword: case, default
		ptn<	identifier_t, // label: jump
			ptn<	expression_t, // case
				ptn<	token_t, // colon: all
					ptn<	statement_t // all
		> > > > > c;

	virtual void accept(class visitor_t& v);
};

struct expression_statement_t : public statement_t
{
	virtual void accept(class visitor_t& v);
	
	ptn<	expression_t, // can be zero
			end_token > c;
};

/**
	represents one of the following rules:
	IF ( expression ) statement ELSE statement
	IF ( expression ) statement
	SWITCH ( expression ) statement
*/
struct selection_statement_t : public statement_t
{
	virtual void accept(class visitor_t& v);
	
	ptn<	token_t, // 1st keyword token
		ptn<	token_t, // (
			ptn<	expression_t,
				ptn<	token_t, // )
					ptn<	statement_t, // all
						ptn<	token_t, // else
							ptn<	statement_t
		> > > > > > > c;
};

/**
	represents one of the following rules:
	WHILE ( expression ) statement
	DO statement WHILE ( expression ) ;
	FOR ( expression_statement expression_statement ) statement
	FOR ( expression_statement expression_statement expression ) statement
	FOR ( declaration expression_statement ) statement
	FOR ( declaration expression_statement expression ) statement
*/
struct iteration_statement_t : public statement_t
{
	virtual void accept(class visitor_t& v);
	
	// careful! order of statement can differ!

	enum type_t {
		while_type,
		do_type,
		for_type
	} type;

	enum for_type_t
	{
		for_type_init_niter,
		for_type_init_iter,
		for_type_decl_niter,
		for_type_decl_iter
	} _for_type;

	enum access {
		do_keyword, // 2
			keyword,
				lbrace,
					while_cond, // 1, 2
						for_init, // 3, 4
							for_decl, // 5, 6
			for_cond, // 3-6
				for_iter, // 4, 6
					rbrace,
						statement,
							semicolon // 2

	};

	ptn<	token_t,
		ptn<	token_t,
			ptn<	token_t,
				ptn<	expression_t,
					ptn<	expression_statement_t,
						ptn<	declaration_t,

		ptn<	expression_statement_t,
			ptn<	expression_t,
				ptn<	token_t,
					ptn<	statement_t,
							end_token
		> > > > > > > > > > c;
};

struct designator_t : public node_t<struct designator_list_t> {};

struct designator_id : public designator_t
{
	ptn<	token_t,
		ptn<	identifier_t > > c;
	virtual void accept(class visitor_t& v);
};

struct designator_constant_expr : public designator_t
{
	ptn<	token_t,
		ptn<	expression_t,
			end_token > > c;
	virtual void accept(class visitor_t& v);
};

struct designator_list_t : public node_t<struct initializer_list_t>
{
	std::list<designator_t*> c;
	virtual void accept(class visitor_t& v);
};

struct initializer_list_t : public node_t<>
{
	ptn<	initializer_list_t,
		ptn<	token_t, // ,
			ptn<	designator_list_t,
				ptn<	token_t, // =
					ptn<	struct initializer_t
		> > > > > c;
	virtual void accept(class visitor_t& v);
};

struct initializer_t : public node_t<>
{
	//	assignment_expr
	//		{	init list
	//				,	}
	ptn<	expression_t,
		ptn<	token_t,
			ptn<	initializer_list_t,
				ptn<	token_t,
						end_token > > > > c;

	virtual void accept(class visitor_t& v);
};

struct init_declarator_t : public node_t<> // TODO: node<init_declarator_list>
{
	ptn<	struct declarator_t,
		ptn<	token_t,
			ptn<	initializer_t > > > c;

	virtual void accept(class visitor_t& v);
};

struct init_declarator_list_t : public node_t<struct declaration_t>
{
	ptn<	init_declarator_list_t,
		ptn<	token_t,
			ptn<	init_declarator_t > > > c;

	virtual void accept(class visitor_t& v);
};

/**
	represents one of the following rules:
	GOTO IDENTIFIER ;
	CONTINUE ;
	BREAK ;
	RETURN ;
	RETURN expression ;
*/
struct jump_statement_t : public statement_t
{
	virtual void accept(class visitor_t& v);

	enum type_t {
		goto_type,
		continue_type,
		break_type,
		return_void,
		return_type
	} type;

	enum {
		keyword,
			goto_id, // only goto_type
				expression, // only return_type
					semicolon
	} access;

	ptn<	token_t,
		ptn<	identifier_t,
			ptn<	expression_t,
					end_token> > > c;
};

struct declaration_t : public block_item_t
{
	virtual void accept(class visitor_t& v);

	ptn<	struct declaration_specifiers_t,
		ptn<	init_declarator_list_t, // optional
			end_token> > c; // semicolon
};

struct type_qualifier_list_t : public node_t<>
{
	std::list<type_qualifier_t*> c;
	virtual void accept(class visitor_t& v);
};

#if 0
struct block_item_list_t : public node_t // TODO: define inside compound_statement_t?
{
	virtual void accept(class visitor_t& v);
	std::list<block_item_t*> items;
};
#endif

// TODO: rename all child members to 'c'
struct compound_statement_t : public statement_t
{
	ptn<	token_t, // left bracket
		ptn<	std::list<block_item_t*>,
				end_token> > c; // right bracket

	virtual void accept(class visitor_t& v);
};

struct pointer_t : public node_t<> // FEATURE: node<base class of declarator and abstract_d.>?
{
	ptn<	token_t,
		ptn<	type_qualifier_list_t,
			ptn<	pointer_t > > > c;
	virtual void accept(class visitor_t& v);
};
struct direct_declarator_t : public node_t<> { // FEATURE: node<base class of ...>?
	/*id_t identifier;
	bool bracktype;
	id_t lbrack, rbrack;*/

	virtual void accept(class visitor_t& v);
}; // TODO: identifier...

struct direct_declarator_id : public direct_declarator_t {
	ptn<identifier_t> c;
	virtual void accept(class visitor_t& v);
};

struct direct_declarator_decl : public direct_declarator_t
{
	ptn<	token_t,
		ptn<	declarator_t,
				end_token > > c;
	virtual void accept(class visitor_t& v);
};

//! number of args: 7
struct direct_declarator_arr : public direct_declarator_t
{
	// careful, order can differ,
	// however, it won't matter much
	// also, first and last token are like in the tuple
	ptn<	direct_declarator_t,
		ptn<	token_t, // [
			ptn<	token_t, // static kw
				ptn<	type_qualifier_list_t,
					ptn<	expression_t,
						ptn<	token_t, // *
								end_token // ]
		> > > > > > c;
	virtual void accept(class visitor_t& v);
};

struct direct_declarator_func : public direct_declarator_t
{
	ptn<	direct_declarator_t,
		ptn<	token_t, // (
			ptn<	parameter_type_list_t,
					end_token > > > c;

	virtual void accept(class visitor_t& v);
};

// FEATUR: what is this?
struct direct_declarator_idlist : public direct_declarator_t
{
	ptn<	direct_declarator_t,
		ptn<	token_t, // (
			ptn<	identifier_list_t,
					end_token > > > c;

	virtual void accept(class visitor_t& v);
};





struct direct_abstract_declarator_t : public node_t<> { // FEATURE: base class of ...?
	/*id_t identifier;
	bool bracktype;
	id_t lbrack, rbrack;*/

	virtual void accept(class visitor_t& v);
}; // TODO: identifier...

struct direct_abstract_declarator_decl : public direct_abstract_declarator_t
{
	ptn<	token_t,
		ptn<	abstract_declarator_t,
				end_token > > c;
	virtual void accept(class visitor_t& v);
};

//! number of args: 7
struct direct_abstract_declarator_arr : public direct_abstract_declarator_t
{
	// careful, order can differ,
	// however, it won't matter much
	// also, first and last token are like in the tuple
	ptn<	direct_abstract_declarator_t,
		ptn<	token_t, // [
			ptn<	expression_t,
				ptn<	token_t, // *
						end_token // ]
		> > > > c;

	virtual void accept(class visitor_t& v);
};

struct direct_abstract_declarator_func : public direct_abstract_declarator_t
{
	ptn<	direct_abstract_declarator_t,
		ptn<	token_t, // (
			ptn<	parameter_type_list_t,
					end_token > > > c;

	virtual void accept(class visitor_t& v);
};


struct declarator_t : public node_t<>
{
	virtual void accept(class visitor_t& v);
	ptn<pointer_t, ptn< direct_declarator_t > > c;
};

struct declaration_specifiers_t : public node_t<>
{
	virtual void accept(class visitor_t& v);
	std::list<node_base*> c;
};

struct function_definition_t : public node_t<struct external_declaration_t>
{
	virtual void accept(class visitor_t& v);
	ptn<	declaration_specifiers_t,
		ptn<	declarator_t,
			ptn<	declaration_list_t,
				ptn<	compound_statement_t > > > > c;



	function_definition_t(declaration_specifiers_t* declaration_specifiers,
		declarator_t* declarator,
		declaration_list_t* declaration_list,
		compound_statement_t* compound_statement) :
			c(declaration_specifiers, declarator,
				declaration_list, compound_statement)
	{}
};

struct external_declaration_t : public node_t<struct translation_unit_t>
{
	virtual void accept(class visitor_t& v);
	ptn<	function_definition_t,
		ptn<	declaration_t> > c;
	external_declaration_t(function_definition_t* f) : c(f) {}
	external_declaration_t(declaration_t* d) : c(NULL, d) {}
};

struct translation_unit_t : public node_t<translation_unit_t> // FEATURE: inherit from node_base?
{
	std::list<external_declaration_t*> c;
	virtual void accept(class visitor_t& v);
};

#endif // NODE_H
