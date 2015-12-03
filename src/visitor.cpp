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

#include <cassert>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include "node.h"
#include "visitor.h"

#include "token.h"

extern std::vector<std::string>& get_files();

void fwd::visit(type_name_t *t)
{
	visit_all(t->c);
}

void fwd::visit(specifier_qualifier_list_t *s)
{
	vaccept(s->c);
}

void fwd::visit(struct_or_union_specifier_t *s)
{
	visit_all(s->c);
}

void fwd::visit(struct_declaration_list_t* n)
{
	vvisit(n->c);
}

void fwd::visit(struct_declaration_t* n)
{
	visit_all(n->c);
}

void fwd::visit(struct_declarator_list_t* n)
{
	visit_all(n->c);
}

void fwd::visit(struct_declarator_t* n)
{
	accept_all(n->c);
}

void fwd::visit(enum_specifier_t* n)
{
	visit_all(n->c);
}

void fwd::visit(enumerator_list_t* n)
{
	visit_all(n->c);
}

void fwd::visit(enumerator_t* n)
{
	accept_all(n->c);
}

void fwd::visit(parameter_type_list_t* n)
{
	visit_all(n->c);
}

void fwd::visit(parameter_list_t* n)
{
	visit_all(n->c);
}

void fwd::visit(parameter_declaration_t* n)
{
	visit_all(n->c);
}

void fwd::visit(identifier_list_t* n)
{
	visit_all(n->c);
}

void fwd::visit(primary_expression_t* p) {
	accept_all(p->c);
}

/*void fwd::visit(primary_identifier_t* n)
{
	visit_all(n->c);
}

void fwd::visit(primary_expression_expression_t* n)
{
	accept_all(n->c);
}*/

void fwd::visit(array_access_expression_t *e)
{
	accept_all(e->c);
}

void fwd::visit(argument_expression_list_t *e)
{
	accept_all(e->c);
}

void fwd::visit(function_call_expression_t *e)
{
	accept_all(e->c);
}

void fwd::visit(struct_access_expression_t *e)
{
	accept_all(e->c);
}

void fwd::visit(compound_literal_t *e)
{
	accept_all(e->c);
}

void fwd::visit(cast_expression_t *e)
{
	accept_all(e->c);
}

/*void fwd::visit(type_specifier_simple_t* e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "simple type specifier, type: " << e->id << std::endl;
}

void fwd::visit(number_t* e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "number, value: " << e->value << std::endl;
}*/

void fwd::visit(token_t* ) {}

void fwd::visit(unary_expression_l *e)
{
	accept_all(e->c);
}

void fwd::visit(unary_expression_r *e)
{
	accept_all(e->c);
}

void fwd::visit(binary_expression_t *e)
{
	accept_all(e->c);
}

void fwd::visit(ternary_expression_t *e)
{
/*	on(e);
	e->n1->accept(*this);
	on(e, 1);
	if(e->n2) { e->n2->accept(*this); on(e, 2); }
	if(e->n3) { e->n3->accept(*this); on(e, 3); }*/

	accept_all(e->c);
}

void fwd::visit(expression_statement_t *e)
{
	accept_all(e->c);
}

void fwd::visit(storage_class_specifier_t* ) { /* FEATURE: remove? */ }

void fwd::visit(iteration_statement_t* i) { accept_all(i->c); }

/*void fwd::visit(primary_expression_t* p)
{
	accept_all(p->c);
}*/

void fwd::visit(attribute_t* n)
{
	visit_all(n->c);
}

void fwd::visit(sizeof_expression_t* n)
{
	accept_all(n->c);
}

void fwd::visit(type_specifier_t *t) { accept_all(t->c); }
//void fwd::visit(type_identifier *t) { visit_all(t->c); }
void fwd::visit(type_qualifier_t *t) { visit_all(t->c); }

void fwd::visit(type_qualifier_list_t *n)
{
	vvisit(n->c);
}
void fwd::visit(function_specifier_t* ) { // FEATURE: remove?
}
void fwd::visit(alignment_specifier_t* ) { // FEATURE: remove?
}
void fwd::visit(declaration_list_t* d) { vvisit(d->c); }
void fwd::visit(compound_statement_t* n) {
		accept_all(n->c);
		/*tvisit(n->c.value);
		vaccept(*n->c.get_next().value);
		tvisit(n->c.get_next().get_next().value);*/
		}

