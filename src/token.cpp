/*************************************************************************/
/* bparser - a bison-based, C99 parser                                   */
/* Copyright (C) 2015-2015                                               */
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
	{ t_mul_asn, "*=" },
	{ t_div_asn, "/=" },
	{ t_mod_asn, "%=" },
	{ t_add_asn, "+=" },
	{ t_sub_asn, "-=" },
	{ t_left_asn, "<<=" },
	{ t_right_asn, ">>=" },
	{ t_and_asn, "&=" },
	{ t_xor_asn, "^=" },
	{ t_or_asn, "|=" },
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

std::size_t token_length(token_id t)
{
	switch(t)
	{
		case t_static_assert:
			return 13;
		case t_thread_local:
			return 12;
		case t_imaginary:
			return 9;
		case t_register:
		case t_restrict:
		case t_volatile:
		case t_unsigned:
		case t_continue:
		case t_noreturn:
		case t_complex:
			return 8;
		case t_typedef:
		case t_default:
		case t_alignas:
		case t_alignof:
		case t_generic:
			return 7;
		case t_extern:
		case t_static:
		case t_sizeof:
		case t_inline:
		case t_signed:
		case t_double:
		case t_struct:
		case t_switch:
		case t_return:
		case t_atomic:
			return 6;
		case t_const:
		case t_short:
		case t_float:
		case t_union:
		case t_while:
		case t_break:
		case t_bool:
			return 5;
		case t_auto:
		case t_char:
		case t_long:
		case t_void:
		case t_enum:
		case t_case:
		case t_else:
		case t_goto:
			return 4;
		case t_left_asn:
		case t_right_asn:
		case t_int:
		case t_ellipsis:
		case t_for:
			return 3;
		default:
			return 2;
	}
}
