#include <cstddef>
#include <iosfwd>
#include <cassert>

#include <list>
#include <limits>

#include "tuple03.h"
#include "node_fwd.h"
#include "node.h" // FEATURE: can this be omitted?
#include "geom.h"

#ifndef VISITOR_H
#define VISITOR_H

// FEATURE: pointers->refs?
class visitor_t
{
public:
	virtual void visit(type_name_t *) {}
	virtual void visit(specifier_qualifier_list_t* ) {}

	virtual void visit(struct_or_union_specifier_t* ) {}
	virtual void visit(struct_declaration_list_t* ) {}
	virtual void visit(struct_declaration_t* ) {}
	virtual void visit(struct_declarator_list_t* ) {}
	virtual void visit(struct_declarator_t* ) {}
	virtual void visit(enum_specifier_t* ) {}
	virtual void visit(enumerator_list_t* ) {}
	virtual void visit(enumerator_t* ) {}
	virtual void visit(parameter_type_list_t* ) {}
	virtual void visit(parameter_list_t* ) {}
	virtual void visit(parameter_declaration_t* ) {}
	virtual void visit(identifier_list_t* ) {}

	virtual void visit(primary_expression_t* ) {}
	virtual void visit(iconstant_t* ) {}
	virtual void visit(fconstant_t* ) {}
	virtual void visit(constant_t<int>* ) {}
	virtual void visit(constant_t<float>* ) {}
	virtual void visit(constant_t<std::string>* ) {}
//	virtual void visit(primary_identifier_t* ) {}
//	virtual void visit(primary_expression_expression_t* ) {}

	virtual void visit(array_access_expression_t* ) {}
	virtual void visit(argument_expression_list_t* ) {}
	virtual void visit(function_call_expression_t* ) {}
	virtual void visit(struct_access_expression_t* ) {}
	virtual void visit(cast_postfix_expression_t* ) {}
	virtual void visit(cast_expression_t* ) {}

	//virtual void visit(type_specifier_simple_t* ) {}
	//virtual void visit(number_t *) {}
	//virtual void visit(expression_t *) {}
	virtual void visit(unary_expression_l* ) {}
	virtual void visit(unary_expression_r* ) {}
	virtual void visit(ternary_expression_t* ) {}
	virtual void visit(binary_expression_t* ) {}
	virtual void visit(storage_class_specifier_t* ) {}
	//virtual void visit(type_specifier_token* ) {}
	//virtual void visit(type_identifier* ) {}
	virtual void visit(type_specifier_t* ) {}
	virtual void visit(type_qualifier_t* ) {}
	virtual void visit(type_qualifier_list_t* ) {}
	virtual void visit(function_specifier_t* ) {}
	virtual void visit(alignment_specifier_t* ) {}
	virtual void visit(declaration_list_t* ) {}
	virtual void visit(compound_statement_t* ) {}
	virtual void visit(pointer_t* ) {}
	virtual void visit(declarator_t* ) {}
	virtual void visit(declaration_specifiers_t* ) {}
	virtual void visit(function_definition_t* ) {}
	virtual void visit(external_declaration_t* ) {}
	virtual void visit(translation_unit_t *) {}
	virtual void visit(token_t* ) {}
	virtual void visit(declaration_t* ) {}
//	virtual void visit(constant_t* ) {}
	virtual void visit(labeled_statement_t* ) {}
	virtual void visit(expression_statement_t* ) {}
	virtual void visit(selection_statement_t* ) {}
	virtual void visit(iteration_statement_t* ) {}
	virtual void visit(jump_statement_t* ) {}
	//virtual void visit(block_item_list_t* ) {}
	virtual void visit(block_item_t* ) {}
	virtual void visit(identifier_t* ) {}
	virtual void visit(enumeration_constant_t* ) {}
	virtual void visit(typedef_name_t* ) {}
	virtual void visit(string_literal_t* ) {}
	virtual void visit(sizeof_expression_t* ) {}
	virtual void visit(init_declarator_list_t* ) {}
	virtual void visit(init_declarator_t* ) {}
	virtual void visit(initializer_t* ) {}
	virtual void visit(initializer_list_t* ) {}
	virtual void visit(designator_list_t* ) {}
	virtual void visit(designator_id* ) {}
	virtual void visit(designator_constant_expr* ) {}

