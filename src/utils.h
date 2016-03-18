/**
	@file utils.h This file specifies useful visitors for the finished AST, as well as
	functions to use them easily.
*/

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "visitor.h"

//! @note currently only for storage class specifiers!
template<class Ftor>
class _for_each_specifier : public visitor_t
{
	Ftor* _ftor;

public:
	Ftor* ftor() { return _ftor; }
	const Ftor* ftor() const { return _ftor; }
	_for_each_specifier(Ftor* _ftor) : _ftor(_ftor) {}

	// should be called with one of those four
	// (with others, it's - of course - still correct)
	void visit(struct_declaration_t& d) { visit(*d.c.get<0>()); }
	void visit(parameter_declaration_t& d) { visit(*d.c.get<0>()); }
	void visit(function_definition_t& d) { visit(*d.c.get<0>()); }
	void visit(declaration_t& d) { visit(*d.c.get<0>()); }

	void visit(declaration_specifiers_t& d) { vaccept(d.c); }
	void visit(specifier_qualifier_list_t& s) { vaccept(s.c); }

	void visit(type_specifier_t& t) {
		// if it's a keyword, then the child is token_t
		t.c.value->accept(*this); }
	void visit(type_qualifier_t& t) {
		// guaranteed that child is token_t
		t.c.value->accept(*this); }
	// storage_class_sp and function_specifier are of type token_t...
	// alignment_specifier_t 
	void visit(token_t& t) { (*_ftor)(t.value()); }
};

//! must be called with non-abstrahizing pointer
template<class Node, class Ftor>
void for_each_specifier(Node& node, Ftor* ftor)
{
	_for_each_specifier<Ftor> _f(ftor);
	_f.visit(node);
}

//! This class trys to find the funtion identifier of a function call expression
//! E.g. f in f(x), but also in (f)(x) or ((fptr)f)(x)
class _function_called : public visitor_t
{
	identifier_t* _identifier;

	void set_id(identifier_t* i) {
		if(!_identifier) // has already been found
		 _identifier = i; }
public:
	identifier_t* get_identifier() { return _identifier; }
	_function_called() : _identifier(NULL) {}
	// maybe future ideas, especially ternary
	//void visit(unary_expression_r &u);
	//void visit(unary_expression_l &u);
	//void visit(binary_expression_t &b);
	// void visit(ternary_expression_t &t);
	// interesting
	void visit(primary_expression_t &p);
	void visit(cast_expression_t& c);
	void visit(identifier_t& i);
	// unlikely...
	//void visit(array_access_expression_t &a);

	// note: function_call_expression is not evaluated: (f + g(3))(x);
};

inline identifier_t* function_called(function_call_expression_t& node) {
	_function_called vis;
	node.c.get<0>()->accept(vis);
	return vis.get_identifier();
}

//! not yet finished
struct type_specifier_list
{
	enum type_t
	{
		t_simple,
		t_struct_or_union,
		t_enum,
		t_typedef
	};
	type_t type;

	short tokens[4];
	std::string name;
};

template<class T>
struct _is
{
	bool value;
	template<class Unused>
	_is(Unused* const) : value(false) {}
	void operator()(T const& ) { value = true; }
	void operator()(node_base const& ) { value = false; }
};

//! returns true iff node is of type T (similar to dynamic_cast, but faster)
template<class T>
inline bool is(node_base& node)
{
	func_visitor< _is<T> > v;
	node.accept(v);
	return v.functor().value;
}

struct raise_to_func_decl : public visitor_t
{
	direct_declarator_func* result;

	raise_to_func_decl() : result(NULL) {}

	void visit(direct_declarator_func& i) { result = &i; }
	void visit(direct_declarator_decl& i) { i.parent->accept(*this); }
	void visit(direct_declarator_id& i) { i.parent->accept(*this); }
	void visit(identifier_t& i) { i.parent->accept(*this); }
};


//! Returns true if the identifier is a function identifier (not a function pointer though)
inline bool is_func_id(identifier_t& id)
{
	raise_to_func_decl vis;
	vis.visit(*id._definition);
	return vis.result;
}

template<class T>
struct _dcast : visitor_t
{
	T* value;
	_dcast() : value(NULL) {}
	void visit(T& n) { value = &n; }
};

//! returns a pointer of type T if it is of exactly that type
//! (similar to dynamic_cast, but faster)
template<class T, class Passed>
inline T* dcast(Passed& node)
{
	_dcast<T> v;
	node.accept(v);
	return v.value;
}

struct get_declarator_t : ftor_base
{
	identifier_t* value;
	template<class V>
	get_declarator_t(V* const v) : ftor_base(v), value(NULL) {}

