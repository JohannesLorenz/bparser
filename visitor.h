#include <cstddef>
#include <iosfwd>
#include <cassert>

#ifndef VISITOR_H
#define VISITOR_H

class visitor_t
{
public:
	virtual void visit(type_specifier_simple_t* ) {}
	virtual void visit(number_t *) {}
	virtual void visit(expression_t *) {}
	virtual void visit(storage_class_specifier_t* ) {}
	virtual void visit(type_specifier_t* ) {}
	virtual void visit(type_qualifier_t* ) {}
	virtual void visit(function_specifier_t* ) {}
	virtual void visit(alignment_specifier_t* ) {}
	virtual void visit(declaration_list_t* ) {}
	virtual void visit(compound_statement_t* ) {}
	virtual void visit(pointer_t* ) {}
	virtual void visit(direct_declarator_t* ) {}
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
	virtual void visit(block_item_list_t* ) {}
	virtual void visit(block_item_t* ) {}
	virtual void visit(identifier_t* ) {}
	virtual void visit(primary_expression_t* ) {}
	virtual void visit(sizeof_expression_t* ) {}
	virtual void visit(init_declarator_list_t* ) {}
	template<class T>
	void visit(ch<T>& c) { visit((T*)c); }
	virtual ~visitor_t() {}
};

namespace std
{
	extern ostream cout;
}

class tree_visit_t : public visitor_t
{
	template<class T>
	bool tvisit(T ptr) { return ptr ? (visit(ptr), true) : false; }

	template<class Self, class T>
	void vvisit(Self* s, const std::list<T*>& v, std::size_t role)
	{
		std::size_t onv_id = 1; // on says: after that edge
		for(typename std::list<T*>::const_iterator itr = v.begin();
			itr != v.end(); ++itr, ++onv_id)
		{
			visit(*itr);
			on(s, role, onv_id);
		}
	}

	template<class Self, class T>
	void vaccept(Self* s, const std::list<T*>& v, std::size_t role)
	{
		std::size_t onv_id = 1; // on says: after that edge
		for(typename std::list<T*>::const_iterator itr = v.begin();
			itr != v.end(); ++itr, ++onv_id)
		{
			//if(!*itr)
			// throw "list element is NULL, can not happen.";
			(*itr)->accept(*this);
			on(s, role, onv_id);
		}
	}
	
	template<class T>
	class counter_t
	{
		T* node;
		tree_visit_t* tv;
		std::size_t time;
	public:
		counter_t(tree_visit_t* tv, T* node) : node(node), tv(tv), time(0) {}
		void next() { tv->on(node, ++time); }
	};
	template<class T>
	friend class counter_t;
	
protected:
	std::size_t depth;
	typedef std::size_t on_t;
public:
	virtual void on(type_specifier_simple_t* , on_t = 0, on_t = 0) {}
	virtual void on(number_t *, on_t = 0, on_t = 0) {}
	virtual void on(token_t* , on_t = 0, on_t = 0) {}
	virtual void on(expression_t *, on_t = 0, on_t = 0) {}
	virtual void on(expression_statement_t *, on_t = 0, on_t = 0) {}
	//void visit(node_t *e); //!< default
	virtual void on(storage_class_specifier_t*, on_t = 0, on_t = 0) {}
	virtual void on(type_specifier_t*, on_t = 0, on_t = 0) {}
	virtual void on(type_qualifier_t*, on_t = 0, on_t = 0) {}
	virtual void on(function_specifier_t*, on_t = 0, on_t = 0) {}
	virtual void on(alignment_specifier_t*, on_t = 0, on_t = 0) {}
	virtual void on(declaration_list_t*, on_t = 0, on_t = 0) {}
	virtual void on(compound_statement_t*, on_t = 0, on_t = 0) {}
	virtual void on(pointer_t*, on_t = 0, on_t = 0) {}
	virtual void on(direct_declarator_t*, on_t = 0, on_t = 0) {}
	virtual void on(declarator_t*, on_t = 0, on_t = 0) {}
	virtual void on(declaration_specifiers_t*, on_t = 0, on_t = 0) {}
	virtual void on(function_definition_t*, on_t = 0, on_t = 0) {}
	virtual void on(external_declaration_t*, on_t = 0, on_t = 0) {}
	virtual void on(translation_unit_t*, on_t = 0, on_t = 0) {}
	virtual void on(declaration_t*, on_t = 0, on_t = 0) {}
	virtual void on(iteration_statement_t*, on_t = 0, on_t = 0) {}
	virtual void on(identifier_t*, on_t = 0, on_t = 0) {}
	virtual void on(primary_expression_t*, on_t = 0, on_t = 0) {}
	virtual void on(sizeof_expression_t*, on_t = 0, on_t = 0) {}
	virtual void on(constant_t* , on_t = 0, on_t = 0) {}
	virtual void on(init_declarator_list_t*, on_t = 0, on_t = 0) {}

public:
	tree_visit_t() : depth(0) {}
	void visit(type_specifier_simple_t* e);
	void visit(number_t *e);
	void visit(token_t* e);
	void visit(expression_t *e);
	void visit(expression_statement_t *e);
	//void visit(node_t *e); //!< default
	void visit(storage_class_specifier_t* n);
	void visit(type_specifier_t* n);
	void visit(type_qualifier_t* n);
	void visit(function_specifier_t* n);
	void visit(alignment_specifier_t* n);
	void visit(declaration_list_t* n);
	void visit(compound_statement_t* n);
	void visit(pointer_t* n);
	void visit(direct_declarator_t* n);
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
	void visit(init_declarator_list_t* ) {}
};

