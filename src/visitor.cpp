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

#include <cassert>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include "node.h"
#include "visitor.h"

extern std::vector<std::string>& get_files();

void fwd::visit(type_name_t &t)
{
	visit_all(t.c);
}

void fwd::visit(specifier_qualifier_list_t &s)
{
	vaccept(s.c);
}

void fwd::visit(struct_or_union_specifier_t &s)
{
	visit_all(s.c);
}

void fwd::visit(struct_declaration_list_t& n)
{
	vvisit(n.c);
}

void fwd::visit(struct_declaration_t& n)
{
	visit_all(n.c);
}

void fwd::visit(struct_declarator_list_t& n)
{
	visit_all(n.c);
}

void fwd::visit(struct_declarator_t& n)
{
	accept_all(n.c);
}

void fwd::visit(enum_specifier_t& n)
{
	visit_all(n.c);
}

void fwd::visit(enumerator_list_t& n)
{
	visit_all(n.c);
}

void fwd::visit(enumerator_t& n)
{
	accept_all(n.c);
}

void fwd::visit(parameter_type_list_t& n)
{
	visit_all(n.c);
}

void fwd::visit(parameter_list_t& n)
{
	visit_all(n.c);
}

void fwd::visit(parameter_declaration_t& n)
{
	visit_all(n.c);
}

void fwd::visit(identifier_list_t& n)
{
	visit_all(n.c);
}

void fwd::visit(primary_expression_t& p) {
	accept_all(p.c);
}

/*void fwd::visit(primary_identifier_t& n)
{
	visit_all(n.c);
}

void fwd::visit(primary_expression_expression_t& n)
{
	accept_all(n.c);
}*/

void fwd::visit(array_access_expression_t &e)
{
	accept_all(e.c);
}

void fwd::visit(argument_expression_list_t &e)
{
	accept_all(e.c);
}

void fwd::visit(function_call_expression_t &e)
{
	accept_all(e.c);
}

void fwd::visit(struct_access_expression_t &e)
{
	accept_all(e.c);
}

void fwd::visit(compound_literal_t &e)
{
	accept_all(e.c);
}

void fwd::visit(cast_expression_t &e)
{
	accept_all(e.c);
}

/*void fwd::visit(type_specifier_simple_t& e)
{
	incr_depth_t x(&depth, stream, e.span);
	stream << "simple type specifier, type: " << e.id << std::endl;
}

void fwd::visit(number_t& e)
{
	incr_depth_t x(&depth, stream, e.span);
	stream << "number, value: " << e.value << std::endl;
}*/

void fwd::visit(token_t& ) {}

void fwd::visit(unary_expression_l &e)
{
	accept_all(e.c);
}

void fwd::visit(unary_expression_r &e)
{
	accept_all(e.c);
}

void fwd::visit(binary_expression_t &e)
{
	accept_all(e.c);
}

void fwd::visit(ternary_expression_t &e)
{
/*	on(e);
	e.n1.accept(*this);
	on(e, 1);
	if(e.n2) { e.n2.accept(*this); on(e, 2); }
	if(e.n3) { e.n3.accept(*this); on(e, 3); }*/

	accept_all(e.c);
}

void fwd::visit(expression_statement_t &e)
{
	accept_all(e.c);
}

void fwd::visit(selection_statement_t &e)
{
	accept_all(e.c);
}

void fwd::visit(labeled_statement_t &s)
{
	accept_all(s.c);
}

void fwd::visit(jump_statement_t &s)
{
	accept_all(s.c);
}

void fwd::visit(storage_class_specifier_t& ) { /* FEATURE: remove? */ }

void fwd::visit(iteration_statement_t& i) { accept_all(i.c); }

/*void fwd::visit(primary_expression_t& p)
{
	accept_all(p.c);
}*/

void fwd::visit(attr_list_t& n)
{
	visit_all(n.c);
}

void fwd::visit(attributes_t& n)
{
	visit_all(n.c);
}

void fwd::visit(sizeof_expression_t& n)
{
	accept_all(n.c);
}

