#include "node.h"
#include "visitor.h"

void number_t::accept(visitor_t& v) { v.visit(this); }
void expression_t::accept(visitor_t& v) { v.visit(this); }

void storage_class_specifier_t::accept(visitor_t& v) { v.visit(this); }
void type_specifier_t::accept(visitor_t& v) { v.visit(this); }
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
