#include <iostream>
#include "node.h"
#include "visitor.h"

class incr_depth_t
{
	std::size_t* depth;
public:
	incr_depth_t(std::size_t* _depth, std::ostream& stream) :
		depth(_depth)
	{
		for(std::size_t i = 0; i < *depth; ++i)
		 stream << "  ";
		++*depth;
	}
	~incr_depth_t() { --*depth; }
};

void dumper_t::visit(type_specifier_simple_t* e)
{
	incr_depth_t x(&depth, stream);
	stream << "simple type specifier, type: " << e->id << std::endl;
}

void dumper_t::visit(number_t* e)
{
	incr_depth_t x(&depth, stream);
	stream << "number, value: " << e->value << std::endl;
}

void dumper_t::visit(token_t* e)
{
	incr_depth_t x(&depth, stream);
	stream << "token, value: " << e->value << std::endl;
}

void dumper_t::visit(expression_t* e)
{
	incr_depth_t x(&depth, stream);
	stream << "expr, op 1: " << +e->op << std::endl;
	e->accept_children(*this);
}

void dumper_t::visit(expression_statement_t *e)
{
	incr_depth_t x(&depth, stream);
	stream << "expression statement" << std::endl;
	e->expression->accept(*this);
}

void dumper_t::visit(storage_class_specifier_t* n)
{
	
}

void dumper_t::visit(iteration_statement_t* n)
{
	incr_depth_t x(&depth, stream);
	const char* loop_type =
		n->for_token
			? "for"
			: (n->do_token
				? "do-while"
				: "while");
	stream << "iteration statement (type " << loop_type << ")"
	       << std::endl;
}

void dumper_t::visit(primary_expression_t* n)
{
	incr_depth_t x(&depth, stream);
	stream << "primary expression";
	if(n->constant) {
		stream << std::endl;
		visit(n->constant);
	}
	else
	 stream << ": string: " << n->identifier << std::endl;
}

void dumper_t::visit(identifier_t *n)
{
	incr_depth_t x(&depth, stream);
	stream << "identifier: " << n->name << std::endl;
}

void dumper_t::visit(type_specifier_t* n)
{
	incr_depth_t x(&depth, stream);
//	stream << "type specifier, id: " << +n->id << ", pos: " << n->span << std::endl;
}

void dumper_t::visit(type_qualifier_t* n) {
	incr_depth_t x(&depth, stream);
	stream << "type qualifier" << std::endl;
}
void dumper_t::visit(function_specifier_t* n) {
	incr_depth_t x(&depth, stream);
	stream << "function specifier" << std::endl;
}
void dumper_t::visit(alignment_specifier_t* n) {
	incr_depth_t x(&depth, stream);
	stream << "alignment specifier" << std::endl;
}
void dumper_t::visit(declaration_list_t* n) {
	incr_depth_t x(&depth, stream);
	stream << "declaration list" << std::endl;
}
void dumper_t::visit(compound_statement_t* n)
{
	incr_depth_t x(&depth, stream);
	stream << "compound statement" << std::endl;
	
	vaccept(n->block_items);
}
void dumper_t::visit(pointer_t* n) {
	incr_depth_t x(&depth, stream);
	stream << "pointer" << std::endl;
}
void dumper_t::visit(direct_declarator_t* n)
{
	incr_depth_t x(&depth, stream);
	stream << "direct declarator" << std::endl;
}
void dumper_t::visit(declarator_t* n)
{
	incr_depth_t x(&depth, stream);
	stream << "declarator" << std::endl;
}

void dumper_t::visit(declaration_specifiers_t* n)
{
	incr_depth_t x(&depth, stream);
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
	incr_depth_t x(&depth, stream);
	stream << "function definition" << std::endl;
	visit(n->declaration_specifiers);
	visit(n->declarator);
	visit(n->declaration_list);
	visit(n->compound_statement);
}
void dumper_t::visit(external_declaration_t* n)
{
	incr_depth_t x(&depth, stream);
	stream << "external_declaration_t" << std::endl;
	tvisit(n->function_definition) || tvisit(n->declaration);
}

void dumper_t::visit(translation_unit_t* n)
{
	incr_depth_t x(&depth, stream);
	stream << "translation unit" << std::endl;
	vvisit(n->v);
}

void dumper_t::visit(declaration_t* n) {
	incr_depth_t x(&depth, stream);
	stream << "declaration" << std::endl;
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