void fwd::visit(type_specifier_t &t) { accept_all(t.c); }
//void fwd::visit(type_identifier &t) { visit_all(t.c); }
void fwd::visit(type_qualifier_t &t) { visit_all(t.c); }

void fwd::visit(type_qualifier_list_t &n)
{
	vvisit(n.c);
}
void fwd::visit(function_specifier_t& ) { // FEATURE: remove?
}
void fwd::visit(alignment_specifier_t& ) { // FEATURE: remove?
}
void fwd::visit(declaration_list_t& d) { vvisit(d.c); }
void fwd::visit(compound_statement_t& n) {
		accept_all(n.c);
		/*tvisit(n.c.value);
		vaccept(*n.c.get_next().value);
		tvisit(n.c.get_next().get_next().value);*/
		}

void fwd::visit(pointer_t& p) { visit_all(p.c); }

void fwd::visit(declarator_t& n) { accept_all(n.c); }

void fwd::visit(declaration_specifiers_t& n)
{
	vaccept(n.c);
}
void fwd::visit(function_definition_t& n)
{
	visit_all(n.c);
}
void fwd::visit(external_declaration_t& n)
{
	visit_all(n.c);
}

void fwd::visit(translation_unit_t& n)
{
	vvisit(n.c);
}

void fwd::visit(declaration_t& n) { visit_all(n.c); }

/*void fwd::visit(constant_t& n)
{
	visit_all(n.c);
}*/

void fwd::visit(init_declarator_t &i)
{
	visit_all(i.c);
}

void fwd::visit(init_declarator_list_t &i)
{
	visit_all(i.c);
}

void fwd::visit(initializer_t &i)
{
	accept_all(i.c);
}

void fwd::visit(initializer_list_t &i)
{ visit_all(i.c);
}


void fwd::visit(designator_list_t& d) {
	vaccept(d.c);
}

void fwd::visit(designator_id& d) {
	visit_all(d.c);
}

void fwd::visit(designator_constant_expr& d) { accept_all(d.c); }

void fwd::visit(abstract_declarator_t &d)
{
	accept_all(d.c);
}



void fwd::visit(direct_declarator_id &d)
{
	visit_all(d.c);
}

void fwd::visit(direct_declarator_decl &d)
{
	visit_all(d.c);
}

void fwd::visit(direct_declarator_arr &d)
{
	accept_all(d.c);
}

void fwd::visit(direct_declarator_func &d)
{
	accept_all(d.c);
}

void fwd::visit(direct_declarator_idlist& d)
{
	accept_all(d.c);
}

void fwd::visit(direct_abstract_declarator_decl &d)
{
	visit_all(d.c);
}

void fwd::visit(direct_abstract_declarator_arr &d)
{
	accept_all(d.c);
}

void fwd::visit(direct_abstract_declarator_func &d)
{
	accept_all(d.c);
}





















class incr_depth_t
{
	std::size_t* depth;
	std::ostream& stream;
	const span_t& span;
public:
	incr_depth_t(std::size_t* _depth, std::ostream& stream, const span_t& span) :
		depth(_depth),
		stream(stream),
		span(span)
	{
		char tmp[64];
		const char* fname = get_files().at(span.first.file_id).c_str();
		const char* last_slash = strrchr(fname, '/');
		snprintf(tmp, 64, "%32s (l%4d c%4d) (l%4d c%4d) ",
			last_slash ? 1+last_slash : fname,
			span.first.line,
			span.first.col, span.second.line, span.second.col);
		stream << tmp;
		for(std::size_t i = 0; i < *depth; ++i)
		 stream << "  ";
		++*depth;
	}
	~incr_depth_t() { --*depth;
		//stream << " (" << span << ')';
	}
};







void dumper_t::visit(type_name_t &t)
{
	incr_depth_t x(&depth, stream, t.span);
	stream << "type name" << std::endl;
	fwd::visit(t);
}

void dumper_t::visit(specifier_qualifier_list_t &s)
{
	if(s.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, s.span);
		stream << "specifier-qualifier-list" << std::endl;
		fwd::visit(s);
	} else fwd::visit(s);
}

