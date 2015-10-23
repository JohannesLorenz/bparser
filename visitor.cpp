#include <cassert>
#include <iostream>
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
		for(std::size_t i = 0; i < *depth; ++i)
		 stream << "  ";
		++*depth;
	}
	~incr_depth_t() { --*depth;
		stream << " (" << span << ')' << std::endl;
	}
};







void tree_visit_t::visit(type_specifier_simple_t* e)
{
	on(e);
}

void tree_visit_t::visit(number_t* e)
{
	on(e);
}

void tree_visit_t::visit(token_t* e)
{
	on(e);
}

void tree_visit_t::visit(expression_t* e)
{
	on(e);
	e->n1->accept(*this);
	on(e, 1);
	if(e->n2) { e->n2->accept(*this); on(e, 2); }
	if(e->n3) { e->n3->accept(*this); on(e, 3); }
}

void tree_visit_t::visit(expression_statement_t *e)
{
	on(e);
	e->expression->accept(*this);
	on(e, 1);
}

void tree_visit_t::visit(storage_class_specifier_t* n)
{
	on(n);
}

void tree_visit_t::visit(iteration_statement_t* n)
{
	on(n);
}

void tree_visit_t::visit(primary_expression_t* n)
{
	on(n);
	switch(n->type)
	{
		case pt_expression:
			visit(n->expression);
			on(n, 1);
			break;
		case pt_constant:
			visit(n->constant);
			on(n, 1);
			break;
		case pt_id:
			break;
		case pt_string:
			break; 
	}
}

void tree_visit_t::visit(sizeof_expression_t* n)
{
	on(n);
}

void tree_visit_t::visit(identifier_t *n)
{
	on(n);
}

void tree_visit_t::visit(type_specifier_t* n)
{
	on(n);
}

void tree_visit_t::visit(type_qualifier_t* n) {on(n);
}
void tree_visit_t::visit(function_specifier_t* n) {on(n);
}
void tree_visit_t::visit(alignment_specifier_t* n) {on(n);
}
void tree_visit_t::visit(declaration_list_t* n) {on(n);
}
void tree_visit_t::visit(compound_statement_t* n)
{
	on(n);
	vaccept(n, n->block_items, 0);
	on(n, 1);
}
void tree_visit_t::visit(pointer_t* n) {on(n);
}
void tree_visit_t::visit(direct_declarator_t* n)
{
	on(n);
}
void tree_visit_t::visit(declarator_t* n)
{
	on(n);
}

void tree_visit_t::visit(declaration_specifiers_t* n)
{
	on(n);
	/*vvisit(n->storage_class_specifiers);
	vvisit(n->type_specifiers);
	vvisit(n->type_qualifiers);
	vvisit(n->function_specifiers);
	vvisit(n->alignment_specifiers);*/
	vaccept(n, n->specifiers, 0);
	on(n, 1);
}
void tree_visit_t::visit(function_definition_t* n)
{
	counter_t<function_definition_t> t(this, n);
	visit(n->declaration_specifiers);
	t.next();
	visit(n->declarator);
	t.next();
	tvisit(n->declaration_list);
	t.next();
	visit(n->compound_statement);
	t.next();
}
void tree_visit_t::visit(external_declaration_t* n)
{
	on(n);
	tvisit(n->function_definition) || tvisit(n->declaration);
	on(n, 1);
}

void tree_visit_t::visit(translation_unit_t* n)
{
	on(n);
	vvisit(n, n->v, 0);
	on(n, 1);
}

void tree_visit_t::visit(declaration_t* n)
{	
	counter_t<declaration_t> t(this, n);
	visit(n->declaration_specifiers);
	t.next();
	visit(n->init_declarator_list);
	t.next();
}

void tree_visit_t::visit(constant_t* n)
{
	switch(n->type)
	{
		case ct_int:
		case ct_float:
			break;
		case ct_enum:
			visit(n->enum_id);
			on(n, 1);
			break;
	}
}











void dumper_t::on(type_specifier_simple_t* e, on_t role, on_t)
{
	assert(!role);
	incr_depth_t x(&depth, stream, e->span);
	stream << "simple type specifier, type: " << e->id;
}

void dumper_t::on(number_t *e, on_t role, on_t)
{
	assert(!role);
	incr_depth_t x(&depth, stream, e->span);
	stream << "number, value: " << e->value;
}

void dumper_t::on(token_t* e, on_t role, on_t)
{
	assert(!role);
	incr_depth_t x(&depth, stream, e->span);
	stream << "token, value: " << e->value;
}

void dumper_t::on(expression_t *e, on_t role, on_t)
{
	if(!role)
	{
		incr_depth_t x(&depth, stream, e->span);
		stream << "expr, op 1: " << +e->op;
	}
}

