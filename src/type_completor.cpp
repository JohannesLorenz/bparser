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

#include <stdexcept>
#include <cstdio> // sscanf...

#include "type_completor.h"
#include "node.h"
#include "token.h"
#include "utils.h"

void struct_type_of::set_spec_from_id(identifier_t* struct_id)
{
	struct_or_union_specifier_t* spec = dcast<struct_or_union_specifier_t>(*struct_id->parent);
	if(! spec) { // B : declared as a declarator?
		spec = dcast<struct_or_union_specifier_t>(struct_rval_of_func(*struct_id));
		if(! spec)
		{
			std::cout << "ABORT INFO: " << *struct_id->parent << std::endl;
			throw std::runtime_error(
				"Unable to cast parent of struct identifier to struct_or_union_specifier_t");
		}
	}
	set_spec(spec);
}

void struct_type_of::visit(unary_expression_r& u)
{
	u.c.get<0>()->accept(*this);
}

void struct_type_of::visit(unary_expression_l& u)
{
	u.c.get<1>()->accept(*this);
}

void struct_type_of::visit(binary_expression_t& b)
{
	if(b.op_id == op_com)
		b.c.get<2>()->accept(*this);
	else {
		// could be something like
		// ptr + 42
		// 42 + ptr
		// strukt = other
		// ptr = voidptr
		// in all cases, this works:
		b.c.get<0>()->accept(*this);
		if(!_struct_specifier)
		 b.c.get<2>()->accept(*this);
	}
}

void struct_type_of::visit(ternary_expression_t& t)
{
	t.c.get<2>()->accept(*this);
	if(!_struct_specifier)
	{
		// maybe we have better luck here:
		t.c.get<4>()->accept(*this);
	}
}

void struct_type_of::visit(primary_expression_t& p)
{
	identifier_t* id = p.c.get<1>();
	if(id)
	{
	//	std::cout << "67: " << *struct_rval_of_func(*id).c.get<struct_or_union_specifier_t::identifier>() << std::endl;
		
		struct_or_union_specifier_t& result = struct_rval_of_func(*id);
		identifier_t* structs_id = result.c.get<struct_or_union_specifier_t::identifier>();
		
		if(structs_id) // TODO: does this ever occur?
		 set_spec_from_id(structs_id->_definition);
		else
		 set_spec(&result);
		
	//	std::cout << "DEF: " << _identifier << std::endl;
		/*if(is_func_id(*id))
		 {}
		else
{}*/
	}
	else if(p.c.get<3>())
	 p.c.get<3>()->accept(*this);
}

void struct_type_of::visit(array_access_expression_t& a)
{
	a.c.get<0>()->accept(*this);
}

void struct_type_of::visit(function_call_expression_t& f)
{
	// careful: what's left to the parantheses must not be
	// directly a function call expression
	// TODO: set variables?
	f.c.get<0>()->accept(*this);
}

void struct_type_of::visit(struct_access_expression_t& s) {
	// should have already been done by other visitors
	set_spec_from_id(s.c.get<2>()->_definition); // TODO: recursive visit?
}

void struct_type_of::visit(compound_literal_t& c){
	tvisit(c.c.get<1>());
}

void struct_type_of::visit(cast_expression_t& c){
	tvisit(c.c.get<1>());
}

void struct_type_of::visit(type_name_t &t)
{
	tvisit(t.c.get<0>());
}

void struct_type_of::visit(declaration_specifiers_t &s)
{
	vaccept(s.c);
}

void struct_type_of::visit(specifier_qualifier_list_t &s)
{
	vaccept(s.c);
}

void struct_type_of::visit(type_specifier_t &t)
{
	t.c.value->accept(*this);
}

void struct_type_of::visit(struct_or_union_specifier_t &s)
{
	set_spec(&s);
}

void struct_type_of::visit(typedef_name_t &t)
{
	// typedef_name
	//	~> identifier
	//		=> direct_declarator
	//			=> declarator
	//				=>... declaration
	//					~> get type specifier
	//				=> function_definition
	//					~> not useful ;)
	//		=> struct_or_union_specifier
	//			~> can not be (typedef ~> would be declaration)
	visit(declaration_from_identifier(*t._definition).decl_spec()); // FEATURE: ref func for tuple?
	//t->_definition->parent->parent->accept(*this);
}

