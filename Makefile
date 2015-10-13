# Makefile

FILES	= Lexer.c Parser.c Expression.c main.c
CC	= g++
# -Werror -Wno-unused ?
CFLAGS	= -Wall -Wextra -Werror -Wno-unused -g -ansi -DYYDEBUG=1
BISON_BIN = /home/t0161834/pakete/bison-3.0.4/install/bin/bison
FLEX_BIN = /home/t0161834/pakete/flex-2.5.39/install/bin/flex

test: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o test

Lexer.c: Lexer.l 
	$(FLEX_BIN) Lexer.l

Parser.c: Parser.y Lexer.c
	$(BISON_BIN) -t --debug Parser.y

clean:
	rm -f *.o *~ Lexer.c Lexer.h Parser.c Parser.h test