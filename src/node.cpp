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

#include <iostream>
#include <iomanip>
#include <vector>

#include "token.h"
#include "node.h"
#include "visitor.h"
#include "parser.h"

extern std::vector<std::string>& get_files();
extern void clear_lookup_table();
extern std::vector<terminal_t*>& get_token_vector();
extern void reset_pos_counter();
extern void reset_variables();

void init_parser()
{
	get_token_vector().clear();

	reset_pos_counter();

	// init default file
	get_files().clear();
	get_files().push_back("default.c");

	clear_lookup_table();

	reset_variables();
}

//template<>
//void type_specifier_simple_t::accept(visitor_t& v) { v.visit(this); }

identifier_t::identifier_t(const char* name, geom_t geom) :
	noconst_1line_terminal_t(geom, t_identifier, name) {}

typedef_name_t::typedef_name_t(const char* name, geom_t geom) :
	noconst_1line_terminal_t(geom, t_identifier, name) {}

enumeration_constant_t::enumeration_constant_t(const char* name, geom_t geom) :
	noconst_1line_terminal_t(geom, t_identifier, name) {}

std::size_t token_t::length() const
{
	return (value() <= std::numeric_limits<signed char>::max())
		? ((value() == '\n') ? 0 : 1)
		: token_length((token_id)value());
}

std::size_t token_t::newlines() const
{
	return (std::size_t)(value() == '\n');
}

std::size_t string_literal_t::length() const { return _length; }
std::size_t string_literal_t::newlines() const { return _newlines; }

const char* next_0_or_n(const char* p)
{
	for(; *p && (*p != '\n'); ++p) ;
	return p;
}

string_literal_t::string_literal_t(const char* value, geom_t geom) :
	noconst_terminal_t(geom, t_string_literal, value),
	_newlines(0)
{
	const char *p = value, *last_linestart = value;
	for(; *p; p = next_0_or_n(p), ++_newlines)
	{
		last_linestart = ++p;
	}
	--_newlines; // \0 has been counted as newline
	_length = p - last_linestart;
}

std::size_t noconst_1line_terminal_t::length() const { return raw.length(); }
std::size_t noconst_1line_terminal_t::newlines() const { return 0; }

void token_t::accept(visitor_t& v) { v.visit(this); }
//void number_t::accept(visitor_t& v) { v.visit(this); }
//void expression_t::accept(visitor_t& v) { v.visit(this); }
void unary_expression_l::accept(visitor_t& v) { v.visit(this); }
void unary_expression_r::accept(visitor_t& v) { v.visit(this); }
void binary_expression_t::accept(visitor_t& v) { v.visit(this); }
void ternary_expression_t::accept(visitor_t& v) { v.visit(this); }

#if 0
void storage_class_specifier_t::accept(visitor_t& v) { v.visit(this); }
#endif
//void type_specifier_t::accept(visitor_t& v) { v.visit(this); }
void type_qualifier_t::accept(visitor_t& v) { v.visit(this); }
//void function_specifier_t::accept(visitor_t& v) { v.visit(this); }
//void alignment_specifier_t::accept(visitor_t& v) { v.visit(this); }
void declaration_list_t::accept(visitor_t& v) { v.visit(this); }
void compound_statement_t::accept(visitor_t& v) { v.visit(this); }
void pointer_t::accept(visitor_t& v) { v.visit(this); }
void direct_declarator_t::accept(visitor_t& v) { v.visit(this); }
void declarator_t::accept(visitor_t& v) { v.visit(this); }
void declaration_specifiers_t::accept(visitor_t& v) { v.visit(this); }
void function_definition_t::accept(visitor_t& v) { v.visit(this); }
void external_declaration_t::accept(visitor_t& v) { v.visit(this); }
void translation_unit_t::accept(visitor_t& v) { v.visit(this); }
void declaration_t::accept(visitor_t& v) { v.visit(this); }
void type_qualifier_list_t::accept(visitor_t& v) { v.visit(this); }
void primary_expression_t::accept(visitor_t &v) { v.visit(this); }
#if 0
template<>
void constant_t<int>::accept(visitor_t& v) { v.visit(this); }
template<>
void constant_t<float>::accept(visitor_t& v) { v.visit(this); }
template<>
void constant_t<std::string>::accept(visitor_t& v) { v.visit(this); }
#endif
//void primary_identifier_t::accept(visitor_t& v) { v.visit(this); }
//void primary_expression_expression_t::accept(visitor_t& v) { v.visit(this); }
//void constant_t::accept(visitor_t& v) { v.visit(this); }
void labeled_statement_t::accept(visitor_t& v) { v.visit(this); }
void expression_statement_t::accept(visitor_t& v) { v.visit(this); }
void selection_statement_t::accept(visitor_t& v) { v.visit(this); }
void iteration_statement_t::accept(visitor_t& v) { v.visit(this); }
void jump_statement_t::accept(visitor_t& v) { v.visit(this); }
//void block_item_list_t::accept(visitor_t& v) { v.visit(this); }
void type_specifier_t::accept(visitor_t& v) { v.visit(this); }
void sizeof_expression_t::accept(visitor_t& v) { v.visit(this); }

