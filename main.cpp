/*
 * main.c file
 */

#include "node.h"
#include "visitor.h"
#include "parser.h"
#include "lexer.h"

extern int yydebug;

int yyparse(node_t **expression, yyscan_t scanner);

node_t *getAST(const char *expr)
{	
	node_t *expression;
	yyscan_t scanner;
	YY_BUFFER_STATE state;
	// yydebug = 1;
	if (yylex_init(&scanner)) {
		// couldn't initialize
		return NULL;
	}
	
	state = yy_scan_string(expr, scanner);
	
	if (yyparse(&expression, scanner)) {
		// error parsing
		return NULL;
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
	node_t *e = NULL;
	//    char test[]=" 4 + 2*10 + 3*( 5 + 1 )";
	char test[] = "int main()\n"
		"{\n"
	//        "int x = 0;\n"
	//        "return x < 3;\n"
		"return 42 + 0;"
		"}\n";
	
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
