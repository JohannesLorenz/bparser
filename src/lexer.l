%{
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

#include "node.h"
#include "parser.h"

#include <cctype>
#include <iostream>
#include <vector>
#include <cstring>
#include <map>

int _recent_tokens[4] = { -1, -1, -1, -1 }; // FEATURE: better initial numbers?
int* recent_tokens = _recent_tokens + 3;
int add_recent_token(int new_one) {
	recent_tokens[-3] = recent_tokens[-2];
	recent_tokens[-2] = recent_tokens[-1];
	recent_tokens[-1] = recent_tokens[0];
	return recent_tokens[ 0] = new_one;
}

std::vector<std::string>& get_files();

//const int FIRST_COL = 1;

struct token_str
{
	static std::vector<terminal_t*> value;
};

std::vector<terminal_t*> token_str::value;

std::vector<terminal_t*>& get_token_vector() { return token_str::value; }

class pos_counter
{
	static int last_file;
	static int last_line;
	static int last_col;
public:
	static span_t count()
	{
		if(get_token_vector().size())
		{
			terminal_t* prev = get_token_vector().back();

			std::size_t newlines = prev->get_newlines(),
				length = prev->get_length();

			if(newlines)
			{
				last_line += newlines;
				last_col = 1 + length;
			}
			else
			{
				last_col += length;
			}
		}
		return span_t();
	}

	// -1 because we are before char -1 respective to the new line
	static void newline(const char* text_pos) {
#if 0
	++last_line; /*last_col=FIRST_COL-1;*/
		/*
		we are actually before the first char of the next line,
		since this is the newline. this is already correct, since the first
		char of the next line must be counted as column 1.
		*/
		last_linebreak = text_pos;
#endif
}

	static geom_t pos() {
		//std::cout << "pos call, last_linebreak was: " << *last_linebreak << std::endl;
		return geom_t(last_file, last_line,
		last_col); }

	static void parse_ppline(const char* p)
	{
		++p; // skip #
		for(; *p==' '; ++p) ;
		last_col = 1;
		{
			int n;
			sscanf(p, "%d%n", &last_line, &n);
			p += n;
		}
		// the last token in the token vector is \n,
		// so we add a decrement manually:
		--last_line;
		for(; *p==' '; ++p) ;
		std::string fname;
		for(; *p!=' '; ++p) fname += *p;
		int v_id = -1, count = 0;
		for(std::vector<std::string>::const_iterator
			itr = get_files().begin();
			itr != get_files().end();
			++itr, ++count)
		{
			if(*itr == fname)
			{
				v_id = count;
				break;
			}
		}
		if(v_id == -1)
		{
			v_id = get_files().size();
			get_files().push_back(fname);
		}
		last_file = v_id;
		for(; *p!='\n'; ++p) ;
		// for explanation, see @a pos_counter::newline
	}

	static void reset()
	{
		last_file = 0; // default file
		// we start at 1, 1 (typical convention)
		last_line = last_col = 1;
	}
};

int pos_counter::last_file = 0; // default file
// we start at 1, 1 (typical convention)
int pos_counter::last_line = 1;
int pos_counter::last_col = 1;

void reset_pos_counter()
{
	pos_counter::reset();
}

span_t cnt() {
	return pos_counter::count(); }

geom_t get_pos() { return pos_counter::pos(); } // FEATURE: common header instead of extern?

struct file_map
{
	static std::vector<std::string> files;
};

std::vector<std::string> file_map::files;

std::vector<std::string>& get_files() { return file_map::files; }




class lookup_table_t
{
	typedef std::pair<lookup_type, std::size_t> value_entry_t; // lookup type, scope
	typedef std::vector<value_entry_t> value_t;
	typedef std::pair<std::string, value_t> entry_t;
	typedef std::map<std::string, value_t> table_t;
	static table_t table;

	static std::size_t bracket_depth;

public:
	static void inc_bracket_depth() { std::cout << "INC to: " << bracket_depth + 1 << std::endl; ++bracket_depth; }
	static void dec_bracket_depth() {
		std::cout << "DEC to: " << bracket_depth - 1 << std::endl;
		table_t::iterator itr = table.begin(),
			next = table.begin();
		for(table_t::const_iterator itr = table.begin();
			itr != table.end(); ++itr)
		{
			std::cout << itr->first << ": ";
			if(itr->second.size() > 1)
			 std::cout << std::endl;
			for(value_t::const_iterator itr2 = itr->second.begin();
				itr2 != itr->second.end(); ++itr2)
				std::cout << " -> at depth " << itr2->second
					<< ": type " << itr2->first
					<< std::endl;
		}
		for(itr = table.begin(); itr != table.end(); itr = next)
		{
			// invariant: itr == next
			if(next != table.end())
			 ++next;
			// invariant: ++itr == next || next == table.end()
			//std::cout << itr->second.second << " <-> " << bracket_depth << std::endl;
			if(itr->second.back().second == bracket_depth)
			{
				// out of scope
				itr->second.pop_back();
				if(itr->second.empty())
				 table.erase(itr);
			}
			else if(itr->second.back().second > (bracket_depth + 1))
			 throw "overseen last scope end";
		}
		--bracket_depth;
	}

	static void flag_symbol(const char* str, lookup_type type, int add = 0)
	{
		int new_depth = bracket_depth + add;
		table_t::iterator itr = table.find(str);
		if(itr != table.end())
		{
			value_t& stack = itr->second;
			if(stack.back().first == lt_identifier_list)
			{
				if(type != lt_identifier && type != lt_identifier_list)
				 throw "declarator type must be identifier if it follows an identifier list.";
				else {
					std::cout << "found *LATE DEFINED* identifier: " << str << std::endl;
					itr->second.back().first = type;
				}
			} else
			{
				if(new_depth != stack.back().second
				|| (stack.back().first == lt_struct_bound &&
					type == lt_struct_bound))
				{
					std::cout << "flagging: " << str << " as " << type << " (" << get_pos() << ")" <<  std::endl;
					stack.push_back(value_entry_t(type, new_depth));
				}
				else
				 throw "identifier has already been declared in this scope!";
			}

		}
		else
		{
			std::cout << "flagging: " << str << " as " << type << " (" << get_pos() << ")" <<  std::endl;

			table[str].push_back(value_entry_t(type, new_depth));
			//table.insert(entry_t(str, value_t()))
			//value_entry_t(type, new_depth)));
		}
	}

	static void leave_scope(const node_base* scope)
	{
	/*	for(table_t::iterator itr = table.begin();
			itr != table.end(); ++itr)
		if(itr->second.first != lt_undefined &&
			itr->second.second == scope)
		 itr->second.first = lt_undefined;*/
	}

	static lookup_type type_of(const char* str) {
		std::cout << "LOOKUP TABLE: " << std::endl;
		for(table_t::const_iterator itr = table.begin();
			itr != table.end(); ++itr)
		{
			std::cout << itr->first << ": ";
			if(itr->second.size() > 1)
			 std::cout << std::endl;
			for(value_t::const_iterator itr2 = itr->second.begin();
				itr2 != itr->second.end(); ++itr2)
				std::cout << " -> at depth " << itr2->second
					<< ": type " << itr2->first
					<< std::endl;
		}
		std::cout << "... looking for: " << str << std::endl;
		table_t::const_iterator itr = table.find(str);
		return (itr == table.end()) ? lt_undefined : itr->second.back().first;
	}

	static void clear() {
		for(table_t::iterator itr = table.begin(); itr != table.end(); ++itr)
		{
			value_t& stack = itr->second;
			if(stack.empty()) throw "stack for identifier should not be empty";
			while(stack.size())
			{
				if(stack.back().first == lt_identifier_list) {
					std::cout << "Error: remaining undeclared identifier" << itr->first << "(from identifier list?)" << std::endl;
					throw "Error remaining undeclared identifier (from identifier list?)";
				}
				else stack.pop_back();
			}
		}
		table.clear(); }
};

lookup_table_t::table_t lookup_table_t::table;
std::size_t lookup_table_t::bracket_depth = 0;

void leave_scope(const node_base* scope)
{
	lookup_table_t::leave_scope(scope);
}

int type_of(const char* str) {
	return lookup_table_t::type_of(str);
}

void clear_lookup_table()
{
	lookup_table_t::clear();
}



enum declaration_state_t
{
	expect_type_specifier,
	expect_declaration_specifiers_braces_pointers_type_qualifiers_identifier,
	expect_braces_pointers_type_qualifiers_identifier,
	declarator_found, //! < this will only be used for a short time
	expect_initializer_or_comma
} declaration_state = expect_type_specifier;
int declaration_state_braces = 0;

enum { MAYBE_IDENTIFIER = 4242 } ; // some value that has not been used

class states_t
{
	int declaration_state_pars_after;
	int declaration_state_braces_after;
	bool in_for_header;
	int par_count;
public:
	states_t() :
		declaration_state_pars_after(0),
		declaration_state_braces_after(0),
		in_for_header(false)
		{}
	
	std::string recent_was_flagged_unknown;

	int add_number(lookup_type lt) {
		return (int)(in_for_header) - (lt == lt_enumeration) + declaration_state_pars_after;
	}

	void set_state(const char* text, int token_id)
	{
	if(token_id != ' ' && token_id != '\n' && token_id != '\t')
	{

		switch(token_id)
		{
			case '(': ++par_count; break;
			case ')': if(!--par_count) in_for_header = false; break;
			case FOR: in_for_header = true; break;
			case ';':
				if(recent_tokens[0] == ')')
				{
					// can be end of function or for
					// erase all with bracket_depth +1
					lookup_table_t::inc_bracket_depth();
					lookup_table_t::dec_bracket_depth();
				}
				break;
		}

		if(declaration_state_braces) // structs, enums
		{
			// ignore everything, except closing braces
			if(token_id == '}')
			 --declaration_state_braces;
		}
		else if(declaration_state_braces_after || declaration_state_pars_after)
		{
			if(token_id == '}')
			 --declaration_state_braces_after;
			else if(token_id == ')')
			 --declaration_state_pars_after;
			std::cout << "DAF 1: " << declaration_state_pars_after << std::endl;
		} // TODO: remove this or put together with lower part?

		if(declaration_state == expect_initializer_or_comma
			|| declaration_state == declarator_found)
		{
			switch(token_id)
			{
				case '{':
					++declaration_state_braces_after; break;
				case '(':
					std::cout << "DAF 2: " << declaration_state_pars_after << std::endl;
					++declaration_state_pars_after; break;
			}
		}


		if(!declaration_state_braces /*&& declaration_state_braces_after && !declaration_state_pars_after*/)
		{
			declaration_state_t next_state = expect_type_specifier;
			switch(declaration_state)
			{
				case expect_type_specifier:
					if(is_type_specifier(text, token_id, false))
						next_state =
							expect_declaration_specifiers_braces_pointers_type_qualifiers_identifier;
					break;
				case expect_declaration_specifiers_braces_pointers_type_qualifiers_identifier:
					if(is_type_specifier(text, token_id, true))
					{
						next_state =
							expect_declaration_specifiers_braces_pointers_type_qualifiers_identifier;
					}
					else switch(token_id)
					{
						// storage specifiers
						case TYPEDEF:
						case EXTERN:
						case STATIC:
						case THREAD_LOCAL:
						case AUTO:
						case REGISTER:
						// type qualifiers
						case CONST:
						case RESTRICT:
						case VOLATILE:
						case ATOMIC:
						// function specifiers
						case INLINE:
						case NORETURN:
							next_state = expect_declaration_specifiers_braces_pointers_type_qualifiers_identifier;
					}
				case expect_braces_pointers_type_qualifiers_identifier:
					switch(token_id)
					{
						case '(':
						case '*':
						// type qualifiers
						case CONST:
						case RESTRICT:
						case VOLATILE:
						case ATOMIC:
							next_state = expect_braces_pointers_type_qualifiers_identifier;
							break;
						case MAYBE_IDENTIFIER:
							next_state = declarator_found;
							break;
					}
					break;
				case declarator_found:
					throw "this function should never be called with declarator_found";
				case expect_initializer_or_comma:
					switch(token_id)
					{
						// these mark the end of a declaration
						case ';':
						case '}':
					//	case ')':
							std::cout << "CASE 1" << std::endl;
							next_state = expect_type_specifier;
							break;
						case ')':
							std::cout << "CASE 1.1" << std::endl;
							next_state = (declaration_state_pars_after > 0)
								? expect_braces_pointers_type_qualifiers_identifier
								: expect_type_specifier;
						case '(':
							std::cout << "CASE 1.2" << std::endl;
							next_state = expect_type_specifier;
							break;
						case ',':
							std::cout << "CASE 2" << std::endl;
							std::cout << declaration_state_pars_after << std::endl;
							next_state = (declaration_state_pars_after > 0)
								// in a function, a comma separates identifiers: "void f(int a, int b)"
								? expect_type_specifier
								// outside a function, we may have multiple declarators of
								// "similar" type: "int a,b";
								: expect_braces_pointers_type_qualifiers_identifier;
							break;
						default: // might be some complicated intializating expression
							std::cout << "CASE 3" << std::endl;
							next_state = expect_initializer_or_comma;
					}
			}
			declaration_state = next_state;

			if(declaration_state == declarator_found)
			{
				/*declaration_state_pars_after =
				declaration_state_braces_after =
				0;*/
			}
		}

		if(declaration_state == expect_initializer_or_comma
			|| declaration_state == declarator_found)
		{
			switch(token_id)
			{
			/*	case '{':
					++declaration_state_braces_after; break;
				case '(':
					std::cout << "DAF 2: " << declaration_state_pars_after << std::endl;
					++declaration_state_pars_after; break;*/
				case '}':
					--declaration_state_braces_after; break;
				case ')':
					std::cout << "DAF 3: " << declaration_state_pars_after << std::endl;
					--declaration_state_pars_after; break;
			}
		}

		std::cout << "SET STATE NOW: " << declaration_state << " after text: "<< text << std::endl;

		add_recent_token(token_id);

		if(token_id == ':' && recent_was_flagged_unknown.size())
		{
			// re-flag
			flag_symbol(recent_was_flagged_unknown.c_str(), lt_identifier);
		}

		recent_was_flagged_unknown.clear();
	}
	}

	void flag_symbol(const char* symbol, lookup_type lt)
	{
		lookup_table_t::flag_symbol(symbol, lt, add_number(lt));
	}

	bool is_type_specifier(const char* text, int token_id, bool id_is_decl)
	{
		switch(token_id)
		{
			case VOID:
			case CHAR:
			case SHORT:
			case INT:
			case LONG:
			case FLOAT:
			case DOUBLE:
			case SIGNED:
			case UNSIGNED:
			case BOOL:
			case COMPLEX:
			case IMAGINARY:
				return true;
			//case TYPEDEF_NAME:
			//	return true;
			case MAYBE_IDENTIFIER:

				// if id => decl, than we have a declaration, no type_specifier
				// otherwise, we have no declaration, so the maybe-id must be known
				// then, it is only a type specifier if it is a typedef name
				if(recent_tokens[0] == STRUCT || recent_tokens[0] == UNION
					|| recent_tokens[0] == ENUM) // TODO: ENUM?
				 return true;
				else
				return (!id_is_decl) && lookup_table_t::type_of(text) == lt_typedef_name;
			case '{':
				if(recent_tokens[0] == ENUM
				|| recent_tokens[0] == STRUCT
				|| recent_tokens[0] == UNION
				|| recent_tokens[-1] == ENUM
				|| recent_tokens[-1] == STRUCT
				|| recent_tokens[-1] == UNION)
				{
					++declaration_state_braces;
				}
				return false;
			case '}': // has already been increased
				return true;
			default:
				return false;
		}
	}

};

states_t states;

//! creates a new token and appends it
int app3(token_t*& token, int token_id, const char* text)
{ // FEATURE: rename to app() or app_token()
	states.set_state(text, token_id);
	cnt();
	token = new token_t(get_pos(), token_id);
	token_str::value.push_back(token);
	return token_id;
}

//! appends an already created terminal
template<class T> int app(T* elem, int lex_id)
{
	token_str::value.push_back(elem);
	return lex_id;
}

template<class T>
int app_with_string(T*& token, int lex_id, const char* text, const char* end)
{
	states.set_state(text, lex_id);
	std::size_t length = end - text;
	cnt();
	char* res = new char[length + 1]; res[length] = 0; /*std::copy(yytext, end, yylval->name);*/ strncpy(res, text, length);
	if(*end) throw "end of token not 0";
	return app(token = new T(res, get_pos()), lex_id);
}

bool icmp(const char* p, char c) {
	return tolower(*p) == tolower(c);
}

int app_float(fconstant_t*& token, const char* text)
{
	const char* p = text;
	{
		float f; int n;
		sscanf(text, "%f%n", &f, &n);
		p += n;
	}
	if(*p && (icmp(p,'f') || icmp(p,'l'))) ++p;
	if(*p) throw "end of token not 0";
	return app_with_string(token, F_CONSTANT, text, p);
}

enum int_suffix_type
{
	no_suffix,
	suf_u,
	suf_ul,
	suf_ull,
	suf_l,
	suf_ll,
};

// FEATURE: not here -> type_completor
const char* skip_suffix(const char*& p)
{
	int_suffix_type suf;
	// (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
	if(icmp(p, 'u')) // FEATURE: ?:
	{
		if(icmp(p+1,'l'))
		{
			if(*(p+1)==*(p+2))
			 suf=suf_ull;
			else
			 suf=suf_ul;
		}
		else
		 suf=suf_u;
	}
	else if(icmp(p, 'l'))
	{
		if(*(p+1) == (*p)) // ll or LL
		{
			suf = icmp(p+2, 'u') ? suf_ull : suf_ll;
		}
		else
		{
			suf = icmp(p+1, 'u') ? suf_ul : suf_l;
		}
	}
	else
	 suf = no_suffix;

	switch(suf)
	{
		case suf_ull:
			++p;
		case suf_ll:
		case suf_ul:
			++p;
		case suf_l:
		case suf_u:
			++p;
		default:
			;
	}

	return p;
}

void skip_esc_seq(const char*& p)
{
	if(*p == '\\')
	{
		++p;
		if(*p == '\'' || *p == '"'
			|| *p == '?' || *p == '\\'
			|| *p == 'a' || *p == 'b'
			|| *p == 'f' || *p == 'n'
			|| *p == 'r' || *p == 't'
			|| *p == 'v')
			++p;
		else if(*p > '0' && *p < '7')
		{
			++p;
			if(*p > '0' && *p < '7')
			{
				++p;
				if(*p > '0' && *p < '7')
				{
					++p;
				}
			}
		}
		else if(*p == 'x')
		{
			while(isxdigit(*p)) ++p;
		}
	}
}

int app_int(iconstant_t*& token, const char* text, char scanf_type)
{
	const char* p = text;
	{
		char scanf_str [5] = {'%', scanf_type, '%', 'n', 0};
		int tmp, n;
		sscanf(text, scanf_str, &tmp, &n);
		p += n;
	}
	skip_suffix(p);
	if(*p) throw "end of token not 0";
	return app_with_string(token, I_CONSTANT, text, p);
}

bool recent_struct_access = false;
bool recent_typedef_keyword = false;
bool in_enum_block = false;
int braces_since_typedef = 0;
node_base* cur_scope = NULL;

void set_scope(node_base* new_scope) {
	cur_scope = new_scope;
}

void reset_variables()
{
	recent_struct_access = false;
	recent_typedef_keyword = false;
	in_enum_block = false;
	braces_since_typedef = 0;
	cur_scope = NULL;
}

%}

