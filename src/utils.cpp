/*************************************************************************/
/* bparser - a bison-based, C99 parser                                   */
/* Copyright (C) 2015-2015                                               */
/* Johannes Lorenz (jlsf2013 @ sourceforge)                              */
/*                                                                       */
/* This program is free software; you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation; either version 3 of the License, or (at */
/* your option) any later version.                                       */
/* This program is distributed in the hope that it will be useful, but   */
/* WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      */
/* General Public License for more details.                              */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program; if not, write to the Free Software           */
/* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA  */
/*************************************************************************/

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
