#include <iostream>

#include "node.h"
#include "visitor.h"

std::ostream& operator<<(std::ostream& stream,
	const geom_t& g)
{
	return stream << "geometry(line: " << g.line << ", col: " << g.col << ")";
}

std::ostream& operator<<(std::ostream& stream,
	const span_t& s)
{
	return stream << "span(" << s.first << ", " << s.second << ")";
}

template<>
void type_specifier_simple_t::accept(visitor_t& v) { v.visit(this); }

void token_t::accept(visitor_t& v) { v.visit(this); }
void number_t::accept(visitor_t& v) { v.visit(this); }
void expression_t::accept(visitor_t& v) { v.visit(this); }
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

void expression_t::accept_children(visitor_t& v)
{
	first->accept(v);
	if(second) second->accept(v);
	if(third) third->accept(v);
}
