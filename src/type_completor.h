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

//! @file type_completor.h
//! This file only declares the @a type_completor class, which
//! completes the AST (abstract syntax tree) after it has been built

#ifndef TYPE_COMPLETOR_H
#define TYPE_COMPLETOR_H

#include <stdexcept>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <typeinfo>

#include "options.h"
#include "visitor.h"

//! only for internal use of type_completor
class struct_type_of : public visitor_t
{
	// TODO: set_identifier function that checks if it's null
	//	in all other node types, set identifier to null!
	struct_or_union_specifier_t* _struct_specifier;

	void set_spec(struct_or_union_specifier_t* s) {
		if(!_struct_specifier) // identifier has been found in
			// a parent node, so noting the identifier
			// does not make any sense
		 _struct_specifier = s; }

	void set_spec_from_id(identifier_t* struct_id);
public:
	struct_or_union_specifier_t* get_struct_specifier() { return _struct_specifier; }
	struct_type_of() : _struct_specifier(NULL) {}
	void visit(unary_expression_r &u);
	void visit(unary_expression_l &u);
	void visit(binary_expression_t &b);
	void visit(ternary_expression_t &t);
	void visit(primary_expression_t &p);
	void visit(array_access_expression_t &a);
	void visit(function_call_expression_t &f);
	void visit(struct_access_expression_t &s);
	void visit(compound_literal_t& c);
	void visit(cast_expression_t& c);
	void visit(type_name_t& t);
	void visit(specifier_qualifier_list_t& s);
	void visit(declaration_specifiers_t& s);
	void visit(type_specifier_t& t);
	void visit(struct_or_union_specifier_t& s);
	void visit(typedef_name_t& t);
	void visit(identifier_t& t);
};

namespace detail
{
	template<class C1, class C2> struct asgn {
		static void exec(C1& , const C2&) { throw std::logic_error("error: incompatible assignment operators"); }
	};
	template<class C> struct asgn<C, C> {
		static void exec(C& dest, const C& src) { dest = src; }
	};
}

class parent_assigner
{
	node_t* parent;

	class set_parent
	{
		node_t* parent;

	public:
		template<class C>
		void operator()(C& child) {
			child.parent = dynamic_cast<typename C::parent_type*>(parent);
		} // :-(((
	
		template<class Unused>
		set_parent(Unused, node_t* p) : parent(p) {}
	};

public:
	parent_assigner(node_t* parent) : parent(parent) {}
	
	template<class C>
	void operator()(C* const& child)
	{
		if(child)
		{
			func_visitor< set_parent > sp(parent);
			child->accept(sp);
		}
	}

	template<class C>
	void operator()(std::list<C*>* children) {
		_foreach< std::list<C*>* >::exec(children, *this);
	}
};

namespace scope_types
{
	template<class T>
	struct inc_depth
	{
		static const bool value = false;
	};

	struct do_inc_depth
	{
		static const bool value = true;
	};

	template<> struct inc_depth<iteration_statement_t> :
		public do_inc_depth {};
	template<> struct inc_depth<compound_statement_t> :
		public do_inc_depth {};
	template<> struct inc_depth<function_definition_t> :
		public do_inc_depth {};
	template<> struct inc_depth<struct_declaration_list_t> :
		public do_inc_depth {};

}

/**
	@class type_completor
	@brief Completes the AST after partially built from parser

	The main task of the parser+lexer is to do anything necessary, but not more.
	This is because the parser+lexer are ugly old-style C code; also the rest
	should be independent of bison+yacc.

	Thus, the type_completor's job is to complete the whole AST (abstract syntax tree)
	after parser+lexer have built it. Completing means: Adding additional info to
	nodes, such as
	 * parent pointers
	 * location of definition of identifiers
	 * practical things, e.g. a flag for whether a struct_or_union_specifier is a struct
	   or a union, or what operator a binary expression has.
	
	After the type completor has run, all nodes should have all their variables set to
	some useful value.
*/
class type_completor : ftor_base
{
	std::size_t decl_depth;
	identifier_t* current_struct_scope;

	class lookup_table_t
	{
		typedef std::pair<std::size_t, identifier_t*> value_entry_t; // depth, declaration
		typedef std::vector<value_entry_t> value_t;
		typedef std::pair<std::string, value_t> entry_t;
		typedef std::map<std::string, value_t> table_t;
		table_t table;

		std::string internal_name_of_new_id(const char* new_id, bool struct_bound) {
			std::string rval = (struct_bound) ? "struct " : "";
			return rval += new_id;
		}

		typedef std::multimap<std::string, identifier_t* > unknowns_t;
		unknowns_t unknown_types;
	public:
		void flag_symbol(identifier_t* id, int new_depth, bool struct_bound);

