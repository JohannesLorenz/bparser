######################################################################
# Automatically generated by qmake (2.01a) Sa. Okt 17 05:48:35 2015
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . src

# Input
HEADERS += src/lexer.h src/node.h src/visitor.h src/tuple03.h \
    src/token.h src/ast.h \
    src/lexer_common.h \
    src/type_completor.h \
    src/function.h \
    src/utils.h
LEXSOURCES += src/lexer.l
YACCSOURCES += src/parser.y
SOURCES += src/lexer.c test/test.cpp src/node.cpp src/visitor.cpp \
    src/token.cpp src/ast.cpp \
    test/input/statements.c \
    test/input/abstract_declarators.c \
    test/input/direct_declarators.c \
    test/input/declaration_specifiers.c \
    test/input/initializers.c \
    test/input/structs.c \
    test/input/enums.c \
    test/input/expressions.c \
    test/input/constants.c \
    test/input/scopes.c \
    test/input/clash.c \
    test/input/clash_2.c \
    test/input/nested.c \
    src/type_completor.cpp \
    src/function.cpp \
    test/input/struct_fwd.c \
    test/input/struct_typedefs.c \
    src/utils.cpp
OTHER_FILES += Makefile src/Makefile test/Makefile

