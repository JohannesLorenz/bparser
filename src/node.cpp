/*************************************************************************/
/* bparser - a bison-based, C99 parser                                   */
/* Copyright (C) 2015-2016                                               */
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

void init_parser(const char* fname)
{
	get_token_vector().clear();

	reset_pos_counter();

	// init default file
	get_files().clear();
	get_files().push_back(fname ? fname : "default.c");

	clear_lookup_table();

	reset_variables();
}

//template<>
//void type_specifier_simple_t::accept(visitor_t& v) { v.vis(this); }

namespace nodes {

identifier_t::identifier_t(const char* name, geom_t geom) :
	defined_t(geom, t_identifier, name) {}

typedef_name_t::typedef_name_t(const char* name, geom_t geom) :
	defined_t(geom, t_identifier, name) {}

enumeration_constant_t::enumeration_constant_t(const char* name, geom_t geom) :
	defined_t(geom, t_identifier, name) {}

attr_name_t::attr_name_t(const char* name, geom_t geom) :
	noconst_1line_terminal_t(geom, t_attr_name, name) {}

std::size_t token_t::length() const
{
	return (value() <= std::numeric_limits<signed char>::max())
		? ((value() == '\n') ? 0 : 1)
		: token_length((token_id)value());
}

std::size_t token_t::tablength() const
{
	return (value() == '\t') ? 8 : 0;
}

std::size_t token_t::newlines() const
{
	return (std::size_t)(value() == '\n');
}

std::ostream& operator<<(std::ostream& stream, const token_t& t)
{
	int value = t.value();
	bool _has_alpha = has_alpha(value);
	if(_has_alpha)
	 stream << '"';

	if(value <= 255)
		stream << (char)value;
	else
		stream << name_of(value);

	if(_has_alpha)
	 stream << '"';
	return stream;
}

std::size_t string_base_t::length() const { return _length; }
std::size_t string_base_t::newlines() const { return _newlines; }

const char* next_0_or_n(const char* p)
{
	for(; *p && (*p != '\n'); ++p) ;
	return p;
}

string_base_t::string_base_t(const char* value, geom_t geom, int tok) :
	noconst_terminal_t(geom, tok, value),
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

string_literal_t::string_literal_t(const char* value, geom_t geom) :
	string_base_t(value, geom, t_string_literal)
{
}

comment_t::comment_t(const std::string& value, geom_t geom) :
	string_base_t(value.c_str(), geom, t_comment)
{
}

func_name_t::func_name_t(const std::string& value, geom_t geom) :
	string_base_t(value.c_str(), geom, t_func_name)
{
}

std::size_t noconst_1line_terminal_t::length() const { return raw.length(); }
std::size_t noconst_1line_terminal_t::newlines() const { return 0; }

expression_t* iteration_statement_t::condition()
{
	return (type == iteration_statement_t::for_type)
		? c.get<iteration_statement_t::for_cond>()->c.get<0>()
		: c.get<iteration_statement_t::while_cond>();
}

void token_t::accept(visitor_t& v) { v.vis(this); }
//void number_t::accept(visitor_t& v) { v.vis(this); }
//void expression_t::accept(visitor_t& v) { v.vis(this); }
void unary_expression_l::accept(visitor_t& v) { v.vis(this); }
void unary_expression_r::accept(visitor_t& v) { v.vis(this); }
void binary_expression_t::accept(visitor_t& v) { v.vis(this); }
void ternary_expression_t::accept(visitor_t& v) { v.vis(this); }

#if 0
void storage_class_specifier_t::accept(visitor_t& v) { v.vis(this); }
#endif
//void type_specifier_t::accept(visitor_t& v) { v.vis(this); }
void type_qualifier_t::accept(visitor_t& v) { v.vis(this); }
//void function_specifier_t::accept(visitor_t& v) { v.vis(this); }
//void alignment_specifier_t::accept(visitor_t& v) { v.vis(this); }
void declaration_list_t::accept(visitor_t& v) { v.vis(this); }
void compound_statement_t::accept(visitor_t& v) { v.vis(this); }
void pointer_t::accept(visitor_t& v) { v.vis(this); }
void direct_declarator_t::accept(visitor_t& v) { v.vis(this); }
void declarator_t::accept(visitor_t& v) { v.vis(this); }
void declaration_specifiers_t::accept(visitor_t& v) { v.vis(this); }
void function_definition_t::accept(visitor_t& v) { v.vis(this); }
void external_declaration_t::accept(visitor_t& v) { v.vis(this); }
void translation_unit_t::accept(visitor_t& v) { v.vis(this); }
void declaration_t::accept(visitor_t& v) { v.vis(this); }
void type_qualifier_list_t::accept(visitor_t& v) { v.vis(this); }
void primary_expression_t::accept(visitor_t &v) { v.vis(this); }
#if 0
template<>
void constant_t<int>::accept(visitor_t& v) { v.vis(this); }
template<>
void constant_t<float>::accept(visitor_t& v) { v.vis(this); }
template<>
void constant_t<std::string>::accept(visitor_t& v) { v.vis(this); }
#endif
//void primary_identifier_t::accept(visitor_t& v) { v.vis(this); }
//void primary_expression_expression_t::accept(visitor_t& v) { v.vis(this); }
//void constant_t::accept(visitor_t& v) { v.vis(this); }
void labeled_statement_t::accept(visitor_t& v) { v.vis(this); }
void expression_statement_t::accept(visitor_t& v) { v.vis(this); }
void selection_statement_t::accept(visitor_t& v) { v.vis(this); }
void iteration_statement_t::accept(visitor_t& v) { v.vis(this); }
void jump_statement_t::accept(visitor_t& v) { v.vis(this); }
//void block_item_list_t::accept(visitor_t& v) { v.vis(this); }
void type_specifier_t::accept(visitor_t& v) { v.vis(this); }
void sizeof_expression_t::accept(visitor_t& v) { v.vis(this); }

/*void expression_t::accept_children(visitor_t& v)
{
	n1->accept(v);
	if(n2) n2->accept(v);
	if(n3) n3->accept(v);
}*/


void block_item_t::accept(visitor_t &v) { v.vis(this); }
void attr_name_t::accept(class visitor_t& v) { v.vis(this); }
void attribute_t::accept(class visitor_t& v) { v.vis(this); }
void identifier_t::accept(visitor_t &v) { v.vis(this); }
void enumeration_constant_t::accept(visitor_t &v) { v.vis(this); }
void typedef_name_t::accept(visitor_t &v) { v.vis(this); }
void string_literal_t::accept(visitor_t &v) { v.vis(this); }
void comment_t::accept(visitor_t &v) { v.vis(this); }
void func_name_t::accept(visitor_t &v) { v.vis(this); }

void init_declarator_t::accept(visitor_t &v) { v.vis(this); }
void init_declarator_list_t::accept(visitor_t &v) { v.vis(this); }
void initializer_list_t::accept(visitor_t &v) { v.vis(this); }
void initializer_t::accept(visitor_t &v) { v.vis(this); }

void direct_declarator_id::accept(visitor_t &v) { v.vis(this); }
void direct_declarator_decl::accept(visitor_t &v) { v.vis(this); }
void direct_declarator_arr::accept(visitor_t &v) { v.vis(this); }
void direct_declarator_func::accept(visitor_t &v) { v.vis(this); }
void direct_declarator_idlist::accept(visitor_t &v) { v.vis(this); }

void direct_abstract_declarator_t::accept(visitor_t &v) { v.vis(this); }
void direct_abstract_declarator_decl::accept(visitor_t &v) { v.vis(this); }
void direct_abstract_declarator_arr::accept(visitor_t &v) { v.vis(this); }


void designator_id::accept(visitor_t &v) { v.vis(this); }
void designator_constant_expr::accept(visitor_t &v) { v.vis(this); }
void designator_list_t::accept(visitor_t &v) { v.vis(this); }
void direct_abstract_declarator_func::accept(visitor_t &v) { v.vis(this); }



void array_access_expression_t::accept(visitor_t &v) { v.vis(this); }
void argument_expression_list_t::accept(visitor_t &v) { v.vis(this); }
void function_call_expression_t::accept(visitor_t &v) { v.vis(this); }
void struct_access_expression_t::accept(visitor_t &v) { v.vis(this); }
void compound_literal_t::accept(visitor_t &v) { v.vis(this); }
void cast_expression_t::accept(visitor_t &v) { v.vis(this); }


void type_name_t::accept(visitor_t &v) { v.vis(this); }
//void type_specifier_token::accept(visitor_t &v) { v.vis(this); }
//void type_identifier::accept(visitor_t &v) { v.vis(this); }
void abstract_declarator_t::accept(visitor_t &v) { v.vis(this); }
void specifier_qualifier_list_t::accept(visitor_t &v) { v.vis(this); }

void struct_or_union_specifier_t::accept(visitor_t &v) { v.vis(this); }
void struct_declaration_list_t::accept(visitor_t &v) { v.vis(this); }
void struct_declaration_t::accept(visitor_t& v) { v.vis(this); }
void struct_declarator_list_t::accept(visitor_t &v) { v.vis(this); }
void struct_declarator_t::accept(visitor_t &v) { v.vis(this); }
void enum_specifier_t::accept(visitor_t &v) { v.vis(this); }
void enumerator_list_t::accept(visitor_t &v) { v.vis(this); }
void enumerator_t::accept(visitor_t &v) { v.vis(this); }
void parameter_type_list_t::accept(visitor_t &v) { v.vis(this); }
void parameter_list_t::accept(visitor_t &v) { v.vis(this); }
void parameter_declaration_t::accept(visitor_t &v) { v.vis(this); }
void identifier_list_t::accept(visitor_t &v) { v.vis(this); }



void iconstant_t::accept(visitor_t &v) { v.vis(this); }
void fconstant_t::accept(visitor_t &v) { v.vis(this); }

}

