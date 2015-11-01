#include <cstddef>
#include <iosfwd>
#include <cassert>

#ifndef VISITOR_H
#define VISITOR_H

class visitor_t
{
public:
	virtual void visit(type_name_t *) {}
	virtual void visit(specifier_qualifier_list_t* ) {}

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
	virtual void visit(type_specifier_token* ) {}
	virtual void visit(type_identifier* ) {}
	virtual void visit(type_specifier_t* ) {}
	virtual void visit(type_qualifier_t* ) {}
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
	virtual void visit(constant_t* ) {}
	virtual void visit(labeled_statement_t* ) {}
	virtual void visit(expression_statement_t* ) {}
	virtual void visit(selection_statement_t* ) {}
	virtual void visit(iteration_statement_t* ) {}
	virtual void visit(jump_statement_t* ) {}
	//virtual void visit(block_item_list_t* ) {}
	virtual void visit(block_item_t* ) {}
	virtual void visit(identifier_t* ) {}
	virtual void visit(primary_expression_t* ) {}
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
	virtual void visit(parameter_type_list_t* ) {}
	virtual void visit(direct_declarator_func* ) {}
	virtual void visit(direct_abstract_declarator_t* ) {}
	virtual void visit(direct_abstract_declarator_decl* ) {}
	virtual void visit(direct_abstract_declarator_arr* ) {}
	virtual void visit(direct_abstract_declarator_func* ) {}
	virtual void visit(direct_declarator_t* ) {}

	template<class T>
	void visit(ch<T>& c) { visit((T*)c); }
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

	template<class T>
	void accept_if_nontoken(T* const& non_token) {
		if(non_token) non_token->accept(*this);
	}

	void accept_if_nontoken(token_t* const & ) {}


	template<class T, class Next>
	void accept_all(const tpl<T, Next>& t) { accept_if_nontoken(t.value); accept_all(t.get_next()); }

	template<class T>
	void accept_all(const tpl<T, null_type>& t) { accept_if_nontoken(t.value); }

	template<class T>
	void visit_if_nontoken(T* const& non_token) {
		if(non_token)
		 visit(non_token);
	}

	void visit_if_nontoken(token_t* const& ) {}

	template<class T, class Next>
	void visit_all(const tpl<T, Next>& t) { visit_if_nontoken(t.value); visit_all(t.get_next()); }

	template<class T>
	void visit_all(const tpl<T, null_type>& t) { visit_if_nontoken(t.value); }
};

namespace std
{
	extern ostream cout;
}

class dumper_t : public visitor_t
{
protected:
	std::size_t depth;
	typedef std::size_t on_t;

	std::ostream& stream;
	void handle_depth();

public:
	dumper_t(std::ostream& stream = std::cout) : depth(0), stream(stream) {}
	virtual ~dumper_t() {}

	void visit(type_name_t *t);
	void visit(specifier_qualifier_list_t* );
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
	void visit(type_specifier_token* t);
	void visit(type_identifier* );
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
	void visit(primary_expression_t* n);
	void visit(sizeof_expression_t* n);
	void visit(constant_t* );
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
	void visit(parameter_type_list_t* );
	void visit(direct_declarator_func* );
	void visit(direct_abstract_declarator_decl* );
	void visit(direct_abstract_declarator_arr* );
	void visit(direct_abstract_declarator_func* );


}; // TODO: identifier...




struct cleaner_t : visitor_t
{
	//void visit(number_t *e) { delete e; }
	void visit(binary_expression_t *e);
};

template<class T>
struct append_t : public visitor_t
{
	T* node; // TODO: const everywhere?
	append_t(T* node) : node(node) {}
	void visit(translation_unit_t* t) { t->v.push_back(node); }
	void visit(node_base* ) { assert(false); }
};

template<class T>
append_t<T> mk_append(T* node) { return append_t<T>(node); }

#endif // VISITOR_H

//node_t* echo(node_t* e);