	virtual void visit(abstract_declarator_t* ) {}

	virtual void visit(direct_declarator_id* ) {}
	virtual void visit(direct_declarator_decl* ) {}
	virtual void visit(direct_declarator_arr* ) {}
	virtual void visit(direct_declarator_func* ) {}
	virtual void visit(direct_declarator_idlist* ) {}
	virtual void visit(direct_abstract_declarator_t* ) {}
	virtual void visit(direct_abstract_declarator_decl* ) {}
	virtual void visit(direct_abstract_declarator_arr* ) {}
	virtual void visit(direct_abstract_declarator_func* ) {}
	virtual void visit(direct_declarator_t* ) {}

/*	template<class T>
	void visit(ptn<T>& c) { visit((T*)c); }*/
	virtual ~visitor_t() {}


	template<class T>
	bool tvisit(T ptr) { return ptr ? (visit(ptr), true) : false; }

	template<class T>
	void vvisit(const std::list<T*>& v)
	{
		for(typename std::list<T*>::const_iterator itr = v.begin();
			itr != v.end(); ++itr)
		{
			visit(*itr);
		}
	}

	template<class T>
	void vaccept(const std::list<T*>& v)
	{
		for(typename std::list<T*>::const_iterator itr = v.begin();
			itr != v.end(); ++itr)
		{
			//if(!*itr)
			// throw "list element is NULL, can not happen.";
			(*itr)->accept(*this);
		}
	}

	template<class T> // FEATURE: remove this func?!
	void accept_if_nontoken(T* const& non_token) {
		if(non_token) non_token->accept(*this);
	}

	//void accept_if_nontoken(token_t* const & ) {}


	template<class T, class Next>
	void accept_all(const tpl<T, Next>& t) { accept_if_nontoken(t.value); accept_all(t.get_next()); }

	template<class T>
	void accept_all(const tpl<T, null_type>& t) { accept_if_nontoken(t.value); }

	template<class T>
	void visit_if_nontoken(T* const& non_token) {
		if(non_token)
		 visit(non_token);
	}

	//void visit_if_nontoken(token_t* const& ) {}

	template<class T, class Next>
	void visit_all(const tpl<T, Next>& t) { visit_if_nontoken(t.value); visit_all(t.get_next()); }

	template<class T>
	void visit_all(const tpl<T, null_type>& t) { visit_if_nontoken(t.value); }

	template<class T>
	void xaccept(const std::list<T*>& v) { vaccept(v); }

	template<class T, class Next>
	void xaccept(const tpl<T, Next>& t) { accept_all(t); }

	void xaccept(const null_type& ) {}

	/*template<class T>
	void xaccept(const ch<T>& c) { if(c) (T* c)->accept(*this); }*/

	// terminals, nothing to do...
	/*void xaccept(const int&) {} -> remove?
	void xaccept(const float&) {}
	void xaccept(const std::string&) {}*/

	// other case: non-terminal -> node ?
	//void xaccept(node_base* node) { node->accept(*this); } ... -> const version?
};

namespace std
{
	extern ostream cout;
}

template<class Functor>
class func_visitor : public visitor_t
{
protected:
	Functor ftor;
private:
	template<class T>
	void f(T* n) { ftor(*n); }
public:
	func_visitor() : ftor(this) {}

	template<class FConstrPar>
	func_visitor(const FConstrPar& constr) : ftor(this, constr) {}

	void visit(iconstant_t* n) { f(n); }
	void visit(fconstant_t* n) { f(n); }

