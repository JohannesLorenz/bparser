# Makefile

FILES	= lexer.o parser.o node.o visitor.o main.o
CC	= g++
# -Werror -Wno-unused ?
CFLAGS	= -Wall -Wextra -Werror -Wno-unused -g -ansi -DYYDEBUG=1
BISON_BIN = /home/t0161834/pakete/bison-3.0.4/install/bin/bison
FLEX_BIN = /home/t0161834/pakete/flex-2.5.39/install/bin/flex

all: main
main: $(FILES)
	$(CC) $(CFLAGS) *.o -o main

lexer.c: lexer.l 
	$(FLEX_BIN) lexer.l

parser.c: parser.y lexer.c
	$(BISON_BIN) -t --debug parser.y

# default compilation rule
%.o: %.cpp lexer.c parser.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
# main.cpp has no header file:
main.o: main.cpp lexer.c parser.c
	$(CC) $(CFLAGS) -c $< -o $@
# rule to compile lexer and parser:
%.o: %.c lexer.c parser.c node.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o *~ lexer.c lexer.h parser.c parser.h main
rebuild: clean all