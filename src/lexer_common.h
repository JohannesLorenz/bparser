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

#ifndef LEXER_COMMON_H
#define LEXER_COMMON_H

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
	static void inc_bracket_depth() { /*std::cout << "INC to: " << bracket_depth + 1 << std::endl; ++bracket_depth;*/ }
	static void dec_bracket_depth() {/*
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
		--bracket_depth;*/
	}

	static void notify_dec_decl_depth(int new_depth)
	{
		std::cout << "DEPTH decreased to: " << new_depth << std::endl;
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
			if(itr->second.back().second == new_depth + 1)
			{
				// out of scope
				itr->second.pop_back();
				if(itr->second.empty())
				 table.erase(itr);
			}
			else if(itr->second.back().second > (new_depth + 2))
			 throw "overseen last scope end";
		}
	}

	static std::string internal_name_of_new_id(const char* new_id, lookup_type lt) {
		std::string rval = (lt == lt_struct_bound) ? "struct " : "";
		return rval += new_id;
	}

	static void flag_symbol(const char* str, lookup_type type, int new_depth = 0)
	{
		std::cout << "NEWDEPTH: " << new_depth << std::endl;
		std::string new_name = internal_name_of_new_id(str, type);

		table_t::iterator itr = table.find(new_name);
		if(itr != table.end())
		{
			value_t& stack = itr->second;
			if(stack.back().first == lt_identifier_list)
			{
				if(type != lt_identifier && type != lt_identifier_list)
				 throw "declarator type must be identifier if it follows an identifier list.";
				else {
					std::cout << "found *LATE DEFINED* identifier: " << new_name << std::endl;
					itr->second.back().first = type;
					++itr->second.back().second;
				}
			} else
			{
				if(stack.back().second != new_depth // just declared in an inner scope
					||
					(stack.back().first == lt_struct_bound && // TODO: not only stack.back?
					type == lt_struct_bound))
				{
					std::cout << "flagging: " << new_name << " as " << type << " (" << get_pos() << ")" <<  std::endl;
					stack.push_back(value_entry_t(type, new_depth));
				}
				else
				 throw "identifier has already been declared in this scope!";
			}

		}
		else
		{
			std::cout << "flagging: " << str << " as " << type << " (" << get_pos() << ")" <<  std::endl;

			table[new_name].push_back(value_entry_t(type, new_depth));
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
//int declaration_state_braces = 0;

enum { MAYBE_IDENTIFIER = 4242 } ; // some value that has not been used

class states_t
{
//	int declaration_state_pars_after;
//	int declaration_state_braces_after;
/*	int after_decl_pars_depth;
	int after_decl_braces_depth;*/
	int decl_depth;
	bool in_for_header;
	int par_count;
	int brack_count;
	bool recent_declaration;
	int lazy_decr_decl_depth;
	bool _maybe_struct_declaration;
	int in_enum;

	/*
	( direct declarator:
		=> case 1: function paramter: increase decl_depth
		=> case 2: int (i); -> do not increase
		=> case 3: FOR( -> do not increase
			i.e.: increase <=> after decl

	direct declarator )
		=> case 1: function parameter list end
		=> case 2:
	*/
public:
	states_t() :
		decl_depth(0),
		in_for_header(false),
		par_count(0),
		brack_count(0),
		recent_declaration(false),
		lazy_decr_decl_depth(0),
		_maybe_struct_declaration(false),
		in_enum(0)
		{}

	void reset()
	{
		if(decl_depth) throw "decl_depth";
		if(in_for_header) throw "in_for_header";
		if(par_count) throw "par_count";
		if(brack_count) throw "brack_count";
	//	if(recent_declaration) throw "recent_declaration";
		if(lazy_decr_decl_depth) throw "lazy_decr_decl_depth";
		if(_maybe_struct_declaration) throw "_maybe_struct_declaration";
		if(in_enum) throw "in_enum";
	}

	int get_brack_count() const { return brack_count; }
	int get_decl_depth() const { return decl_depth; }
	void maybe_struct_declaration() { _maybe_struct_declaration = true; }
	int enum_state() const { return in_enum; }

	std::string recent_was_flagged_unknown;

	int add_number(lookup_type lt) {
		// parantheses after are only valid
		return (int)(in_for_header) - (lt == lt_enumeration) /*+ declaration_state_pars_after*/
			- (lt == lt_identifier_list);
	}

	void set_state(const char* text, int token_id)
	{
	if(token_id != ' ' && token_id != '\n' && token_id != '\t')
	{
		if(lazy_decr_decl_depth)
		{
			std::cout << "DECR DECR DECR" << std::endl;
			--lazy_decr_decl_depth;
			decl_depth--;
		}

		// count tokens, do not yet read or change decl state
		switch(token_id)
		{
			case '(': ++par_count;
				if(recent_declaration)
				{
					++decl_depth;
					recent_declaration = false;
				} break;
			case ')': if(decl_depth == par_count) {
					if(token_id != '{') // keep parameters in function
					// --decl_depth;
						lazy_decr_decl_depth = 2;
					lookup_table_t::notify_dec_decl_depth(decl_depth + brack_count);
					}
				if(!--par_count)
				if(in_for_header)
				{
					in_for_header = false;
					lookup_table_t::notify_dec_decl_depth(decl_depth + brack_count);
				}
				break;
			case ENUM:
				in_enum = 1; break;
			case '=':
				if(in_enum)
					in_enum = 2; break;
			case ',':
				if(in_enum)
					in_enum = 1;
				recent_declaration = false;
				break;

			case FOR: in_for_header = true; break;
			case ';':
				recent_declaration = false;
				break;
			/*case ';':
				if(recent_tokens[0] == ')')
				{
					// can be end of function or for
					// erase all with bracket_depth +1
					lookup_table_t::inc_bracket_depth();
					lookup_table_t::dec_bracket_depth();
				}
				break;*/
			case '{':
				++brack_count;
				break;
			case '}':
				--brack_count;
				lookup_table_t::notify_dec_decl_depth(decl_depth + brack_count);
		}

		if(recent_tokens[0] == '}')
		 in_enum = false; // enums always end on next }

		if(lazy_decr_decl_depth == 1)
		{
			lazy_decr_decl_depth = 0;
			lookup_table_t::notify_dec_decl_depth(decl_depth + brack_count);
		}


#if 0
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
		} // TODO: remove this or put together with lower part?

		if(declaration_state == expect_initializer_or_comma
			|| declaration_state == declarator_found)
		{
			switch(token_id)
			{
				case '{':
					++declaration_state_braces_after; break;
				case '(':
					++declaration_state_pars_after; break;
			}
		}
#endif

		//if(!declaration_state_braces /*&& declaration_state_braces_after && !declaration_state_pars_after*/)
		if(enum_state()) {
			if(token_id == '}')
				declaration_state = expect_declaration_specifiers_braces_pointers_type_qualifiers_identifier;
		}
		else
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
						case ':':
							// special case: anonymous bitfields in structs
							next_state = expect_initializer_or_comma;
							break;
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
					//	case '}':
					//	case ')':
							std::cout << "CASE 1" << std::endl;
							next_state = (enum_state())
								// end of enum block can be reached right
								// after declarator was found:
								? expect_braces_pointers_type_qualifiers_identifier
								: expect_type_specifier;
							break;
						case ')':
							std::cout << "CASE 1.1" << std::endl;
						/*	next_state = (declaration_state_pars_after > 0)
								? expect_braces_pointers_type_qualifiers_identifier
								: expect_type_specifier;*/
							next_state = expect_type_specifier;

						//	if(declaration_state_pars_after > 0)
						//	 throw "impossible";
						case '(':
							std::cout << "CASE 1.2" << std::endl;
							next_state = expect_type_specifier;
							break;
						case ',':
							std::cout << "CASE 2" << std::endl;
							next_state = (decl_depth > 0)
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
			if (declaration_state == declarator_found)
			 recent_declaration = true;
		}

		std::cout << "SET STATE NOW: " << declaration_state << " after text: "<< text << std::endl;

		add_recent_token(token_id);

		// handle labels
		// TODO: above everything
		if(recent_was_flagged_unknown.size())
		{
			switch(token_id)
			{
				case ':': // flag goto-label
					flag_symbol(recent_was_flagged_unknown.c_str(), lt_identifier);
					break;
				case '{':
					if(_maybe_struct_declaration)
						// -1 because the depth has already been increased
						flag_symbol(recent_was_flagged_unknown.c_str(), lt_struct_bound, -1);
						declaration_state = expect_type_specifier;
					break;
				default: ;
			}
		}
		recent_was_flagged_unknown.clear();
		_maybe_struct_declaration = false;

	}
	}

	void flag_symbol(const char* symbol, lookup_type lt, int special = 0)
	{
		std::cout << "flag: brack_count: " << brack_count << ", decl_depth: " << decl_depth << std::endl;
		lookup_table_t::flag_symbol(symbol, lt, add_number(lt) + brack_count + decl_depth + special);
		if(lt != lt_identifier_list)
		 declaration_state = expect_initializer_or_comma;
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
			case '}':
				return true;
			default:
				return false;
		}
	}

};

states_t states;

void reset_states() { states.reset(); }

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
	suf_ll
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
int recent_typedef_keyword = -1;
bool in_enum_block = false;
int braces_since_typedef = 0;

void reset_variables()
{
	recent_struct_access = false;
	recent_typedef_keyword = -1;
	in_enum_block = false;
	braces_since_typedef = 0;
	reset_states();
	add_recent_token(-1);
	add_recent_token(-1);
	add_recent_token(-1);
	add_recent_token(-1);
}

#endif // LEXER_COMMON_H
