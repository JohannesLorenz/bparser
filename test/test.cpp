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

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstring>

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

extern std::vector<terminal_t*>& get_token_vector();

void run_test(const char* str,
	const char* out_name)
{
	//std::cout << "code: " << std::endl << str << std::endl;
	translation_unit_t *e = get_ast(str, out_name);

	std::ostringstream os;
	dumper_t dumper(os);
	e->accept(dumper);
	std::string res_str = os.str();

	std::cout << res_str << std::endl;

	if(out_name)
	{
		std::string out_name_with_path = "test/output/";
		out_name_with_path += out_name;
		out_name_with_path += ".out";
		std::ifstream outfile(out_name_with_path.c_str());

		if(outfile)
		{
		try{
			const char* res = res_str.data();
			std::string line;
			std::size_t line_no = 0;
			while (std::getline(outfile, line))
			{
				++line_no;
				for(const char* exp = line.data(); *exp; ++exp, ++res)
				{
					if(*exp != *res) {
						std::cout << "line " << line_no
							<< ": expecting: " << exp
							<< ", got: " << res << std::endl;
						throw "lines not equal";
					}
				}
				if(*res != '\n') {
					throw std::string("expected newline, got: ") + res;
				}
				++res; // newline char is discarded in line
			}
		}
		catch(const char* s)
		{
			std::cout << "Error, dumping result to tmp_result.txt!" << std::endl;
			std::cout << "Try kdiff3 " << out_name_with_path << " tmp_result.txt" << std::endl;
			system(("diff -b " + out_name_with_path + " tmp_result.txt").c_str());
			std::cout << "If the new result is better, do: mv tmp_result.txt " << out_name_with_path << std::endl;
			std::ofstream of("tmp_result.txt");
			of << res_str;
			throw s;
		}
		catch(const std::string s)
		{
			std::cout << "Error, dumping result to tmp_result.txt!" << std::endl;
			std::cout << "Try diff " << out_name_with_path << " tmp_result.txt" << std::endl;
			std::cout << "If the new result is better, do: mv tmp_result.txt " << out_name_with_path << std::endl;
			std::ofstream of("tmp_result.txt");
			of << res_str;
			throw s;
		}

		}
		else
		{
			std::ofstream of(out_name_with_path.c_str());
			of << res_str;
		}
	}
	else
	 throw "outfile name required";

	typedef std::vector<terminal_t*> term_v;
	{
		term_v terms = get_token_vector();
		for(term_v::const_iterator itr = terms.begin();
			itr != terms.end(); ++itr)
		{
			//dumper_t dumper;
			//(*itr)->accept(dumper);
			if((*itr)->value() == ' ' && *str != ' ')
			 throw "invalid position of whitespace detected";
			std::size_t newlines = (*itr)->get_newlines();
			if(newlines)
			{
				for(; newlines && *str; str = strchr(str, '\n') + 1, --newlines)
				 ;
			}
			str += (*itr)->get_length();
		}
	}

	char c;
	//std::cin >> c;
}

void run_test_file(const char* file)
{
	std::cout << ":::::::::::::::" << std::endl;
	std::cout << "running test: " << file << std::endl;
	std::cout << ":::::::::::::::" << std::endl;

	std::string in_name_with_path = "test/input/";
	in_name_with_path += file;
	in_name_with_path += ".c";

	std::ifstream t(in_name_with_path.c_str());
	if(!t)
	 throw "Could not open input file.";

	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string buffer_res = buffer.str();

	run_test(buffer_res.c_str(), file);
}


void run(int argc, char** argv)
{
	translation_unit_t *e = NULL;
	//    char test[]=" 4 + 2*10 + 3*( 5 + 1 )";

	if(argc == 1)
	{
	#if 0
		char test[] = "int main()\n"
			"{\n"
		//        "int x = 0;\n"
		//        "return x < 3;\n"
			"return 42 + 0;"
			"}\n";
	#else
		char test[] = "enum { I_AM_AN_ENUM };\n"
			"int main(int) {\n"
			"\t\tfor(;;) {;}\n"
			"\t\twhile(1) {;}\n"
			"\t\tdo {;} while (1);\n"
			"\t\tint x = 3 + 0 * 4;\n"
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
	#if 0
		e = get_ast(test);

		//    result = evaluate(e);

		//    printf("Result of '%s' is %d\n", test, result);
		std::cout << "Result:" << std::endl;
		dumper_t dumper;
		e->accept(dumper);
		cleaner_t cleaner;
		e->accept(cleaner);
	#endif

		run_test("int main() {}", "minimal");

		run_test("typedef struct { int x; } s;\n"
			"typedef s (*g)(int);",
			"typedefs"
			);

		/*labeled_statement { $$=$1; }
		| compound_statement { $$=$1; }
		| expression_statement { $$=$1; }
		| selection_statement { $$=$1; }
		| iteration_statement { $$=$1; }
		| jump_statement { $$=$1; }*/


		// translation_unit
		// external_declaration
		// function_definition
		// declaration_list
		// compound_statement
		run_test("int x;\n"
			"int f(int, char* c) {}\n"
			"int g(a, b) int a,b; {}\n"
			"int main() {\n"
			"}",
			"basics"
			);

		//
		//
		//
		//
		//
		run_test_file("statements");
		run_test_file("initializers");
		run_test_file("abstract_declarators");
		run_test_file("direct_declarators");
		run_test_file("declaration_specifiers");
		run_test_file("structs");
		run_test_file("struct_fwd"); // this is a bug which does not harm
		run_test_file("enums");
		run_test_file("expressions");
		run_test_file("constants");
		run_test_file("clash");
		run_test_file("clash_2");
		run_test_file("scopes");
		run_test_file("nested");

		run_test_file("struct_acc");
		run_test_file("struct_typedefs");
		run_test_file("list");
	}
	else // argc > 1
	{
		run_test_file(argv[1]);

	}

}

int main(int argc, char** argv)
{
	int exit_value = EXIT_SUCCESS;
	try {
		run(argc, argv);
	} catch (const char* err_msg) {
		std::cout << "FEHLER: " << std::endl;
		std::cout << err_msg << std::endl;
		std::cout << "ABBRUCH." << std::endl;
		exit_value = EXIT_FAILURE;
	} catch (std::string& err_msg) { // FEATURE: code factorize
		std::cout << "FEHLER: " << std::endl;
		std::cout << err_msg << std::endl;
		std::cout << "ABBRUCH." << std::endl;
		exit_value = EXIT_FAILURE;
	} catch (...) {
		std::cout << "FEHLER: " << std::endl;
		std::cout << "unbekannt exception" << std::endl;
		std::cout << "ABBRUCH." << std::endl;
		exit_value = EXIT_FAILURE;
	}

	return exit_value;
}
