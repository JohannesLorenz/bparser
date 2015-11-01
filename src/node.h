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

struct geom_t
{
	int line;
	int col;
	geom_t(int line, int col) : line(line), col(col) {}
	geom_t() : line(-1), col(-1) {}

	friend std::ostream& operator<<(std::ostream& stream, const geom_t&);
};

struct span_t
{
	geom_t first;
	geom_t second;
	span_t(int line, int col, std::size_t width) :
		first(line, col),
		second(line, col + width)
	{
	}
	span_t(geom_t first, geom_t second) : first(first), second(second) {}
	span_t() {}

	friend std::ostream& operator<<(std::ostream& stream, const span_t&);
};


//! class representing a child pointer
//! only intend: set the pointer to 0
template<class T>
class ch
{
	T* ptr;
public:
	ch() : ptr(NULL) {}
	explicit ch(T* ptr) : ptr(ptr) {}
	ch& operator=(T* _ptr) { return ptr = _ptr, *this; }

	operator T*() { return ptr; }
	operator const T*() const { return ptr; }
	
	T& operator* () { return *ptr; }
	const T& operator* () const { return *ptr; }
	T* operator-> () { return ptr; }
	const T* operator-> () const { return ptr; }

	operator bool() const { return ptr; }
};

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
	node_ch(const geom_t& geom) : node_t(geom) {}

	virtual ~node_ch() {}
	void accept_children(visitor_t& v); // TODO
};

struct token_t : public node_t<>
{
	int value;
public:
	token_t(const geom_t& pos, int value) : node_t(pos), value(value) {}
	virtual void accept(class visitor_t& v);
};

typedef ch<token_t> tok;
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
	declaration_specifier_type(const geom_t& geom) : node_t(geom) {}
};

struct storage_class_specifier_t : public declaration_specifier_type {
	virtual void accept(class visitor_t& v);
};

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

enum op_t
{
	op_addr,
	op_ptr,
	op_pos,
	op_neg,
	op_compl,
	op_not,
	op_inc_post, //!< ++
	op_dec_post, //!< --
	op_inc_pre,
	op_dec_pre,

	
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
	op_tern,

	op_arr_acc,
	op_func_call,
	op_struct_access_ref,
	op_struct_access_ptr,
	op_cast_postfix,
	op_cast
};

typedef ptn<token_t> end_token;

struct expression_t : public node_t<> {
	op_t op_id;
//	virtual void accept(class visitor_t& v);
};

struct unary_expression_r : public expression_t
{
	ptn<	node_t,
		end_token > c;

	virtual void accept(class visitor_t& v);
};

struct unary_expression_l : public expression_t
{
	ptn<	token_t,
		ptn<	node_t> > c;

	virtual void accept(class visitor_t& v);
};

struct binary_expression_t : public expression_t
{
	ptn<	node_t,
		ptn<	token_t,
			ptn<	node_t> > > c;
	virtual void accept(class visitor_t& v);
};

struct ternary_expression_t : public expression_t
{
	void accept_children(visitor_t& v);
	virtual void accept(class visitor_t& v);

	ptn<	node_t,
		ptn<	token_t,
			ptn<	node_t,
				ptn<	token_t,
					ptn<	node_t > > > > > c;

//	expression_t(op_t op, token_t* op_token, token_t* op_token_2, node_t *n1, node_t* n2 = NULL, node_t* n3 = NULL) :
//		op(op), op_token(op_token), op_token_2(op_token_2), n1(n1), n2(n2), n3(n3) {}
};

struct abstract_declarator_t : public node_t<>
{
	ptn<	struct pointer_t,
		ptn<	struct direct_abstract_declarator_t > > c;
	void accept(class visitor_t& v);
};

struct specifier_qualifier_list_t : public node_t<>
{
	std::list<declaration_specifier_type*> c;
	void accept(class visitor_t& v);
};

struct type_name_t : public node_t<>
{
	ptn<	specifier_qualifier_list_t,
		ptn<	abstract_declarator_t > > c;
	void accept(class visitor_t& v);
};

template<class Next>
struct to
{
	typedef ptn<token_t, Next> type;
};

struct sizeof_expression_t : public expression_t
{
	void accept(class visitor_t& v);
	/*tok sizeof_token, lbrace, rbrace;
	ch<type_name_t> type_name;*/
	//to< ptn< type_name_t, end_token > >::type c;
	ptn<	token_t,
		ptn <	token_t,
			ptn <	type_name_t, end_token > > > c;
};

struct identifier_t : public node_t<>
{
	std::string name;
	virtual void accept(class visitor_t& v);
	identifier_t(const char* name, geom_t geom) : node_t(geom), name(name) {}
};

enum constant_type
{
	ct_int,
	ct_float,
	ct_enum
};

struct constant_t : public node_t<struct primary_expression>
{
	virtual void accept(class visitor_t& v);
	constant_type type;
	union {
		int i;
		float f;
	} value;
	ch<identifier_t> enum_id;
};

enum primary_type
{
	pt_expression,
	pt_constant,
	pt_id,
	pt_string
};

