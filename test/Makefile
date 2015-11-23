# Makefile

FILES	= lexer.o parser.o node.o visitor.o
#CC	= gcc
#CXX	= g++
# -Werror -Wno-unused ?
FLAGS	= -Wall -Wextra -Wno-unused -Wno-overloaded-virtual  -g -ggdb -DYYDEBUG=1 -pipe
CFLAGS	= -I../src
LFLAGS	= -L../src -lbparser

all: test
test: test.o ../src/libbparser.so
	$(CXX) $(FLAGS) $(LFLAGS) test.o -o test

# test.cpp has no header file:
test.o: test.cpp ../src/*.h
	$(CXX) $(FLAGS) $(CFLAGS) -c $< -o $@
clean:
	rm -f test.o test
rebuild: clean all
