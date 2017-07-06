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
	@file lexer_common.h Common routines for the lexer.
	Only included by the lexer.
	Contains complicated logic and is not easy to understand.
*/

#ifndef LEXER_COMMON_H
#define LEXER_COMMON_H

#include <stdexcept>
#include <cassert>
#include <cctype>
#include <iostream>
#include <vector>
#include <cstring>
#include <map>

#include "options.h"
#include "node.h"
#include "parser.h"
using namespace nodes;

int strict_mode;
bool bparser_debug;
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
	static int last_col, last_tabcol;

	static bool is_whitespace_no_newline(int c)
	{
		switch(c)
		{
			case ' ':
			case '\t':
			case '\v':
			case '\f':
				return true;
			default:
				return false;
		}
	}

	static bool is_whitespace(int c)
	{
		switch(c)
		{
			case ' ':
			case '\t':
			case '\v':
			case '\f':
			case '\n':
				return true;
			default:
				return false;
		}
	}

public:
	static span_t count()
	{
		if(get_token_vector().size())
		{
			terminal_t* prev = get_token_vector().back();

			std::size_t newlines = prev->get_newlines(),
				length = prev->get_length(),
				tablength = prev->get_tablength();

			if(newlines)
			{
				last_line += newlines;
				last_col = 1 + length;
				last_tabcol = tablength ? (1 + tablength) : (1 + length);
			}
			else
			{
				last_col += length;
				if(tablength)
				 last_tabcol += tablength - ((last_tabcol-1) % tablength);
				else
				 last_tabcol += length;
				// -> -1 because the tabs are aligned at 1+8*N
			}
		}
		return span_t();
	}

	static geom_t pos() {
		//std::cout << "pos call, last_linebreak was: " << *last_linebreak << std::endl;
		return geom_t(last_file, last_line,
		last_col, last_tabcol); }

	static void parse_ppline(const char* p)
	{
		++p; // skip #
		for(; is_whitespace_no_newline(*p); ++p) ;
		last_col = last_tabcol = 1;
		
		// no line number - ignore such macros
		if( *p < '0' || *p > '9')
		 return;

		{
			int n;
			sscanf(p, "%d%n", &last_line, &n);
			p += n;
		}
		// the last token in the token vector is \n,
		// so we add a decrement manually:
		--last_line;
		for(; is_whitespace_no_newline(*p); ++p) ;
		std::string fname;
		if(*p == '\"') ++p;
		for(; !is_whitespace(*p); ++p) {
			if(*p == '\"' && is_whitespace(p[1])) {}
			else
			 fname += *p;
		}
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
		last_line = last_col = last_tabcol = 1;
	}
};

int pos_counter::last_file = 0; // default file
// we start at 1, 1 (typical convention)
int pos_counter::last_line = 1;
int pos_counter::last_col = 1;
int pos_counter::last_tabcol = 1;

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


int recent_typedef_keyword = -1;

class lookup_table_t
{
	typedef std::pair<lookup_type, std::size_t> value_entry_t; // lookup type, scope
	typedef std::vector<value_entry_t> value_t;
	typedef std::pair<std::string, value_t> entry_t;
	typedef std::map<std::string, value_t> table_t;
	static table_t table;

	static std::size_t bracket_depth;

public:
	static void dump()
	{
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
	}
	
	static void notify_dec_decl_depth(int new_depth)
	{
#ifdef LEXER_DEBUG
		std::cout << "DEPTH decreased to: " << new_depth << std::endl;
#endif
		if(recent_typedef_keyword > new_depth)
		 recent_typedef_keyword = -1;

		table_t::iterator itr = table.begin(),
			next = table.begin();
		if(bparser_debug)
		 dump();
		for(itr = table.begin(); itr != table.end(); itr = next)
		{
			// invariant: itr == next
			if(next != table.end())
			 ++next;
			// FEATURE: only the last element should be affected
			// this is just for the lazy thing with )) working wrong

			for(bool find_more = true; find_more; )
			{
				find_more = false;

				// invariant: ++itr == next || next == table.end()
				//std::cout << itr->second.second << " <-> " << bracket_depth << std::endl;
				if(itr->second.back().second >= (std::size_t)new_depth + 1)
				{
				//	std::cout << itr->first << ": " << itr->second.back().first << std::endl;
					if(itr->second.back().first == lt_identifier_list)
					{
						dump();
						std::cout << "undefined: " << itr->first << std::endl;
						throw std::runtime_error("Left scope with undefined variable!");
					}

					// out of scope
					itr->second.pop_back();
					if(itr->second.empty())
					 table.erase(itr);
					else
					 find_more = true;
				}
			}
		//	else if(itr->second.back().second > (new_depth + 2))
		//	 throw "overseen last scope end";
		}
	}

