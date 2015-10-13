#include "node.h"
#include "visitor.h"

void number_t::accept(visitor_t& v) { v.visit(this); }
void expression_t::accept(visitor_t& v) { v.visit(this); }

void expression_t::accept_children(visitor_t& v)
{
	first->accept(v);
	if(second) second->accept(v);
	if(third) third->accept(v);
}
