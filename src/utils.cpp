#include "utils.h"
#include "node.h"

void function_called::visit(primary_expression_t& p)
{
	identifier_t* id = p.c.get<1>();
	if(id)
	{
		if(is_func_id(*id))
		 _identifier = id;
	}
	else if(p.c.get<3>())
	 p.c.get<3>()->accept(*this);
}

void function_called::visit(cast_expression_t& c){
	c.c.get<3>()->accept(*this);
}


void struct_type_specifier_of_declaration_t::visit(typedef_name_t &t)
{
	func_visitor< declaration_from_declarator_t > v0;
	t._definition->parent->accept(v0);
	return visit(*v0.functor().declaration_found);
}
