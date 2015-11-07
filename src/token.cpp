#include "token.h"

struct id_and_name
{
	int id;
	const char* name;
};

const id_and_name names[] =
{
	{ t_identifier, "<identifier>" },
	{ t_i_constant, "<integer constant>" },
	{ t_f_constant, "<float constant>" },
	{ t_string_literal, "<string literal>" },
	{ t_func_name, "__func__" },
	{ t_sizeof, "sizeof" },
	{ t_ptr_op, "->" },
	{ t_inc_op, "++" },
	{ t_dec_op, "--" },
	{ t_left_op, "<<" },
	{ t_right_op, ">>" },
	{ t_le_op, "<=" },
	{ t_ge_op, ">=" },
	{ t_eq_op, "==" },
	{ t_ne_op, "!=" },
	{ t_and_op, "&&" },
	{ t_or_op, "||" },
	{ t_mul_assign, "*=" },
	{ t_div_assign, "/=" },
	{ t_mod_assign, "%=" },
	{ t_add_assign, "+=" },
	{ t_sub_assign, "-=" },
	{ t_left_assign, "<<=" },
	{ t_right_assign, ">>=" },
	{ t_and_assign, "&=" },
	{ t_xor_assign, "^=" },
	{ t_or_assign, "|=" },
	{ t_typedef_name, "<typedef name>" },
	{ t_enumeration_constant, "<enumeration constant>" },

	{ t_typedef, "typedef" },
	{ t_extern, "extern" },
	{ t_static, "static" },
	{ t_auto, "auto" },
	{ t_register, "register" },
	{ t_inline, "inline" },
	{ t_const, "const" },
	{ t_restrict, "restrict" },
	{ t_volatile, "volatile" },
	{ t_bool, "bool" },
	{ t_char, "char" },
	{ t_short, "short" },
	{ t_int, "int" },
	{ t_long, "long" },
	{ t_signed, "signed" },
	{ t_unsigned, "unsigned" },
	{ t_float, "float" },
	{ t_double, "double" },
	{ t_void, "void" },
	{ t_complex, "complex" },
	{ t_imaginary, "imaginary" },

	{ t_struct, "struct" },
	{ t_union, "union" },
	{ t_enum, "enum" },
	{ t_ellipsis, "ellipsis" },

	{ t_case, "case" },
	{ t_default, "default" },
	{ t_if, "if" },
	{ t_else, "else" },
	{ t_switch, "switch" },
	{ t_while, "while" },
	{ t_do, "do" },
	{ t_for, "for" },
	{ t_goto, "goto" },
	{ t_continue, "continue" },
	{ t_break, "break" },
	{ t_return, "return" },

	{ t_alignas, "alignas" },
	{ t_alignof, "alignof" },
	{ t_atomic, "atomic" },
	{ t_generic, "generic" },
	{ t_noreturn, "noreturn" },
	{ t_static_assert, "static_assert" },
	{ t_thread_local, "thread_local" }
};
#include <iostream>
const char* name_of(int token_id)
{
	const unsigned max = sizeof(names)/sizeof(id_and_name);
	for(unsigned i = 0; i < max; ++i)
	 if(names[i].id == token_id)
	  return names[i].name;
	throw "name for invalid token id requested";
}

