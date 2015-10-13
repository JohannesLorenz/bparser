/**
 * @file node.h
 * definition of a node of the resulting AST
 */
#ifndef NODE_H
#define NODE_H

#include <cstddef>

class node_t
{
public:
	virtual void accept(class visitor_t& v) = 0;
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

#endif // NODE_H
