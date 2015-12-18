#include "utils.h"
#include "node.h"

void function_called::visit(primary_expression_t& p)
{ //TODO: if-else
	identifier_t* id = p.c.get<1>();
	if(id)
	{
		set_id(struct_rval_of_func(*id));
		/*if(is_func_id(*id))
		 {}
		else
{}*/
	}
	else if(p.c.get<3>())
	 p.c.get<3>()->accept(*this);
}

void function_called::visit(cast_expression_t& c){
	tvisit(c.c.get<3>());
}