	static std::string internal_name_of_new_id(const char* new_id, lookup_type lt) {
		std::string rval = (lt == lt_struct_bound) ? "struct " : "";
		return rval += new_id;
	}

	static void flag_symbol(const char* str, lookup_type type, int new_depth = 0)
	{
		if(new_depth < 0)
		{
#ifdef LEXER_DEBUG
			std::cout << "flagging " << str << " of type " << type << ": depth " << new_depth << " ..." << std::endl;
#endif
			throw std::logic_error("Invalid depth calculation for variable");
		}
		
		std::string new_name = internal_name_of_new_id(str, type);

		table_t::iterator itr = table.find(new_name);
		if(itr != table.end())
		{
			value_t& stack = itr->second;
			if(stack.back().first == lt_identifier_list)
			{
				if(type != lt_identifier && type != lt_identifier_list)
				 throw std::runtime_error("declarator type must be identifier if it follows an identifier list.");
				else {
					std::cout << "found *LATE DEFINED* identifier: " << new_name << std::endl;
					itr->second.back().first = type;
					++itr->second.back().second;
				}
			} else
			{
				if(stack.back().second != (std::size_t)new_depth // just declared in an inner scope
					||
					(stack.back().first == lt_struct_bound && // TODO: not only stack.back?
					type == lt_struct_bound))
				{
#ifdef LEXER_DEBUG
					std::cout << "flagging: " << new_name << " as " << type << " (" << get_pos() << ", depth " << new_depth <<  ")" <<  std::endl;
#endif
					stack.push_back(value_entry_t(type, new_depth));
				}
				else
				{
					if(strict_mode)
					 throw std::logic_error("identifier has already been declared in this scope!");
					else
					 { /* Nothing to do - the variable is already defined */ }
				}
			}

		}
		else
		{
#ifdef LEXER_DEBUG
			std::cout << "flagging: " << str << " as " << type << " (" << get_pos() << ", depth " << new_depth << ")" <<  std::endl;
#endif
			table[new_name].push_back(value_entry_t(type, new_depth));
			//table.insert(entry_t(str, value_t()))
			//value_entry_t(type, new_depth)));
		}
	}

	static lookup_type type_of(const char* str) {
		if(bparser_debug)
		{
			dump();
			std::cout << "... looking for: " << str << std::endl;
		}
		table_t::const_iterator itr = table.find(str);
		return (itr == table.end()) ? lt_undefined : itr->second.back().first;
	}

	static void clear() {
		for(table_t::iterator itr = table.begin(); itr != table.end(); ++itr)
		{
			value_t& stack = itr->second;
			if(stack.empty()) throw std::logic_error("stack for identifier should not be empty");
			while(stack.size())
			{
				if(stack.back().first == lt_identifier_list) {
					std::cout << "Error: remaining undeclared identifier" << itr->first << "(from identifier list?)" << std::endl;
					throw std::logic_error("Error remaining undeclared identifier (from identifier list?)");
				}
				else stack.pop_back();
			}
		}
		table.clear(); }
};

void dump_lookup_table()
{
	lookup_table_t::dump();
}

lookup_table_t::table_t lookup_table_t::table;
std::size_t lookup_table_t::bracket_depth = 0;

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
	declarator_found, //!< this will only be used for a short time
	expect_initializer_or_comma,
	inside_initializer
} declaration_state = expect_type_specifier;
//int declaration_state_braces = 0;

enum { MAYBE_IDENTIFIER = 4242 } ; // some value that has not been used