	void visit(type_name_t *n) { f(n); }
	void visit(specifier_qualifier_list_t* n) { f(n); }

	void visit(struct_or_union_specifier_t* n) { f(n); }
	void visit(struct_declaration_list_t* n) { f(n); }
	void visit(struct_declaration_t* n) { f(n); }
	void visit(struct_declarator_list_t* n) { f(n); }
	void visit(struct_declarator_t* n) { f(n); }
	void visit(enum_specifier_t* n) { f(n); }
	void visit(enumerator_list_t* n) { f(n); }
	void visit(enumerator_t* n) { f(n); }
	void visit(parameter_type_list_t* n) { f(n); }
	void visit(parameter_list_t* n) { f(n); }
	void visit(parameter_declaration_t* n) { f(n); }
	void visit(identifier_list_t* n) { f(n); }

	void visit(primary_expression_t* n) { f(n); }
	void visit(constant_t<int>* n) { f(n); }
	void visit(constant_t<float>* n) { f(n); }
	void visit(constant_t<std::string>* n) { f(n); }
//	void visit(primary_identifier_t* n) { f(n); }
//	void visit(primary_expression_expression_t* n) { f(n); }

	void visit(array_access_expression_t* n) { f(n); }
	void visit(argument_expression_list_t* n) { f(n); }
	void visit(function_call_expression_t* n) { f(n); }
	void visit(struct_access_expression_t* n) { f(n); }
	void visit(cast_postfix_expression_t* n) { f(n); }
	void visit(cast_expression_t* n) { f(n); }

	//void visit(type_specifier_simple_t* n) { f(n); }
	//void visit(number_t *n) { f(n); }
	//void visit(expression_t *n) { f(n); }
	void visit(unary_expression_l* n) { f(n); }
	void visit(unary_expression_r* n) { f(n); }
	void visit(ternary_expression_t* n) { f(n); }
	void visit(binary_expression_t* n) { f(n); }
#if 0
	void visit(storage_class_specifier_t* n) { f(n); }
#endif
	//void visit(type_specifier_token* n) { f(n); }
	//void visit(type_identifier* n) { f(n); }
	void visit(type_specifier_t* n) { f(n); }
	void visit(type_qualifier_t* n) { f(n); }
	void visit(type_qualifier_list_t* n) { f(n); }
#if 0
	void visit(function_specifier_t* n) { f(n); }
	void visit(alignment_specifier_t* n) { f(n); }
#endif
	void visit(declaration_list_t* n) { f(n); }
	void visit(compound_statement_t* n) { f(n); }
	void visit(pointer_t* n) { f(n); }
	void visit(declarator_t* n) { f(n); }
	void visit(declaration_specifiers_t* n) { f(n); }
	void visit(function_definition_t* n) { f(n); }
	void visit(external_declaration_t* n) { f(n); }
	void visit(translation_unit_t *n) { f(n); }
	void visit(token_t* n) { f(n); }
	void visit(declaration_t* n) { f(n); }
//	void visit(constant_t* n) { f(n); }
	void visit(labeled_statement_t* n) { f(n); }
	void visit(expression_statement_t* n) { f(n); }
	void visit(selection_statement_t* n) { f(n); }
	void visit(iteration_statement_t* n) { f(n); }
	void visit(jump_statement_t* n) { f(n); }
	//void visit(block_item_list_t* n) { f(n); }
	void visit(block_item_t* n) { f(n); }
	void visit(identifier_t* n) { f(n); }
	void visit(enumeration_constant_t* n) { f(n); }
	void visit(typedef_name_t* n) { f(n); }
	void visit(string_literal_t* n) { f(n); }
	void visit(sizeof_expression_t* n) { f(n); }
	void visit(init_declarator_list_t* n) { f(n); }
	void visit(init_declarator_t* n) { f(n); }
	void visit(initializer_t* n) { f(n); }
	void visit(initializer_list_t* n) { f(n); }
	void visit(designator_list_t* n) { f(n); }
	void visit(designator_id* n) { f(n); }
	void visit(designator_constant_expr* n) { f(n); }

