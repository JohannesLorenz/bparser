# FAQ

Questions that might be frequently asked:
```
Q:  The Makefile does not work right?
A:  Always do a `make clean' after you have made any changes in any headers.
Q:  How save is the C parser?
A:  The author has written them with a lot of care and has double-checked every aspect.
    Additionally, there is a test suite which covers all possible branches of the C parser
    as well as most relevant aspects like definition.
Q:  Is the precompiler also included?
A:  No, you have to precompile your code with a suited compiler. However, a few aspects
    like comments or some special macros might be supported.
Q:  How complete is the C99 support?
A:  The parser is complete, the lookup still misses minor issues like designators,
    goto-labels, forward declarations.
Q:  What more than C99 is currently supported?
A:  Only attributes, and those only partially. It is currently suggested to erase them
    before passing the code.
Q:  How far is the C11 support?
A:  It is prepared, but almost completely disabled.
```