class dumper_t : public tree_visit_t
{
	std::ostream& stream;
	void handle_depth();

public:
	dumper_t(std::ostream& stream = std::cout) : stream(stream) {}
	
	void on(type_specifier_simple_t* e, on_t role, on_t idx);
	void on(number_t *e, on_t role, on_t idx);
	void on(token_t* e, on_t role, on_t idx);
	void on(expression_t *e, on_t role, on_t idx);
	void on(expression_statement_t *e, on_t role, on_t idx);
	//void visit(node_t *e); //!< default
	void on(storage_class_specifier_t* n, on_t role, on_t idx);
	void on(type_specifier_t* n, on_t role, on_t idx);
	void on(type_qualifier_t* n, on_t role, on_t idx);
	void on(function_specifier_t* n, on_t role, on_t idx);
	void on(alignment_specifier_t* n, on_t role, on_t idx);
	void on(declaration_list_t* n, on_t role, on_t idx);
	void on(compound_statement_t* n, on_t role, on_t idx);
	void on(pointer_t* n, on_t role, on_t idx);
	void on(direct_declarator_t* n, on_t role, on_t idx);
	void on(declarator_t* n, on_t role, on_t idx);
	void on(declaration_specifiers_t* n, on_t role, on_t idx);
	void on(function_definition_t* n, on_t role, on_t idx);
	void on(external_declaration_t* n, on_t role, on_t idx);
	void on(translation_unit_t* n, on_t role, on_t idx);
	void on(declaration_t* n, on_t role, on_t idx);
	void on(iteration_statement_t* n, on_t role, on_t idx);
	void on(identifier_t* n, on_t role, on_t idx);
	void on(primary_expression_t* n, on_t role, on_t idx);
	void on(sizeof_expression_t* n, on_t role, on_t idx);
	void on(constant_t* , on_t role, on_t idx);
	void on(init_declarator_list_t* , on_t, on_t) {}
};

struct cleaner_t : visitor_t
{
	void visit(number_t *e) { delete e; }
	void visit(expression_t *e);
};

template<class T>
struct append_t : public visitor_t
{
	T* node; // TODO: const everywhere?
	append_t(T* node) : node(node) {}
	void visit(translation_unit_t* t) { t->v.push_back(node); }
	void visit(node_t* ) { assert(false); }
};

template<class T>
append_t<T> mk_append(T* node) { return append_t<T>(node); }

#endif // VISITOR_H

//node_t* echo(node_t* e);
