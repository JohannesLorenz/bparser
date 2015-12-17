# Makefile

FILES	= lexer.o parser.o node.o visitor.o token.o geom.o ast.o type_completor.o
#CC	= gcc
#CXX	= g++
# -Werror -Wno-unused ?
FLAGS	= -Wall -Wextra -Wno-unused -Wno-overloaded-virtual  -g -ggdb -ansi -DYYDEBUG=1 -pipe
CFLAGS	= -fPIC
LFLAGS	= -shared
#BISON_BIN = /home/t0161834/pakete/bison-3.0.4/install/bin/bison
#FLEX_BIN = /home/t0161834/pakete/flex-2.5.39/install/bin/flex

all: libbparser.so
libbparser.so: $(FILES)
	$(CXX) $(FLAGS) $(LFLAGS) *.o -o libbparser.so

lexer.c: lexer.l 
	$(FLEX_BIN) lexer.l

parser.c: parser.y lexer.c
	$(BISON_BIN) -t --debug parser.y

# default compilation rule
%.o: %.cpp lexer.c parser.c %.h
	$(CXX) $(FLAGS) $(CFLAGS) -c $< -o $@
# main.cpp has no header file:
#main.o: main.cpp lexer.c parser.c
#	$(CXX) $(FLAGS) $(CFLAGS) -c $< -o $@
# rule to compile lexer and parser:
%.o: %.c lexer.c parser.c node.h lexer_common.h
	$(CXX) $(FLAGS) $(CFLAGS) -ansi -c $< -o $@
clean:
	rm -f *.o *~ lexer.c lexer.h parser.c parser.h libbparser.so # main
rebuild: clean all
