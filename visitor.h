#include <cstddef>
#include <iosfwd>
#include <cassert>

#ifndef VISITOR_H
#define VISITOR_H

class visitor_t
{
public:
	virtual void visit(type_specifier_simple_t* e) {}
	virtual void visit(number_t *e) {}
	virtual void visit(expression_t *e) {}
	virtual void visit(storage_class_specifier_t* e) {}
	virtual void visit(type_specifier_t* e) {}
	virtual void visit(type_qualifier_t* e) {}
	virtual void visit(function_specifier_t* e) {}
	virtual void visit(alignment_specifier_t* e) {}
	virtual void visit(declaration_list_t* e) {}
	virtual void visit(compound_statement_t* e) {}
	virtual void visit(pointer_t* e) {}
	virtual void visit(direct_declarator_t* e) {}
	virtual void visit(declarator_t* e) {}
	virtual void visit(declaration_specifiers_t* e) {}
	virtual void visit(function_definition_t* e) {}
	virtual void visit(external_declaration_t* e) {}
	virtual void visit(translation_unit_t *e) {}
	virtual void visit(token_t* t) {}
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
	virtual void visit(sizeof_expression_t* n) {}
	virtual void visit(init_declarator_list_t* n) {}
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
	std::size_t depth;

	template<class T>
	bool tvisit(T ptr) { return ptr ? (visit(ptr), true) : false; }

	template<class T>
	void vvisit(const std::list<T*>& v)
	{
		std::size_t onv_id = 0;
		for(typename std::list<T*>::const_iterator itr = v.begin();
			itr != v.end(); ++itr, ++onv_id)
		{
			visit(*itr);
			onv(onv_id);
		}
	}

	template<class T>
	void vaccept(const std::list<T*>& v)
	{
		std::size_t onv_id = 0;
		for(typename std::list<T*>::const_iterator itr = v.begin();
			itr != v.end(); ++itr, ++onv_id)
		{
			//if(!*itr)
			// throw "list element is NULL, can not happen.";
			(*itr)->accept(*this);
			onv(onv_id);
		}
	}
	
	class counter_t
	{
		tree_visit_t* tv;
		std::size_t time;
	public:
		counter_t(tree_visit_t* tv) : tv(tv), time(0) {}
		void next() { tv->on(++time); }
	};
	friend class counter_t;
	
	typedef std::size_t on_t;
	virtual void on(on_t type_id) {};
	virtual void onv(on_t type_id) {};

public:
	tree_visit_t(std::ostream& stream = std::cout) : depth(0), stream(stream) {};
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
	void visit(init_declarator_list_t* n) {}
};

class dumper_t : public tree_visit_t
{
	std::ostream& stream;
	void handle_depth();

public:
	tree_visit_t(std::ostream& stream = std::cout) : depth(0), stream(stream) {};
	void on(type_specifier_simple_t* e, on_t id);
	void visit(number_t *e, on_t id);
	void visit(token_t* e, on_t id);
	void visit(expression_t *e, on_t id);
	void visit(expression_statement_t *e, on_t id);
	//void visit(node_t *e); //!< default
	void visit(storage_class_specifier_t* n, on_t id);
	void visit(type_specifier_t* n, on_t id);
	void visit(type_qualifier_t* n, on_t id);
	void visit(function_specifier_t* n, on_t id);
	void visit(alignment_specifier_t* n, on_t id);
	void visit(declaration_list_t* n, on_t id);
	void visit(compound_statement_t* n, on_t id);
	void visit(pointer_t* n, on_t id);
	void visit(direct_declarator_t* n, on_t id);
	void visit(declarator_t* n, on_t id);
	void visit(declaration_specifiers_t* n, on_t id);
	void visit(function_definition_t* n, on_t id);
	void visit(external_declaration_t* n, on_t id);
	void visit(translation_unit_t* n, on_t id);
	void visit(declaration_t* n, on_t id);
	void visit(iteration_statement_t* n, on_t id);
	void visit(identifier_t* n, on_t id);
	void visit(primary_expression_t* n, on_t id);
	void visit(sizeof_expression_t* n, on_t id);
	void visit(constant_t* , on_t id);
	void visit(init_declarator_list_t* n, on_t id) {}
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
