#include <iostream>

#include "node.h"
#include "visitor.h"
#include "ast.h"

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

void run_test(const char* str)
{
	std::cout << "code: " << std::endl << str << std::endl;
	translation_unit_t *e = get_ast(str);

	dumper_t dumper;
	e->accept(dumper);
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
		"\t\t42uLL;\n"
		"\t\t'c';\n"
		"\t\t10.0e+3f;\n"
		"\t\t0xDEADBEEF;\n"
//		"\t\t41.999f;\n"
		"# 6 test.c 5 4\n"
		"\t\tI_AM_AN_ENUM;\n"
//		"\t\t'c';\n"
		"\t\t\"Hallo Welt! Das ist keine newline: \\\\n.\";\n"
		"\t\tsizeof(unsigned int);\n"
		"\t}";
#endif
	std::cerr << test << std::endl;

	int result = 0;
	
	e = get_ast(test);
	
	//    result = evaluate(e);
	
	//    printf("Result of '%s' is %d\n", test, result);
	std::cout << "Result:" << std::endl;
	dumper_t dumper;
	e->accept(dumper);
	cleaner_t cleaner;
	e->accept(cleaner);

	run_test("typedef struct s { int x; };\n"
		"typedef union u { int x; };");
	
	return 0;
}
