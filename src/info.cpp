/*************************************************************************/
/* bparser - a bison-based, C99 parser                                   */
/* Copyright (C) 2015-2018                                               */
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
#include <cstdio>

#include "node.h"
#include "token.h"
#include "utils.h"
#include "visitor.h"

#include "info.h"

std::string pointer_info_t::to_str() const
{
	std::string res;
	for(unsigned i = 0; i < pointers; ++i)
		res += '*';
	res += type_qualifiers.to_str();
	if(next)
		res += next->to_str();
	return res;
}

std::string pointer_info_t::to_human() const
{
	std::string res;
	for(unsigned i = 0; i < pointers - 1; ++i)
		res += "pointer to ";
	res += type_qualifiers.to_str();
	res += " pointer to ";
	return res;
}

std::string declarator_info_t::to_str(const std::string& inside) const
{
	std::string res;
	if(identifier)
	{
		res += identifier->raw;
	}
	else if(is_decl)
	{
		res += '(' + inside + ')';
	}
	else if(is_ellipsis)
	{
		res += "...";
	}
	else
	{
		if(declspec)
			res += declspec->to_str();
		if(pointers)
			res += pointers->to_str();
		res += inside;

		if(is_array)
			res += "[]";
		else if(is_func)
		{
			res += '(';
			std::vector<decl_auto_ptr<declarator_info_t> >::const_iterator itr;
			for(itr = params.begin(); itr != params.end(); )
			{
				res += (*itr)->to_str("");
				++itr;
				if(itr != params.end())
					res += ", ";
			}
			res += ')';
		}
	}

	if(next)
		res = next->to_str(res);
	else
		if(res[res.length()-1] == ' ')
			res.resize(res.size() - 1);

	if(bitfield_width != 0)
	{
		res += " : ";
		if (bitfield_width == -1)
		{
			res += "(unknown-bitfield-width)";
		}
		else
		{
			char tmp[16];
			snprintf(tmp, 15, "%d", bitfield_width);
			res += tmp;
		}
	}

	return res;
}

std::string declarator_info_t::to_human() const
{
	std::string res;
	
	if(pointers)
		res += pointers->to_human();
	if(declspec)
		res += declspec->to_human();
	
	if(identifier)
	{
		res += identifier->raw + " is ";
	}
	else if(is_decl)
	{
	}
	else if(is_ellipsis)
	{
		res += "ellipsis ";
	}
	else if(is_array)
	{
		res += "array of ";
	}
	else if(is_func)
	{
		if(params.empty())
		{
			res += "function without argument specifier returning ";
		}
		else if(
			! params.front()->pointers &&
			! params.front()->next &&
			params.front()->declspec &&
			params.front()->declspec->type_specifiers.base_type == declspec_info_t::type_specifiers_t::is_void)
		{
			res += "function without arguments returning ";
		}
		else
		{
			res += "function taking ";
			std::vector<decl_auto_ptr<declarator_info_t> >::const_iterator itr;
			for(itr = params.begin(); itr != params.end(); )
			{
				res += (*itr)->to_human();
				++itr;
				if(itr != params.end())
					res += ", ";
			}
			res += "returning ";
		}
	}
	
	if(next)
		res += next->to_human();
	
	return res;
}

std::string has_type_qualifiers::type_qualifiers_t::to_str() const
{
	std::string res;
	if(is_const)
	{
		res += "const ";
	}
	if(is_restrict)
	{
		res += "restrict ";
	}
	if(is_volatile)
	{
		res += "volatile ";
	}
	if(is_atomic)
	{
		res += "_Atomic ";
	}
	return res;
}

