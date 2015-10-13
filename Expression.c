/*
 * Expression.c
 * Implementation of functions used to build the syntax tree.
 */

#include "Expression.h"

#include <stdio.h>
#include <stdlib.h>

void number_t::accept(class visitor_t& v) { v.visit(this); }
void expression_t::accept(class visitor_t& v) { v.visit(this); }

/*
void dumpExpression(node_t *b, unsigned depth = 0)
{
	if(b)
{
	for(unsigned i = 0; i < depth; ++i)
	 printf("  ");
	if(b->type == e_value)
		printf("value: %d\n", b->value);
	else
		printf("type: %d\n", b->type);
	unsigned depth_1 = depth + 1;
	
	dumpExpression(b->first, depth_1);
	
	dumpExpression(b->second, depth_1);

	dumpExpression(b->third, depth_1);
}
}*/

class incr_depth_t
{
	std::size_t* depth;
public:
	incr_depth_t(std::size_t* _depth) : depth(_depth)
	{
		for(std::size_t i = 0; i < *depth; ++i)
		 printf("  ");
		++*depth;
	}
	~incr_depth_t() { --*depth; }
};

void dumper_t::visit(number_t* e)
{
	incr_depth_t x(&depth);
	printf("value: %d\n", e->value);
}

void dumper_t::visit(expression_t* e)
{
	incr_depth_t x(&depth);
	printf("expr, type: %d\n", e->type);
	e->first->accept(*this);
	if(e->second) e->second->accept(*this);
	if(e->third) e->third->accept(*this);
}

void cleaner_t::visit(expression_t* e)
{
	// TODO: code dupl
	e->first->accept(*this);
	if(e->second) e->second->accept(*this);
	if(e->third) e->third->accept(*this);
	delete e;
}

node_t* echo(node_t* e)
{
	printf("echo:\n");
	dumper_t dumper;
	e->accept(dumper);
	return e;
}
