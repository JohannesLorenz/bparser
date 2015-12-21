//! @file node_fwd.h forward declarations for node classes

class node_t;

namespace nodes {

// virtual nodes:
struct statement_t;
struct noconst_terminal_t;

// other nodes:
struct type_name_t;
struct specifier_qualifier_list_t;

struct struct_or_union_specifier_t;
struct struct_declaration_list_t;
struct struct_declaration_t;
struct struct_declarator_list_t;
struct struct_declarator_t;
struct enum_specifier_t;
struct enumerator_list_t;
struct enumerator_t;
struct parameter_type_list_t;
struct parameter_list_t;
struct parameter_declaration_t;
struct identifier_list_t;

struct primary_expression_t;
struct iconstant_t;
struct fconstant_t;
//template<class T>
//struct constant_t;
struct primary_identifier_t;
struct primary_expression_expression_t;

struct array_access_expression_t;
struct argument_expression_list_t;
struct function_call_expression_t;
struct struct_access_expression_t;
struct compound_literal_t;
struct cast_expression_t;

struct type_specifier_simple_t;
struct number_t;
struct expression_t;
struct assignment_expression_t;
struct unary_expression_l;
struct unary_expression_r;
struct ternary_expression_t;
struct binary_expression_t;
struct storage_class_specifier_t;
struct type_specifier_token;
struct type_identifier;
struct type_specifier_t;
struct type_qualifier_t;
struct type_qualifier_list_t;
struct function_specifier_t;
struct alignment_specifier_t;
struct declaration_list_t;
struct compound_statement_t;
struct pointer_t;
struct declarator_t;
struct declaration_specifiers_t;
struct function_definition_t;
struct external_declaration_t;
struct translation_unit_t;
struct token_t;
struct declaration_t;
struct labeled_statement_t;
struct expression_statement_t;
struct selection_statement_t;
struct iteration_statement_t;
struct jump_statement_t;
struct block_item_list_t;
struct block_item_t;
struct attr_name_t;
struct attribute_t;
struct identifier_t;
struct typedef_name_t;
struct enumeration_constant_t;
struct string_literal_t;
struct sizeof_expression_t;
struct init_declarator_list_t;
struct init_declarator_t;
struct initializer_t;
struct initializer_list_t;
struct designator_list_t;
struct designator_t;
struct designator_id;
struct designator_constant_expr;

struct abstract_declarator_t;

struct direct_declarator_id;
struct direct_declarator_decl;
struct direct_declarator_arr;
struct direct_declarator_func;
struct direct_abstract_declarator_t;
struct direct_abstract_declarator_decl;
struct direct_abstract_declarator_arr;
struct direct_abstract_declarator_func;
struct direct_declarator_t;

}

