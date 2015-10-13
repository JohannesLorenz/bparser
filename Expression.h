/*
 * Expression.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <cstddef>

/**
 * @brief The operation type
 */
typedef enum tag_op_type
{
    e_value,
    e_multiply,
    e_plus
} op_type;

/**
 * @brief The expression structure
 */
class node_t
{
public:
//	op_type type; ///< type of operation
	
	//int value; ///< valid only when type is e_value
/*	node_t *first;
	node_t *second;
	node_t *third;*/
	
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

struct expression_t : public node_t
{
	node_t *first;
	node_t *second;
	node_t *third;
	op_type type;
public:
	expression_t(op_type type, node_t *n1, node_t* n2 = NULL, node_t* n3 = NULL) :
		first(n1), second(n2), third(n3) {}
	virtual void accept(class visitor_t& v);
};

// TODO: new .h file
class visitor_t
{
public:
	virtual void visit(number_t *e) = 0; // TODO: use references?
	virtual void visit(expression_t *e) = 0;
	virtual ~visitor_t() {}
};

class dumper_t : public visitor_t
{
	std::size_t depth;
	void handle_depth();
public:
	dumper_t() : depth(0) {};
	void visit(number_t *e);
	void visit(expression_t *e);
};

struct cleaner_t : visitor_t
{
	void visit(number_t *e) { delete e; }
	void visit(expression_t *e);
};

#if 0
/**
 * @brief It creates an identifier
 * @param value The number value
 * @return The expression or NULL in case of no memory
 */
node_t *createNumber(int value);

/**
 * @brief It creates an operation
 * @param type The operation type
 * @param left The left operand
 * @param right The right operand
 * @return The expression or NULL in case of no memory
 */
node_t *createOperation(op_type type, node_t *left, node_t *right);
#endif

#if 0
/**
 * @brief Deletes a expression
 * @param b The expression
 */
void deleteExpression(node_t *b);

void dumpExpression(node_t *b, unsigned depth);
#endif
node_t* echo(node_t* e);

#endif // __EXPRESSION_H__