void struct_type_of::visit(identifier_t& t)
{
	visit(*t._definition);
}

#if 0
struct compound_literal_t : public expression_t
{
	ptn<	token_t,
		ptn<	type_name_t,
			ptn<	token_t,
				ptn<	token_t,
					ptn<	struct initializer_list_t,
						ptn<	token_t,
								end_token
		> > > > > > c;
	virtual void accept(class visitor_t& v);
};

struct cast_expression_t : public expression_t
#endif





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

void type_completor::connect_identifier(identifier_t* identifier,
	bool connect_as_struct)
{
	if(identifier)
	{
		std::string lookup_name;
		if(connect_as_struct)
		 lookup_name += "struct ";
		lookup_name += identifier->raw;

		identifier->_definition = v_lookup_table.declaration_of(lookup_name.c_str());
		// TODO

		if(!identifier->_definition)
		 v_lookup_table.dangling_identifier(identifier, connect_as_struct);
	}
}

void type_completor::on(unary_expression_l& u, enter)
{
	u.op_id = unary_op_l(u.c.get<0>()->value());
}
void type_completor::on(unary_expression_r& u, enter)
{
	u.op_id = unary_op_r(u.c.get<1>()->value());
}
void type_completor::on(binary_expression_t& b, enter) {
	b.op_id = binary_op_of(b.c.get<1>()->value());
}


void type_completor::on(iteration_statement_t& i, enter)
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

void type_completor::on(labeled_statement_t& l, enter)
{
	l.type =
		l.c.get<labeled_statement_t::keyword>()
			? (l.c.get<labeled_statement_t::case_expr>()
				? labeled_statement_t::case_label
				: labeled_statement_t::default_label)
			: labeled_statement_t::jump_label;

	v_lookup_table.flag_symbol(l.c.get<labeled_statement_t::identifier>(),
		decl_depth, false);
}

void type_completor::on(jump_statement_t& j, enter)
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
	if(j.type == jump_statement_t::goto_type)
		connect_identifier(j.c.get<jump_statement_t::goto_id>());
}

void type_completor::on(struct_or_union_specifier_t& s, leave)
{
	const int keyword = s.c.get<struct_or_union_specifier_t::keyword>()->value();
	s.is_union_type = (keyword == t_union);

	bool is_definition =
		// C99 standard: brackets always ensure a definition
		s.c.get<struct_or_union_specifier_t::lbrack>()
		// if no brackets, it does not provide everything needed
		// so it may be a declaration, but no definition
		;

	identifier_t* id = s.c.get<struct_or_union_specifier_t::identifier>();
	if(is_definition)
	 v_lookup_table.flag_symbol(id,
		decl_depth, true); // TODO: should only declarations be flagged?
	else
	 connect_identifier(id, true);
}

void type_completor::on(struct_declaration_list_t& l, leave)
{
	// all members are not declared before the struct is being finished!
	// -> this has to be done now
	for(std::list<struct_declaration_t*>::const_iterator itr = l.c.begin(); itr != l.c.end(); ++itr)
	{
		struct_declarator_list_t* cur = (*itr)->c.get<1>();
		for(; cur; cur = cur->c.get<0>()) {
			declarator_t* dtor = cur->c.get<2>()->c.get<0>();
			if(dtor)
			 v_lookup_table.flag_symbol(&get_declarator(*dtor), decl_depth, false);
		}

	}
}