std::string declspec_info_t::to_str() const
{
	std::string res;
	typedef declspec_info_t::type_specifiers_t tyt;

	if(function_specifiers.is_noreturn)
	{
		res += "noreturn ";
	}
	if(function_specifiers.is_inline)
	{
		res += "inline ";
	}

	if(storage_class.is_typedef)
	{
		res += "typedef ";
	}
	if(storage_class.is_extern)
	{
		res += "extern ";
	}
	if(storage_class.is_static)
	{
		res += "static ";
	}
	if(storage_class.is_thread_local)
	{
		res += "_Thread_local ";
	}
	if(storage_class.is_register)
	{
		res += "register ";
	}
	
	res += type_qualifiers.to_str();

	const type_specifiers_t& tys = type_specifiers;
	if(tys.is_unsigned)
		res += "unsigned ";
	else if(tys.is_signed)
		res += "signed ";
	
	switch(tys.length_mod)
	{
		case tyt::is_short: res += "short "; break;
		case tyt::is_long: res += "long "; break;
		case tyt::is_long_long: res += "long long "; break;
		default: break;
	}
	
	switch(tys.base_type)
	{
		case tyt::is_void: res += "void "; break;
		case tyt::is_bool: res += "_Bool "; break;
		case tyt::is_complex: res += "_Complex "; break;
		case tyt::is_char: res += "char "; break;
		case tyt::is_int:
			if(tys.length_mod == tyt::no_length_mod && !tys.is_unsigned)
				res += "int ";
			break;
		case tyt::is_float: res += "float "; break;
		case tyt::is_double: res += "double "; break;
		case tyt::is_enum_spec:
		{
			assert(tys.enum_spec);
			const identifier_t* id = tys.enum_spec->c.
				get<enum_specifier_t::identifier>();
			res += "enum ";
			if(id)
				res += id->raw;
			break;
		}
		case tyt::is_struct_or_union:
		{
			assert(tys.struct_or_union_spec);
			const identifier_t* id = tys.struct_or_union_spec->c.
				get<struct_or_union_specifier_t::identifier>();
			res += tys.is_union ? "union " : "struct ";
			if(id)
				res += id->raw;
			break;
		}
	}

	return res;
}

struct get_declspec_info_t : public visitor_t
{
	declspec_info_t* decl_spec_info;
	bool internal_signed_helper;
	typedef declspec_info_t::type_specifiers_t tyt;

	/*
	 * internal
	 */
	
	void no_bt() const
	{
		assert(decl_spec_info->type_specifiers.base_type == tyt::is_void);
	}
	
	void visit(token_t& tok)
	{
		declspec_info_t& inf = *decl_spec_info;
		declspec_info_t::type_specifiers_t& tys = inf.type_specifiers;
		declspec_info_t::type_specifiers_t::length_mod_t& len = tys.length_mod;
		switch(tok.value())
		{
			// type specifiers
			case t_void: break;
			case t_bool: no_bt(); tys.base_type = tyt::is_bool; break;
			case t_complex: no_bt(); tys.base_type = tyt::is_complex; break;
			case t_char: no_bt(); tys.base_type = tyt::is_char; break;
			case t_short: assert(len == tyt::no_length_mod); len = tyt::is_short; break;
			case t_int: no_bt(); tys.base_type = tyt::is_int; break;
			case t_long:
				if(len == tyt::is_long)
					len = tyt::is_long_long;
				else if (len == tyt::no_length_mod)
					len = tyt::is_long;
				else
					assert(false);
				break;
			case t_float: no_bt(); tys.base_type = tyt::is_float; break;
			case t_double: no_bt(); tys.base_type = tyt::is_double; break;
			case t_signed: assert(!tys.is_unsigned); internal_signed_helper = true; break;
			case t_unsigned: assert(!internal_signed_helper); tys.is_unsigned = true; break;
			// function specifiers
			case t_inline: inf.function_specifiers.is_inline = 1; break;
			case t_noreturn: inf.function_specifiers.is_noreturn = 1; break;
			// type qualifiers
			case t_const: inf.type_qualifiers.is_const = 1; break;
			case t_restrict: inf.type_qualifiers.is_restrict = 1; break;
			case t_volatile: inf.type_qualifiers.is_volatile = 1; break;
			case t_atomic: inf.type_qualifiers.is_atomic = 1; break;
			// storage class
			case t_typedef: inf.storage_class.is_typedef = 1; break;
			case t_extern: inf.storage_class.is_extern = 1; break;
			case t_static: inf.storage_class.is_static = 1; break;
			case t_thread_local: inf.storage_class.is_thread_local = 1; break;
			case t_auto: break;
			case t_register: inf.storage_class.is_register = 1; break;
			// other - can't be
			default: assert(false); break;
		}
	}
	
	void visit(type_qualifier_t& t) { t.c.get<0>()->accept(*this); }
	void visit(type_specifier_t& t) { t.c.get<0>()->accept(*this); }
	
