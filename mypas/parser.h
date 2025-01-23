// parser.h
#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "symtab.h"
#include "keywords.h"

void mypas(void);      // Função principal do parser
void block(void);      // Analisa um bloco de código (declaracoes e comandos)
void vardef(void);     // Analisa definições de variáveis
void sbprgdef(void);   // Analisa definições de procedimentos e funções
void parmlist(void);   // Analisa listas de parâmetros
void idlist(void);     // Analisa listas de identificadores
void beginend(void);   // Analisa blocos BEGIN ... END
void stmtlist(void);   // Analisa listas de comandos
void stmt(void);       // Analisa um único comando
void idstmt(void);     // Analisa comandos que começam com um identificador
void exprlist(void);   // Analisa listas de expressões
void ifstmt(void);     // Analisa comandos IF
void whlstmt(void);    // Analisa comandos WHILE
void repstmt(void);    // Analisa comandos REPEAT ... UNTIL
void expr(void);       // Analisa expressões completas
void smpexpr(void);    // Analisa expressões simples (sem operadores relacionais)
void term(void);       // Analisa termos em expressões (com multiplicação/divisão)
void factor(void);     // Analisa fatores em termos (identificadores, números, subexpressões)
void match(int token); // Verifica e consome o token esperado
void type(void);       // Analisa tipos de dados (INTEGER, REAL, etc.)

#endif // PARSER_H
