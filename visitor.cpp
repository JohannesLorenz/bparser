#include <cassert>
#include <iostream>
#include <cstdio>
#include "node.h"
#include "visitor.h"

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
		char tmp[32];
		snprintf(tmp, 32, "(l%4d c%4d) (l%4d c%4d)", span.first.line,
			span.first.col, span.second.line, span.second.col);

		for(std::size_t i = 0; i < *depth; ++i)
		 stream << "  ";
		++*depth;
	}
	~incr_depth_t() { --*depth;
		//stream << " (" << span << ')';
	}
};







void dumper_t::visit(type_specifier_simple_t* e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "simple type specifier, type: " << e->id << std::endl;
}

void dumper_t::visit(number_t* e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "number, value: " << e->value << std::endl;
}

void dumper_t::visit(token_t* e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "token, value: " << e->value << std::endl;
}

void dumper_t::visit(unary_expression_l *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "unary_expression, op 1: TODO" << std::endl;
	accept_all(e->c);
}

void dumper_t::visit(unary_expression_r *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "unary_expression, op 1: TODO" << std::endl;
	accept_all(e->c);
}

void dumper_t::visit(binary_expression_t *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "binary_expression " << e->op_id << std::endl;
	accept_all(e->c);
}

void dumper_t::visit(ternary_expression_t *e)
{
/*	on(e);
	e->n1->accept(*this);
	on(e, 1);
	if(e->n2) { e->n2->accept(*this); on(e, 2); }
	if(e->n3) { e->n3->accept(*this); on(e, 3); }*/

	incr_depth_t x(&depth, stream, e->span);
	stream << "ternary_expression, op 1: TODO" << std::endl;
	accept_all(e->c);
}

void dumper_t::visit(expression_statement_t *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "expression statement" << std::endl;
	e->expression->accept(*this);
}

void dumper_t::visit(storage_class_specifier_t* )
{
}

void dumper_t::visit(iteration_statement_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	const char* loop_type =
		n->for_token
			? "for"
			: (n->do_token
				? "do-while"
				: "while");
	stream << "iteration statement (type " << loop_type << ")" << std::endl;
}

void dumper_t::visit(primary_expression_t* n)
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
			stream << ": identifier: " << n->identifier << std::endl;
			break;
		case pt_string:
			stream << ": string constant: " << n->string << std::endl;
			break;
	}

	switch(n->type)
	{
		case pt_expression:
			n->expression->accept(*this);
			break;
		case pt_constant:
			n->constant->accept(*this);
			break;
		case pt_id:
			break;
		case pt_string:
			break; 
	}
}

void dumper_t::visit(sizeof_expression_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "sizeof ..." << std::endl;
}

void dumper_t::visit(identifier_t *n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "identifier: " << n->name << std::endl;
}

void dumper_t::visit(type_specifier_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
//	stream << "type specifier, id: " << +n->id << ", pos: " << n->span << std::endl;
}

void dumper_t::visit(type_qualifier_t* n) {
	incr_depth_t x(&depth, stream, n->span);
	stream << "type qualifier" << std::endl;
}
void dumper_t::visit(function_specifier_t* n) {
	incr_depth_t x(&depth, stream, n->span);
	stream << "function specifier" << std::endl;
}
void dumper_t::visit(alignment_specifier_t* n) {
	incr_depth_t x(&depth, stream, n->span);
	stream << "alignment specifier" << std::endl;
}
void dumper_t::visit(declaration_list_t* n) {
	incr_depth_t x(&depth, stream, n->span);
	stream << "declaration list" << std::endl;
}
void dumper_t::visit(compound_statement_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "compound statement" << std::endl;

	vaccept(n->block_items);
}

void dumper_t::visit(pointer_t* n) {
	incr_depth_t x(&depth, stream, n->span);
	stream << "pointer" << std::endl;
}

void dumper_t::visit(declarator_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "declarator" << std::endl;
	accept_all(n->c);
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
	vaccept(n->specifiers);
}
void dumper_t::visit(function_definition_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "function definition" << std::endl;

	visit(n->declaration_specifiers);
	visit(n->declarator);
	tvisit(n->declaration_list);
	visit(n->compound_statement);
}
void dumper_t::visit(external_declaration_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "external declaration" << std::endl;

	tvisit(n->function_definition) || tvisit(n->declaration);
}

void dumper_t::visit(translation_unit_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "translation unit" << std::endl;

	vvisit(n->v);
}

void dumper_t::visit(declaration_t* n)
{	
	incr_depth_t x(&depth, stream, n->span);
	stream << "declaration" << std::endl;

	visit(n->declaration_specifiers);
	visit(n->init_declarator_list);
}

void dumper_t::visit(constant_t* n)
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

	switch(n->type)
	{
		case ct_int:
		case ct_float:
			break;
		case ct_enum:
			visit(n->enum_id);
			break;
	}
}

void dumper_t::visit(init_declarator_t *i)
{
	incr_depth_t x(&depth, stream, i->span);
	stream << "init declarator" << std::endl;
	visit_all(i->c);
}

void dumper_t::visit(init_declarator_list_t *i)
{
	incr_depth_t x(&depth, stream, i->span);
	stream << "init declarator list" << std::endl;
	visit_all(i->c);
}

void dumper_t::visit(initializer_t *i)
{
	incr_depth_t x(&depth, stream, i->span);
	stream << "initializer" << std::endl;
	accept_all(i->c);
}

void dumper_t::visit(initializer_list_t *i)
{
	incr_depth_t x(&depth, stream, i->span);
	stream << "initializer list" << std::endl;
	//visit_all(i->c);
}



void dumper_t::visit(abstract_declarator_t *n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "abstract declarator" << std::endl;
	// visit ... - TODO
}



void dumper_t::visit(direct_declarator_id *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct declarator 1 (TODO: specify?)" << std::endl;
	visit(d->value);
}

void dumper_t::visit(direct_declarator_decl *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct declarator 2 (TODO: specify?)" << std::endl;
	visit_all(d->c);
}

void dumper_t::visit(direct_declarator_arr *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct declarator 3 (TODO: specify?)" << std::endl;
	accept_all(d->c);
}

void dumper_t::visit(parameter_type_list_t *)
{
	// TODO
}

void dumper_t::visit(direct_declarator_func *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct declarator 4 (TODO: specify?)" << std::endl;
	visit_all(d->c);
}

void dumper_t::visit(direct_abstract_declarator_decl *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct abstract declarator (TODO: specify?)" << std::endl;
	visit_all(d->c);
}

void dumper_t::visit(direct_abstract_declarator_arr *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct abstract declarator (TODO: specify?)" << std::endl;
	accept_all(d->c);
}

void dumper_t::visit(direct_abstract_declarator_func *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct abstract declarator (TODO: specify?)" << std::endl;
	visit_all(d->c);
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
