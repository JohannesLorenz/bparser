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
	virtual void accept(class visitor_t& v);

	op_t op;
	token_t* op_token;
	token_t* op_token_2; //!< ternary only
	node_t* n1; // TODO: expression base class, including const?
	node_t* n2; //!< only binary and ternary ops	
	node_t* n3; //!< only ternary ops
	
	expression_t(op_t op, token_t* op_token, token_t* op_token_2, node_t *n1, node_t* n2 = NULL, node_t* n3 = NULL) :
		op(op), op_token(op_token), op_token_2(op_token_2), n1(n1), n2(n2), n3(n3) {}
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
	type_specifier_simple_t* simple;
	type_specifier_complex_t* complex;
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

struct statement_t : public block_item_t {
	virtual void accept(class visitor_t& v);
};

struct identifier_t : public node_t {
	std::string name;
	virtual void accept(class visitor_t& v);
};

struct conditional_expression_t : public node_t
{
	virtual void accept(class visitor_t& v);
};

struct labeled_statement_t : public statement_t
{
	// all
	token_t* colon;
	statement_t* statement;

	// label:
	identifier_t* identifier;
	
	// case ...:
	token_t* case_token;
	conditional_expression_t constant_expression;
	
	// default:
	token_t* default_token;

	virtual void accept(class visitor_t& v);
};

struct expression_statement_t : public statement_t
{
	virtual void accept(class visitor_t& v);
	
	expression_t* expression; // can be zero
	token_t* semicolon;
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
	expression_t* expression;
	statement_t* statement;
	token_t* lbrace;
	token_t* rbrace;

	// 1 + 2
	token_t* if_token;

	// 1
	token_t* else_token;
	statement_t* else_statement;
	
	// 3
	token_t* switch_token;
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
	statement_t* statement;
	token_t* lbrace;
	token_t* rbrace;
	
	// 1, 2
	expression_t* while_condition;
	token_t* while_token;

	// 2
	token_t* do_token;
	token_t* semicolon;

	// 3 - 6
	expression_statement_t* for_condition;
	token_t* for_token;
	
	// 3, 4
	expression_statement_t* for_init_statement;

	// 5, 6
	expression_statement_t* for_init_declaration;

	// 4, 6
	expression_statement_t* for_expression;
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

	token_t* keyword;
	token_t* semicolon;
	identifier_t goto_identifier; //!< only GOTO IDENTIFIER ;
	expression_t expression; //!< only RETURN expression ;
};

struct declaration_t : public block_item_t
{
	virtual void accept(class visitor_t& v);

	struct declaration_specifiers_t* declaration_specifiers;
	token_t* semicolon;
	init_declarator_list_t* init_declarator_list; //!< optional
};






struct block_item_list_t : public node_t
{
	virtual void accept(class visitor_t& v);
	std::list<block_item_t*> items;
};

struct compound_statement_t : public node_t
{
	token_t* lbrack, rbrack;
	block_item_list_t* block_item_list;
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
	pointer_t* pointer;
	direct_declarator_t* direct_declarator;
};

struct declaration_specifiers_t : public node_t
{
	virtual void accept(class visitor_t& v);
	std::list<declaration_specifier_type*> specifiers;
};

struct function_definition_t : public node_t
{
	virtual void accept(class visitor_t& v);
	declaration_specifiers_t* declaration_specifiers;
	declarator_t* declarator;
	declaration_list_t* declaration_list;
	compound_statement_t* compound_statement;

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
	function_definition_t* function_definition;
	declaration_t* declaration;
	external_declaration_t(function_definition_t* f) : function_definition(f) {}
	external_declaration_t(declaration_t* d) : declaration(d) {}
};

struct translation_unit_t : public node_t
{
	std::list<external_declaration_t*> v;
	virtual void accept(class visitor_t& v);
};

#endif // NODE_H