void type_completor::on(struct_access_expression_t& s, leave)
{
	const int optype = s.c.get<1>()->value();
	s.pointer_access = (optype == t_ptr_op);

	// step 1: link all identifiers in the left tree
	// (already done, since we are leaving!)

	// step 2: get struct type of left identifier
#if 1
	struct_type_of struct_type;
	s.c.get<0>()->accept(struct_type); // <- this does almost all the work, including typedef resolution
#if 0
	current_struct_scope = struct_type.get_identifier();
#ifdef TYPE_COMPLETOR_VERBOSE
	std::cout << "STRUCT ACCESS EXPRESSION: " << s << std::endl;
	std::cout << "STRUCT ACCESS: SCOPE: " << *current_struct_scope << std::endl;
#endif
	if(!current_struct_scope) {
		std::cout << "struct access expression at " << s.span
			<< "implicits a struct left of the operator,"
				"however, there was no such struct found" << std::endl;
		throw "struct access expression with no struct left to the operator";
	}

	// step 3: finally, we have the struct scope
	// of the left expression. use it to connect the
	// right identifier
	// TODO: get struct_...list
	//declaration_from_identifier(*current_struct_scope).decl_spec();
	//struct_or_union_specifier_t* spec = dcast<struct_or_union_specifier_t>(*current_struct_scope->parent);
	// A: declared in a struct specifier?
	struct_or_union_specifier_t* spec = dcast<struct_or_union_specifier_t>(*current_struct_scope->parent);
	if(! spec) { // B : declared as a declarator?
		spec = dcast<struct_or_union_specifier_t>(struct_rval_of_func(*current_struct_scope));
		if(! spec)
		{
			std::cout << "ABORT INFO: " << *current_struct_scope->parent << std::endl;
			throw "Unable to cast parent of struct identifier to struct_or_union_specifier_t";
		}
	}
#ifdef TYPE_COMPLETOR_VERBOSE
	std::cout << "ACC ACC:" << *spec << std::endl;
#endif
#else
	safe_ptr<struct_or_union_specifier_t> spec = struct_type.get_struct_specifier();
#endif
	struct_declaration_list_t& l = *spec->c.get<struct_or_union_specifier_t::declaration_list>();
	bool searching = true;
	for(std::list<struct_declaration_t*>::const_iterator itr = l.c.begin(); searching && itr != l.c.end(); ++itr)
	{
		struct_declarator_list_t* cur = (*itr)->c.get<1>();
		for(; cur && searching; cur = cur->c.get<0>()) {
			declarator_t* dtor = cur->c.get<2>()->c.get<0>();
			if(dtor) { // FEATURE: never NULL?
				// once a declarator exists, it must have an identifier
				identifier_t& struct_member = get_declarator(*dtor); // FEATURE: rename: id_from_declarator?
				if(struct_member.raw == s.c.get<2>()->raw) {
					s.c.get<2>()->_definition = &struct_member;
#ifdef TYPE_COMPLETOR_VERBOSE
					std::cout << "connecting id: " << *dtor << std::endl;
#endif
					searching = false;
				}
			}
			//if(dtor)
			// v_lookup_table.flag_symbol(&get_declarator(*dtor), decl_depth, false);
		}

	}
	if(searching)
	 throw std::runtime_error("Identifier not found in struct!");

	//connect_identifier(s.c.get<2>());
#endif
}

void type_completor::on(declaration_t& d, leave)
{
	if(d.c.get<1>())
	{
		init_declarator_list_t* cur = d.c.get<1>();
		for(; cur; cur = cur->c.get<0>()) {
			v_lookup_table.flag_symbol(&get_declarator(*cur->c.get<2>()->c.get<0>()), decl_depth, false);
		}
	}
}

void type_completor::on(function_definition_t& f, enter)
{
	// for functions, the depth has already been increased,
	// however, the function identifier itself is at level 0
	v_lookup_table.flag_symbol(&get_declarator(*f.c.get<1>()), decl_depth - 1, false);
}

struct check_is_fptr : public visitor_t
{
	bool fptr;
public:
	check_is_fptr() : fptr(false) {}
	bool result() const { return fptr; }

	void visit(declarator_t& d) {
		if(d.c.get<0>())
			fptr = true;
		else // maybe we'll still find one below?
			d.c.get<1>()->accept(*this);
	}
	void visit(direct_declarator_decl& d) {
		d.c.get<1>()->accept(*this);
	}
	void visit(direct_declarator_func& f) {
		f.c.get<0>()->accept(*this);
	}
};

void type_completor::on(direct_declarator_func& f, enter)
{
	check_is_fptr fptr_checker;
	fptr_checker.visit(f);
	f.fptr = fptr_checker.result();
}

void type_completor::on(parameter_declaration_t& p, leave) {
	if(p.c.get<1>())
	 v_lookup_table.flag_symbol(&get_declarator(*p.c.get<1>()), decl_depth, false);
}

void type_completor::on(enum_specifier_t& e, enter) {
	v_lookup_table.flag_symbol(e.c.get<enum_specifier_t::identifier>(), decl_depth, false);
}