	void visit(struct_or_union_specifier_t& s)
	{
		declspec_info_t::type_specifiers_t& tys = decl_spec_info->type_specifiers;
		tys.base_type = tyt::is_struct_or_union;
		tys.is_union = s.is_union_type;
		tys.struct_or_union_spec = &s;
	}
	
	void visit(enum_specifier_t& e)
	{
		declspec_info_t::type_specifiers_t& tys = decl_spec_info->type_specifiers;
		tys.base_type = tyt::is_enum_spec;
		tys.enum_spec = &e;
	}
	
	void visit(typedef_name_t& t)
	{
		identifier_t* def = t._definition;
		declaration_base& dion = declaration_from_identifier(*def);
		dion.accept(*this); // once again...
	}
	
	void finalize()
	{
		declspec_info_t& inf = *decl_spec_info;
		declspec_info_t::type_specifiers_t& tys = inf.type_specifiers;
		declspec_info_t::type_specifiers_t::length_mod_t& len = tys.length_mod;
		if(tys.base_type == tyt::is_void)
		{
			if(len == tyt::is_short || len == tyt::is_long || len == tyt::is_long_long ||
				tys.is_unsigned || internal_signed_helper)
				tys.base_type = tyt::is_int;
		}
		else if(tys.base_type == tyt::is_char && internal_signed_helper)
		{
			  tys.is_signed = true; // char and signed char are separate types
		}
	}

public:
	/*
	 * entry points
	 */
	// declaration specifiers:
	void visit(declaration_specifiers_t& s)
	{
		typedef std::list<node_t*>::const_iterator itr_t;
		for(itr_t itr = s.c.begin(); itr != s.c.end(); ++itr)
		{
			(*itr)->accept(*this);
		}
		finalize();
	}
	void visit(specifier_qualifier_list_t& s)
	{
		assert(false); // deprecated struct, can not occur => can not be visited!
		typedef std::list<declaration_specifier_type*>::const_iterator itr_t;
		for(itr_t itr = s.c.begin(); itr != s.c.end(); ++itr)
		{
			(*itr)->accept(*this);
		}
		finalize();
	}

	void start(declaration_specifiers_t& specifiers)
	{
		specifiers.accept(*this);
	}

	/*
	 * ctor
	 */
	get_declspec_info_t(decl_auto_ptr<declspec_info_t>& decl_spec_info) :
		decl_spec_info(decl_spec_info.raw()),
		internal_signed_helper(false)
	{
	}
};

struct get_integral_from_expression_t : public visitor_t
{
	long value; // default is -1: could not compute the value
	void visit(iconstant_t& i) { value = i.value; }
	void visit(primary_expression_t& e) { e.c.get<0>()->accept(*this); }
public:
	get_integral_from_expression_t(expression_t& e) : value(-1) { e.accept(*this); }
	long result() const { return value; }
};

class get_declarator_info : public visitor_t
{
	decl_auto_ptr<declarator_info_t> last_decl; //!< current decl that is being filled
	declarator_info_t* res; //!< result. almost not used during computation, refer to "last_decl"
	bool m_specifiers_found;

	void recurse_tqs(type_qualifier_t& t, pointer_info_t::type_qualifiers_t& tqs)
	{
		switch(t.c.get<0>()->value())
		{
			case t_const: tqs.is_const = 1; break;
			case t_restrict: tqs.is_restrict = 1; break;
			case t_volatile: tqs.is_volatile = 1; break;
			case t_atomic: tqs.is_atomic = 1; break;
			// other - can't be
			default: assert(false); break;
		}
	}
	void recurse_tqs(type_qualifier_list_t& tql, pointer_info_t::type_qualifiers_t& tqs)
	{
		for(std::list<type_qualifier_t*>::iterator itr = tql.c.begin(); itr != tql.c.end(); ++itr)
		{
			recurse_tqs(**itr, tqs);
		}
	}
  
	void recurse_pointers(pointer_t& x, decl_auto_ptr<pointer_info_t>& self)
	{
		if(!self)
			self.alloc();
		++self->pointers;
		nodes::type_qualifier_list_t* tql = x.c.get<1>();
		if(tql)
			recurse_tqs(*tql, self->type_qualifiers);
		if(x.c.get<2>())
		{
			recurse_pointers(*x.c.get<2>(), tql ? self->next : self);
		}
	}