void fwd::visit(pointer_t* p) { visit_all(p->c); }

void fwd::visit(declarator_t* n) { accept_all(n->c); }

void fwd::visit(declaration_specifiers_t* n)
{
	vaccept(n->c);
}
void fwd::visit(function_definition_t* n)
{
	visit_all(n->c);
}
void fwd::visit(external_declaration_t* n)
{
	visit_all(n->c);
}

void fwd::visit(translation_unit_t* n)
{
	vvisit(n->c);
}

void fwd::visit(declaration_t* n) { visit_all(n->c); }

/*void fwd::visit(constant_t* n)
{
	visit_all(n->c);
}*/

void fwd::visit(init_declarator_t *i)
{
	visit_all(i->c);
}

void fwd::visit(init_declarator_list_t *i)
{
	visit_all(i->c);
}

void fwd::visit(initializer_t *i)
{
	accept_all(i->c);
}

void fwd::visit(initializer_list_t *i)
{ visit_all(i->c);
}


void fwd::visit(designator_list_t* d) {
	vaccept(d->c);
}

void fwd::visit(designator_id* d) {
	visit_all(d->c);
}

void fwd::visit(designator_constant_expr* d) { accept_all(d->c); }

void fwd::visit(abstract_declarator_t *d)
{
	accept_all(d->c);
}



void fwd::visit(direct_declarator_id *d)
{
	visit_all(d->c);
}

void fwd::visit(direct_declarator_decl *d)
{
	visit_all(d->c);
}

void fwd::visit(direct_declarator_arr *d)
{
	accept_all(d->c);
}

void fwd::visit(direct_declarator_func *d)
{
	visit_all(d->c);
}

void fwd::visit(direct_declarator_idlist* d)
{
	visit_all(d->c);
}

void fwd::visit(direct_abstract_declarator_decl *d)
{
	visit_all(d->c);
}

void fwd::visit(direct_abstract_declarator_arr *d)
{
	accept_all(d->c);
}

