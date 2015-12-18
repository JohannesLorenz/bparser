#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "visitor.h"

class function_called : public visitor_t
{
	identifier_t* _identifier;

	void set_id(identifier_t* i) {
		if(!_identifier) // has already been found
		 _identifier = i; }
public:
	identifier_t* get_identifier() { return _identifier; }
	function_called() : _identifier(NULL) {}
	// maybe future ideas, especially ternary
	//void visit(unary_expression_r &u);
	//void visit(unary_expression_l &u);
	//void visit(binary_expression_t &b);
	// void visit(ternary_expression_t &t);
	// interesting
	void visit(primary_expression_t &p);
	void visit(cast_expression_t& c);
	// unlikely...
	//void visit(array_access_expression_t &a);
};


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

template<class T>
inline bool is(node_base& node)
{
	func_visitor< _is<T> > v;
	node.accept(v);
	return v.functor().value;
}

inline bool is_func_id(identifier_t& id)
{
	return is<direct_declarator_func>(*id._definition->parent);
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

inline identifier_t* get_declarator(declarator_t* d)
{
	func_visitor< get_declarator_t > v;
	v.functor()(*d);
	return v.functor().value;
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

struct struct_type_specifier_of_declaration_t : visitor_t
{
	identifier_t* result;
	struct_type_specifier_of_declaration_t() : result(NULL) {}
	void visit(declaration_t & d) { tvisit(d.c.get<0>()); }
	void visit(declaration_specifiers_t & d) { vaccept(d.c); }
	void visit(type_specifier_t & t) { t.c.get<0>()->accept(*this); }
	void visit(struct_or_union_specifier_t& s) {
		result = s.c.get<struct_or_union_specifier_t::identifier>(); }
	void visit(typedef_name_t& t); // see below
};

// FEATURE: structs etc?
struct declaration_from_declarator_t : ftor_base
{
	/*enum declaration_type
	{
		decl_type_declaration,
		decl_type_function
	};

	union
	{
		declaration_t* declaration;
		function_definition_t* function;
	} declaration_found;*/
	declaration_t* declaration_found;

	declaration_from_declarator_t(visitor_t* vref) :
		ftor_base(vref) {}

	void operator()(node_base& ) { declaration_found = NULL; }
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
	void operator ()(declaration_t& d) {
		declaration_found = &d;
	}
};

inline declaration_t* declaration_from_declarator(declarator_t* dtor)
{
	func_visitor< declaration_from_declarator_t > v0;
	dtor->accept(v0);
	return v0.functor().declaration_found;
}

inline declaration_t* declaration_from_identifier(identifier_t* id)
{
	func_visitor< declaration_from_declarator_t > v0;
	id->parent->accept(v0);
	return v0.functor().declaration_found;
}

inline identifier_t* struct_rval_of_func(identifier_t& id)
{
	struct_type_specifier_of_declaration_t v;
	declaration_from_identifier(id._definition)->accept(v);

	return v.result;

/*	func_visitor< is<struct_return_value_of_function> > v;

	v0.functor().declaration_found->accept(v); // TODO: multiple parent calls?
	//return v.functor().value;
	return NULL; // TODO*/
	//return get_declarator(v0.functor().declaration_found);
}

#endif // UTILS_H

