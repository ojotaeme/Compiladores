// symtab.h
#ifndef SYMTAB_H
#define SYMTAB_H
#define VARIABLE 1
#define PROCEDURE_OBJ 2
#define FUNCTION_OBJ 3

#include "lexer.h"
#include "keywords.h"

typedef struct _symtab_
{
    char name[MAXIDLEN];
    int objtype;
    int type;
    int lexlevel;
    int parmflag;
} SYMTAB;

extern SYMTAB symtab[];
extern int current_lexlevel;

int symtab_append(const char *name, int objtype, int type, int lexlevel, int parmflag);
int symtab_lookup(const char *query, int lexlevel);
void symtab_release(int lexlevel);

#endif