void type_completor::on(enumerator_t& e, leave) {
	v_lookup_table.flag_symbol(e.c.get<0>(), decl_depth, false);
}

void type_completor::on(primary_expression_t& p, enter)
{
	connect_identifier(p.c.get<1>());
	p.type = (p.c.get<1>())	? pt_id
				: (p.c.get<2>())
					? pt_expression
					: (is<iconstant_t>(*p.c.get<0>()) || is<fconstant_t>(*p.c.get<0>()) || is<enumeration_constant_t>(*p.c.get<0>()))
						? pt_constant
						: pt_string;
}

void type_completor::on(designator_id& , enter) {
	//connect_identifier(d.c.get<1>());
}

void type_completor::on(identifier_list_t&, enter) {
	// TODO
}
void type_completor::on(type_name_t &, enter) {

}
void type_completor::on(enumeration_constant_t& e, enter) {
	e._definition = v_lookup_table.declaration_of(e.raw.c_str());
}
void type_completor::on(typedef_name_t& t, enter) {
	t._definition = v_lookup_table.declaration_of(t.raw.c_str());
}

bool _icmp(const char* p, char c) {
	return tolower(*p) == tolower(c);
}

void type_completor::on(iconstant_t& i, enter)
{
	iconstant_t::suf_type_t suf;
	const char* p = i.raw.c_str();
	
	if(*p == '\'' || *p == 'L' || _icmp(p,'u'))
	{
		i.number_system = iconstant_t::character;
		if(*p == '\'')
		 suf = iconstant_t::no_suffix;
		else {
			suf = (*p == 'L') ? iconstant_t::suf_l : iconstant_t::suf_u;
			++p;
		}
		++p;
		i.value = *p; // TODO: multi-chars don't work yet
	}
	else
	{

	
		i.number_system = (*p == '0')
			? (_icmp(p+1, 'x'))
				? iconstant_t::hexadecimal
				: isdigit(*(p+1))
					? iconstant_t::octal
					: iconstant_t::decimal // e.g. "0", "0L"
			: iconstant_t::decimal;

		{
			char scanf_str [5] = {'%', i.scanf_modifier(), '%', 'n', 0};
			int tmp, n;
			sscanf(p, scanf_str, &tmp, &n);
			i.value = tmp;
			p += n;
		}

		// find suffix
		// (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
		if(_icmp(p, 'u')) // FEATURE: ?:
		{
			if(_icmp(p+1,'l'))
			{
				if(*(p+1)==*(p+2))
				suf=iconstant_t::suf_ull;
				else
				suf=iconstant_t::suf_ul;
			}
			else
			suf=iconstant_t::suf_u;
		}
		else if(_icmp(p, 'l'))
		{
			if(*(p+1) == (*p)) // ll or LL
			{
				suf = _icmp(p+2, 'u') ? iconstant_t::suf_ull : iconstant_t::suf_ll;
			}
			else
			{
				suf = _icmp(p+1, 'u') ? iconstant_t::suf_ul : iconstant_t::suf_l;
			}
		}
		else
		 suf = iconstant_t::no_suffix;
	}
	
	i.suf_type = suf;
}

void type_completor::lookup_table_t::flag_symbol(identifier_t *id, int new_depth, bool struct_bound)
{
	if(id)
	{
		std::string new_name = internal_name_of_new_id(id->raw.c_str(), struct_bound);
#ifdef TYPE_COMPLETOR_DEBUG
		std::cout << "flagging " << new_name << " at depth " << new_depth << std::endl;
#endif
		table[new_name].push_back(value_entry_t(new_depth, id));
		id->_definition = id;

		// resolve dangling forward declaration and throw them away
		unknowns_t::iterator itr = unknown_types.begin(),
				next = unknown_types.begin();
		for(itr = unknown_types.begin(); itr != unknown_types.end(); itr = next)
		{
			// invariant: itr == next
			if(next != unknown_types.end())
				++next;

			// invariant: ++itr == next || next == unknown_types.end()
			if(itr->first == new_name)
			{
				itr->second->_definition = id;
				//declaration_t* d = declaration_from_identifier(itr->second);
				// FEATURE: map definitions

				unknown_types.erase(itr);
			}
		}
	}
}