/*void expression_t::accept_children(visitor_t& v)
{
	n1->accept(v);
	if(n2) n2->accept(v);
	if(n3) n3->accept(v);
}*/


void block_item_t::accept(visitor_t &v) { v.visit(this); }
void identifier_t::accept(visitor_t &v) { v.visit(this); }
void enumeration_constant_t::accept(visitor_t &v) { v.visit(this); }
void typedef_name_t::accept(visitor_t &v) { v.visit(this); }
void string_literal_t::accept(visitor_t &v) { v.visit(this); }

void init_declarator_t::accept(visitor_t &v) { v.visit(this); }
void init_declarator_list_t::accept(visitor_t &v) { v.visit(this); }
void initializer_list_t::accept(visitor_t &v) { v.visit(this); }
void initializer_t::accept(visitor_t &v) { v.visit(this); }

void direct_declarator_id::accept(visitor_t &v) { v.visit(this); }
void direct_declarator_decl::accept(visitor_t &v) { v.visit(this); }
void direct_declarator_arr::accept(visitor_t &v) { v.visit(this); }
void direct_declarator_func::accept(visitor_t &v) { v.visit(this); }
void direct_declarator_idlist::accept(visitor_t &v) { v.visit(this); }

void direct_abstract_declarator_t::accept(visitor_t &v) { v.visit(this); }
void direct_abstract_declarator_decl::accept(visitor_t &v) { v.visit(this); }
void direct_abstract_declarator_arr::accept(visitor_t &v) { v.visit(this); }


void designator_id::accept(visitor_t &v) { v.visit(this); }
void designator_constant_expr::accept(visitor_t &v) { v.visit(this); }
void designator_list_t::accept(visitor_t &v) { v.visit(this); }
void direct_abstract_declarator_func::accept(visitor_t &v) { v.visit(this); }



void array_access_expression_t::accept(visitor_t &v) { v.visit(this); }
void argument_expression_list_t::accept(visitor_t &v) { v.visit(this); }
void function_call_expression_t::accept(visitor_t &v) { v.visit(this); }
void struct_access_expression_t::accept(visitor_t &v) { v.visit(this); }
void compound_literal_t::accept(visitor_t &v) { v.visit(this); }
void cast_expression_t::accept(visitor_t &v) { v.visit(this); }


void type_name_t::accept(visitor_t &v) { v.visit(this); }
//void type_specifier_token::accept(visitor_t &v) { v.visit(this); }
//void type_identifier::accept(visitor_t &v) { v.visit(this); }
void abstract_declarator_t::accept(visitor_t &v) { v.visit(this); }
void specifier_qualifier_list_t::accept(visitor_t &v) { v.visit(this); }

void struct_or_union_specifier_t::accept(visitor_t &v) { v.visit(this); }
void struct_declaration_list_t::accept(visitor_t &v) { v.visit(this); }
void struct_declaration_t::accept(visitor_t& v) { v.visit(this); }
void struct_declarator_list_t::accept(visitor_t &v) { v.visit(this); }
void struct_declarator_t::accept(visitor_t &v) { v.visit(this); }
void enum_specifier_t::accept(visitor_t &v) { v.visit(this); }
void enumerator_list_t::accept(visitor_t &v) { v.visit(this); }
void enumerator_t::accept(visitor_t &v) { v.visit(this); }
void parameter_type_list_t::accept(visitor_t &v) { v.visit(this); }
void parameter_list_t::accept(visitor_t &v) { v.visit(this); }
void parameter_declaration_t::accept(visitor_t &v) { v.visit(this); }
void identifier_list_t::accept(visitor_t &v) { v.visit(this); }



void iconstant_t::accept(visitor_t &v) { v.visit(this); }
void fconstant_t::accept(visitor_t &v) { v.visit(this); }

