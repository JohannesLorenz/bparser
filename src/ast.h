/*************************************************************************/
/* bparser - a bison-based, C99 parser                                   */
/* Copyright (C) 2015-2016                                               */
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

/**
	@file ast.h functionality to build complete ASTs

	returns an AST (abstract syntax tree) from given C99 input string
	This is the function most application will want to access

	@param input precompiled-ANSII-C99 input string
	@param fname name of the C99 file. This is not used for opening any
		file (input is given as a string already), but this is for
		labeling in which file the nodes (declarations, definitions,
		expressions etc) are
	@param param_allow_undefined Allow undefined identifiers. In this case,
		the first occurence is the implicit definition (the last part
		may be buggy). This will fail to detect identifier lists!
	@param _strict_mode if this is set to true, the parser is annoyed about
		duplicate declarations etc. This mode is too strict (a bug), but
		it is useful for testing.
*/

struct translation_unit_t *get_ast(
	const char *input,
	const char* fname = NULL,
	bool param_allow_undefined = false,
	bool param_strict_mode = true);