	void recurse(parameter_declaration_t& pd, std::list< decl_auto_ptr<declarator_info_t> >& par_list);
	void recurse(token_t& tok, std::list< decl_auto_ptr<declarator_info_t> >& par_list); // ellipsis
	
	void recurse(parameter_list_t& pl, std::list< decl_auto_ptr<declarator_info_t> >& par_list)
	{
		if(pl.c.get<0>())
			recurse(*pl.c.get<0>(), par_list); // recurse into next param
		recurse(*pl.c.get<2>(), par_list); // parameter_declaration_t
	}
	
	void recurse(parameter_type_list_t& ptl)
	{
		std::list< decl_auto_ptr<declarator_info_t> > par_list;
		recurse(*ptl.c.get<0>(), par_list);
		if(ptl.c.get<2>()) // ellipsis
			recurse(*ptl.c.get<2>(), par_list);
		
		// we needed a list before to not copy any pointers when resizing the params
		// now, transfer into a vector
		last_decl->params.resize(par_list.size());
		std::list< decl_auto_ptr<declarator_info_t> >::iterator litr = par_list.begin();
		for(std::size_t i = 0; i < par_list.size(); ++i, ++litr)
		{
			last_decl->params[i].take_ownership(*litr);
		}
	}
	
	template<class NodeType> void go_up(const NodeType& self) { self.parent->accept(*this); }
	
	void visit(declaration_specifiers_t& specifiers)
	{
		new_decl();
		last_decl->declspec.alloc();
		get_declspec_info_t gdi(last_decl->declspec);
		gdi.start(specifiers);
		m_specifiers_found = true;
	}
	
	// reached the top to get the specifiers
	void visit(declaration_t& d) { visit(*d.c.get<0>()); }
	void visit(struct_declaration_t& s) { visit(*s.c.get<0>()); }
	void visit(parameter_declaration_t& x) { visit(*x.c.get<0>()); }
	void visit(type_name_t& x) { visit(*x.c.get<0>()); }
	void visit(function_definition_t& x) { visit(*x.c.get<0>()); }
	
	// go upwards for specifiers
	void visit(init_declarator_list_t& x) { go_up(x); }
	void visit(struct_declarator_list_t& x) { go_up(x); }
	
	// reached the top to get the declarator
	void visit(init_declarator_t& x) { go_up(x); }
	void visit(struct_declarator_t& x) {
		expression_t* bitfield_width_ex = x.c.get<2>();
		if(bitfield_width_ex)
		{
			get_integral_from_expression_t gife(*bitfield_width_ex);
			last_decl->bitfield_width = gife.result();
		}
		go_up(x); }

	// append new decl to current, and make the child the current decl
	void new_decl()
	{
		declarator_info_t* parent = last_decl.steal();
		last_decl.alloc();
		parent->next.assign(last_decl.raw());
	}
	
	// go upwards
	template<class NodeT>
	void visit_common_declarator(NodeT& x)
	{
		if(x.c.template get<0>())
		{
			new_decl();
			recurse_pointers(*x.c.template get<0>(), last_decl->pointers);
		}
		go_up(x);
	}
	
	void visit(abstract_declarator_t& x) { visit_common_declarator(x); }
	void visit(declarator_t& x) { visit_common_declarator(x); }
	
	template<int ExprNum, class NodeT>
	void visit_common_arr(NodeT& x)
	{
		new_decl();
		last_decl->is_array = true;
		last_decl->is_array_without_size = !!x.c.template get<ExprNum>();
		go_up(x);
	}
	
	template<class NodeT>
	void visit_common_decl(NodeT& x)
	{
		new_decl();
		last_decl->is_decl = true;
		go_up(x);
	}
	
	template<class NodeT>
	void visit_common_func(NodeT& x)
	{
		new_decl();
		last_decl->is_func = true;
		if(x.c.template get<2>())
			recurse(*x.c.template get<2>()); // parameter type list
		go_up(x);
	}
	
	void visit(direct_abstract_declarator_arr& x) { visit_common_arr<2>(x); }
	void visit(direct_abstract_declarator_decl& x) { visit_common_decl(x); }
	void visit(direct_abstract_declarator_func& x) { visit_common_func(x); }
	void visit(direct_declarator_arr& x) { visit_common_arr<4>(x); }
	void visit(direct_declarator_decl& x) { visit_common_decl(x); }
	void visit(direct_declarator_func& x) { visit_common_func(x); }
	void visit(direct_declarator_id& x) { /* done in identifier*/ go_up(x); }
	void visit(direct_declarator_idlist& x) {
		new_decl();
		last_decl->is_idlist = true;
		go_up(x);
	}

