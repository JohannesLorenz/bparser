#include <vector>
#include <iostream>

#include "node.h"
#include "visitor.h"
#include "parser.h"
#include "lexer.h"

extern int yydebug;
extern std::vector<terminal_t*>& get_token_vector();

int yyparse(translation_unit_t **expression, yyscan_t scanner);

translation_unit_t *get_ast(const char *input)
{
	init_parser();

	translation_unit_t *transl_unit;
	yyscan_t scanner;
	YY_BUFFER_STATE state;
	
	// yydebug = 1;
	if (yylex_init(&scanner)) {
		// couldn't initialize
		return NULL;
	}
	std::cerr << "text: " << yyget_text(scanner) << std::endl;
	state = yy_scan_string(input, scanner);
	
	std::cerr << "text: " << yyget_text(scanner) << std::endl;

	if (yyparse(&transl_unit, scanner)) {
		// error parsing
		return NULL;
	}
	std::cerr << "text: " << yyget_text(scanner) << std::endl;

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

	func_visitor<type_completor> f;
	transl_unit->accept(f);

	func_visitor<geom_completor> g;
	transl_unit->accept(g);

	return transl_unit;
}
