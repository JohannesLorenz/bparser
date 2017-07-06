/*************************************************************************/
/* bparser - a bison-based, C99 parser                                   */
/* Copyright (C) 2015-2016                                               */
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

/**
	@file node.h
	definition of a node of the resulting AST
	A node is a node of the AST graph depicting a precompiled-ANSII-C99
	program. Nodes can be declarations, definitions, expressions etc.
	The header defines the basic node class and its derived classes.
*/
#ifndef NODE_H
#define NODE_H

#include <stdexcept>
#include <cstddef>
#include <list>
#include <string>

#include "node_fwd.h"
#include "tuple03.h"
#include "geom.h"

template<class P>
class safe_ptr
{
	P* p;
	static void assert_p(const P* p) {
		if(!p) throw std::logic_error("pointer access");
	}
public:
	safe_ptr() : p(NULL) {}
	safe_ptr(P* p) : p(p) {}
	safe_ptr& operator=(P* _p) { return p = _p, *this; }
	operator P*() { return p; }
	operator P const*() const { return p; }
	//bool operator==(const P* _p) const { return p == _p; }
	P& operator*() { assert_p(p); return *p; }
	P const& operator*() const { assert_p(p); return *p; }
	P* operator->() { assert_p(p); return p; }
	P const* operator->() const { assert_p(p); return p; }
	bool operator!() const { return p == NULL; }
};

// FEATURE: not here?
enum lookup_type
{
	lt_enumeration,
	lt_typedef_name,
	lt_identifier,
	lt_identifier_list,
	lt_struct_bound,
	lt_undefined
};

void init_parser(const char* fname = NULL); //!< see node.cpp

class node_t
{
public:
	null_type c; //!< base case, overwrite this

	typedef std::size_t id_t;

	span_t span; // FEATURE: not public
	virtual void accept(class visitor_t& v) = 0;

	node_t() {}
	node_t(const geom_t& geom) : span(geom, geom_t()) {}

	virtual ~node_t() {}
};

typedef node_t node_base;

template<class ParType = node_t>
class has_par
{
public:
	typedef ParType parent_type;
	ParType* parent;
};

#if 0
template<class First, class Next = null_type>
class node_ch : public node_t, public has_par<>
{
	ptn<First, Next> c;
public:
	node_ch() {}
	node_ch(const geom_t& geom) : node_t<>(geom) {}

	virtual ~node_ch() {}
	void accept_children(visitor_t& v);
};
#endif

namespace nodes {

class terminal_t : public node_t, public has_par<>
{
	int _value;
	virtual std::size_t length() const = 0;
	virtual std::size_t newlines() const = 0;
	virtual std::size_t tablength() const = 0; //!< shall return 0 or 8
public:
	int value() const { return _value; }
	std::size_t get_tablength() const { return tablength(); }
	std::size_t get_length() const { return length(); }
	std::size_t get_newlines() const { return newlines(); }
	terminal_t(const geom_t& geom, int _value) : node_t(geom), _value(_value) {}
};

struct token_t : public terminal_t
{
	std::size_t length() const;
	std::size_t tablength() const;
	std::size_t newlines() const;
public:
	token_t(const geom_t& pos, int value) : terminal_t(pos, value) {}
	virtual void accept(class visitor_t& v);
	friend std::ostream& operator<<(std::ostream& , const token_t& );
};

std::ostream& operator<<(std::ostream& , const token_t& );

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
	std::size_t tablength() const { return 0; /* no tabs */ }
	std::size_t newlines() const;
public:
	noconst_1line_terminal_t(const geom_t& geom, int value,
		const char* raw) :
		noconst_terminal_t(geom, value, raw) {}
};

struct defined_t : public noconst_1line_terminal_t
{
public:
	safe_ptr<struct identifier_t> _definition;
	defined_t(const geom_t& geom, int value,
		const char* raw) :
		noconst_1line_terminal_t(geom, value, raw),
		_definition(NULL) {}
};

struct identifier_t : public defined_t
{
	virtual void accept(class visitor_t& v);
	identifier_t(const char* name, geom_t geom);
};

struct typedef_name_t : public defined_t
{
	virtual void accept(class visitor_t& v);
	typedef_name_t(const char* name, geom_t geom);
};

struct enumeration_constant_t : public defined_t
{
	virtual void accept(class visitor_t& v);
	enumeration_constant_t(const char* name, geom_t geom);
};

struct string_base_t : public noconst_terminal_t
{
	std::size_t _length, _newlines;
	std::size_t length() const;
	std::size_t tablength() const { return 0; /* no tabs */ }
	std::size_t newlines() const;
public:
	string_base_t(const char* value, geom_t geom, int tok);
};

