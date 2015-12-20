# Greetings
Welcome! Thank you for considering the README file.

**Contents**
  1. What is bparser?
  2. Why using bparser?
  3. Documentation
  4. Credits
  5. Contact
  6. Enclosed documents

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

# 3 Documentation
Type `make doc` for doxygen. TODO: more stuff here...

# 4 Credits
I thank Jutta Degener et al. for providing the useful
[ANSI C Yacc grammar](http://www.quut.com/c/ANSI-C-grammar-y.html).

# 5 Contact

Feel free to give feedback. The author's e-mail address is shown if you
execute this in a shell:
```sh
printf "\x6a\x6f\x68\x61\x6e\x6e\x65\x73\x40\x6c\x6f\
\x72\x65\x6e\x7a\x2d\x68\x6f\x2e\x6d\x65\x0a"
```

# 6 Enclosed documents
 * [FAQ](FAQ.md)
 * [installation](INSTALL.md)
 * [license](LICENSE.txt)