	void visit(abstract_declarator_t* n) { f(n); }

	void visit(direct_declarator_id* n) { f(n); }
	void visit(direct_declarator_decl* n) { f(n); }
	void visit(direct_declarator_arr* n) { f(n); }
	void visit(direct_declarator_func* n) { f(n); }
	void visit(direct_declarator_idlist* n) { f(n); }
	void visit(direct_abstract_declarator_t* n) { f(n); }
	void visit(direct_abstract_declarator_decl* n) { f(n); }
	void visit(direct_abstract_declarator_arr* n) { f(n); }
	void visit(direct_abstract_declarator_func* n) { f(n); }
	void visit(direct_declarator_t* n) { f(n); }
};

class ftor_base
{
	visitor_t* vref; // FEATURE: this can not be avoided?
			// required for the accept() calls?

protected:
	template<class T>
	void vvisit(const std::list<T*>& v)
	{
		for(typename std::list<T*>::const_iterator itr = v.begin();
			itr != v.end(); ++itr)
		{
			visit(*itr);
		}
	}

	template<class T>
	void vaccept(const std::list<T*>& v)
	{
		for(typename std::list<T*>::const_iterator itr = v.begin();
			itr != v.end(); ++itr)
		{
			//if(!*itr)
			// throw "list element is NULL, can not happen.";
			(*itr)->accept(*vref);
		}
	}

	template<class T> // FEATURE: remove this func?! non_token: wrong name
	void accept_if_nontoken(T* const& non_token) {
		if(non_token) non_token->accept(*vref);
	}

	template<class T>
	void accept_if_nontoken(std::list<T*>* const& list) {
		if(list) vaccept(*list);
	}

	//void accept_if_nontoken(token_t* const & ) {}


	template<class T, class Next>
	void accept_all(const tpl<T, Next>& t) { accept_if_nontoken(t.value); accept_all(t.get_next()); }

	template<class T>
	void accept_all(const tpl<T, null_type>& t) { accept_if_nontoken(t.value); }

	template<class T>
	void visit_if_nontoken(T* const& non_token) {
		if(non_token)
		 visit(non_token);
	}

	//void visit_if_nontoken(token_t* const& ) {}

	template<class T, class Next>
	void visit_all(const tpl<T, Next>& t) { visit_if_nontoken(t.value); visit_all(t.get_next()); }

	template<class T>
	void visit_all(const tpl<T, null_type>& t) { visit_if_nontoken(t.value); }

	template<class T>
	void xaccept(const std::list<T*>& v) { vaccept(v); }

	template<class T, class Next>
	void xaccept(const tpl<T, Next>& t) { accept_all(t); }

	void xaccept(const null_type& ) {}
public:
	ftor_base(visitor_t* vref) : vref(vref) {}
};

struct enter {};
struct leave {};

template<class Functor2>
class io_visitor;
//#include <typeinfo>
template<class Functor2>
class io_functor : ftor_base
{
	Functor2 f;
public:
	template<class F2Constr> // FEATURE: solution with variable args
	io_functor(visitor_t* vref, const F2Constr& constr) :
		ftor_base(vref),
		f(vref, constr) {}
	io_functor(visitor_t* vref) :
		ftor_base(vref),
		f(vref) {}
	template<class T>
	void operator()(T& n) {
	//	std::cout << n.span << ": io: " << typeid(n).name() << std::endl; 
		f(n, enter());
		xaccept(n.c);
		f(n, leave());
	}
};

template<class Functor2>
class io_visitor : public func_visitor< io_functor<Functor2> >
{
public:
	//io_visitor() : iof(this) {}
	template<class F2Constr> // FEATURE: solution with variable args
	io_visitor(const F2Constr& constr)
		: func_visitor< io_functor<Functor2> >(constr) {}

