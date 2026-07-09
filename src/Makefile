# Makefile

FILES	= lexer.o parser.o node.o visitor.o token.o geom.o ast.o type_completor.o utils.o info.o
#CC	= gcc
#CXX	= g++
# -Werror -Wno-unused ?
DBGFLAGS=-g -ggdb -DYYDEBUG=1
#DBGFLAGS=-O2
FLAGS	= -Wall -Wextra -Wno-unused -Wno-overloaded-virtual ${DBGFLAGS} -ansi -pipe
CFLAGS	= -fPIC
LFLAGS	= -shared
#BISON_BIN = /home/t0161834/pakete/bison-3.0.4/install/bin/bison
#FLEX_BIN = /home/t0161834/pakete/flex-2.5.39/install/bin/flex
HEADERS := lexer.h parser.h $(shell find . -name '*.h' 2>/dev/null)

all: libbparser.so
libbparser.so: $(FILES)
	$(CXX) $(FLAGS) $(LFLAGS) *.o -o libbparser.so

lexer.c: lexer.l 
	$(FLEX_BIN) lexer.l

# lexer.h/parser.h are generated as side outputs of flex/bison runs.
# Keep explicit targets so Make can satisfy header prerequisites without
# falling back to implicit C compile rules for lexer.c/parser.c.
lexer.h: lexer.c

parser.c: parser.y lexer.c
	$(BISON_BIN) -t --debug -Wno-conflicts-sr parser.y

parser.h: parser.c

# default compilation rule
%.o: %.cpp lexer.c parser.c $(HEADERS)
	$(CXX) $(FLAGS) $(CFLAGS) -c $< -o $@
# main.cpp has no header file:
#main.o: main.cpp lexer.c parser.c
#	$(CXX) $(FLAGS) $(CFLAGS) -c $< -o $@
# rule to compile lexer and parser:
%.o: %.c lexer.c parser.c $(HEADERS)
	$(CXX) $(FLAGS) $(CFLAGS) -ansi -c $< -o $@
clean:
	rm -f *.o *~ lexer.c lexer.h parser.c parser.h libbparser.so # main
rebuild: clean all