		identifier_t* declaration_of(const char* str) {
			table_t::const_iterator itr = table.find(str);
			if(itr == table.end())
			 return NULL;
			 //throw "Identifier not found";
			else
			 return itr->second.back().second;
		}

		void notify_dec_decl_depth(std::size_t new_depth)
		{
#ifdef TYPE_COMPLETOR_DEBUG
			std::cout << "DEPTH decreased to: " << new_depth << std::endl;
#endif
			table_t::iterator itr = table.begin(),
				next = table.begin();
			for(itr = table.begin(); itr != table.end(); itr = next)
			{
				// invariant: itr == next
				if(next != table.end())
				 ++next;

				if(itr->second.back().first > new_depth + 1)
				{
					// that variable should have been removed at last scope end (below)
					throw std::logic_error("overseen last scope end");
				}
				else
				{
					// remove all variables at that depth

					// invariant: itr+1 == next || next == table.end()
					while(itr->second.back().first == new_depth + 1)
					{
						// variable out of scope
						itr->second.pop_back();
						if(itr->second.empty())
						 table.erase(itr);
					}
				}
			}
		}

		void dangling_identifier(identifier_t* i, bool is_struct)
		{
			std::string s = internal_name_of_new_id(i->raw.c_str(), is_struct);
			unknown_types.insert(std::pair<std::string, identifier_t*>(s, i));
		}

	} v_lookup_table;



	void connect_identifier(identifier_t* , bool connect_as_struct = false);

	bool allow_undefined;
public:
	type_completor(visitor_t* vref, bool allow_undefined) :
		ftor_base(vref),
		decl_depth(0),
		current_struct_scope(NULL),
		allow_undefined(allow_undefined) {}

	~type_completor() {
		if(decl_depth) throw std::logic_error("Declaration depth counted wrong");
	}

	// expressions...
	void on(unary_expression_l& , enter);
	void on(unary_expression_r& , enter);
	void on(binary_expression_t& , enter);

	// FEATURE: float, int

	// statements...
	void on(iteration_statement_t& , enter);
	void on(labeled_statement_t& , enter);
	void on(jump_statement_t& , enter);

	// structs...
	void on(struct_or_union_specifier_t& , leave);
	void on(struct_declaration_list_t&, leave);
	void on(struct_access_expression_t& , leave);

	// declarators
	void on(enum_specifier_t& , enter);
	void on(declaration_t&, leave);
	void on(function_definition_t&, enter);
	void on(parameter_declaration_t&, leave);
	void on(enumerator_t&, leave);
	void on(direct_declarator_func& f, enter);

	// variables
	void on(primary_expression_t&, enter);
	void on(designator_id&, enter);
	void on(identifier_list_t&, enter);
	void on(type_name_t&, enter);
	void on(enumeration_constant_t&, enter);
	void on(typedef_name_t&, enter);

	// literals
	void on(iconstant_t&, enter);

	// default case
	void on(node_base& , direction_t ) {}

//	template<class NodeType>
//	void on(/*const*/ NodeType& ) { }
#if 0
	template<class NodeType>
	void on(const NodeType&)
#endif
	template<class NodeType>
	void handle_depth_inc(NodeType& n, enter)
	{
#ifdef TYPE_COMPLETOR_DEBUG
		if(scope_types::inc_depth<NodeType>::value)
		{
			for(std::size_t i = 0; i < decl_depth; ++i)
			 std::cout << "  ";
			std::cout << "++ " << typeid(n).name() << " " << n.span.first << std::endl;
		}
#else
		(void)n;
#endif
		decl_depth += scope_types::inc_depth<NodeType>::value;
	}

	template<class NodeType>
	void handle_depth_inc(NodeType& , leave) {}

	template<class NodeType>
	void handle_depth_dec(NodeType& , enter) {}

	template<class NodeType>
	void handle_depth_dec(NodeType& n, leave)
	{
#ifdef TYPE_COMPLETOR_DEBUG
		if(scope_types::inc_depth<NodeType>::value)
		{
			for(std::size_t i = 0; i < decl_depth - 1; ++i)
			 std::cout << "  ";
			std::cout << "-- " << typeid(n).name() << " " << n.span.first << std::endl;
		}
#else
		(void)n;
#endif
		int dec_val = scope_types::inc_depth<NodeType>::value;
		if(dec_val) {
			v_lookup_table.notify_dec_decl_depth(--decl_depth);
		}
	}

	template<class NodeType, class Direction>
	void operator()(NodeType& n, Direction d) {
		// assign parents
		parent_assigner pa(&n);
		foreach(n.c, pa);
		// variable scoping
		handle_depth_inc(n, d);
		// do something
		on(n, d);
		// variable scoping
		handle_depth_dec(n, d);
	}
};

#endif // TYPE_COMPLETOR_H