%option outfile="lexer.c" header-file="lexer.h"
%option warn nodefault
 
%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

%e  1019
%p  2807
%n  371
%k  284
%a  1213
%o  1117

O   [0-7]
D   [0-9]
NZ  [1-9]
L   [a-zA-Z_]
A   [a-zA-Z_0-9]
H   [a-fA-F0-9]
HP  (0[xX])
E   ([Ee][+-]?{D}+)
P   ([Pp][+-]?{D}+)
FS  (f|F|l|L)
IS  (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP  (u|U|L)
SP  (u8|u|U|L)
ES  (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WS  [ \t\v\n\f]

%{
extern void yyerror(const char *);  /* prints grammar violation message */

extern int sym_type(const char *);  /* returns type from symbol table */

#define sym_type(identifier) IDENTIFIER /* with no symbol table, fake it */

static void comment(void);
static int check_type(const char* text);
%}

%%
"/*"					{ comment(); }
"//".*					{ /* consume //-comment */ }

"auto"					{ return app3(yylval->token, AUTO, yytext); }
"break"					{ return app3(yylval->token, BREAK, yytext); }
"case"					{ return app3(yylval->token, CASE, yytext); }
"char"					{ return app3(yylval->token, CHAR, yytext); }
"const"					{ return app3(yylval->token, CONST, yytext); }
"continue"				{ return app3(yylval->token, CONTINUE, yytext); }
"default"				{ return app3(yylval->token, DEFAULT, yytext); }
"do"					{ return app3(yylval->token, DO, yytext); }
"double"				{ return app3(yylval->token, DOUBLE, yytext); }
"else"					{ return app3(yylval->token, ELSE, yytext); }
"enum"					{ return app3(yylval->token, ENUM, yytext); }
"extern"				{ return app3(yylval->token, EXTERN, yytext); }
"float"					{ return app3(yylval->token, FLOAT, yytext); }
"for"					{ return app3(yylval->token, FOR, yytext); }
"goto"					{ return app3(yylval->token, GOTO, yytext); }
"if"					{ return app3(yylval->token, IF, yytext); }
"inline"				{ return app3(yylval->token, INLINE, yytext); }
"int"					{ return app3(yylval->token, INT, yytext); }
"long"					{ return app3(yylval->token, LONG, yytext); }
"register"				{ return app3(yylval->token, REGISTER, yytext); }
"restrict"				{ return app3(yylval->token, RESTRICT, yytext); }
"return"				{ return app3(yylval->token, RETURN, yytext); }
"short"					{ return app3(yylval->token, SHORT, yytext); }
"signed"				{ return app3(yylval->token, SIGNED, yytext); }
"sizeof"				{ return app3(yylval->token, SIZEOF, yytext); }
"static"				{ return app3(yylval->token, STATIC, yytext); }
"struct"				{ return app3(yylval->token, STRUCT, yytext); }
"switch"				{ return app3(yylval->token, SWITCH, yytext); }
"typedef"				{ recent_typedef_keyword = true;
						braces_since_typedef = 0;
						return app3(yylval->token, TYPEDEF, yytext); }
"union"					{ return app3(yylval->token, UNION, yytext); }
"unsigned"				{ return app3(yylval->token, UNSIGNED, yytext); }
"void"					{ return app3(yylval->token, VOID, yytext); }
"volatile"				{ return app3(yylval->token, VOLATILE, yytext); }
"while"					{ return app3(yylval->token, WHILE, yytext); }
"_Alignas"                              { return app3(yylval->token, ALIGNAS, yytext); }
"_Alignof"                              { return app3(yylval->token, ALIGNOF, yytext); }
"_Atomic"                               { return app3(yylval->token, ATOMIC, yytext); }
"_Bool"                                 { return app3(yylval->token, BOOL, yytext); }
"_Complex"                              { return app3(yylval->token, COMPLEX, yytext); }
"_Generic"                              { return app3(yylval->token, GENERIC, yytext); }
"_Imaginary"                            { return app3(yylval->token, IMAGINARY, yytext); }
"_Noreturn"                             { return app3(yylval->token, NORETURN, yytext); }
"_Static_assert"                        { return app3(yylval->token, STATIC_ASSERT, yytext); }
"_Thread_local"                         { return app3(yylval->token, THREAD_LOCAL, yytext); }
"__func__"                              { return app3(yylval->token, FUNC_NAME, yytext); }
"__attribute__"				{ return app3(yylval->token, ATTRIBUTE, yytext); }

{L}{A}*					{	
						// it should rather be: set_state (IDENTIFIER_OR_ENUMERATOR_OR...)
						// nonetheless, it should be clear what was meant here.
						states.set_state(yytext, MAYBE_IDENTIFIER);

						// this could only be set to true if a . or -> preceded
						// thus, this is the only place to set it to false
						recent_struct_access = false;

						// find end of identifier
						const char* end = yytext + 1;
						for(; isalnum(*end) || *end == '_'; ++end) ;
						std::size_t length = end - yytext;
						cnt();
						char* res = new char[length + 1]; res[length] = 0; /*std::copy(yytext, end, yylval->name);*/ strncpy(res, yytext, length);
						
						int type = check_type(yytext);
						switch(type)
						{
							case IDENTIFIER:
							std::cout << " -> IDENTIFIER" << std::endl;
							return app(yylval->name = new identifier_t(res, get_pos()), type);
							case TYPEDEF_NAME:
							std::cout << " -> TYPEDEF NAME" << std::endl;
							return app(yylval->typedef_name = new typedef_name_t(res, get_pos()), type);
							case ENUMERATION_CONSTANT:
							std::cout << " -> ENUMERATION_CONSTANT" << std::endl;
							return app(yylval->enumeration_constant = new enumeration_constant_t(res, get_pos()), type);
							case ATTR_NAME:
							std::cout << " -> ATTR_NAME" << std::endl;
							return app(yylval->attr_name = new attr_name_t(res, get_pos()), type);
						}
					}

{HP}{H}+{IS}?				{	return app_int(yylval->iconstant, yytext, 'x'); }
{NZ}{D}*{IS}?				{ 	return app_int(yylval->iconstant, yytext, 'd'); }
"0"{O}*{IS}?				{ 	return app_int(yylval->iconstant, yytext, 'o'); }
{CP}?"'"([^'\\\n]|{ES})+"'"		{ const char* p = yytext; if(icmp(p, 'u') || *p == 'L') ++p;
						++p; // '
						// ES = (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
						bool ok = true;
						do
						{
							if(*p != '\'' && *p != '\\' && *p != '\n') // exclude single chars
							 ++p;
							else if(*p == '\\')
							{
								skip_esc_seq(p);
							}
							else // must be '
							{
								if(*p != '\'')
								 throw "lexer error for char parsing";
								ok = false; // end found
							}
						} while(ok);
						++p; // '
						if(*p) throw "end of token not 0";
						return app_with_string(yylval->iconstant, I_CONSTANT, yytext, p); }

{D}+{E}{FS}?				{ return app_float(yylval->fconstant, yytext); }
{D}*"."{D}+{E}?{FS}?			{ return app_float(yylval->fconstant, yytext); }
{D}+"."{E}?{FS}?			{ return app_float(yylval->fconstant, yytext); }
{HP}{H}+{P}{FS}?			{ return app_float(yylval->fconstant, yytext); }
{HP}{H}*"."{H}+{P}{FS}?			{ return app_float(yylval->fconstant, yytext); }
{HP}{H}+"."{P}{FS}?			{ return app_float(yylval->fconstant, yytext); }

({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+	{ const char* p = yytext;
						do
						{
							if(*p == 'u' || *p == 'U' || *p == 'L') ++p;
							++p; // "
							if(*p == '8')
							 ++p;
							bool ok = true;
							do
							{
								if(*p != '"' && *p != '\\' && *p != '\n') // exclude single chars
								 ++p;
								else if(*p == '\\')
								{
									skip_esc_seq(p);
								}
								else // must be "
								{
									if(*p != '"')
									 throw "lexer error for char parsing";
									ok = false; // end found
								}
							} while(ok);
							++p; // "'"

							for(; *p == ' ' || *p == '\t' || *p == '\n'; ++p);


						} while (*p == '"'
							|| (*p == 'u' && (*(p+1) == '"'
								|| (*(p+1) == '8' && *(p+2) == '"') ))
							|| (*p == 'U' && *(p+1) == '"')
							|| (*p == 'L' && *(p+1) == '"')
							); // only strings start like this

						std::size_t length = p - yytext;
						char* res = new char[length + 1]; res[length] = 0; /*std::copy(yytext, end, yylval->name);*/ strncpy(res, yytext, length);
						if(*p) throw "end of token not 0";
						// FEATURE: common routine with identifier
						return app_with_string(yylval->string_literal, STRING_LITERAL, yytext, p); }

"..."					{ return app3(yylval->token, ELLIPSIS, yytext); }
">>="					{ return app3(yylval->token, RIGHT_ASSIGN, yytext); }
"<<="					{ return app3(yylval->token, LEFT_ASSIGN, yytext); }
"+="					{ return app3(yylval->token, ADD_ASSIGN, yytext); }
"-="					{ return app3(yylval->token, SUB_ASSIGN, yytext); }
"*="					{ return app3(yylval->token, MUL_ASSIGN, yytext); }
"/="					{ return app3(yylval->token, DIV_ASSIGN, yytext); }
"%="					{ return app3(yylval->token, MOD_ASSIGN, yytext); }
"&="					{ return app3(yylval->token, AND_ASSIGN, yytext); }
"^="					{ return app3(yylval->token, XOR_ASSIGN, yytext); }
"|="					{ return app3(yylval->token, OR_ASSIGN, yytext); }
">>"					{ return app3(yylval->token, RIGHT_OP, yytext); }
"<<"					{ return app3(yylval->token, LEFT_OP, yytext); }
"++"					{ return app3(yylval->token, INC_OP, yytext); }
"--"					{ return app3(yylval->token, DEC_OP, yytext); }
"->"					{ recent_struct_access = true; return app3(yylval->token, PTR_OP, yytext); }
"&&"					{ return app3(yylval->token, AND_OP, yytext); }
"||"					{ return app3(yylval->token, OR_OP, yytext); }
"<="					{ return app3(yylval->token, LE_OP, yytext); }
">="					{ return app3(yylval->token, GE_OP, yytext); }
"=="					{ return app3(yylval->token, EQ_OP, yytext); }
"!="					{ return app3(yylval->token, NE_OP, yytext); }
";"					{ return app3(yylval->token, ';', yytext); }
("{"|"<%")				{
if(recent_tokens[0] == ENUM || recent_tokens[-1] == ENUM)
 in_enum_block = true;
lookup_table_t::inc_bracket_depth();
++braces_since_typedef;
return app3(yylval->token, '{', yytext); }
("}"|"%>")				{
						if(in_enum_block)
						 in_enum_block = false;
						lookup_table_t::dec_bracket_depth();
						--braces_since_typedef;
						return app3(yylval->token, '}', yytext);
}
","					{ return app3(yylval->token, ',', yytext); }
":"					{ return app3(yylval->token, ':', yytext); }
"="					{ return app3(yylval->token, '=', yytext); }
"("					{ return app3(yylval->token, '(', yytext); }
")"					{ return app3(yylval->token, ')', yytext); }
("["|"<:")				{ return app3(yylval->token, '[', yytext); }
("]"|":>")				{ return app3(yylval->token, ']', yytext); }
"."					{ recent_struct_access = true; return app3(yylval->token, '.', yytext); }
"&"					{ return app3(yylval->token, '&', yytext); }
"!"					{ return app3(yylval->token, '!', yytext); }
"~"					{ return app3(yylval->token, '~', yytext); }
"-"					{ return app3(yylval->token, '-', yytext); }
"+"					{ return app3(yylval->token, '+', yytext); }
"*"					{ return app3(yylval->token, '*', yytext); }
"/"					{ return app3(yylval->token, '/', yytext); }
"%"					{ return app3(yylval->token, '%', yytext); }
"<"					{ return app3(yylval->token, '<', yytext); }
">"					{ return app3(yylval->token, '>', yytext); }
"^"					{ return app3(yylval->token, '^', yytext); }
"|"					{ return app3(yylval->token, '|', yytext); }
"?"					{ return app3(yylval->token, '?', yytext); }

"\t"					{ app3(yylval->token, '\t', yytext); }
"\n"					{ app3(yylval->token, '\n', yytext); pos_counter::newline(yytext); }
"\v"					{ throw "non-supported sign \\v"; }
"\f"					{ throw "non-supported sign \\f"; }
" "					{ app3(yylval->token, ' ', yytext); }
"#"([^\\\n])*"\n"			{ pos_counter::parse_ppline(yytext); }
.					{ std::cerr << "At: " << yytext << std::endl; throw "bad character"; }

%%

#if 0
int yywrap(void)        /* called at end of input */
{
    return 1;           /* terminate now */
}
#endif	

static void comment(void)
{
#if 0
    int c;

    while ((c = input()) != 0)
        if (c == '*')
        {
            while ((c = input()) == '*')
                ;

            if (c == '/')
                return;

            if (c == 0)
                break;
        }
    yyerror("unterminated comment");
#endif
}

static int check_type(const char* text) // FEATURE: lookup_table function?
{
	/*return (typeof(yytext) == lt_undefined)
		? recent_typedef_keyword
			? recent_typedef_keyword=false, TYPEDEF_NAME
			: recent_struct_access*/

	// attribute names go separate
	if(recent_tokens[-3] == ATTRIBUTE)
	 return ATTR_NAME;

	/*
		Grammar: Each type_specifier is followed by either '<declaration_specifier>*['('<type_qualifier_list>*]*IDENTIFIER' => declaration,
		Grammar: if it's followed by something else it's not a declaration
			i.e. <this regexp> => declaration
		The C standard forces that at least one declaration specifier is a type specifier
		(http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf 6.7.2.2)

		Thus:
		type_specifier + regex + IDENTIFIER (grammar)=> declaration, otherwise => no declaration
		declaration (C standard)=> type_specifier (above)=> type_specifier + regex + IDENTIFIER

		Finally:
		type_specifier + regex + IDENTIFIER (grammar) <=> declaration
		
	*/

	std::cout << "STATE NOW: " << declaration_state << std::endl;


	if(recent_tokens[-1] == STRUCT || recent_tokens[-1] == UNION || recent_tokens[-1] == ENUM)
	{
		states.flag_symbol(text, lt_struct_bound);
		return IDENTIFIER;
	}
	/*
		first of all, check if this is declaration
		lookup table does not matter, since this could be an identifier with the same name
		this comparison is reliable in all cases:
	*/
	else if(!declaration_state_braces && declaration_state == declarator_found)
	{
		states.flag_symbol(text, recent_typedef_keyword
							? lt_typedef_name
							: lt_identifier);
		recent_typedef_keyword = false;

//		declaration_state = expect_type_specifier; // reset for next declaration
		std::cout << " -> declaration, no braces" << std::endl;

		declaration_state = expect_initializer_or_comma;

		return IDENTIFIER;
	}
	else if(declaration_state_braces)
	{
		// the last IF found *all* declarations, except all inside of enum {} and struct {}
		// find these manually, this is easy
		states.flag_symbol(text,
			(in_enum_block &&
			(recent_tokens[-1] == '{' ||
			recent_tokens[-1] == ',') )
			? lt_enumeration
			: lt_identifier);
// TODO: the comma might be from the comma op ! error !
		std::cout << " -> declaration, braces" << std::endl;
		return IDENTIFIER;
	}/* else if(states.declaration_state_pars_after > 0)
	{
		// must be a function. all identifiers must be declarators
		states.flag_symbol(text, lt_identifier);
		std::cout << " -> declaration, function parameter" << std::endl;
		return IDENTIFIER;
	}*/
		else switch(lookup_table_t::type_of(text))
		{
			// now it really can't be a declaration, so it must be known
			case lt_typedef_name:                /* previously defined */
			return TYPEDEF_NAME;
			case lt_enumeration:        /* previously defined */
			return ENUMERATION_CONSTANT;
			case lt_identifier:                          /* includes undefined */
			case lt_struct_bound:
			return IDENTIFIER;
			default:
			states.flag_symbol(text, lt_identifier_list);
			states.recent_was_flagged_unknown = text;
			return IDENTIFIER;
			//throw "This is neither a declaration, nor is this type known.";
		}

#if 0	
lookup_type lt = lookup_table_t::type_of(text);
	if(lt == lt_undefined)
	{
		// the braces check is a stupid fix
		// however, it covers all problematic cases, like
		// typedef struct { int x } s;
		if(recent_typedef_keyword && (braces_since_typedef == 0))
		{
			lt = lt_typedef_name;
			recent_typedef_keyword=false;
		}
		else if(in_enum_block &&
			(get_token_vector().back()->value() == '{' ||
			get_token_vector().back()->value() == ',') )
		{
			lt = lt_enumeration;
		}
		else // everything else...
		 lt = lt_identifier;

		states.flag_symbol(text, lt);

		// since lt == lt_undefined, this is the definition
		// in all case, for definitions, return IDENTIFIER
		// for later use, return the real symbol type
		return IDENTIFIER;
	}
	else switch(lt)
	{
		case lt_typedef_name:                /* previously defined */
		 return TYPEDEF_NAME;
		case lt_enumeration:        /* previously defined */
		 return ENUMERATION_CONSTANT;
		default:                          /* includes undefined */
		 return IDENTIFIER;
	}
#endif
#if 0
    switch (sym_type(yytext))
    {
    case TYPEDEF_NAME:                /* previously defined */
        return TYPEDEF_NAME;
    case ENUMERATION_CONSTANT:        /* previously defined */
        return ENUMERATION_CONSTANT;
    default:                          /* includes undefined */
	return IDENTIFIER;
    }
#endif
}

/*int yyerror(const char *msg) {
    fprintf(stderr,"Error:%s\n",msg); return 0;
}*/
