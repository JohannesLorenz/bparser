#include <iostream>
#include <vector>

#include "node.h"
#include "visitor.h"
#include "parser.h"
#include "lexer.h"

extern int yydebug;
extern std::vector<token_t*>& get_token_vector();

int yyparse(node_t **expression, yyscan_t scanner);

translation_unit_t *getAST(const char *expr)
{	
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

	if (yyparse((node_t**)&expression, scanner)) { // TODO bad cast??
		// error parsing
		return NULL;
	}
	std::cerr << "text: " << yyget_text(scanner) << std::endl;

	std::cout << "token vector: " << std::endl;
	dumper_t dumper;
	std::vector<token_t*>& tokens = get_token_vector();
	for(std::vector<token_t*>::const_iterator itr = tokens.begin();
		itr != tokens.end(); ++itr) 
	{
		(*itr)->accept(dumper);
	}

	yy_delete_buffer(state, scanner);
	
	yylex_destroy(scanner);
	
	return expression;
}
 
int evaluate(node_t *e)
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
	char test[] = "\tint main() {\n"
		"for(;;) {;}\n"
		"while(true) {;}\n"
		"do {;} while (true);\n"
		"int x = 3;\n"
		"x++;\n"
		"42;\n"
		"41.999f;\n"
		"I_AM_AN_ENUM;\n"
		"'c';\n"
		"\"Hallo Welt!\";\n"
		"sizeof(unsigned int);\n"
		"}";
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
