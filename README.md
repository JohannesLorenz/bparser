# Greetings
Welcome! Thank you for considering the README file.

**Contents**
  1. What is bparser?
  2. Why using bparser?
  3. Getting started
  4. Documentation
  5. Credits
  6. Contact
  7. Enclosed documents

# 1 What is bparser?
It is a library for parsing ANSII C99 code. It currently supports C99 almost completely
(with some minor lookup problems in rare cases) + struct attributes. C11 is not yet
completely supported.

For generating the parser, it uses bison and flex. The corresponding rules
were taken from [Jutta Degener, quut.com](http://www.quut.com/c/ANSI-C-grammar-y.html)
and have been filled with code.

# 2 Why using bparser?

  * bparser has no library dependencies (bison and flex are used for code
    generating only)
  * bparser is a very fast parser (due to being an LALR parser)
  * bparser is minimal: a very small library with no more features than you need
  * bparser is easy to use and extend. It uses visitor- and similar concepts.

# 3 Getting started

C-Code is basically parsed like this:

  * The flexer reads tokens (using GNU flex, see files [lexer.l](src/lexer.l) and
    [lexer_common.h](src/lexer_common.h)), and the parser (see
    [parser.y](src/parser.y)) builts an incomplete AST (abstract syntax tree) from
    them. These files can be hard to read.
  * The AST is yet incomplete. visitors are run on its leafs in order to
    - assign parent pointers for all leafs
    - pass the token geometry (source file, line and column) to all leaves
    - complete type info for the AST nodes ([type_completor.h](src/type_completor.h)).
      E.g. for the occurence of a variable, you might want to know where it has been
      defined and what type it is.

All this is being done with the routine from [ast.h](src/ast.h).

For then using the AST, you might want to implement your own [visitor](src/visitor.h).
A few useful visitors already exist in [src/utils.h](src/utils.h). For more details
about the AST nodes themselves, see [node.h](src/node.h). This file also explains how C
grammar works.

# 4 Documentation
Type `make doc` for doxygen. TODO: more stuff here...

# 5 Credits
I thank Jutta Degener et al. for providing the useful
[ANSI C Yacc grammar](http://www.quut.com/c/ANSI-C-grammar-y.html).

# 6 Contact

Feel free to give feedback. The author's e-mail address is shown if you
execute this in a shell:
```sh
printf "\x6a\x6f\x68\x61\x6e\x6e\x65\x73\x40\x6c\x6f\
\x72\x65\x6e\x7a\x2d\x68\x6f\x2e\x6d\x65\x0a"
```

# 7 Enclosed documents
 * [FAQ](FAQ.md)
 * [installation](INSTALL.md)
 * [license](LICENSE.txt)

