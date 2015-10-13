#include <iostream>
#include "node.h"
#include "visitor.h"

class incr_depth_t
{
	std::size_t* depth;
public:
	incr_depth_t(std::size_t* _depth, std::ostream& stream) :
		depth(_depth)
	{
		for(std::size_t i = 0; i < *depth; ++i)
		 stream << "  ";
		++*depth;
	}
	~incr_depth_t() { --*depth; }
};

void dumper_t::visit(number_t* e)
{
	incr_depth_t x(&depth, stream);
	stream << "value: " << e->value << std::endl;
}

void dumper_t::visit(expression_t* e)
{
	incr_depth_t x(&depth, stream);
	stream << "expr, type: " << +e->type << std::endl;
	e->accept_children(*this);
}

void cleaner_t::visit(expression_t* e)
{
	e->accept_children(*this);
	delete e;
}

/*
node_t* echo(node_t* e)
{
	printf("echo:\n");
	dumper_t dumper;
	e->accept(dumper);
	return e;
}
*/
