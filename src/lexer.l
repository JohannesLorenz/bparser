%{

#include "lexer_common.h"

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

static void consume_comment(yyscan_t yyscanner);
static int check_type(const char* text);
%}

%%
"/*"					{ consume_comment(yyscanner); }
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
"typedef"				{ recent_typedef_keyword = states.get_brack_count();
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
#ifdef LEXER_DEBUG
							std::cout << " -> IDENTIFIER: " << res << std::endl;
#endif
							return app(yylval->name = new identifier_t(res, get_pos()), type);
							case TYPEDEF_NAME:
#ifdef LEXER_DEBUG
							std::cout << " -> TYPEDEF NAME: " << res << std::endl;
#endif
							return app(yylval->typedef_name = new typedef_name_t(res, get_pos()), type);
							case ENUMERATION_CONSTANT:
#ifdef LEXER_DEBUG
							std::cout << " -> ENUMERATION_CONSTANT: " << res << std::endl;
#endif
							return app(yylval->enumeration_constant = new enumeration_constant_t(res, get_pos()), type);
							case ATTR_NAME:
#ifdef LEXER_DEBUG
							std::cout << " -> ATTR_NAME: " << res << std::endl;
#endif
							return app(yylval->attr_name = new attr_name_t(res, get_pos()), type);
						}
					}

{HP}{H}+{IS}?				{	return app_int(yylval->iconstant, yytext, 'x', yyleng); }
{NZ}{D}*{IS}?				{ 	return app_int(yylval->iconstant, yytext, 'd', yyleng); }
"0"{O}*{IS}?				{ 	return app_int(yylval->iconstant, yytext, 'o', yyleng); }
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
						return app_with_string(yylval->iconstant, I_CONSTANT, yytext, yyleng); }

{D}+{E}{FS}?				{ return app_float(yylval->fconstant, yytext, yyleng); }
{D}*"."{D}+{E}?{FS}?			{ return app_float(yylval->fconstant, yytext, yyleng); }
{D}+"."{E}?{FS}?			{ return app_float(yylval->fconstant, yytext, yyleng); }
{HP}{H}+{P}{FS}?			{ return app_float(yylval->fconstant, yytext, yyleng); }
{HP}{H}*"."{H}+{P}{FS}?			{ return app_float(yylval->fconstant, yytext, yyleng); }
{HP}{H}+"."{P}{FS}?			{ return app_float(yylval->fconstant, yytext, yyleng); }

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

							for(; *p == ' ' || *p == '\t' || *p == '\n' || *p == '\v' || *p == '\f'; ++p);


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
						return app_with_string(yylval->string_literal, STRING_LITERAL, yytext, yyleng); }

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
++braces_since_typedef;
return app3(yylval->token, '{', yytext); }
("}"|"%>")				{
						if(in_enum_block)
						 in_enum_block = false;
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

"\t"					{ app2(yylval->token, '\t'); }
"\n"					{ app2(yylval->token, '\n'); }
"\v"					{ app2(yylval->token, '\v'); }
"\f"					{ app2(yylval->token, '\f'); }
" "					{ app2(yylval->token, ' '); }
"#"([^\\\n])*"\n"			{ pos_counter::parse_ppline(yytext); }
.					{ std::cerr << "At: " << yytext << std::endl; throw "bad character"; }

%%

#if 0
int yywrap(void)        /* called at end of input */
{
    return 1;           /* terminate now */
}
#endif	

static void consume_comment(yyscan_t yyscanner)
{
    int c;
    std::string com_str = "/*";
    while ((c = yyinput(yyscanner)) != 0)
    {
        com_str += (char)c;
        if (c == '*')
        {
            while ((c = yyinput(yyscanner)) == '*')
                com_str += c;

            if (c == '/') {
                app_comment(com_str += '/');
                return;
            }

            if (c == 0)
                break;
        }
    }
    yyerror("unterminated comment");
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
#ifdef LEXER_DEBUG
	std::cout << "STATE NOW: " << declaration_state << std::endl;
#endif
	if(recent_tokens[-1] == '.' || recent_tokens[-1] == PTR_OP)
	{
		return IDENTIFIER;
	}

	if(recent_tokens[-1] == STRUCT || recent_tokens[-1] == UNION || recent_tokens[-1] == ENUM)
	{
		states.recent_was_flagged_unknown = text;
		states.maybe_struct_definition();
		//states.flag_symbol(text, lt_struct_bound);
		return IDENTIFIER;
	}
	/*
		first of all, check if this is declaration
		lookup table does not matter, since this could be an identifier with the same name
		this comparison is reliable in all cases:
	*/
	else if(states.enum_state() == 1)
	{
		// the last IF found *all* declarations, except all inside of enum {}
		// find these manually, this is easy
		states.flag_symbol(text,lt_enumeration);
#ifdef LEXER_DEBUG
		std::cout << " -> declaration, enum" << std::endl;
#endif
		return IDENTIFIER;
	}
	else if(declaration_state == declarator_found)
	{
		// typedefs can not occur in structs
		if(recent_typedef_keyword == states.get_brack_count())
		{
			states.flag_symbol(text, lt_typedef_name);
			recent_typedef_keyword = -1;
		}
		else
			states.flag_symbol(text, lt_identifier);

//		declaration_state = expect_type_specifier; // reset for next declaration
#ifdef LEXER_DEBUG
		std::cout << " -> declaration, no braces" << std::endl;
#endif
		declaration_state = expect_initializer_or_comma;

		return IDENTIFIER;
	}

#if 0
	else if(states.get_brack_count())
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
	}
#endif
	/* else if(states.declaration_state_pars_after > 0)
	{
		// must be a function. all identifiers must be declarators
		states.flag_symbol(text, lt_identifier);
#ifdef LEXER_DEBUG
		std::cout << " -> declaration, function parameter" << std::endl;
#endif
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

void yyerror(const char *msg) {
    fprintf(stderr,"Error:%s\n",msg);
}