class states_t
{
//	int declaration_state_pars_after;
//	int declaration_state_braces_after;
/*	int after_decl_pars_depth;
	int after_decl_braces_depth;*/
	int decl_depth; //!< used for scoping
	bool in_for_header;
	int par_count; //!< counts parantheses depth: )(
	int brack_count; //!< counts bracket depth: }{

	//! marked from declarator to first of ,;(
	//! used to increase depth on the first ( after declarator, but not after subsequent ones
	//! removed for subsequent declarators like b in 'int a, b;'
	//! for initializers, it does not matter, since the declaration depth is irrelevant
	bool recent_declaration;
	int lazy_decr_decl_depth;
	bool _maybe_struct_definition; //!< some struct keywords introduce declarations, others don't
	int in_enum;
	int struct_depth;
	bool struct_depth_decreased;

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
		_maybe_struct_definition(false),
		in_enum(0),
		struct_depth(0)
		{}
	
	void check_variables_initial() {
		// if(*this == cmp) {} FEATURE?
#if 0
		if(decl_depth) throw std::logic_error("decl_depth");
		if(in_for_header) throw std::logic_error("in_for_header");
		if(par_count) throw std::logic_error("par_count");
		if(brack_count) throw std::logic_error("brack_count");
		if(recent_declaration) throw std::logic_error("recent_declaration");
		if(lazy_decr_decl_depth) throw std::logic_error("lazy_decr_decl_depth");
		if(_maybe_struct_definition) throw std::logic_error("_maybe_struct_definition");
		if(in_enum) throw std::logic_error("in_enum");
		if(struct_depth) throw std::logic_error("struct_depth");
		if(struct_depth_decreased) throw std::logic_error("struct_depth_decreased");
#endif
	}

	~states_t() { check_variables_initial(); }

	//! resets all variables to initial states
	void reset()
	{
		// actually, no variables are reset - they are assumed to already
		// carry initial values, if not, we exit
		check_variables_initial();
	}

	int get_brack_count() const { return brack_count; }
	int get_decl_depth() const { return decl_depth; }
	void maybe_struct_definition() { _maybe_struct_definition = true; }
	int enum_state() const { return in_enum; }

	std::string recent_was_flagged_unknown; // FEATURE: use token pointer

	//! returns additional number to add to the declaration depth
	int get_add_number(lookup_type lt) {
		// parantheses after are only valid
		return /*(int)(in_for_header)*/ - (lt == lt_enumeration) /*+ declaration_state_pars_after*/
			- (lt == lt_identifier_list);
	}

