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

#include <vector>
#include <iostream>

#include "node.h"
#include "visitor.h"
#include "type_completor.h"
#include "parser.h"
#include "lexer.h"
#include "ast.h"

extern int strict_mode;
extern bool bparser_debug;
extern int yydebug;
extern std::vector<terminal_t*>& get_token_vector();

int yyparse(translation_unit_t **expression, yyscan_t scanner);

translation_unit_t *get_ast(const char *input, const char* fname,
	bool _strict_mode)
{
	strict_mode = _strict_mode;
	bparser_debug = false;

	init_parser(fname);

	translation_unit_t *transl_unit;
	yyscan_t scanner;
	YY_BUFFER_STATE state;
	
//	yydebug = 1;
	if (yylex_init(&scanner)) {
		// couldn't initialize
		return NULL;
	}
//	std::cerr << "text: " << yyget_text(scanner) << std::endl;
	state = yy_scan_string(input, scanner);
	
//	std::cerr << "text: " << yyget_text(scanner) << std::endl;

	if (yyparse(&transl_unit, scanner)) {
		// error parsing
		return NULL;
	}
//	std::cerr << "text: " << yyget_text(scanner) << std::endl;

/*	std::cout << "token vector: " << std::endl;
	dumper_t dumper;
	std::vector<token_t*>& tokens = get_token_vector();
	for(std::vector<token_t*>::const_iterator itr = tokens.begin();
		itr != tokens.end(); ++itr) 
	{
		(*itr)->accept(dumper);
	}*/
#if 0
	std::cout << "comparing input buffer with token string..." << std::endl;
	dumper_t dumper;
	std::vector<token_t*>& tokens = get_token_vector();
	for(std::vector<token_t*>::const_iterator itr = tokens.begin();
		itr != tokens.end(); ++itr)
	{
		(*itr)->accept(dumper);
	}
#endif

	yy_delete_buffer(state, scanner);
	
	yylex_destroy(scanner);

	// run geometry visitor at first so other visitors can
	// show the geometry of an error
	std::vector<terminal_t*>& tv = get_token_vector();
	func_visitor<geom_completor> g;
	for(std::vector<terminal_t*>::const_iterator itr = tv.begin();
		itr != tv.end(); ++itr)
	{
		int val = (*itr)->value();
		if(val == ' ' || val == '\t' || val == '\n')
		 (*itr)->accept(g);
	}
	transl_unit->accept(g);
	
	// output incomplete AST for debugging purposes
	// not valgrind-safe yet :( TODO!
	// std::cout << *transl_unit << std::endl;
	
	// run the type completor
	io_visitor<type_completor> f;
	transl_unit->accept(f);

	return transl_unit;
}
