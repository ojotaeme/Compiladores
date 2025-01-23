#ifndef KEYWORDS_H
#define KEYWORDS_H

enum
{
    BEGIN = 2048,
    END,
    INTEGER,
    REAL,
    DOUBLE,
    BOOLEAN,
    CHARACTER,
    STRING,
    PROGRAM,
    PROCEDURE,
    FUNCTION,
    VAR,
    IF,
    THEN,
    ELSE,
    REPEAT,
    UNTIL,
    WHILE,
    DO
};

extern char *keywords[];

int iskeyword(char *identifier);

#endif