	void set_state(const char* text, int token_id)
	{
		// called on all tokens except whitespace
		assert(token_id != ' ' && token_id != '\n' && token_id != '\t' && token_id != '\v' && token_id != '\f');
		
		bool brack_depth_decreased = false;
		struct_depth_decreased = false;
		
		/*
			step 1: recent tokens -> variables
		*/
		// decrease declaration depth if requested from last token
		if(lazy_decr_decl_depth)
		{
			--lazy_decr_decl_depth;
			decl_depth--;
		}

		// count tokens, do not yet read or change decl state
		switch(token_id)
		{
			case '(': ++par_count;
				if(recent_declaration || in_for_header)
				{
					++decl_depth;
					recent_declaration = false;
				} break;
			case ')':
				if(decl_depth == par_count) {
					//if(token_id != '{') // keep parameters in function
					// --decl_depth;
					lazy_decr_decl_depth = 2;
//					lookup_table_t::notify_dec_decl_depth(decl_depth + brack_count);
				}
				if(!--par_count)
				if(in_for_header) // for headers can not be in parantheses, so
				{ // !par_count is the exact condition (CITE)
					in_for_header = false;
					// might need to smash out variables
					//lookup_table_t::notify_dec_decl_depth(decl_depth + brack_count);
				}
				break;
			case ENUM:
				in_enum = 1; break;
			case '=':
				if(in_enum)
					in_enum = 2;
				recent_declaration = false;
				break;
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
				{
					int check = recent_tokens[(recent_tokens[0] == IDENTIFIER || recent_tokens[0] == MAYBE_IDENTIFIER) ? -1 : 0];
					if(check == STRUCT || check == UNION || check == ENUM)
					 ++struct_depth;
				}
				++brack_count;
				break;
			case '}':
				--brack_count, brack_depth_decreased = true;
				if(struct_depth) // this is no C++ ;) }{ inside structs means: another struct
				 --struct_depth, struct_depth_decreased = true;
		}

		if(recent_tokens[0] == '}')
		 in_enum = false; // enums always end on next }
		
		/*
			step 2: variables -> notify lookup_table about decreasing decl_depth
		*/
		// there are two such cases: } and )
		// basically, in almost all cases, } and ) (from previous token) decrease the lookup table,
		// except for ){, which can only occur after for loops and function headers
		// FEATURE: only one function call, set parameters in if-else
		if(brack_depth_decreased)
		 lookup_table_t::notify_dec_decl_depth(decl_depth + brack_count);
		else if(lazy_decr_decl_depth == 1)
		{
			lazy_decr_decl_depth = 0;

			// declaration depth has changed see above), notify lookup table now
			switch(token_id)
			{
				// the calculations differ because decl_depth or brack_count may
				// already have changed since last token
				case '(':
					// erase temporarily
					lookup_table_t::notify_dec_decl_depth(decl_depth + brack_count - 1);
				default:
					// keep variables from for loop or function header (FEATURE: useless?)
					// otherwise, the brace is not reopening, so smash out
					//   (in this case decl_depth + brack_count is smaller than in the
					//   first case)
					lookup_table_t::notify_dec_decl_depth(decl_depth + brack_count);
			}
		}
		
		/*
			step 3: variables -> set next declaration state
		*/
		if(enum_state()) {
			if(token_id == '}') // can not be in enum (TODO?) => end of enum => type specifier
				declaration_state = expect_declaration_specifiers_braces_pointers_type_qualifiers_identifier;
		}
		else
		{
			declaration_state_t next_state = expect_type_specifier;
			switch(declaration_state)
			{
				case expect_type_specifier:
					if(is_type_specifier(text, token_id, false)) // TODO: put this together with enum_state() ?
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
							// this is the only case where an initializer can follow
							// *without* a declarator identifier
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
							// of course, we expect initializer or comma
							// declarator_found will be changed to that below
							break;
					}
					break;
				case declarator_found:
					throw std::logic_error("this function should never be called with declarator_found");
				case expect_initializer_or_comma:
				case inside_initializer:
					switch(token_id)
					{
						// these mark the end of a declaration
						case ';':
					//	case '}':
					//	case ')':
							next_state = (enum_state())
								// end of enum block can be reached right
								// after declarator was found:
								// TODO: bad comment, is this a bug??
								? expect_braces_pointers_type_qualifiers_identifier
								: expect_type_specifier;
							break;
						case ')':
						/*	next_state = (declaration_state_pars_after > 0)
								? expect_braces_pointers_type_qualifiers_identifier
								: expect_type_specifier;*/
							next_state = (declaration_state == expect_initializer_or_comma)
								? expect_type_specifier : declaration_state;
							// TODO: put these cases together
						//	if(declaration_state_pars_after > 0)
						//	 throw std::logic_error("impossible");
							break;
						case '(':
							next_state = (declaration_state == expect_initializer_or_comma)
								? expect_type_specifier : declaration_state;
							break;
						case ',':
							next_state = (decl_depth > 0)
								// in a function, a comma separates identifiers: "void f(int a, int b)"
								? expect_type_specifier
								// outside a function, we may have multiple declarators of
								// "similar" type: "int a,b";
								: expect_braces_pointers_type_qualifiers_identifier;
							break;
						case '=':
							next_state = inside_initializer;
							break;
						case '{':
							next_state = (declaration_state == inside_initializer)
								? inside_initializer
								: expect_type_specifier;
							break;
						default: // might be some complicated intializating expression
							next_state = declaration_state;
					}
			}


			declaration_state = next_state;
			if (declaration_state == declarator_found)
			 recent_declaration = true;
		}

		// std::cout << "SET STATE NOW: " << declaration_state << " after text: "<< text << std::endl;
		