	void visit(identifier_t& x) {
		new_decl();
		last_decl->identifier = &x;
		go_up(x);
	}

	template<class NodeType>
	void do_start(NodeType& x)
	{
		/* allocate fake start node above our result */
		last_decl.alloc();
		declarator_info_t* fake_node = last_decl.raw();
		x.accept(*this);
		res = fake_node->next.raw();
	}
public:
	// legal entries
	void start(identifier_t& x) { do_start(x); } // in its definition
	void start(abstract_declarator_t& x) { do_start(x);}
	void start(direct_abstract_declarator_t& x) { do_start(x); }
	void start(declaration_specifiers_t& x) { do_start(x); }
	
	get_declarator_info() : m_specifiers_found(false) {}
	~get_declarator_info() { last_decl.steal(); /* forbid memory cleanup */ }
	bool specifiers_found() const { return m_specifiers_found; }
	declarator_info_t* result() { return res; }
};

// parameters don't always start as an identifier, they can be e.g. "int", "int []"
struct entry_point_for_parameter : public get_declarator_t
{
	direct_abstract_declarator_t* direct_abstract; // int []
	abstract_declarator_t* abstract; // not sure, maybe int*
	declaration_specifiers_t* declspec; // e.g. "int"
	entry_point_for_parameter() : direct_abstract(NULL), abstract(NULL), declspec(NULL) {}

	void visit(parameter_declaration_t& d) { if(d.c.get<1>()) get_declarator_t::visit(*d.c.get<1>()); else if (d.c.get<2>()) visit(*d.c.get<2>()); else declspec = d.c.get<0>(); }

	void visit(abstract_declarator_t& d) { if(d.c.get<1>()) d.c.get<1>()->accept(*this); else abstract = &d; }
	void visit(direct_abstract_declarator_decl& d) { if(d.c.get<1>()) visit(*d.c.get<1>()); else direct_abstract = &d; }
	void visit(direct_abstract_declarator_arr& d) { if(d.c.get<0>()) d.c.get<0>()->accept(*this); else direct_abstract = &d; }
	void visit(direct_abstract_declarator_func& d) { if(d.c.get<0>()) d.c.get<0>()->accept(*this); else direct_abstract = &d; }
};

void get_declarator_info::recurse(parameter_declaration_t& pd, std::list< decl_auto_ptr<declarator_info_t> >& par_list)
{
	entry_point_for_parameter epfp;
	epfp.visit(pd);
	get_declarator_info getter;
	if(epfp.value)
		getter.start(*epfp.value);
	else if(epfp.abstract)
		getter.start(*epfp.abstract);
	else if(epfp.direct_abstract)
		getter.start(*epfp.direct_abstract);
	else if(epfp.declspec)
		getter.start(*epfp.declspec);
	
	par_list.push_back(decl_auto_ptr<declarator_info_t>());
	par_list.back().assign(getter.res);
}

void get_declarator_info::recurse(token_t& tok, std::list< decl_auto_ptr<declarator_info_t> >& par_list) // = ellipsis
{
	assert(tok.value() == t_ellipsis);

	decl_auto_ptr<declarator_info_t> decl;
	decl.alloc();
	decl->is_ellipsis = true;

	par_list.push_back(decl_auto_ptr<declarator_info_t>());
	par_list.back().take_ownership(decl);
}

declarator_info_t* get_declspec_info(nodes::defined_t& node)
{
	get_declarator_info getter;
	if(node._definition)
		getter.start(*node._definition);
	else
		getter.start(*dcast<nodes::identifier_t>(&node)); // node is a forward declaration
	return getter.specifiers_found()
		? getter.result()
		: NULL; // no declaration specifiers => this was no declaration :-/
}

declarator_info_t* get_declspec_info(nodes::declarator_t& node)
{
	identifier_t* id = get_identifier(node);
	return id ? get_declspec_info(*id) : NULL;
}

std::ostream& operator<<(std::ostream& os, const declarator_info_t& dsi)
{
	return os << dsi.to_str();
}