void dumper_t::on(expression_statement_t *e, on_t role, on_t)
{
	if(!role) {
		incr_depth_t x(&depth, stream, e->span);
		stream << "expression statement";
	}
}

void dumper_t::on(storage_class_specifier_t* , on_t role, on_t)
{
	assert(!role);
}

void dumper_t::on(iteration_statement_t* n, on_t role, on_t)
{
	if(!role)
	{
		incr_depth_t x(&depth, stream, n->span);
		const char* loop_type =
			n->for_token
				? "for"
				: (n->do_token
					? "do-while"
					: "while");
		stream << "iteration statement (type " << loop_type << ")";
	}
}

void dumper_t::on(primary_expression_t* n, on_t role, on_t)
{
	if(!role)
	{
		incr_depth_t x(&depth, stream, n->span);
		stream << "primary expression";
		switch(n->type)
		{
			case pt_expression:
				break;
			case pt_constant:
				break;
			case pt_id:
				stream << ": identifier: " << n->identifier;
				break;
			case pt_string:
				stream << ": string constant: " << n->string;
				break; 
		}
	}
}

void dumper_t::on(sizeof_expression_t* n, on_t role, on_t)
{
	assert(!role);
	incr_depth_t x(&depth, stream, n->span);
	stream << "sizeof ...";
}

void dumper_t::on(identifier_t *n, on_t role, on_t)
{
	assert(!role);
	incr_depth_t x(&depth, stream, n->span);
	stream << "identifier: " << n->name;
}

void dumper_t::on(type_specifier_t* n, on_t role, on_t)
{
	assert(!role);
	incr_depth_t x(&depth, stream, n->span);
//	stream << "type specifier, id: " << +n->id << ", pos: " << n->span << std::endl;
}

void dumper_t::on(type_qualifier_t* n, on_t role, on_t) {
	assert(!role);
	incr_depth_t x(&depth, stream, n->span);
	stream << "type qualifier";
}
void dumper_t::on(function_specifier_t* n, on_t role, on_t) {
	assert(!role);
	incr_depth_t x(&depth, stream, n->span);
	stream << "function specifier";
}
void dumper_t::on(alignment_specifier_t* n, on_t role, on_t) {
	assert(!role);
	incr_depth_t x(&depth, stream, n->span);
	stream << "alignment specifier";
}
void dumper_t::on(declaration_list_t* n, on_t role, on_t) {
	assert(!role);
	incr_depth_t x(&depth, stream, n->span);
	stream << "declaration list";
}
void dumper_t::on(compound_statement_t* n, on_t role, on_t)
{
	if(!role) {
		incr_depth_t x(&depth, stream, n->span);
		stream << "compound statement";
	}
}
void dumper_t::on(pointer_t* n, on_t role, on_t) {
	assert(!role);
	incr_depth_t x(&depth, stream, n->span);
	stream << "pointer";
}
void dumper_t::on(direct_declarator_t* n, on_t role, on_t)
{
	assert(!role);
	incr_depth_t x(&depth, stream, n->span);
	stream << "direct declarator";
}
void dumper_t::on(declarator_t* n, on_t role, on_t)
{
	assert(!role);
	incr_depth_t x(&depth, stream, n->span);
	stream << "declarator";
}

void dumper_t::on(declaration_specifiers_t* n, on_t role, on_t)
{
	if(! role)
	{
		incr_depth_t x(&depth, stream, n->span);
		stream << "declaration specifiers";
	}
}
void dumper_t::on(function_definition_t* n, on_t role, on_t)
{
	if(! role)
	{
		incr_depth_t x(&depth, stream, n->span);
		stream << "function definition";
	}
}
void dumper_t::on(external_declaration_t* n, on_t role, on_t)
{
	if(! role)
	{
		incr_depth_t x(&depth, stream, n->span);
		stream << "external_declaration_t";
	}
}

void dumper_t::on(translation_unit_t* n, on_t role, on_t)
{
	if(! role)
	{
		incr_depth_t x(&depth, stream, n->span);
		stream << "translation unit";
	}
}

void dumper_t::on(declaration_t* n, on_t role, on_t)
{
	if(! role)
	{
		incr_depth_t x(&depth, stream, n->span);
		stream << "declaration";
	}
}

void dumper_t::on(constant_t* n, on_t role, on_t)
{
	if(! role)
	{
		incr_depth_t x(&depth, stream, n->span);
		switch(n->type)
		{
			case ct_int:
				stream << "int constant: " << n->value.i;
				break;
			case ct_float:
				stream << "float constant: " << n->value.f;
				break;
			case ct_enum:
				stream << "enumeration constant";
				break;
		}
	}
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