struct primary_expression_t : public expression_t
{
	virtual void accept(class visitor_t& v);
	primary_type type;
	
	tok lbrace, rbrace;
	ch<constant_t> constant;
	identifier_t* identifier;
	identifier_t* string;
	expression_t* expression;
};

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

struct type_specifier_t : public declaration_specifier_type
{
	virtual void accept(class visitor_t& v);
};

struct type_specifier_token : public type_specifier_t
{
	token_t* c;
	virtual void accept(class visitor_t& v);
};

struct type_identifier : public type_specifier_t
{
	identifier_t* c;
	virtual void accept(class visitor_t& v);
};

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

struct type_specifier_complex_t : public type_specifier_t {
	virtual void accept(class visitor_t& v);
};

struct struct_or_union_specifier_t : public type_specifier_complex_t
{
	virtual void accept(class visitor_t& v);
};
struct enum_specifier_t : public type_specifier_complex_t {
	virtual void accept(class visitor_t& v);
};

/*struct type_specifier_t : public declaration_specifier_type
{
	ch<type_specifier_simple_t> simple;
	ch<type_specifier_complex_t> complex;
	virtual void accept(class visitor_t& v);
};*/

struct type_qualifier_t : public declaration_specifier_type
{
	token_t* c;
	virtual void accept(class visitor_t& v);
};
struct function_specifier_t : public declaration_specifier_type {
	
	virtual void accept(class visitor_t& v);
};
struct alignment_specifier_t : public declaration_specifier_type {
	virtual void accept(class visitor_t& v);
};
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
	// all
	tok colon;
	ch<statement_t> statement;

	// label:
	ch<identifier_t> identifier;
	
	// case ...:
	tok case_token;
	ch<expression_t> expression;
	
	// default:
	tok default_token;

	virtual void accept(class visitor_t& v);
};

struct expression_statement_t : public statement_t
{
	virtual void accept(class visitor_t& v);
	
	ch<expression_t> expression; // can be zero
	tok semicolon;
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
	
	// all
	ch<expression_t> expression;
	ch<statement_t> statement;
	tok lbrace;
	tok rbrace;

	// 1 + 2
	tok if_token;

	// 1
	tok else_token;
	ch<statement_t> else_statement;
	
	// 3
	tok switch_token;
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
	
	// all
	ch<statement_t> statement;
	tok lbrace;
	tok rbrace;
	
	// 1, 2
	ch<expression_t> while_condition;
	tok while_token;

	// 2
	tok do_token;
	tok semicolon;

	// 3 - 6
	ch<expression_statement_t> for_condition;
	tok for_token;
	
	// 3, 4
	ch<expression_statement_t> for_init_statement;

	// 5, 6
	ch<declaration_t> for_init_declaration;

	// 4, 6
	ch<expression_t> for_expression;
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

	tok keyword, semicolon;
	ch<identifier_t> goto_identifier; //!< only GOTO IDENTIFIER ;
	ch<expression_t> expression; //!< only RETURN expression ;
};

struct declaration_t : public block_item_t
{
	virtual void accept(class visitor_t& v);

	ch<struct declaration_specifiers_t> declaration_specifiers;
	ch<init_declarator_list_t> init_declarator_list; //!< optional
	tok semicolon;
};



struct type_qualifier_list_t : public node_t<>
{
	std::list<type_qualifier_t*> value;
	virtual void accept(class visitor_t& v);
};

#if 0
struct block_item_list_t : public node_t // TODO: define inside compound statement_t?
{
	virtual void accept(class visitor_t& v);
	std::list<block_item_t*> items;
};
#endif

// TODO: rename all child members to 'c'
struct compound_statement_t : public statement_t
{
	tok lbrack, rbrack;
	//block_item_list_t* block_item_list;
	std::list<block_item_t*> block_items;
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
	identifier_t* value;
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

struct parameter_type_list_t : public node_t<> { // FEATURE: base class of ...?
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
	std::list<node_t*> specifiers;
};

struct function_definition_t : public node_t<struct external_declaration_t>
{
	virtual void accept(class visitor_t& v);
	ch<declaration_specifiers_t> declaration_specifiers;
	ch<declarator_t> declarator;
	ch<declaration_list_t> declaration_list;
	ch<compound_statement_t> compound_statement;

	function_definition_t(declaration_specifiers_t* declaration_specifiers,
		declarator_t* declarator,
		declaration_list_t* declaration_list,
		compound_statement_t* compound_statement) :
		declaration_specifiers(declaration_specifiers),
		declarator(declarator),
		declaration_list(declaration_list),
		compound_statement(compound_statement)
	{}
};

struct external_declaration_t : public node_t<struct translation_unit_t>
{
	virtual void accept(class visitor_t& v);
	ch<function_definition_t> function_definition;
	ch<declaration_t> declaration;
	external_declaration_t(function_definition_t* f) : function_definition(f) {}
	external_declaration_t(declaration_t* d) : declaration(d) {}
};

struct translation_unit_t : public node_t<translation_unit_t> // FEATURE: inherit from node_base?
{
	std::list<external_declaration_t*> v;
	virtual void accept(class visitor_t& v);
};

#endif // NODE_H