void dumper_t::visit(struct_or_union_specifier_t &s)
{
	incr_depth_t x(&depth, stream, s.span);
	stream << (s.is_union_type
		? "union specifier"
		: "struct specifier")
		<< std::endl;
	fwd::visit(s);
}

void dumper_t::visit(struct_declaration_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	stream << "struct declaration" << std::endl; // FEATURE: good name?
	fwd::visit(n);
}

void dumper_t::visit(struct_declaration_list_t& n)
{
	if(n.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "struct declaration list" << std::endl; // FEATURE: good name?
		fwd::visit(n);
	} else fwd::visit(n);
}

void dumper_t::visit(struct_declarator_list_t& n)
{
	if(n.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "struct declarator list" << std::endl; // FEATURE: endl autom?
		fwd::visit(n);
	}
	else fwd::visit(n);
}

void dumper_t::visit(struct_declarator_t& n)
{
	if(n.c.get<2>())
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "struct declarator" << std::endl;
		fwd::visit(n);
	} else fwd::visit(n);
}

void dumper_t::visit(enum_specifier_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	stream << "enum specifier" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(enumerator_list_t& n)
{
	if(n.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "enumerator list" << std::endl;
		fwd::visit(n);
	}
	else fwd::visit(n);
}

void dumper_t::visit(enumerator_t& n)
{
	if(n.c.get<2>())
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "enumerator" << std::endl;
		fwd::visit(n);
	} else fwd::visit(n);
}

void dumper_t::visit(parameter_type_list_t& n)
{
	if(n.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "parameter type list" << std::endl; // FEATURE: better description?
		fwd::visit(n);
	} else fwd::visit(n);
}

void dumper_t::visit(parameter_list_t& n)
{
	if(n.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "parameter list" << std::endl;
		fwd::visit(n);
	} else fwd::visit(n);
}