void fwd::visit(direct_abstract_declarator_func *d)
{
	accept_all(d->c);
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
		snprintf(tmp, 64, "%32s (l%4d c%4d) (l%4d c%4d) ",
			get_files().at(span.first.file_id).c_str(),
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







void dumper_t::visit(type_name_t *t)
{
	incr_depth_t x(&depth, stream, t->span);
	stream << "type name" << std::endl;
	fwd::visit(t);
}

void dumper_t::visit(specifier_qualifier_list_t *s)
{
	incr_depth_t x(&depth, stream, s->span);
	stream << "specifier-qualifier-list" << std::endl;
	fwd::visit(s);
}

void dumper_t::visit(struct_or_union_specifier_t *s)
{
	incr_depth_t x(&depth, stream, s->span);
	stream << (s->is_union_type
		? "union specifier"
		: "struct specifier")
		<< std::endl;
	fwd::visit(s);
}

void dumper_t::visit(struct_declaration_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "struct declaration" << std::endl; // FEATURE: good name?
	fwd::visit(n);
}

void dumper_t::visit(struct_declaration_list_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "struct declaration list" << std::endl; // FEATURE: good name?
	fwd::visit(n);
}

void dumper_t::visit(struct_declarator_list_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "struct declarator list" << std::endl; // FEATURE: endl autom?
	fwd::visit(n);
}

void dumper_t::visit(struct_declarator_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "struct declarator" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(enum_specifier_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "enum specifier" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(enumerator_list_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "enumerator list" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(enumerator_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "enumerator" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(parameter_type_list_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "parameter type list" << std::endl; // FEATURE: better description?
	fwd::visit(n);
}

void dumper_t::visit(parameter_list_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "parameter list" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(parameter_declaration_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "parameter declaration" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(identifier_list_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "identifier list" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(primary_expression_t* p) {
	incr_depth_t x(&depth, stream, p->span);
	stream << "primary expression" << std::endl;
	fwd::visit(p);
}

#if 0
void dumper_t::visit(constant_t<int>* c)
{
	incr_depth_t x(&depth, stream, c->span);
	stream << "int constant: " << c->value << std::endl;
}
#endif

void dumper_t::visit(iconstant_t* c) {
	incr_depth_t x(&depth, stream, c->span);
	stream << "int constant: " << c->raw << std::endl; // FEATURE...
}

void dumper_t::visit(fconstant_t* c) {
	incr_depth_t x(&depth, stream, c->span);
	stream << "float constant: " << c->raw << std::endl; // FEATURE...
}

#if 0
void dumper_t::visit(constant_t<float>* c)
{
	incr_depth_t x(&depth, stream, c->span);
	stream << "float constant: " << c->value << std::endl;
}

void dumper_t::visit(constant_t<std::string>* c)
{
	incr_depth_t x(&depth, stream, c->span);
	stream << "string constant: " << c->value << std::endl;
}
#endif

/*void dumper_t::visit(primary_identifier_t* p)
{
	incr_depth_t x(&depth, stream, p->span);
	stream << "primary expression" << std::endl;
	fwd::visit(p);
}

void dumper_t::visit(primary_expression_expression_t* p)
{
	incr_depth_t x(&depth, stream, p->span);
	stream << "primary expression" << std::endl;
	fwd::visit(p);
}*/

void dumper_t::visit(array_access_expression_t *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "array access op" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(argument_expression_list_t *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "argument expression list" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(function_call_expression_t *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "function call" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(struct_access_expression_t *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "struct access" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(compound_literal_t *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "compound literal" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(cast_expression_t *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "cast" << std::endl;
	fwd::visit(e);
}

/*void dumper_t::visit(type_specifier_simple_t* e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "simple type specifier, type: " << e->id << std::endl;
}

void dumper_t::visit(number_t* e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "number, value: " << e->value << std::endl;
}*/

void dumper_t::visit(token_t* e)
{
	incr_depth_t x(&depth, stream, e->span);

	stream << "token: ";
	int value = e->value();
	if(value <= 255)
		stream << (char)value;
	else
		stream << name_of(value);
	stream << std::endl;
}

void dumper_t::visit(unary_expression_l *e)
{
	// FEATURE: full op names
	incr_depth_t x(&depth, stream, e->span);
	stream << "unary_expression (prefix)" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(unary_expression_r *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "unary_expression (postfix)" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(binary_expression_t *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "binary expression " << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(ternary_expression_t *e)
{
/*	on(e);
	e->n1->accept(*);
	on(e, 1);
	if(e->n2) { e->n2->accept(*); on(e, 2); }
	if(e->n3) { e->n3->accept(*); on(e, 3); }*/

	incr_depth_t x(&depth, stream, e->span);
	stream << "ternary expression" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(expression_statement_t *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "expression statement" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(storage_class_specifier_t* )
{
}

void dumper_t::visit(iteration_statement_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	const char* loop_type =
		n->type == iteration_statement_t::for_type
			? "for"
			: (n->type == iteration_statement_t::do_type
				? "do-while"
				: "while");
	stream << "iteration statement (type " << loop_type << ")" << std::endl;
	fwd::visit(n);
}

/*void dumper_t::visit(primary_expression_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "primary expression";
	switch(n->type)
	{
		case pt_expression:
			stream << std::endl;
			break;
		case pt_constant:
			stream << std::endl;
			break;
		case pt_id:
			stream << ": identifier: " << n->c.value->name << std::endl;
			break;
		case pt_string:
			stream << ": string constant: " << n->c.get_next().value->name << std::endl;
			break;
	}

	fwd::visit(n);
}*/

void dumper_t::visit(sizeof_expression_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "sizeof ..." << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(attr_name_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "attribute name: " << n->raw << std::endl;
}
void dumper_t::visit(attribute_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "attribute" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(identifier_t *n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "identifier: " << n->raw << std::endl;
}

void dumper_t::visit(enumeration_constant_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "enumeration constant: " << n->raw << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(typedef_name_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "typedef name: " << n->raw << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(string_literal_t *s)
{
	incr_depth_t x(&depth, stream, s->span);
	stream << "string literal: " << s->raw << std::endl;
	fwd::visit(s);
}

void dumper_t::visit(type_specifier_t *t)
{
	incr_depth_t x(&depth, stream, t->span);
	stream << "type specifier" << std::endl;
	fwd::visit(t);
}

/*void dumper_t::visit(type_identifier *t)
{
	incr_depth_t x(&depth, stream, t->span);
	stream << "type identifier: " << t->c.value->raw << std::endl;
}*/

void dumper_t::visit(type_qualifier_t* n) {
	incr_depth_t x(&depth, stream, n->span);
	stream << "type qualifier" << std::endl;
	fwd::visit(n);
}
void dumper_t::visit(function_specifier_t* ) {
#if 0
	incr_depth_t x(&depth, stream, n->span);
	stream << "function specifier" << std::endl;
	fwd::visit(n);
#endif
}
void dumper_t::visit(alignment_specifier_t* ) {
#if 0
	incr_depth_t x(&depth, stream, n->span);
	stream << "alignment specifier" << std::endl;
	fwd::visit(n);
#endif
}
void dumper_t::visit(declaration_list_t* n) {
	incr_depth_t x(&depth, stream, n->span);
	stream << "declaration list" << std::endl;
	fwd::visit(n);
}
void dumper_t::visit(compound_statement_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "compound statement" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(pointer_t* n) {
	incr_depth_t x(&depth, stream, n->span);
	stream << "pointer" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(declarator_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "declarator" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(declaration_specifiers_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "declaration specifiers" << std::endl;

	/*vvisit(n->storage_class_specifiers);
	vvisit(n->type_specifiers);
	vvisit(n->type_qualifiers);
	vvisit(n->function_specifiers);
	vvisit(n->alignment_specifiers);*/
	fwd::visit(n);
}
void dumper_t::visit(function_definition_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "function definition" << std::endl;
	fwd::visit(n);
}
void dumper_t::visit(external_declaration_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "external declaration" << std::endl;

	fwd::visit(n);
}

void dumper_t::visit(translation_unit_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "translation unit" << std::endl;
	fwd::visit(n);
}

void dumper_t::visit(declaration_t* n)
{	
	incr_depth_t x(&depth, stream, n->span);
	stream << "declaration" << std::endl;
	fwd::visit(n);
}

/*void dumper_t::visit(constant_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	switch(n->type)
	{
		case ct_int:
			stream << "int constant: " << n->value.i << std::endl;
			break;
		case ct_float:
			stream << "float constant: " << n->value.f << std::endl;
			break;
		case ct_enum:
			stream << "enumeration constant" << std::endl;
			break;
	}

	fwd::visit(n);
}*/

void dumper_t::visit(init_declarator_t *i)
{
	incr_depth_t x(&depth, stream, i->span);
	stream << "init declarator" << std::endl;
	fwd::visit(i);
}

void dumper_t::visit(init_declarator_list_t *i)
{
	incr_depth_t x(&depth, stream, i->span);
	stream << "init declarator list" << std::endl;
	fwd::visit(i);
}

void dumper_t::visit(initializer_t *i)
{
	incr_depth_t x(&depth, stream, i->span);
	stream << "initializer" << std::endl;
	fwd::visit(i);
}

void dumper_t::visit(initializer_list_t *i)
{
	incr_depth_t x(&depth, stream, i->span);
	stream << "initializer list" << std::endl;
	fwd::visit(i);
}


void dumper_t::visit(designator_list_t* d) {
	incr_depth_t x(&depth, stream, d->span);
	stream << "designator list" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(designator_id* d) {
	incr_depth_t x(&depth, stream, d->span);
	stream << "designator: ." << d->c.get_next().value << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(designator_constant_expr* d) {
	incr_depth_t x(&depth, stream, d->span);
	stream << "designator: [" << d->c.get_next().value << ']' << std::endl;
	fwd::visit(d);
}



void dumper_t::visit(abstract_declarator_t *n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "abstract declarator" << std::endl;
	fwd::visit(n);
}



void dumper_t::visit(direct_declarator_id *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct declarator 1 (identifier)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_declarator_decl *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct declarator 2 (declaration)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_declarator_arr *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct declarator 3 (array)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_declarator_func *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct declarator (function)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_declarator_idlist *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct declarator (identifier list)" << std::endl;
	fwd::visit(d);
}


void dumper_t::visit(direct_abstract_declarator_decl *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct abstract declarator (declaration)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_abstract_declarator_arr *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct abstract declarator (array)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_abstract_declarator_func *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct abstract declarator (function)" << std::endl;
	fwd::visit(d);
}


void cleaner_t::visit(binary_expression_t* e)
{
	//e->accept_children(*this);
	accept_all(e->c);
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

unary_op_t unary_op_l(int c)
{
	switch(c)
	{
		case '&': return op_addr; 
		case '*': return op_ptr;
		case '+': return op_pos;
		case '-': return op_neg;
		case '~': return op_compl;
		case '!': return op_not;
		case t_inc_op: return op_inc_pre;
		case t_dec_op: return op_dec_pre;
		default: assert(false);
	}
}

unary_op_t unary_op_r(int c)
{
	switch(c)
	{
		case t_inc_op: return op_inc_post;
		case t_dec_op: return op_dec_post;
		default: assert(false);
	}
}

binary_op_t binary_op_of(int c)
{
	switch(c)
	{
		case '=': return op_asn;
		case t_mul_asn: return op_asn_mul;
		case t_div_asn: return op_asn_div;
		case t_mod_asn: return op_asn_mod;
		case t_add_asn: return op_asn_add;
		case t_sub_asn: return op_asn_sub;
		case t_left_asn: return op_asn_left;
		case t_right_asn: return op_asn_right;
		case t_and_asn: return op_asn_and;
		case t_xor_asn: return op_asn_xor;
		case t_or_asn: return op_asn_or;
	
		case t_or_op: return op_or;
		case t_and_op: return op_and;
		case '|': return op_bor;
		case '&': return op_band;
		case '^': return op_xor;
		case t_left_op: return op_lshift;
		case t_right_op: return op_rshift;
	
		case '<': return op_lt;
		case '>': return op_gt;
		case t_le_op: return op_le;
		case t_ge_op: return op_ge;
		case t_eq_op: return op_eq;
		case t_ne_op: return op_ne;
	
		case '+': return op_plus;
		case '-': return op_minus;
		case '*': return op_mult;
		case '/': return op_div;
		case '%': return op_mod;

		case ',': return op_com;

		default: std::cout << +c << std::endl; assert(false);
	}
}

void type_completor::on(unary_expression_l& u)
{
	u.op_id = unary_op_l(u.c.get<0>()->value());
}
void type_completor::on(unary_expression_r& u)
{
	u.op_id = unary_op_r(u.c.get<1>()->value());
}
void type_completor::on(binary_expression_t& b) {
	b.op_id = binary_op_of(b.c.get<1>()->value());
}


void type_completor::on(iteration_statement_t& i)
{
	i.type =
		i.c.get<iteration_statement_t::for_cond>()
			? iteration_statement_t::for_type // FEATURE: better ("global") names: for_loop ?
			: (i.c.get<iteration_statement_t::do_keyword>()
				? iteration_statement_t::do_type
				: iteration_statement_t::while_type);

	bool for_type_iter = i.c.get<iteration_statement_t::for_iter>();

	i._for_type =
		i.c.get<iteration_statement_t::for_init>()
			? (for_type_iter
				? iteration_statement_t::for_type_init_iter
				: iteration_statement_t::for_type_init_niter)
			: (for_type_iter
				? iteration_statement_t::for_type_decl_iter
				: iteration_statement_t::for_type_decl_niter);

}

void type_completor::on(labeled_statement_t& l)
{
	l.type =
		l.c.get<labeled_statement_t::keyword>()
			? (l.c.get<labeled_statement_t::case_expr>()
				? labeled_statement_t::case_label
				: labeled_statement_t::default_label)
			: labeled_statement_t::jump_label;
}

void type_completor::on(jump_statement_t& j)
{
	const int keyword = j.c.get<jump_statement_t::keyword>()->value();
	j.type =
		(keyword == t_return)
			? (j.c.get<jump_statement_t::expression>()
				? jump_statement_t::return_type
				: jump_statement_t::return_void)
			: ((keyword == t_break)
				? jump_statement_t::break_type
				: ((keyword == t_continue)
					? jump_statement_t::continue_type
					: jump_statement_t::goto_type
				)
			);
}

void type_completor::on(struct_or_union_specifier_t& s)
{
	const int keyword = s.c.get<struct_or_union_specifier_t::keyword>()->value();
	s.is_union_type = (keyword == t_union);
}

void type_completor::on(struct_access_expression_t& s)
{
	const int optype = s.c.get<1>()->value();
	s.pointer_access = (optype == t_ptr_op);
}