struct string_literal_t : public string_base_t
{
	void accept(class visitor_t& v);
	string_literal_t(const char* value, geom_t geom);
};

struct comment_t : public string_base_t
{
	void accept(class visitor_t& v);
	comment_t(const std::string& value, geom_t geom);
};

struct func_name_t : public string_base_t
{
	void accept(class visitor_t& v);
	func_name_t(const std::string& value, geom_t geom);
};

struct declaration_specifier_type : public node_t, public has_par<> {
	declaration_specifier_type() {}
	declaration_specifier_type(const geom_t& geom) : node_t(geom) {}
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

inline bool makes_assignment(binary_op_t b) {
	return b >= op_asn && b <= op_asn_or;
}

typedef ptn<token_t> end_token;

struct expression_t : public node_t, public has_par<> {};

struct unary_expression_r : public expression_t
{
	unary_op_t op_id;
	ptn<	expression_t, // FEATURE: more exactly?
			end_token > c;

	virtual void accept(class visitor_t& v);
};

struct unary_expression_l : public expression_t
{
	unary_op_t op_id;
	ptn<	token_t,
		ptn<	expression_t> > c;

	virtual void accept(class visitor_t& v);
};

struct binary_expression_t : public expression_t
{
	binary_op_t op_id;
	ptn<	expression_t,
		ptn<	token_t,
			ptn<	expression_t> > > c;
	virtual void accept(class visitor_t& v);
};

struct ternary_expression_t : public expression_t
{
	void accept_children(visitor_t& v);
	virtual void accept(class visitor_t& v);

	ptn<	expression_t,
		ptn<	token_t,
			ptn<	expression_t,
				ptn<	token_t,
					ptn<	expression_t > > > > > c;

//	expression_t(op_t op, token_t* op_token, token_t* op_token_2, node_t *n1, node_t* n2 = NULL, node_t* n3 = NULL) :
//		op(op), op_token(op_token), op_token_2(op_token_2), n1(n1), n2(n2), n3(n3) {}
};

struct abstract_declarator_t : public node_t, public has_par<>
{
	ptn<	struct pointer_t,
		ptn<	struct direct_abstract_declarator_t > > c;
	void accept(class visitor_t& v);
};

struct type_specifier_t : public declaration_specifier_type
{
	ptn<	node_t> c; // FEATURE: more granular?
	virtual void accept(class visitor_t& v);
};

struct type_specifier_complex_t : public type_specifier_t {
	//virtual void accept(class visitor_t& v);
};

struct declaration_base
{
	virtual declaration_specifiers_t& decl_spec() = 0;
	virtual ~declaration_base() {}
	virtual void accept(class visitor_t& v) = 0;
};

struct attr_name_t : public noconst_1line_terminal_t
{
	virtual void accept(class visitor_t& v);
	attr_name_t(const char* name, geom_t geom);
};

struct attribute_t : public node_t, public has_par<struct_or_union_specifier_t>
{
	virtual void accept(class visitor_t& v);
	ptn<	token_t,
		ptn<	token_t,
			ptn<	attr_name_t,
				ptn<	token_t,
						end_token
		> > > > c;
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
					ptn<	token_t,
						ptn<	attribute_t >
		> > > > > c;

	virtual void accept(class visitor_t& v);
};

struct struct_declaration_list_t : public node_t, public has_par<> // FEATURE: when alt list
{
	std::list<struct struct_declaration_t*> c;
	void accept(class visitor_t& v);
};

struct struct_declaration_t : public node_t, public has_par<struct_declaration_list_t>,
	public declaration_base
{
	ptn<	struct declaration_specifiers_t, // FEATURE: don't use node_base there
		ptn<	struct struct_declarator_list_t,
				end_token > > c;
	void accept(class visitor_t& v);
	declaration_specifiers_t& decl_spec() { return *c.value; }
};



struct specifier_qualifier_list_t : public node_t, public has_par<>
{
	std::list<declaration_specifier_type*> c;
	void accept(class visitor_t& v);
};

struct struct_declarator_list_t : public node_t, public has_par<> // FEATURE: when alt list
{
	ptn<	struct struct_declarator_list_t,
		ptn<	token_t, // :
			ptn<	struct struct_declarator_t
			> > > c;
	void accept(class visitor_t& v);
};

struct struct_declarator_t : public node_t, public has_par<struct_declarator_list_t>
{
	// careful! all optional!
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

struct enumerator_list_t : public node_t, public has_par<> // FEATURE: when alt list
{
	ptn<	enumerator_list_t,
		ptn<	token_t,
			ptn<	struct enumerator_t > > > c;
	void accept(class visitor_t& v);
};

struct enumerator_t : public node_t, public has_par<enumerator_list_t>
{
	ptn<	identifier_t,
		ptn<	token_t, // =, optional
			ptn<	expression_t // optional
		> > > c;
	void accept(class visitor_t& v);
};

struct parameter_type_list_t : public node_t, public has_par<> // FEATURE: declar. or abstr. declar.
{
	ptn<	struct parameter_list_t,
		ptn<	token_t,
				end_token > > c;
	void accept(class visitor_t& v);
};

struct parameter_list_t : public node_t, public has_par<> // FEATURE: when alt list
{
	ptn<	parameter_list_t,
		ptn<	token_t,
			ptn<	struct parameter_declaration_t
		> > > c;
	void accept(class visitor_t& v);
};

struct parameter_declaration_t : public node_t, public has_par<parameter_list_t>, public declaration_base
{
	ptn<	struct declaration_specifiers_t,
		ptn<	declarator_t,
			ptn<	abstract_declarator_t
		> > > c;