void dumper_t::visit(parameter_declaration_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	stream << "parameter declaration" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(identifier_list_t& n)
{
	if(n.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "identifier list" << std::endl;
		fwd::visit(n);
	} else fwd::visit(n);
}

void dumper_t::visit(primary_expression_t& p) {
	incr_depth_t x(&depth, stream, p.span);
	stream << "primary expression" << std::endl;
	fwd::visit(p);
}

#if 0
void dumper_t::visit(constant_t<int>& c)
{
	incr_depth_t x(&depth, stream, c.span);
	stream << "int constant: " << c.value << std::endl;
}
#endif

void dumper_t::visit(iconstant_t& c)
{
	incr_depth_t x(&depth, stream, c.span);
	stream << "int constant: ";
	if(c.number_system == iconstant_t::character) stream << (char)c.value;
	else stream << c.value;
	stream << " (type: " << c.scanf_modifier();
	if(c.suf_type != iconstant_t::no_suffix)
	 stream << ", suffix: " << c.suffix();
	stream << ")" << std::endl;
}

void dumper_t::visit(fconstant_t& c) {
	incr_depth_t x(&depth, stream, c.span);
	stream << "float constant: " << c.raw << std::endl; // FEATURE...
}

#if 0
void dumper_t::visit(constant_t<float>& c)
{
	incr_depth_t x(&depth, stream, c.span);
	stream << "float constant: " << c.value << std::endl;
}

void dumper_t::visit(constant_t<std::string>& c)
{
	incr_depth_t x(&depth, stream, c.span);
	stream << "string constant: " << c.value << std::endl;
}
#endif

/*void dumper_t::visit(primary_identifier_t& p)
{
	incr_depth_t x(&depth, stream, p.span);
	stream << "primary expression" << std::endl;
	fwd::visit(p);
}

void dumper_t::visit(primary_expression_expression_t& p)
{
	incr_depth_t x(&depth, stream, p.span);
	stream << "primary expression" << std::endl;
	fwd::visit(p);
}*/

void dumper_t::visit(array_access_expression_t &e)
{
	incr_depth_t x(&depth, stream, e.span);
	stream << "array access op" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(argument_expression_list_t &e)
{
	if(e.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, e.span);
		stream << "argument expression list" << std::endl;
		fwd::visit(e);
	} else fwd::visit(e);
}

void dumper_t::visit(function_call_expression_t &e)
{
	incr_depth_t x(&depth, stream, e.span);
	stream << "function call" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(struct_access_expression_t &e)
{
	incr_depth_t x(&depth, stream, e.span);
	stream << "struct access" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(compound_literal_t &e)
{
	incr_depth_t x(&depth, stream, e.span);
	stream << "compound literal" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(cast_expression_t &e)
{
	incr_depth_t x(&depth, stream, e.span);
	stream << "cast" << std::endl;
	fwd::visit(e);
}

/*void dumper_t::visit(type_specifier_simple_t& e)
{
	incr_depth_t x(&depth, stream, e.span);
	stream << "simple type specifier, type: " << e.id << std::endl;
}

void dumper_t::visit(number_t& e)
{
	incr_depth_t x(&depth, stream, e.span);
	stream << "number, value: " << e.value << std::endl;
}*/

void dumper_t::visit(token_t& e)
{
	incr_depth_t x(&depth, stream, e.span);

	//stream << "token: ";

	stream << e << std::endl;
}

void dumper_t::visit(unary_expression_l &e)
{
	// FEATURE: full op names
	incr_depth_t x(&depth, stream, e.span);
	stream << "unary_expression (prefix)" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(unary_expression_r &e)
{
	incr_depth_t x(&depth, stream, e.span);
	stream << "unary_expression (postfix)" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(binary_expression_t &e)
{
	incr_depth_t x(&depth, stream, e.span);
	stream << "binary expression " << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(ternary_expression_t &e)
{
/*	on(e);
	e.n1.accept(*);
	on(e, 1);
	if(e.n2) { e.n2.accept(*); on(e, 2); }
	if(e.n3) { e.n3.accept(*); on(e, 3); }*/

	incr_depth_t x(&depth, stream, e.span);
	stream << "ternary expression" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(expression_statement_t &e)
{
	incr_depth_t x(&depth, stream, e.span);
	stream << "expression statement" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(selection_statement_t &e)
{
	incr_depth_t x(&depth, stream, e.span);
	stream << "selection statement" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(labeled_statement_t &s)
{
	incr_depth_t x(&depth, stream, s.span);
	stream << "labeled statement (type ";
	switch(s.type)
	{
		case labeled_statement_t::case_label: stream << "case label"; break; // FEATURE: type.string func in node.h
		case labeled_statement_t::default_label: stream << "default label"; break;
		case labeled_statement_t::jump_label: stream << "jump label"; break;
	}
	stream << ")" << std::endl;
	fwd::visit(s);
}

void dumper_t::visit(jump_statement_t &s)
{
	incr_depth_t x(&depth, stream, s.span);
	stream << "jump statement (type ";
	switch(s.type)
	{
		case jump_statement_t::goto_type: stream << "goto"; break; // FEATURE: type.string func in node.h
		case jump_statement_t::continue_type: stream << "continue"; break;
		case jump_statement_t::break_type: stream << "break"; break;
		case jump_statement_t::return_void: stream << "return"; break;
		case jump_statement_t::return_type: stream << "return (with type)"; break;
	}
	stream << ")" << std::endl;
	fwd::visit(s);
}

void dumper_t::visit(storage_class_specifier_t& )
{
}

void dumper_t::visit(iteration_statement_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	const char* loop_type =
		n.type == iteration_statement_t::for_type
			? "for"
			: (n.type == iteration_statement_t::do_type
				? "do-while"
				: "while");
	stream << "iteration statement (type " << loop_type << ")" << std::endl;
	fwd::visit(n);
}

/*void dumper_t::visit(primary_expression_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	stream << "primary expression";
	switch(n.type)
	{
		case pt_expression:
			stream << std::endl;
			break;
		case pt_constant:
			stream << std::endl;
			break;
		case pt_id:
			stream << ": identifier: " << n.c.value.name << std::endl;
			break;
		case pt_string:
			stream << ": string constant: " << n.c.get_next().value.name << std::endl;
			break;
	}

	fwd::visit(n);
}*/

void dumper_t::visit(sizeof_expression_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	stream << "sizeof ..." << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(attr_name_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	stream << "attribute name: " << n.raw << std::endl;
}

void dumper_t::visit(attr_list_t& n)
{
	if(n.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "attribute list" << std::endl;
		fwd::visit(n);
	}
	else fwd::visit(n);	
}

void dumper_t::visit(attributes_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	stream << "attribute" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(identifier_t &n)
{
	incr_depth_t x(&depth, stream, n.span);
	if(!n._definition)
	{
		stream << "identifier (definition MISSING)" << std::endl;
	} // should not happen
	else
	{
	if(n._definition == &n)
		stream << "identifier (definition): " << n.raw << std::endl;
	else
		stream << "identifier: " << n.raw
			<< " (definition: " << n._definition->span
			<< ")" << std::endl;
	}
}

void dumper_t::visit(enumeration_constant_t& n)
{
	if(!n._definition)
	{
		stream << "enumeration constant (definition MISSING)" << std::endl;
	} // should not happen
	else
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "enumeration constant: " << n.raw
			<< " (definition: " << n._definition->span
			<< ")" << std::endl;
	}
	fwd::visit(n);
}

void dumper_t::visit(typedef_name_t& n)
{
	if(!n._definition)
	{
		stream << "typedef name (definition MISSING)" << std::endl;
	} // should not happen
	else
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "typedef name: " << n.raw
			<< " (definition: " << n._definition->span
			<< ")" << std::endl;
	}
	fwd::visit(n);
}

void dumper_t::visit(string_literal_t &s)
{
	incr_depth_t x(&depth, stream, s.span);
	stream << "string literal: " << s.raw << std::endl;
	fwd::visit(s);
}

void dumper_t::visit(comment_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	stream << "comment: " << n.raw << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(func_name_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	stream << "__FUNCTION__" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(type_specifier_t &t)
{
	incr_depth_t x(&depth, stream, t.span);
	stream << "type specifier" << std::endl;
	fwd::visit(t);
}

/*void dumper_t::visit(type_identifier &t)
{
	incr_depth_t x(&depth, stream, t.span);
	stream << "type identifier: " << t.c.value.raw << std::endl;
}*/

void dumper_t::visit(type_qualifier_t& n) {
	incr_depth_t x(&depth, stream, n.span);
	stream << "type qualifier" << std::endl;
	fwd::visit(n);
}
void dumper_t::visit(function_specifier_t& ) {
#if 0
	incr_depth_t x(&depth, stream, n.span);
	stream << "function specifier" << std::endl;
	fwd::visit(n);
#endif
}
void dumper_t::visit(alignment_specifier_t& ) {
#if 0
	incr_depth_t x(&depth, stream, n.span);
	stream << "alignment specifier" << std::endl;
	fwd::visit(n);
#endif
}
void dumper_t::visit(declaration_list_t& n) {
	if(n.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "declaration list" << std::endl;
		fwd::visit(n);
	} else fwd::visit(n);
}
void dumper_t::visit(compound_statement_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	stream << "compound statement" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(pointer_t& n) {
	incr_depth_t x(&depth, stream, n.span);
	stream << "pointer" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(declarator_t& n)
{
	if(n.c.get<0>()) // pointer prepended?
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "declarator" << std::endl;
		fwd::visit(n);
	}
	else // no pointer, just a direct_declarator
	 fwd::visit(n);
}

void dumper_t::visit(declaration_specifiers_t& n)
{
	if(n.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "declaration specifiers" << std::endl;
		fwd::visit(n);
	}
	else
		fwd::visit(n);
}
void dumper_t::visit(function_definition_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	stream << "function definition" << std::endl;
	fwd::visit(n);
}
void dumper_t::visit(external_declaration_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	stream << "external declaration" << std::endl;

	fwd::visit(n);
}

void dumper_t::visit(translation_unit_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	stream << "translation unit" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(declaration_t& n)
{	
	incr_depth_t x(&depth, stream, n.span);
	stream << "declaration" << std::endl;
	fwd::visit(n);
}

/*void dumper_t::visit(constant_t& n)
{
	incr_depth_t x(&depth, stream, n.span);
	switch(n.type)
	{
		case ct_int:
			stream << "int constant: " << n.value.i << std::endl;
			break;
		case ct_float:
			stream << "float constant: " << n.value.f << std::endl;
			break;
		case ct_enum:
			stream << "enumeration constant" << std::endl;
			break;
	}

	fwd::visit(n);
}*/

void dumper_t::visit(init_declarator_t &i)
{
	if(i.c.get<2>())
	{
		incr_depth_t x(&depth, stream, i.span);
		stream << "init declarator" << std::endl;
		fwd::visit(i);
	}
	else fwd::visit(i);
}

void dumper_t::visit(init_declarator_list_t &i)
{
	if(i.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, i.span);
		stream << "init declarator list" << std::endl;
		fwd::visit(i);
	} else fwd::visit(i);
}

void dumper_t::visit(initializer_t &i)
{
	incr_depth_t x(&depth, stream, i.span);
	stream << "initializer" << std::endl;
	fwd::visit(i);
}

void dumper_t::visit(initializer_list_t &i)
{
	if(i.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, i.span);
		stream << "initializer list" << std::endl;
		fwd::visit(i);
	} else fwd::visit(i);
}


void dumper_t::visit(designator_list_t& d) {
	if(d.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, d.span);
		stream << "designator list" << std::endl;
		fwd::visit(d);
	} else fwd::visit(d);
}

void dumper_t::visit(designator_id& d) {
	incr_depth_t x(&depth, stream, d.span);
	//stream << "designator: ." << d.c.get_next().value << std::endl;
	stream << "designator (struct)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(designator_constant_expr& d) { // FEATURE: rename designator_array?
	incr_depth_t x(&depth, stream, d.span);
	//stream << "designator: [" << d.c.get_next().value << ']' << std::endl;
	stream << "designator (array)" << std::endl;
	fwd::visit(d);
}



void dumper_t::visit(abstract_declarator_t &n)
{
	if(n.c.get<0>()) // pointer prepended?
	{
		incr_depth_t x(&depth, stream, n.span);
		stream << "abstract declarator" << std::endl;
		fwd::visit(n);
	}
	else // no pointer, just a direct_abstract_declarator
	 fwd::visit(n);
}



void dumper_t::visit(direct_declarator_id &d)
{
	incr_depth_t x(&depth, stream, d.span);
	stream << "direct declarator (identifier)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_declarator_decl &d)
{
	incr_depth_t x(&depth, stream, d.span);
	stream << "direct declarator (declaration)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_declarator_arr &d)
{
	incr_depth_t x(&depth, stream, d.span);
	stream << "direct declarator (array)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_declarator_func &d)
{
	incr_depth_t x(&depth, stream, d.span);
	stream << "direct declarator (function)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_declarator_idlist &d)
{
	if(d.c.size() > 1)
	{
		incr_depth_t x(&depth, stream, d.span);
		stream << "direct declarator (identifier list)" << std::endl;
		fwd::visit(d);
	} else fwd::visit(d);
}


void dumper_t::visit(direct_abstract_declarator_decl &d)
{
	incr_depth_t x(&depth, stream, d.span);
	stream << "direct abstract declarator (declaration)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_abstract_declarator_arr &d)
{
	incr_depth_t x(&depth, stream, d.span);
	stream << "direct abstract declarator (array)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_abstract_declarator_func &d)
{
	incr_depth_t x(&depth, stream, d.span);
	stream << "direct abstract declarator (function)" << std::endl;
	fwd::visit(d);
}


void cleaner_t::visit(binary_expression_t& e)
{
	//e.accept_children(*this);
	accept_all(e.c);
	delete &e;
}

/*
node_t& echo(node_t& e)
{
	printf("echo:\n");
	dumper_t dumper;
	e.accept(dumper);
	return e;
}
*/

std::ostream& operator<<(std::ostream& o, node_base& n)
{
	dumper_t d(o);
	n.accept(d);
	return o;
}

