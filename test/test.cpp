#include <iostream>
#include <vector>

#include "node.h"
#include "visitor.h"
#include "parser.h"
#include "lexer.h"

extern int yydebug;
extern std::vector<terminal_t*>& get_token_vector();

int yyparse(translation_unit_t **expression, yyscan_t scanner);

translation_unit_t *getAST(const char *expr)
{
	init_files();

	translation_unit_t *expression;
	yyscan_t scanner;
	YY_BUFFER_STATE state;
	
	// yydebug = 1;
	if (yylex_init(&scanner)) {
		// couldn't initialize
		return NULL;
	}
	std::cerr << "text: " << yyget_text(scanner) << std::endl;
	state = yy_scan_string(expr, scanner);
	
	std::cerr << "text: " << yyget_text(scanner) << std::endl;

	if (yyparse(&expression, scanner)) {
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
	expression->accept(f);

	func_visitor<geom_completor> g;
	expression->accept(g);

	return expression;
}
 
int evaluate(node_base* /*e*/)
{/*
	switch (e->type) {
		case eVALUE:
		return e->value;
		case eMULTIPLY:
		return evaluate(e->left) * evaluate(e->right);
		case ePLUS:
		return evaluate(e->left) + evaluate(e->right);
		default:
		// shouldn't be here
		return 0;
	}*/
	return 0;
}
 
int main(void)
{
	translation_unit_t *e = NULL;
	//    char test[]=" 4 + 2*10 + 3*( 5 + 1 )";

#if 0
	char test[] = "int main()\n"
		"{\n"
	//        "int x = 0;\n"
	//        "return x < 3;\n"
		"return 42 + 0;"
		"}\n";
#else
	char test[] = "\tint main(int) {\n"
		"\t\tfor(;;) {;}\n"
		"\t\twhile(true) {;}\n"
		"\t\tdo {;} while (true);\n"
//		"\t\tint x = 3 + 0 * 4;\n"
		"\t\tx++;\n"
		"# 4 test.c 3 2\n"
		"# 7 test.c 3 2\n"
//		"\t\t42;\n"
		"\t\t0xDEADBEEF;\n"
//		"\t\t41.999f;\n"
		"# 6 test.c 5 4\n"
		"\t\tI_AM_AN_ENUM;\n"
//		"\t\t'c';\n"
//		"\t\t\"Hallo Welt!\";\n"
		"\t\tsizeof(unsigned int);\n"
		"\t}";
#endif
	std::cerr << test << std::endl;
	
	int result = 0;
	
	e = getAST(test);
	
	//    result = evaluate(e);
	
	//    printf("Result of '%s' is %d\n", test, result);
	puts("Result:");
	dumper_t dumper;
	e->accept(dumper);
	cleaner_t cleaner;
	e->accept(cleaner);
	
	return 0;
}