	declaration_specifiers_t& decl_spec() { return *c.get<0>(); }
	void accept(class visitor_t& v);
};

struct identifier_list_t : public node_t, public has_par<> // FEATURE: when alt list
{
	ptn<	identifier_list_t,
		ptn<	token_t,
			ptn<	identifier_t
		> > > c;
	void accept(class visitor_t& v);
};

struct type_name_t : public node_t, public has_par<>
{
	ptn<	declaration_specifiers_t,
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

#if 0
// strings, ints, floats
template<class T>
struct constant_t : public terminal_t
{
	std::string value;
//	ptn<token_t> c;
//	constant_t() {}
	constant_t(const char* value, geom_t geom) : terminal_t(geom, 0), value(value) {}

	virtual void accept(class visitor_t& v);
};
#endif

struct iconstant_t : public noconst_1line_terminal_t
{
	long value;
	//! canonical type of suffix, or prefix for char literals
	enum suf_type_t
	{
		no_suffix,
		suf_u,
		suf_ul,
		suf_ull,
		suf_l,
		suf_ll
	} suf_type;
	
	//! literal number system if an integer, otherwise 'character'
	enum number_system_t
	{
		octal,
		decimal,
		hexadecimal,
		character
	} number_system;

	//! returns the scanf 'type modifier character', as in scanf("%d", &value); 
	char scanf_modifier() const
	{
		switch(number_system)
		{
			case octal: return 'o';
			case decimal: return 'd';
			case hexadecimal: return 'x';
			default: return 'c';
		}
	}

	//! returns an equivalent, canonical suffix string, or prefix for char literals 
	const char* suffix() const
	{
		switch(suf_type)
		{
			case suf_u: return "u";
			case suf_ul: return "ul";
			case suf_ull: return "ull";
			case suf_l: return "l";
			case suf_ll: return "ll";
			default: return "";
		}
	}
	
	bool is_signed() const { return suf_type == no_suffix || suf_type == suf_l || suf_type == suf_ll; }

	iconstant_t(const char* value, geom_t geom) :
		noconst_1line_terminal_t(geom, 0, value) {}

	virtual void accept(class visitor_t& v);
};

struct fconstant_t : public noconst_1line_terminal_t
{
	fconstant_t(const char* value, geom_t geom) :
		noconst_1line_terminal_t(geom, 0, value) {}

	virtual void accept(class visitor_t& v);
};

struct primary_expression_t : public expression_t
{
	ptn<	noconst_terminal_t, // TODO: very bad name!!?
		ptn<	struct identifier_t, // variable or function
			ptn<	token_t,
				ptn<	expression_t,
						end_token
		> > > > c;
	
	virtual void accept(class visitor_t& v);
};

struct array_access_expression_t : public expression_t
{
	ptn<	expression_t,
		ptn<	token_t,
			ptn<	expression_t,
					end_token > > > c;
	virtual void accept(class visitor_t& v);
};

struct argument_expression_list_t : public node_t, public has_par<>
{
private:
	expression_t& rget(std::size_t idx) {
		return idx ? c.get<0>()->rget(idx-1) : *c.get<2>();
	}
public:
	ptn<	argument_expression_list_t,
		ptn<	token_t,
			ptn<	expression_t> > > c;
	
