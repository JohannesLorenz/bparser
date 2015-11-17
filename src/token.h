#ifndef TOKEN_H
#define TOKEN_H

#include <cstddef>

// TODO: remove this include,
//	make enum independent (other values),
//	and show conversion only to parser.c
#include "parser.h"

enum token_id
{
	t_identifier = IDENTIFIER,
	t_i_constant = I_CONSTANT,
	t_f_constant = F_CONSTANT,
	t_string_literal = STRING_LITERAL,
	t_func_name = FUNC_NAME,
	t_sizeof = SIZEOF,
	t_ptr_op = PTR_OP,
	t_inc_op = INC_OP,
	t_dec_op = DEC_OP,
	t_left_op = LEFT_OP,
	t_right_op = RIGHT_OP,
	t_le_op = LE_OP,
	t_ge_op = GE_OP,
	t_eq_op = EQ_OP,
	t_ne_op = NE_OP,
	t_and_op = AND_OP,
	t_or_op = OR_OP,
	t_mul_assign = MUL_ASSIGN,
	t_div_assign = DIV_ASSIGN,
	t_mod_assign = MOD_ASSIGN,
	t_add_assign = ADD_ASSIGN,
	t_sub_assign = SUB_ASSIGN,
	t_left_assign = LEFT_ASSIGN,
	t_right_assign = RIGHT_ASSIGN,
	t_and_assign = AND_ASSIGN,
	t_xor_assign = XOR_ASSIGN,
	t_or_assign = OR_ASSIGN,
	t_typedef_name = TYPEDEF_NAME,
	t_enumeration_constant = ENUMERATION_CONSTANT,

	t_typedef = TYPEDEF,
	t_extern = EXTERN,
	t_static = STATIC,
	t_auto = AUTO,
	t_register = REGISTER,
	t_inline = INLINE,
	t_const = CONST,
	t_restrict = RESTRICT,
	t_volatile = VOLATILE,
	t_bool = BOOL,
	t_char = CHAR,
	t_short = SHORT,
	t_int = INT,
	t_long = LONG,
	t_signed = SIGNED,
	t_unsigned = UNSIGNED,
	t_float = FLOAT,
	t_double = DOUBLE,
	t_void = VOID,
	t_complex = COMPLEX,
	t_imaginary = IMAGINARY,

	t_struct = STRUCT,
	t_union = UNION,
	t_enum = ENUM,
	t_ellipsis = ELLIPSIS,

	t_case = CASE,
	t_default = DEFAULT,
	t_if = IF,
	t_else = ELSE,
	t_switch = SWITCH,
	t_while = WHILE,
	t_do = DO,
	t_for = FOR,
	t_goto = GOTO,
	t_continue = CONTINUE,
	t_break = BREAK,
	t_return = RETURN,

	t_alignas = ALIGNAS,
	t_alignof = ALIGNOF,
	t_atomic = ATOMIC,
	t_generic = GENERIC,
	t_noreturn = NORETURN,
	t_static_assert = STATIC_ASSERT,
	t_thread_local = THREAD_LOCAL
};

const char* name_of(int token_id);
std::size_t token_length(token_id t);

#endif // TOKEN_H
