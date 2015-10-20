/**
 * @file node.h
 * definition of a node of the resulting AST
 */
#ifndef NODE_H
#define NODE_H

#include <cstddef>
#include <list>
#include <string>

struct geom_t
{
	std::size_t line;
	std::size_t col;
	geom_t(std::size_t line, std::size_t col) : line(line), col(col) {}
	geom_t() {}

	friend std::ostream& operator<<(std::ostream& stream, const geom_t&);
};

struct span_t
{
	geom_t first;
	geom_t second;
	span_t(std::size_t line, std::size_t col, std::size_t width) :
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

class node_t
{
public:
	typedef std::size_t id_t;

	span_t span; // TODO: public
	virtual void accept(class visitor_t& v) = 0;

	node_t() {}
	node_t(const geom_t& geom) : span(geom, geom_t()) {}

	virtual ~node_t() {}
};

struct token_t : public node_t
{
	int value;
public:
	token_t(const geom_t& pos, int value) : node_t(pos), value(value) {}
	virtual void accept(class visitor_t& v);
};

typedef ch<token_t> tok;

struct number_t : public node_t
{
	int value;
public:
	number_t(int value) : value(value) {}
	virtual void accept(class visitor_t& v);
};

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


/*
template<class T>
struct ch
{
	T* child_ptr;
};
*/

struct declaration_specifier_type : public node_t {
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
	op_inc, //!< ++
	op_dec, //!< --

	
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
	op_asn_or
};

struct expression_t : public node_t
{
	void accept_children(visitor_t& v);
	virtual void accept(class visitor_t& v);
	expression_t() {} // TODO?

	op_t op;
	tok op_token;
	tok op_token_2; //!< ternary only
	ch<node_t> n1; // TODO: expression base class, including const? // TODO: expression_t should suffice because of primary_expression_t
	ch<node_t> n2; //!< only binary and ternary ops	
	ch<node_t> n3; //!< only ternary ops
	
	expression_t(op_t op, token_t* op_token, token_t* op_token_2, node_t *n1, node_t* n2 = NULL, node_t* n3 = NULL) :
		op(op), op_token(op_token), op_token_2(op_token_2), n1(n1), n2(n2), n3(n3) {}
};

struct constant_t : public node_t {
	virtual void accept(class visitor_t& v);
	int iconstant;
};

struct primary_expression_t : public expression_t
{
	virtual void accept(class visitor_t& v);
	tok lbrace, rbrace;
	ch<constant_t> constant;
	char* identifier;
};

struct type_specifier_t : public declaration_specifier_type {
	virtual void accept(class visitor_t& v);
	type_specifier_t(const geom_t& geom) : declaration_specifier_type(geom) {}
};

template<class IdType, class Base = node_t>
struct token_base : public Base
{
	IdType id;
	virtual void accept(class visitor_t& v);
	token_base(geom_t pos, const IdType& id) : Base(pos), id(id) {}
};

typedef token_base<type_specifier_id, type_specifier_t> type_specifier_simple_t;

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
	virtual void accept(class visitor_t& v);
};
struct function_specifier_t : public declaration_specifier_type {
	
	virtual void accept(class visitor_t& v);
};
struct alignment_specifier_t : public declaration_specifier_type {
	virtual void accept(class visitor_t& v);
};
struct declaration_list_t : public node_t
{
	virtual void accept(class visitor_t& v);
	std::list<struct declaration_t*> declarations;
};


struct block_item_t : public node_t {
	virtual void accept(class visitor_t& v);
};

struct statement_t : public block_item_t {};

struct identifier_t : public node_t {
	std::string name;
	virtual void accept(class visitor_t& v);
	identifier_t(const char* name) : name(name) {}
};

struct conditional_expression_t : public expression_t
{
	virtual void accept(class visitor_t& v);
};

struct labeled_statement_t : public statement_t
{
	// all
	ch<token_t> colon;
	ch<statement_t> statement;

	// label:
	ch<identifier_t> identifier;
	
	// case ...:
	ch<token_t> case_token;
	ch<expression_t> expression;
	
	// default:
	ch<token_t> default_token;

	virtual void accept(class visitor_t& v);
};

struct expression_statement_t : public statement_t
{
	virtual void accept(class visitor_t& v);
	
	ch<expression_t> expression; // can be zero
	ch<token_t> semicolon;
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
	ch<token_t> lbrace;
	ch<token_t> rbrace;

	// 1 + 2
	ch<token_t> if_token;

	// 1
	ch<token_t> else_token;
	ch<statement_t> else_statement;
	
	// 3
	ch<token_t> switch_token;
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
	ch<token_t> lbrace;
	ch<token_t> rbrace;
	
	// 1, 2
	ch<expression_t> while_condition;
	ch<token_t> while_token;

	// 2
	ch<token_t> do_token;
	ch<token_t> semicolon;

	// 3 - 6
	ch<expression_statement_t> for_condition;
	ch<token_t> for_token;
	
	// 3, 4
	ch<expression_statement_t> for_init_statement;

	// 5, 6
	ch<declaration_t> for_init_declaration;

	// 4, 6
	ch<expression_t> for_expression;
};

struct init_declarator_list_t : public node_t
{
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

	ch<token_t> keyword, semicolon;
	ch<identifier_t> goto_identifier; //!< only GOTO IDENTIFIER ;
	ch<expression_t> expression; //!< only RETURN expression ;
};

struct declaration_t : public block_item_t
{
	virtual void accept(class visitor_t& v);

	ch<struct declaration_specifiers_t> declaration_specifiers;
	ch<token_t> semicolon;
	ch<init_declarator_list_t> init_declarator_list; //!< optional
};






struct block_item_list_t : public node_t
{
	virtual void accept(class visitor_t& v);
	std::list<block_item_t*> items;
};

struct compound_statement_t : public statement_t
{
	ch<token_t> lbrack, rbrack;
	//block_item_list_t* block_item_list;
	std::list<block_item_t*> block_items;
	virtual void accept(class visitor_t& v);
};

struct pointer_t : public node_t {
	virtual void accept(class visitor_t& v);
};
struct direct_declarator_t : public node_t {
	id_t identifier;
	bool bracktype;
	id_t lbrack, rbrack;

	virtual void accept(class visitor_t& v);
}; // TODO: identifier...

struct declarator_t : public node_t
{
	virtual void accept(class visitor_t& v);
	ch<pointer_t> pointer;
	ch<direct_declarator_t> direct_declarator;
};

struct declaration_specifiers_t : public node_t
{
	virtual void accept(class visitor_t& v);
	std::list<declaration_specifier_type*> specifiers;
};

struct function_definition_t : public node_t
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

struct external_declaration_t : public node_t
{
	virtual void accept(class visitor_t& v);
	ch<function_definition_t> function_definition;
	ch<declaration_t> declaration;
	external_declaration_t(function_definition_t* f) : function_definition(f) {}
	external_declaration_t(declaration_t* d) : declaration(d) {}
};

struct translation_unit_t : public node_t
{
	std::list<external_declaration_t*> v;
	virtual void accept(class visitor_t& v);
};

#endif // NODE_H