	expression_t& get(std::size_t idx) {
		return rget(size() - 1 - idx);
	}
	std::size_t size() const {
		return c.get<0>() ? (1 + c.get<0>()->size()) : 0;
	}
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

// FEATURE: base for postfix expressions? probably not?
struct struct_access_expression_t : public expression_t
{
	bool pointer_access;
	ptn<	expression_t,
		ptn<	token_t,	// . or ->
			ptn<	identifier_t > > > c;
	virtual void accept(class visitor_t& v);
};

struct compound_literal_t : public expression_t
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

struct declaration_list_t : public node_t, public has_par<struct function_definition_t>
{
	virtual void accept(class visitor_t& v);
	std::list<struct declaration_t*> c;
};

// FEATURE: use compound_statement_t here as a parent and put declaration into wrapper?
struct block_item_t : public node_t, public has_par<> {
	virtual void accept(class visitor_t& v);
};

struct statement_t : public block_item_t {};

/*struct conditional_expression_t : public expression_t
{
	virtual void accept(class visitor_t& v);
};*/

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

	//! returns the conditional expression (can be NULL in for loops)
	expression_t* condition();
};

struct designator_t : public node_t, public has_par<struct designator_list_t> {};

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

struct designator_list_t : public node_t, public has_par<struct initializer_list_t>
{
	std::list<designator_t*> c;
	virtual void accept(class visitor_t& v);
};

struct initializer_list_t : public node_t, public has_par<>
{
	ptn<	initializer_list_t,
		ptn<	token_t, // ,
			ptn<	designator_list_t,
				ptn<	token_t, // =
					ptn<	struct initializer_t
		> > > > > c;
	virtual void accept(class visitor_t& v);
};

struct initializer_t : public node_t, public has_par<>
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

struct init_declarator_t : public node_t, public has_par<> // FEATURE: node<init_declarator_list>
{
	ptn<	struct declarator_t,
		ptn<	token_t,
			ptn<	initializer_t > > > c;

	virtual void accept(class visitor_t& v);
};

struct init_declarator_list_t : public node_t, public has_par<> // FEATURE: struct declaration_t
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

struct declaration_t : public block_item_t, public declaration_base
{
	virtual void accept(class visitor_t& v);

	ptn<	struct declaration_specifiers_t,
		ptn<	init_declarator_list_t, // optional
			end_token> > c; // semicolon

	declaration_specifiers_t& decl_spec() { return *c.get<0>(); }
};

struct type_qualifier_list_t : public node_t, public has_par<>
{
	std::list<type_qualifier_t*> c;
	virtual void accept(class visitor_t& v);
};

#if 0
struct block_item_list_t : public node_t
{
	virtual void accept(class visitor_t& v);
	std::list<block_item_t*> items;
};
#endif

struct compound_statement_t : public statement_t
{
	ptn<	token_t, // left bracket
		ptn<	std::list<block_item_t*>,
				end_token> > c; // right bracket

	virtual void accept(class visitor_t& v);
};

struct pointer_t : public node_t, public has_par<> // FEATURE: node<base class of declarator and abstract_d.>?
{
	ptn<	token_t,
		ptn<	type_qualifier_list_t,
			ptn<	pointer_t > > > c;
	virtual void accept(class visitor_t& v);
};
struct direct_declarator_t : public node_t, public has_par<> { // FEATURE: node<base class of ...>?
	virtual void accept(class visitor_t& v);
};

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

//! node representing old-C-style identifier list
//! for example: void f(a, b) int a, b {}
struct direct_declarator_idlist : public direct_declarator_t
{
	ptn<	direct_declarator_t,
		ptn<	token_t, // (
			ptn<	identifier_list_t,
					end_token > > > c;

	virtual void accept(class visitor_t& v);
};





struct direct_abstract_declarator_t : public node_t, public has_par<> { // FEATURE: base class of ...?
	virtual void accept(class visitor_t& v);
};

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


struct declarator_t : public node_t, public has_par<>
{
	virtual void accept(class visitor_t& v);
	ptn<pointer_t, ptn< direct_declarator_t > > c;
};

struct declaration_specifiers_t : public node_t, public has_par<>
{
	virtual void accept(class visitor_t& v);
	std::list<node_t*> c;
};

struct function_definition_t : public node_t, public has_par<struct external_declaration_t>, public declaration_base
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

	declaration_specifiers_t& decl_spec() { return *c.get<0>(); }
};

struct external_declaration_t : public node_t, public has_par<struct translation_unit_t>
{
	virtual void accept(class visitor_t& v);
	ptn<	function_definition_t,
		ptn<	declaration_t> > c;
	external_declaration_t(function_definition_t* f) : c(f) {}
	external_declaration_t(declaration_t* d) : c(NULL, d) {}
};

struct translation_unit_t : public node_t, public has_par<translation_unit_t> // FEATURE: inherit from node_base?
{
	std::list<external_declaration_t*> c;
	virtual void accept(class visitor_t& v);
};

}

#endif // NODE_H