	/*template<class NodeType>
	void operator()(NodeType& n) { iof(n); }*/
};

class fwd : public visitor_t
{
protected:
	std::size_t depth;

	void handle_depth();

public:
	fwd() : depth(0) {}
	virtual ~fwd() {}

	void visit(type_name_t *t);
	void visit(specifier_qualifier_list_t* );

	void visit(struct_or_union_specifier_t* );
	void visit(struct_declaration_list_t* );
	void visit(struct_declaration_t* );
	void visit(struct_declarator_list_t* );
	void visit(struct_declarator_t* );
	void visit(enum_specifier_t* );
	void visit(enumerator_list_t* );
	void visit(enumerator_t* );
	void visit(parameter_type_list_t* );
	void visit(parameter_list_t* );
	void visit(parameter_declaration_t* );
	void visit(identifier_list_t* );

	void visit(primary_expression_t* );
	void visit(iconstant_t* ) {}
	void visit(fconstant_t* ) {}
	void visit(constant_t<int>* ) {}
	void visit(constant_t<float>* ) {}
	void visit(constant_t<std::string>* ) {}
//	void visit(primary_identifier_t* n);
//	void visit(primary_expression_expression_t* n);

	void visit(array_access_expression_t* e);
	void visit(argument_expression_list_t* e);
	void visit(function_call_expression_t* e);
	void visit(struct_access_expression_t* e);
	void visit(cast_postfix_expression_t* e);
	void visit(cast_expression_t* e);
	//void visit(type_specifier_simple_t* e);
	//void visit(number_t *e);
	void visit(token_t* e);
	void visit(unary_expression_l *e);
	void visit(unary_expression_r *e);
	void visit(ternary_expression_t *e);
	void visit(binary_expression_t *e);
	void visit(expression_statement_t *e);
	//void visit(node_t *e); //!< default
	void visit(storage_class_specifier_t* n);
	//void visit(type_specifier_token* t);
	//void visit(type_identifier* );
	void visit(type_specifier_t* t);
	void visit(type_qualifier_t* n);
	void visit(type_qualifier_list_t* n);
	void visit(function_specifier_t* n);
	void visit(alignment_specifier_t* n);
	void visit(declaration_list_t* n);
	void visit(compound_statement_t* n);
	void visit(pointer_t* n);
	void visit(declarator_t* n);
	void visit(declaration_specifiers_t* n);
	void visit(function_definition_t* n);
	void visit(external_declaration_t* n);
	void visit(translation_unit_t* n);
	void visit(declaration_t* n);
	void visit(iteration_statement_t* n);
	void visit(identifier_t* ) {}
	void visit(enumeration_constant_t* ) {}
	void visit(typedef_name_t* ) {}

	void visit(string_literal_t* ) {}
	void visit(sizeof_expression_t* n);
//	void visit(constant_t* );
	void visit(init_declarator_t* );
	void visit(init_declarator_list_t* );
	void visit(initializer_t* );
	void visit(initializer_list_t* );
	void visit(designator_list_t* );
	void visit(designator_id* );
	void visit(designator_constant_expr* );

	void visit(abstract_declarator_t* );

	void visit(direct_declarator_id* );
	void visit(direct_declarator_decl* );
	void visit(direct_declarator_arr* );
	void visit(direct_declarator_func* );
	void visit(direct_declarator_idlist* );
	void visit(direct_abstract_declarator_decl* );
	void visit(direct_abstract_declarator_arr* );
	void visit(direct_abstract_declarator_func* );


};

class fwd_functor : ftor_base
{
	std::size_t depth;

public:
	fwd_functor(visitor_t* vref) : ftor_base(vref), depth(0) {}

