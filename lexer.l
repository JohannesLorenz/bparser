%{
 
/**
 * @file lexer.l
 * To generate the lexical analyzer run: "flex lexer.l"
 */
 
#include "node.h"
#include "parser.h"

#include <cassert>
#include <cstdio> // TODO: remove?
#include <iostream>
#include <vector>

class pos_counter
{
	static std::size_t last_line;
	static std::size_t last_col;
	static std::size_t last_mult;
public:
	static span_t count(const char* text_pos, std::size_t mult = 1) {
		static const char* last_text = text_pos;
		last_col += ((text_pos - last_text) * last_mult);
		std::cout << "last_col: " << last_col << ", mult: " << mult << std::endl;
		last_text = text_pos;
		last_mult = mult;
		
		//geom_t geom(last_pos::value.line, yytext - last_text) last_span::value =  }
		return span_t();
	}

	static void newline() { ++last_col; last_line=0; }

	static geom_t pos() { return geom_t(last_line, last_col); }
};

std::size_t pos_counter::last_line = 0;
std::size_t pos_counter::last_col = 0;
std::size_t pos_counter::last_mult = 1;

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
"/*"                                    { comment(); }
"//".*                                    { /* consume //-comment */ }

"auto"					{ return(AUTO); }
"break"					{ return(BREAK); }
"case"					{ return(CASE); }
"char"					{ return(CHAR); }
"const"					{ return(CONST); }
"continue"				{ return(CONTINUE); }
"default"				{ return(DEFAULT); }
"do"					{ return(DO); }
"double"				{ return(DOUBLE); }
"else"					{ return(ELSE); }
"enum"					{ return(ENUM); }
"extern"				{ return(EXTERN); }
"float"					{ return(FLOAT); }
"for"					{ return(FOR); }
"goto"					{ return(GOTO); }
"if"					{ return(IF); }
"inline"				{ return(INLINE); }
"int"					{ return app(INT, yytext); }
"long"					{ return(LONG); }
"register"				{ return(REGISTER); }
"restrict"				{ return(RESTRICT); }
"return"				{ return(RETURN); }
"short"					{ return(SHORT); }
"signed"				{ return(SIGNED); }
"sizeof"				{ return(SIZEOF); }
"static"				{ return(STATIC); }
"struct"				{ return(STRUCT); }
"switch"				{ return(SWITCH); }
"typedef"				{ return(TYPEDEF); }
"union"					{ return(UNION); }
"unsigned"				{ return(UNSIGNED); }
"void"					{ return(VOID); }
"volatile"				{ return(VOLATILE); }
"while"					{ return(WHILE); }
"_Alignas"                              { return ALIGNAS; }
"_Alignof"                              { return ALIGNOF; }
"_Atomic"                               { return ATOMIC; }
"_Bool"                                 { return BOOL; }
"_Complex"                              { return COMPLEX; }
"_Generic"                              { return GENERIC; }
"_Imaginary"                            { return IMAGINARY; }
"_Noreturn"                             { return NORETURN; }
"_Static_assert"                        { return STATIC_ASSERT; }
"_Thread_local"                         { return THREAD_LOCAL; }
"__func__"                              { return FUNC_NAME; }

{L}{A}*					{ yylval->name = new char[2]; yylval->name[0] = 'x'; yylval->name[1] = 0; return app(check_type(), yytext); }

{HP}{H}+{IS}?				{ sscanf(yytext, "%x", &tmp_i); return app_num(tmp_i); }
{NZ}{D}*{IS}?				{ sscanf(yytext, "%d", &tmp_i); return app_num(tmp_i); }
"0"{O}*{IS}?				{ sscanf(yytext, "%o", &tmp_i); return app_num(tmp_i); }
{CP}?"'"([^'\\\n]|{ES})+"'"		{ return I_CONSTANT; }

{D}+{E}{FS}?				{ return F_CONSTANT; }
{D}*"."{D}+{E}?{FS}?			{ return F_CONSTANT; }
{D}+"."{E}?{FS}?			{ return F_CONSTANT; }
{HP}{H}+{P}{FS}?			{ return F_CONSTANT; }
{HP}{H}*"."{H}+{P}{FS}?			{ return F_CONSTANT; }
{HP}{H}+"."{P}{FS}?			{ return F_CONSTANT; }

({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+	{ return STRING_LITERAL; }

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
"\n"					{ pos_counter::newline(); app('\n', yytext); /* whitespace separates tokens */ }
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
