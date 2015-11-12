#include <iostream>
#include <iomanip>

#include "token.h"
#include "node.h"
#include "visitor.h"
#include "parser.h"

std::ostream& operator<<(std::ostream& stream,
	const geom_t& g)
{
	return stream << "geometry(line: " << g.line << ", col: " << g.col << ")";
}

std::ostream& operator<<(std::ostream& stream,
	const span_t& s)
{
	return stream << "l" << s.first.line << ", c" << s.first.col;
}

//template<>
//void type_specifier_simple_t::accept(visitor_t& v) { v.visit(this); }

identifier_t::identifier_t(const char* name, geom_t geom) :
	terminal_t(geom, t_identifier),
	name(name) {}

void token_t::accept(visitor_t& v) { v.visit(this); }
//void number_t::accept(visitor_t& v) { v.visit(this); }
//void expression_t::accept(visitor_t& v) { v.visit(this); }
void unary_expression_l::accept(visitor_t& v) { v.visit(this); }
void unary_expression_r::accept(visitor_t& v) { v.visit(this); }
void binary_expression_t::accept(visitor_t& v) { v.visit(this); }
void ternary_expression_t::accept(visitor_t& v) { v.visit(this); }


void storage_class_specifier_t::accept(visitor_t& v) { v.visit(this); }
//void type_specifier_t::accept(visitor_t& v) { v.visit(this); }
void type_qualifier_t::accept(visitor_t& v) { v.visit(this); }
void function_specifier_t::accept(visitor_t& v) { v.visit(this); }
void alignment_specifier_t::accept(visitor_t& v) { v.visit(this); }
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
template<>
void constant_t<int>::accept(visitor_t& v) { v.visit(this); }
template<>
void constant_t<float>::accept(visitor_t& v) { v.visit(this); }
template<>
void constant_t<std::string>::accept(visitor_t& v) { v.visit(this); }
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

void init_declarator_t::accept(visitor_t &v) { v.visit(this); }
void init_declarator_list_t::accept(visitor_t &v) { v.visit(this); }
void initializer_list_t::accept(visitor_t &v) { v.visit(this); }
void initializer_t::accept(visitor_t &v) { v.visit(this); }

void direct_declarator_id::accept(visitor_t &v) { v.visit(this); }
void direct_declarator_decl::accept(visitor_t &v) { v.visit(this); }
void direct_declarator_arr::accept(visitor_t &v) { v.visit(this); }
void direct_declarator_func::accept(visitor_t &v) { v.visit(this); }

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
void cast_postfix_expression_t::accept(visitor_t &v) { v.visit(this); }
void cast_expression_t::accept(visitor_t &v) { v.visit(this); }


void type_name_t::accept(visitor_t &v) { v.visit(this); }
void type_specifier_token::accept(visitor_t &v) { v.visit(this); }
void type_identifier::accept(visitor_t &v) { v.visit(this); }
void abstract_declarator_t::accept(visitor_t &v) { v.visit(this); }
void specifier_qualifier_list_t::accept(visitor_t &v) { v.visit(this); }

void struct_or_union_specifier_t::accept(visitor_t &v) { v.visit(this); }
void struct_declaration_list_t::accept(visitor_t &v) { v.visit(this); }
void struct_declarator_list_t::accept(visitor_t &v) { v.visit(this); }
void struct_declarator_t::accept(visitor_t &v) { v.visit(this); }
void enum_specifier_t::accept(visitor_t &v) { v.visit(this); }
void enumerator_list_t::accept(visitor_t &v) { v.visit(this); }
void enumerator_t::accept(visitor_t &v) { v.visit(this); }
void parameter_type_list_t::accept(visitor_t &v) { v.visit(this); }
void parameter_list_t::accept(visitor_t &v) { v.visit(this); }
void parameter_declaration_t::accept(visitor_t &v) { v.visit(this); }
void identifier_list_t::accept(visitor_t &v) { v.visit(this); }