	template<class NodeType>
	void operator()(const NodeType& nt) { ++depth; xaccept(nt.c); --depth; }
};

class type_completor : ftor_base
{
public:
	type_completor(visitor_t* vref) : ftor_base(vref) {}

	// expressions...
	void operator()(unary_expression_l& );
	void operator()(unary_expression_r& );
	void operator()(binary_expression_t& );

	// FEATURE: float, int

	// statements...
	void operator()(iteration_statement_t& );
	void operator()(labeled_statement_t& );
	void operator()(jump_statement_t& );
	void operator()(struct_or_union_specifier_t& );


	// default case
	template<class NodeType>
	void operator()(/*const*/ NodeType& n) { xaccept(n.c); }
};

// TODO: all private functions?
template<class T> // TODO: also send file via parameter?
inline span_t get_span_min(const ptn<T, null_type>& c) {

	return c.value
		? c.value->span
		: span_t(	geom_t(0, std::numeric_limits<int>::max(),
					std::numeric_limits<int>::max()),
				geom_t(0, std::numeric_limits<int>::min(),
					std::numeric_limits<int>::min())
			);
}

template<class T, class N>
inline span_t get_span_min(const ptn<T, N>& c)
{
	span_t next_span = get_span_min(c.get_next());
	//std::cout << "VALUE: " << c.value << std::endl;
	return c.value
		? span_t(std::min(c.value->span.first, next_span.first),
			std::max(c.value->span.second, next_span.second))
		: next_span;
}

template<class T, class N>
inline span_t get_span_min(const ptn<std::list<T*>, N>& c)
{
	span_t next_span = get_span_min(c.get_next());
	span_t list_span = get_span_min(*c.value);
	//std::cout << "VALUE: " << c.value << std::endl;
	return c.value
		? span_t(std::min(list_span.first, next_span.first),
			std::max(list_span.second, next_span.second))
		: next_span;
}

// if we have a tuple of lists...
template<class T>
inline span_t get_span_min(const std::list<T*>& c) {
	return span_t(c.front()->span.first, c.back()->span.second);
}

template<class T>
inline span_t get_span(const T& c) {
	return get_span_min(c);
}

template<class T>
inline span_t get_span(const std::list<T*>& c) {
	return span_t(c.front()->span.first, c.back()->span.second);
}

inline span_t get_span(const null_type&) {
	throw "should not happen. error in grammar!";
	return span_t();
}

class geom_completor : ftor_base
{
	template<class T>
	void add_geom(T& n)
	{
		std::size_t newlines = n.newlines(),
			length = n.length(); // FEATURE: unify with lexer.l?
		geom_t& end = n.span.second;
		geom_t& start = n.span.first;
		end.file_id = start.file_id;
		if(newlines)
		{
			end.line = start.line + newlines;
			end.col = 1 + length;
		}
		else
		{
			end.line = start.line;
			end.col = start.col + length;
		}
	}
public:
	geom_completor(visitor_t* vref) : ftor_base(vref) {}

	// default case
	// TODO: use node& and terminal& below?
	template<class NodeType, class CT>
	void operator()(/*const*/ NodeType& n) {
		xaccept(n.c);
		n.span = get_span(n.c);
	}

	// terminals have no children
	//  -> get geom directly from them
	// TODO: make sure these are all terminals
	void operator()(/*const*/ token_t& n) { add_geom(n); }
	void operator()(/*const*/ iconstant_t& n) { add_geom(n); }
	void operator()(/*const*/ fconstant_t& n) { add_geom(n); }
	void operator()(/*const*/ identifier_t& n) { add_geom(n); }
	void operator()(enumeration_constant_t& n) { add_geom(n); }
	void operator()(typedef_name_t& n) { add_geom(n); }

	void operator()(/*const*/ string_literal_t& n) { add_geom(n); }
};


class ffwd : public func_visitor< fwd_functor >
{
};

