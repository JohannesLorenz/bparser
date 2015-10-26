%{
 
/**
 * @file lexer.l
 * To generate the lexical analyzer run: "flex lexer.l"
 */
 
#include "node.h"
#include "parser.h"

#include <cctype>
#include <cassert>
#include <cstdio> // TODO: remove?
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

std::size_t int = 0;
std::size_t int = 0;
std::size_t int = 1;

span_t cnt(const char* text_pos, std::size_t mult = 1) {
	return pos_counter::count(text_pos, mult); }

geom_t get_pos() { return pos_counter::pos(); } // TODO: common header instead of extern?

struct token_str
{
	static std::vector<node_t*> value;
};

std::vector<node_t*> token_str::value;

std::vector<node_t*>& get_token_vector() { return token_str::value; }

int app(int token_id, const char* text, int mult = 1) {
	cnt(text, mult);
	return token_str::value.push_back(new token_t(get_pos(), token_id)), token_id;
}

token_t* app2(int token_id, const char* text, int mult = 1) {
	cnt(text, mult);
	token_t* rval = new token_t(get_pos(), token_id);
	return token_str::value.push_back(rval), rval;
}

int app3(token_t*& token, int token_id, const char* text, int mult = 1)
{
	cnt(text, mult);
	token = new token_t(get_pos(), token_id);
	token_str::value.push_back(token);
	return token_id;
}

int app_num(int value) {
	return token_str::value.push_back(new number_t(value)), I_CONSTANT;
}

template<class T> void app(T* elem) {
	token_str::value.push_back(elem);
}

int tmp_i;

token_t* tk(int id, const char* text) {
	cnt(text);
	token_t* t = new token_t(get_pos(), id);
	app(t);
	return t;
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

"auto"					{ yylval->token = app2(AUTO, yytext); return(AUTO); } // TODO: 2x AUTO
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
"long"					{ return(LONG); }
"register"				{ yylval->token = app2(REGISTER, yytext); return(REGISTER); }
"restrict"				{ return(RESTRICT); }
"return"				{ return(RETURN); }
"short"					{ return(SHORT); }
"signed"				{ return(SIGNED); }
"sizeof"				{ return(SIZEOF); }
"static"				{ yylval->token = app2(STATIC, yytext); return(STATIC); }
"struct"				{ return(STRUCT); }
"switch"				{ return(SWITCH); }
"typedef"				{ yylval->token = app2(TYPEDEF, yytext); return(TYPEDEF); }
"union"					{ return(UNION); }
"unsigned"				{ yylval->token = app2(UNSIGNED, yytext); return(UNSIGNED); }
"void"					{ return(VOID); }
"volatile"				{ return(VOLATILE); }
"while"					{ return(WHILE); }
"_Alignas"                              { return ALIGNAS; }
"_Alignof"                              { return ALIGNOF; }
"_Atomic"                               { return ATOMIC; }
"_Bool"                                 { return app3(yylval->token, BOOL, yytext); }
"_Complex"                              { return COMPLEX; }
"_Generic"                              { return GENERIC; }
"_Imaginary"                            { return IMAGINARY; }
"_Noreturn"                             { return NORETURN; }
"_Static_assert"                        { return STATIC_ASSERT; }
"_Thread_local"                         { return THREAD_LOCAL; }
"__func__"                              { return FUNC_NAME; }

{L}{A}*					{ const char* end = yytext + 1; // TODO: TEST ALL TERMINALS...
						for(; isalnum(*end) || *end == '_'; ++end) ;
						std::size_t length = end - yytext;
						yylval->name = new char[length + 1]; yylval->name[length] = 0; /*std::copy(yytext, end, yylval->name);*/ strncpy(yylval->name, yytext, length);
						return app(check_type(), yytext); }

{HP}{H}+{IS}?				{ sscanf(yytext, "%x", &yylval->_int); return I_CONSTANT; }
{NZ}{D}*{IS}?				{ sscanf(yytext, "%d", &yylval->_int); return I_CONSTANT; }
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
						std::cout << "LENGTH: " << length << std::endl;
						yylval->name = new char[length + 1]; yylval->name[length] = 0; /*std::copy(yytext, end, yylval->name);*/ strncpy(yylval->name, pars, length);
						std::cout << "STR: " << pars << std::endl;
						// TODO: common routine with identifier
						return STRING_LITERAL; }

"..."					{ return ELLIPSIS; }
">>="					{ return RIGHT_ASSIGN; }
"<<="					{ return LEFT_ASSIGN; }
"+="					{ return ADD_ASSIGN; }
"-="					{ return SUB_ASSIGN; }
"*="					{ return MUL_ASSIGN; }
"/="					{ return DIV_ASSIGN; }
"%="					{ return MOD_ASSIGN; }
"&="					{ return AND_ASSIGN; }
"^="					{ return XOR_ASSIGN; }
"|="					{ return OR_ASSIGN; }
">>"					{ return RIGHT_OP; }
"<<"					{ return LEFT_OP; }
"++"					{ return INC_OP; }
"--"					{ return DEC_OP; }
"->"					{ return PTR_OP; }
"&&"					{ return AND_OP; }
"||"					{ return OR_OP; }
"<="					{ return LE_OP; }
">="					{ return GE_OP; }
"=="					{ return EQ_OP; }
"!="					{ return NE_OP; }
";"					{ return ';'; }
("{"|"<%")				{ return app('{', yytext); }
("}"|"%>")				{ return app('}', yytext); }
","					{ return ','; }
":"					{ return ':'; }
"="					{ return '='; }
"("					{ return app('(', yytext); }
")"					{ return app(')', yytext); }
("["|"<:")				{ return '['; }
("]"|":>")				{ return ']'; }
"."					{ return '.'; }
"&"					{ return '&'; }
"!"					{ return '!'; }
"~"					{ return '~'; }
"-"					{ return '-'; }
"+"					{ return '+'; }
"*"					{ return '*'; }
"/"					{ return '/'; }
"%"					{ return '%'; }
"<"					{ return '<'; }
">"					{ return '>'; }
"^"					{ return '^'; }
"|"					{ return '|'; }
"?"					{ return '?'; }

"\t"					{ app('\t', yytext, 8); /* whitespace separates tokens */ }
"\n"					{ app('\n', yytext); pos_counter::newline(); /* whitespace separates tokens */ }
"\v"					{ throw "non-supported sign \\v"; }
"\f"					{ throw "non-supported sign \\f"; }
" "					{ cnt(yytext); app(' ', yytext); }
.					{ std::cerr << "At: " << yytext << std::endl; throw "bad character"; /* discard bad characters */ }

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
