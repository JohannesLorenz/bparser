/**
 * @file node.h
 * definition of a node of the resulting AST
 */
#ifndef NODE_H
#define NODE_H

#include <cstddef>
#include <vector>

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
	span_t span; // TODO: public
	virtual void accept(class visitor_t& v) = 0;

	node_t() {}
	node_t(const geom_t& geom) : span(geom, geom_t()) {}

	virtual ~node_t() {}
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

struct expression_t : public node_t
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
};
/*
template<class T>
struct ch
{
	T* child_ptr;
};
*/

struct storage_class_specifier_t : public node_t {
	virtual void accept(class visitor_t& v);
};

enum type_specifier_id
{
	t_int,
	t_float
};

struct type_specifier_t : public node_t
{
	type_specifier_id id;
	virtual void accept(class visitor_t& v);
	type_specifier_t(geom_t g, type_specifier_id id) : node_t(g), id(id) {}
};

struct type_qualifier_t : public node_t
{
	virtual void accept(class visitor_t& v);
};
struct function_specifier_t : public node_t{
	virtual void accept(class visitor_t& v);
};
struct alignment_specifier_t : public node_t{
	virtual void accept(class visitor_t& v);
};
struct declaration_list_t : public node_t
{
	virtual void accept(class visitor_t& v);
	std::vector<struct declaration_t*> declarations;
};
struct compound_statement_t : public node_t {
	virtual void accept(class visitor_t& v);
};

struct pointer_t : public node_t {
	virtual void accept(class visitor_t& v);
};
struct direct_declarator_t : public node_t {
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
	std::vector<storage_class_specifier_t*> storage_class_specifiers;
	std::vector<type_specifier_t*> type_specifiers;
	std::vector<type_qualifier_t*> type_qualifiers;
	std::vector<function_specifier_t*> function_specifiers;
	std::vector<alignment_specifier_t*> alignment_specifiers;
	declaration_specifiers_t(storage_class_specifier_t* s)
		{ storage_class_specifiers.push_back(s); }
	declaration_specifiers_t(type_specifier_t* s)
		{ type_specifiers.push_back(s); }
	declaration_specifiers_t(type_qualifier_t* s)
		{ type_qualifiers.push_back(s); }
	declaration_specifiers_t(function_specifier_t* s)
		{ function_specifiers.push_back(s); }
	declaration_specifiers_t(alignment_specifier_t* s)
		{ alignment_specifiers.push_back(s); }
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
		declaration_list_t* declarator_list,
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
	std::vector<external_declaration_t*> v;
	virtual void accept(class visitor_t& v);
};

#endif // NODE_H