class dumper_t : public fwd
{
protected:
	std::ostream& stream;

public:
	dumper_t(std::ostream& stream = std::cout) : stream(stream) {}
	virtual ~dumper_t() {}

	void visit(type_name_t *t);
	void visit(specifier_qualifier_list_t* );

	void visit(struct_or_union_specifier_t* );
	void visit(struct_declaration_list_t* );
	void visit(struct_declaration_t* );
	void visit(struct_declarator_list_t* );
	void visit(struct_declarator_t* );
	void visit(enum_specifier_t* );
	void visit(enumerator_list_t* );
	void visit(enumerator_t* );
	void visit(parameter_type_list_t* );
	void visit(parameter_list_t* );
	void visit(parameter_declaration_t* );
	void visit(identifier_list_t* );

	void visit(iconstant_t* c);
	void visit(fconstant_t* c);
	void visit(constant_t<int>* );
	void visit(constant_t<float>* );
	void visit(constant_t<std::string>* );
	void visit(primary_expression_t* );
//	void visit(primary_identifier_t* );
//	void visit(primary_expression_expression_t* );

	void visit(array_access_expression_t* e);
	void visit(argument_expression_list_t* e);
	void visit(function_call_expression_t* e);
	void visit(struct_access_expression_t* e);
	void visit(cast_postfix_expression_t* e);
	void visit(cast_expression_t* e);
	//void visit(type_specifier_simple_t* e);
	//void visit(number_t *e);
	void visit(token_t* e);
	void visit(unary_expression_l *e);
	void visit(unary_expression_r *e);
	void visit(ternary_expression_t *e);
	void visit(binary_expression_t *e);
	void visit(expression_statement_t *e);
	//void visit(node_t *e); //!< default
	void visit(storage_class_specifier_t* n);
	//void visit(type_specifier_token* t);
	//void visit(type_identifier* );
	void visit(type_specifier_t* t);
	void visit(type_qualifier_t* n);
	void visit(function_specifier_t* n);
	void visit(alignment_specifier_t* n);
	void visit(declaration_list_t* n);
	void visit(compound_statement_t* n);
	void visit(pointer_t* n);
	void visit(declarator_t* n);
	void visit(declaration_specifiers_t* n);
	void visit(function_definition_t* n);
	void visit(external_declaration_t* n);
	void visit(translation_unit_t* n);
	void visit(declaration_t* n);
	void visit(iteration_statement_t* n);
	void visit(identifier_t* n);
	void visit(enumeration_constant_t* n);
	void visit(typedef_name_t* n);
	void visit(string_literal_t* s);
	void visit(sizeof_expression_t* n);
//	void visit(constant_t* );
	void visit(init_declarator_t* );
	void visit(init_declarator_list_t* );
	void visit(initializer_t* );
	void visit(initializer_list_t* );
	void visit(designator_list_t* );
	void visit(designator_id* );
	void visit(designator_constant_expr* );

	void visit(abstract_declarator_t* );

	void visit(direct_declarator_id* );
	void visit(direct_declarator_decl* );
	void visit(direct_declarator_arr* );
	void visit(direct_declarator_func* );
	void visit(direct_declarator_idlist* );
	void visit(direct_abstract_declarator_decl* );
	void visit(direct_abstract_declarator_arr* );
	void visit(direct_abstract_declarator_func* );


}; // TODO: identifier...




struct cleaner_t : visitor_t
{
	//void visit(number_t *e) { delete e; }
	void visit(binary_expression_t *e);
};

/*
template<class T>
struct append_t : public visitor_t
{
	T* node; // TODO: const everywhere?
	append_t(T* node) : node(node) {}
	void visit(translation_unit_t* t) { t->c.push_back(node); }
	void visit(node_base* ) { assert(false); }
};

template<class T>
append_t<T> mk_append(T* node) { return append_t<T>(node); }
*/

#endif // VISITOR_H

//node_t* echo(node_t* e);
