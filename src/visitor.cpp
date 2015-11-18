#include <cassert>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include "node.h"
#include "visitor.h"

#include "token.h"

extern std::vector<std::string>& get_files();













// TODO: fwd does not visit tokens!




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

void fwd::visit(cast_postfix_expression_t *e)
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

void fwd::visit(storage_class_specifier_t* ) { /* TODO... (when structs are made) */ }

void fwd::visit(iteration_statement_t* i) { accept_all(i->c); }

/*void fwd::visit(primary_expression_t* p)
{
	accept_all(p->c);
}*/

void fwd::visit(sizeof_expression_t* n)
{
	visit_all(n->c);
}

void fwd::visit(identifier_t *) {}
void fwd::visit(type_specifier_t *t) { accept_all(t->c); }
//void fwd::visit(type_identifier *t) { visit_all(t->c); }
void fwd::visit(type_qualifier_t *t) { visit_all(t->c); }

void fwd::visit(type_qualifier_list_t *n)
{
	vvisit(n->c);
}
void fwd::visit(function_specifier_t* ) { // TODO
}
void fwd::visit(alignment_specifier_t* ) { // TODO
}
void fwd::visit(declaration_list_t* d) { vvisit(d->declarations); }
void fwd::visit(compound_statement_t* n) {
		tvisit(n->c.value);
		vaccept(*n->c.get_next().value);
		tvisit(n->c.get_next().get_next().value);
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
	visit_all(d->c);
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
		snprintf(tmp, 64, "%32s (l%4d c%4d) (l%4d c%4d)",
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
	stream << "struct declaration" << std::endl; // TODO: good name?
	fwd::visit(n);
}

void dumper_t::visit(struct_declaration_list_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "struct declaration list" << std::endl; // TODO: good name?
	fwd::visit(n);
}

void dumper_t::visit(struct_declarator_list_t* n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "struct declarator list" << std::endl; // TODO: endl autom?
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

void dumper_t::visit(constant_t<int>* c)
{
	incr_depth_t x(&depth, stream, c->span);
	stream << "int constant: " << c->value << std::endl;
}

void dumper_t::visit(iconstant_t* c) {
	incr_depth_t x(&depth, stream, c->span);
	stream << "int constant: (TODO)" << std::endl;
}

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

void dumper_t::visit(cast_postfix_expression_t *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "cast postfix expression (what is that?)" << std::endl;
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

	stream << "token, value: ";
	int value = e->value;
	if(value <= 255)
		stream << (char)value;
	else
		stream << name_of(value);
	stream << std::endl;
}

void dumper_t::visit(unary_expression_l *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "unary_expression, op 1: TODO" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(unary_expression_r *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "unary_expression, op 1: TODO" << std::endl;
	fwd::visit(e);
}

void dumper_t::visit(binary_expression_t *e)
{
	incr_depth_t x(&depth, stream, e->span);
	stream << "binary expression " << e->op_id << std::endl;
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
	stream << "ternary expression, op 1: TODO" << std::endl;
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
	// TODO?
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

void dumper_t::visit(identifier_t *n)
{
	incr_depth_t x(&depth, stream, n->span);
	stream << "identifier: " << n->raw << std::endl;
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
void dumper_t::visit(function_specifier_t* n) {
	incr_depth_t x(&depth, stream, n->span);
	stream << "function specifier" << std::endl;
	fwd::visit(n);
}
void dumper_t::visit(alignment_specifier_t* n) {
	incr_depth_t x(&depth, stream, n->span);
	stream << "alignment specifier" << std::endl;
	fwd::visit(n);
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
	stream << "direct declarator 1 (TODO: specify?)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_declarator_decl *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct declarator 2 (TODO: specify?)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_declarator_arr *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct declarator 3 (TODO: specify?)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_declarator_func *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct declarator 4 (TODO: specify?)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_abstract_declarator_decl *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct abstract declarator (TODO: specify?)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_abstract_declarator_arr *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct abstract declarator (TODO: specify?)" << std::endl;
	fwd::visit(d);
}

void dumper_t::visit(direct_abstract_declarator_func *d)
{
	incr_depth_t x(&depth, stream, d->span);
	stream << "direct abstract declarator (TODO: specify?)" << std::endl;
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



void type_completor::operator()(iteration_statement_t& i)
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

	xaccept(i.c);
}

void type_completor::operator()(labeled_statement_t& l)
{
	l.type =
		l.c.get<labeled_statement_t::keyword>()
			? (l.c.get<labeled_statement_t::case_expr>()
				? labeled_statement_t::case_label
				: labeled_statement_t::default_label)
			: labeled_statement_t::jump_label;
	xaccept(l.c);
}

void type_completor::operator()(jump_statement_t& j)
{
	const int keyword = j.c.get<jump_statement_t::keyword>()->value;
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
	xaccept(j.c);
}

void type_completor::operator()(struct_or_union_specifier_t& s)
{
	const int keyword = s.c.get<struct_or_union_specifier_t::keyword>()->value;
	s.is_union_type = (keyword == t_union);
	xaccept(s.c);
}

