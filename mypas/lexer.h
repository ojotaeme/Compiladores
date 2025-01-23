// lexer.h
#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "keywords.h"

#define MAXIDLEN 32

enum
{
    ID = 1024,
    DEC,
    OCT,
    HEX,
    ASGN,     // ":="
    RELOP_LE, // "<="
    RELOP_GE, // ">="
    RELOP_NE, // "<>"
    MOD,      // "mod"
    DIV,      // "div"
};

extern int gettoken(FILE *);
extern int linenum;
extern char lexeme[];
extern void isNumber(FILE *);
void skipspaces(FILE *tape);
void skipcomments(FILE *tape, char end_char);

#endif