		add_recent_token(token_id); // updates recent_tokens

		// handle earlier defined labels, maybe flag symbols + set declaration state
		// TODO: above everything
		if(recent_was_flagged_unknown.size())
		{
			switch(token_id)
			{
				case ':': // flag goto-label
					flag_symbol(recent_was_flagged_unknown.c_str(), lt_identifier);
					break;
				case '{': // C99: struct + { => struct declaration
					if(_maybe_struct_definition)
					 flag_symbol(recent_was_flagged_unknown.c_str(), lt_struct_bound, -1);
					declaration_state = expect_type_specifier;
					break;
				default: ;
			}
		}
		recent_was_flagged_unknown.clear();
		_maybe_struct_definition = false;

	}

	void flag_symbol(const char* symbol, lookup_type lt, int special = 0)
	{
#ifdef LEXER_DEBUG
		std::cout << "flag: brack_count: " << brack_count << ", decl_depth: " << decl_depth << std::endl;
#endif
		// gcc somehow allows enums in structs to be globally visible...
		int new_depth = (lt == lt_enumeration && struct_depth) ? 0 : 
			get_add_number(lt) + brack_count + decl_depth + special;

		lookup_table_t::flag_symbol(symbol, lt, new_depth);
		if(lt != lt_identifier_list)
		 declaration_state = expect_initializer_or_comma;
	}

	//! @param id_is_decl this means that if the token is maybe an identifier,
	//!  then this means that the token makes a declarator
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
				if(recent_tokens[0] == STRUCT || recent_tokens[0] == UNION
					|| recent_tokens[0] == ENUM) // FEATURE: remove or keep the ENUM check?
				 return true;
				else
				// if identifier is a declarator, than we have a declaration, no type_specifier
				// otherwise, we have no declaration, so the maybe-id must be known
				// then, it is only a type specifier if it is a typedef name
				 return (!id_is_decl) && lookup_table_t::type_of(text) == lt_typedef_name;
			case '}':
				// only structs + enums, but enums currently bypass this function
				return struct_depth_decreased;
			default:
				return false;
		}
	}

};

states_t states;

void reset_states() { states.reset(); }

// TODO: fwd declare overloaded?

//! creates a new token and appends it
int app2(token_t*& token, int token_id)
{
	cnt();
	token = new token_t(get_pos(), token_id);
	token_str::value.push_back(token);
	return token_id;
}

int app3(token_t*& token, int token_id, const char* text)
{ // FEATURE: rename to app() or app_token()
	states.set_state(text, token_id);
	return app2(token, token_id);
}

//! appends an already created terminal
template<class T> int app(T* elem, int lex_id)
{
	token_str::value.push_back(elem);
	return lex_id;
}

//! appends with a string given that is only restricted by @a length. sets state
template<class T>
int app_with_string(T*& token, int lex_id, const char* text, std::size_t length)
{
	states.set_state(text, lex_id);
	cnt();
	char* res = new char[length + 1]; res[length] = 0; /*std::copy(yytext, end, yylval->name);*/ strncpy(res, text, length);
	return app(token = new T(res, get_pos()), lex_id);
}

//! appends a comment
void app_comment(const std::string& com)
{
	cnt();
	app(new comment_t(com, get_pos()), 0);
}

bool icmp(const char* p, char c) {
	return tolower(*p) == tolower(c);
}

int app_float(fconstant_t*& token, const char* text, std::size_t length)
{
	const char* p = text;
	{
		float f; int n;
		sscanf(text, "%f%n", &f, &n);
		p += n;
	}
	if(*p && (icmp(p,'f') || icmp(p,'l'))) ++p;
	if(*p) throw std::logic_error("end of token not 0");
	return app_with_string(token, F_CONSTANT, text, length);
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

int app_int(iconstant_t*& token, const char* text, char scanf_type, std::size_t length)
{
	const char* p = text;
	{
		char scanf_str [5] = {'%', scanf_type, '%', 'n', 0};
		int tmp, n;
		sscanf(text, scanf_str, &tmp, &n);
		p += n;
	}
	return app_with_string(token, I_CONSTANT, text, length);
}

bool recent_struct_access = false;
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
