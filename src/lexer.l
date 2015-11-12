%{
 
/**
 * @file lexer.l
 * To generate the lexical analyzer run: "flex lexer.l"
 */
 
#include "node.h"
#include "parser.h"

#include <cctype>
#include <iostream>
#include <vector>
#include <cstring>

class pos_counter
{
	static int last_line;
	static int last_col;
	static int last_mult;
public:
	static span_t count(const char* text_pos, std::size_t mult = 1) {
		static const char* last_text = text_pos;
		last_col += ((text_pos - last_text) * last_mult);
		std::cout << "text: " << text_pos << std::endl;
		std::cout << "last_col: " << last_col << ", mult: " << mult << std::endl;
		last_text = text_pos;
		last_mult = mult;
		
		//geom_t geom(last_pos::value.line, yytext - last_text) last_span::value =  }
		return span_t();
	}

	// -1 because we are before char -1 respective to the new line
	static void newline() { ++last_line; last_col=-1; }

	static geom_t pos() { return geom_t(last_line, last_col); }
};

int pos_counter::last_line = 0;
int pos_counter::last_col = 0;
int pos_counter::last_mult = 1;

span_t cnt(const char* text_pos, std::size_t mult = 1) {
	return pos_counter::count(text_pos, mult); }

geom_t get_pos() { return pos_counter::pos(); } // FEATURE: common header instead of extern?

struct token_str
{
	static std::vector<node_base*> value;
};

std::vector<node_base*> token_str::value;

std::vector<node_base*>& get_token_vector() { return token_str::value; }

//! creates a new token and appends it
int app3(token_t*& token, int token_id, const char* text, int mult = 1)
{ // FEATURE: rename to app() or app_token()
	cnt(text, mult);
	token = new token_t(get_pos(), token_id);
	token_str::value.push_back(token);
	return token_id;
}

/*int app_num(int value) {
	return token_str::value.push_back(new number_t(value)), I_CONSTANT;
}*/

//! appends an already created terminal
template<class T> int app(T* elem, int lex_id/*, const char* text, int mult = 1*/)
{
	//cnt(text, mult);
	token_str::value.push_back(elem);
	return lex_id;
}

template<class T>
int app_with_string(T*& token, int lex_id, const char* text, const char* end) {
	std::size_t length = end - text;
	cnt(text);
	char* res = new char[length + 1]; res[length] = 0; /*std::copy(yytext, end, yylval->name);*/ strncpy(res, text, length);

	return app(token = new T(res, get_pos()), lex_id);
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
static int check_type(void);
%}

%%
"/*"					{ comment(); }
"//".*					{ /* consume //-comment */ }

"auto"					{ return app3(yylval->token, AUTO, yytext); } // TODO: 2x AUTO
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
"typedef"				{ return app3(yylval->token, TYPEDEF, yytext); }
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

{L}{A}*					{ const char* end = yytext + 1; // TODO: write TESTS for ALL TERMINALS...
						for(; isalnum(*end) || *end == '_'; ++end) ;
						std::size_t length = end - yytext;
						cnt(yytext);
						char* res = new char[length + 1]; res[length] = 0; /*std::copy(yytext, end, yylval->name);*/ strncpy(res, yytext, length);
						
						return app(yylval->name = new identifier_t(res, get_pos()), check_type()); }

{HP}{H}+{IS}?				{ sscanf(yytext, "%x", &yylval->_int); return I_CONSTANT; } // TODO: position
{NZ}{D}*{IS}?				{ /*int i; sscanf(yytext, "%d", &i); return new cireturn I_CONSTANT;*/ sscanf(yytext, "%x", &yylval->_int); return I_CONSTANT; }
"0"{O}*{IS}?				{ sscanf(yytext, "%o", &yylval->_int); return I_CONSTANT; }
{CP}?"'"([^'\\\n]|{ES})+"'"		{ const char* pars = yytext; if(*pars == 'u' || *pars == 'U' || *pars == 'L') ++pars; ++pars;
						char tmp_c; sscanf(pars, "%c", &tmp_c); yylval->_int = tmp_c; return I_CONSTANT; }

{D}+{E}{FS}?				{ sscanf(yytext, "%f", &yylval->_float); return F_CONSTANT; }
{D}*"."{D}+{E}?{FS}?			{ sscanf(yytext, "%f", &yylval->_float); return F_CONSTANT; }
{D}+"."{E}?{FS}?			{ sscanf(yytext, "%f", &yylval->_float); return F_CONSTANT; }
{HP}{H}+{P}{FS}?			{ sscanf(yytext, "%f", &yylval->_float); return F_CONSTANT; }
{HP}{H}*"."{H}+{P}{FS}?			{ sscanf(yytext, "%f", &yylval->_float); return F_CONSTANT; }
{HP}{H}+"."{P}{FS}?			{ sscanf(yytext, "%f", &yylval->_float); return F_CONSTANT; }

({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+	{ const char* pars = yytext; if(*pars == 'u' || *pars == 'U' || *pars == 'L') ++pars; ++pars;
						const char* end = pars + 1;
						for(; *end != '"'; ++end) ;
						--end;
						std::size_t length = end - yytext;
						cnt(yytext);
						std::cout << "LENGTH: " << length << std::endl;
						char* res = new char[length + 1]; res[length] = 0; /*std::copy(yytext, end, yylval->name);*/ strncpy(res, pars, length);
						std::cout << "STR: " << res << std::endl;
						// FEATURE: common routine with identifier
						return app(yylval->name = new identifier_t(res, get_pos()), STRING_LITERAL); }

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
"->"					{ return app3(yylval->token, PTR_OP, yytext); }
"&&"					{ return app3(yylval->token, AND_OP, yytext); }
"||"					{ return app3(yylval->token, OR_OP, yytext); }
"<="					{ return app3(yylval->token, LE_OP, yytext); }
">="					{ return app3(yylval->token, GE_OP, yytext); }
"=="					{ return app3(yylval->token, EQ_OP, yytext); }
"!="					{ return app3(yylval->token, NE_OP, yytext); }
";"					{ return app3(yylval->token, ';', yytext); }
("{"|"<%")				{ return app3(yylval->token, '{', yytext); }
("}"|"%>")				{ return app3(yylval->token, '}', yytext); }
","					{ return app3(yylval->token, ',', yytext); }
":"					{ return app3(yylval->token, ':', yytext); }
"="					{ return app3(yylval->token, '=', yytext); }
"("					{ return app3(yylval->token, '(', yytext); }
")"					{ return app3(yylval->token, ')', yytext); }
("["|"<:")				{ return app3(yylval->token, '[', yytext); }
("]"|":>")				{ return app3(yylval->token, ']', yytext); }
"."					{ return app3(yylval->token, '.', yytext); }
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

"\t"					{ app3(yylval->token, '\t', yytext, 8); }
"\n"					{ app3(yylval->token, '\n', yytext); pos_counter::newline(); }
"\v"					{ throw "non-supported sign \\v"; }
"\f"					{ throw "non-supported sign \\f"; }
" "					{ app3(yylval->token, ' ', yytext); }
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

static int check_type(void)
{
    switch (sym_type(yytext))
    {
    case TYPEDEF_NAME:                /* previously defined */
        return TYPEDEF_NAME;
    case ENUMERATION_CONSTANT:        /* previously defined */
        return ENUMERATION_CONSTANT;
    default:                          /* includes undefined */
	return IDENTIFIER;
    }
}

/*int yyerror(const char *msg) {
    fprintf(stderr,"Error:%s\n",msg); return 0;
}*/