	void operator()(node_base const& ) { value = NULL; }
	void operator()(declarator_t& d) { accept(*d.c.get<1>()); }
	void operator()(direct_declarator_id& d) { value = d.c.value; }
	void operator()(direct_declarator_decl& d) { visit(*d.c.get<1>()); }
	void operator()(direct_declarator_arr& d) { accept(*d.c.get<0>()); }
	void operator()(direct_declarator_func& d) { accept(*d.c.get<0>()); }
	void operator()(direct_declarator_idlist& d) { accept(*d.c.get<0>()); }
};

//! Returns the identifier of the innermost declarator, e.g. f in void (const *f[3])(int)
inline identifier_t& get_declarator(declarator_t& d)
{
	func_visitor< get_declarator_t > v;
	v.functor()(d);
	return *v.functor().value;
}

#if 0
struct struct_return_value_of_function
{
	identifier_t* return_type;
	template<class Unused>
	struct_return_value_of_function(Unused* const)
		: return_type(NULL) {}
	void operator()(node_base& ) { return_type = NULL; }
	void operator()(declaration_t& d) {
		//return_type = get_declarator(&d);
	}
/*	void operator()(function_definition_t& f) {
		return_type = get_declarator(f.c.get<1>());
	}*/
};
#endif

// trys to find the struct or union type of a declaration, ignoring pointers etc.
// e.g., in const struct s* , it should return s 
struct struct_type_specifier_of_declaration_t : visitor_t
{
	struct_or_union_specifier_t* result;
	struct_type_specifier_of_declaration_t() : result(NULL) {}
	void visit(declaration_t & d) { tvisit(d.c.get<0>()); }
	void visit(struct_declaration_t & d) { tvisit(d.c.get<0>()); }
	void visit(parameter_declaration_t & d) { tvisit(d.c.get<0>()); }
	void visit(declaration_specifiers_t & d) { vaccept(d.c); }
	void visit(type_specifier_t & t) { t.c.get<0>()->accept(*this); }
	void visit(struct_or_union_specifier_t& s) {
		if(s.c.get<struct_or_union_specifier_t::lbrack>())
		 result = &s;
		else
		{ // FEATURE: remove this once forward declarations have been mapped
			identifier_t& definition = *s.c.get<struct_or_union_specifier_t::identifier>()->_definition;
			struct_or_union_specifier_t* src = dcast<struct_or_union_specifier_t>(*definition.parent);
			if(src)
			 result = src;
			else // not sure if this is possible, but let's just throw for now
			 throw "struct has definition which is not a struct...";
		}
		}
	void visit(typedef_name_t& t);
};

// FEATURE: structs etc?
struct declaration_from_declarator_t : ftor_base
{
	declaration_base* declaration_found;
	

	declaration_from_declarator_t(visitor_t* vref) :
		ftor_base(vref) {}

	void operator()(node_base& ) {
		declaration_found = NULL; }
	void operator()(identifier_t& d) {
		accept(*d.parent);
	}
	void operator()(direct_declarator_t& d) {
		accept(*d.parent);
	}
	void operator()(declarator_t& d) {
		accept(*d.parent);
	}
	void operator()(init_declarator_t& i) {
		accept(*i.parent);
	}
	void operator()(init_declarator_list_t& i) {
		accept(*i.parent);
	}
	void operator()(struct_or_union_specifier_t& i) {
		accept(*i.parent);
	}
	void operator ()(declaration_t& d) {
		declaration_found = &d;
	}
	void operator()(parameter_declaration_t& d) {
		declaration_found = &d;
	}
	void operator()(struct_declarator_list_t& s) {
		accept(*s.parent);
	}
	void operator()(struct_declarator_t& s) {
		visit(*s.parent);
	}
	void operator()(struct_declaration_t& s) {
		declaration_found = &s;
	}
	void operator()(function_definition_t& s) {
		declaration_found = &s;
	}
};

//! returns the declaration in which the given declarator is
inline declaration_base& declaration_from_declarator(declarator_t& dtor)
{
	func_visitor< declaration_from_declarator_t > v0;
	dtor.accept(v0);
	return *v0.functor().declaration_found;
}

//! returns the declaration in which the given identifier is the declarator
inline declaration_base& declaration_from_identifier(identifier_t& id)
{
	func_visitor< declaration_from_declarator_t > v0;
	id.parent->accept(v0);
	return *v0.functor().declaration_found;
}

inline struct_or_union_specifier_t& struct_rval_of_func(identifier_t& id)
{
	struct_type_specifier_of_declaration_t v;
//	std::cout << "DECLSPEC: " << declaration_from_identifier(*id._definition).decl_spec() << std::endl;
	declaration_from_identifier(*id._definition).decl_spec().accept(v);
	return *v.result;

/*	func_visitor< is<struct_return_value_of_function> > v;

	v0.functor().declaration_found->accept(v); // TODO: multiple parent calls?
	//return v.functor().value;
	return NULL; // TODO*/
	//return get_declarator(v0.functor().declaration_found);
}

#endif // UTILS_H

