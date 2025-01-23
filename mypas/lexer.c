#include <ctype.h>  // Inclui funções para classificação de caracteres, como isalpha e isdigit
#include <stdio.h>  // Inclui funções padrão de I/O, como getc e ungetc
#include "lexer.h"  // Inclui o cabeçalho do lexer com definições e declarações necessárias
#include <string.h> // Inclui funções de manipulação de strings, como strcmp e strcpy

// Array para armazenar o lexema atual com tamanho máximo definido
char lexeme[MAXIDLEN + 1];

// Função para verificar se o próximo caractere forma um operador de atribuição ":="
int isASGN(FILE *tape)
{
    if ((lexeme[0] = getc(tape)) == ':') {
        if ((lexeme[1] = getc(tape)) == '=') {
            lexeme[2] = 0; // Termina a string do lexema
            return ASGN; // Retorna o token de atribuição
        }
        ungetc(lexeme[1], tape); // Coloca de volta o último caractere lido
    }
    ungetc(lexeme[0], tape); // Coloca de volta o caractere lido anteriormente
    return 0; // Não reconhecido
}

// Função para verificar e processar identificadores (IDs)
int isID(FILE *tape)
{
    lexeme[0] = getc(tape); // Obtém o primeiro caractere

    if (isalpha(lexeme[0])) // Verifica se é uma letra
    {
        int i = 1;
        // Continua lendo caracteres alfanuméricos
        while (isalnum(lexeme[i] = getc(tape)))
            i++;

        ungetc(lexeme[i], tape); // Retorna o caractere não alfanumérico ao stream
        lexeme[i] = '\0';        // Termina a string

        return ID; // Retorna o token ID
    }

    ungetc(lexeme[0], tape); // Retorna o caractere ao stream se não for letra
    lexeme[0] = '\0';        // Limpa o lexeme

    return 0; // Retorna 0 indicando que não é um ID
}

// Função para verificar e processar números decimais (DEC)
int isDEC(FILE *tape)
{
    int head = getc(tape); // Obtém o primeiro caractere

    if (isdigit(head)) // Verifica se é um dígito
    {
        if (head == '0')
        {
            return DEC; // Retorna DEC para número começando com 0
        }
        // Continua lendo dígitos
        while (isdigit(head = getc(tape)))
            ;
        ungetc(head, tape); // Retorna o caractere não dígito ao stream
        return DEC;         // Retorna DEC para número válido
    }
    ungetc(head, tape); // Retorna o caractere ao stream se não for dígito
    return 0;           // Retorna 0 indicando que não é um DEC
}

// Variável global para rastrear o número da linha atual
int linenum = 1;

// Função para ignorar espaços em branco e comentários
void skipspaces(FILE *tape)
{
    int head;

    while ((head = getc(tape)) != EOF)
    {
        if (isspace(head)) // Verifica se é um espaço em branco
        {
            if (head == '\n')
                linenum++; // Incrementa o contador de linhas em nova linha
        }
        else if (head == '{') // Início de comentário do tipo { }
        {
            skipcomments(tape, '}'); // Ignora até '}'
        }
        else if (head == '(') // Possível início de comentário (* *)
        {
            int next_char = getc(tape);
            if (next_char == '*')
            {
                skipcomments(tape, '*'); // Ignora até '*)'
            }
            else
            {
                ungetc(next_char, tape); // Retorna o caractere se não for '*'
                ungetc(head, tape);      // Retorna '(' ao stream
                break;                   // Sai do loop
            }
        }
        else
        {
            ungetc(head, tape); // Retorna o caractere não tratado ao stream
            break;              // Sai do loop
        }
    }
}

// Função para ignorar comentários com base no caractere de término
void skipcomments(FILE *tape, char end_char)
{
    int head;

    if (end_char == '}')
    {
        // Comentário do tipo { }
        while ((head = getc(tape)) != EOF)
        {
            if (head == '}')
                break; // Fim do comentário
            if (head == '\n')
                linenum++; // Incrementa o contador de linhas
        }
    }
    else if (end_char == '*')
    {
        // Comentário do tipo (* *)
        int prev_char = 0;
        while ((head = getc(tape)) != EOF)
        {
            if (head == '\n')
                linenum++; // Incrementa o contador de linhas
            if (prev_char == '*' && head == ')')
                break;        // Fim do comentário
            prev_char = head; // Atualiza o caractere anterior
        }
    }
}

// Função principal do lexer para obter o próximo token do arquivo fonte
int gettoken(FILE *source)
{
    skipspaces(source);    // Ignora espaços em branco e comentários
    int c = fgetc(source); // Obtém o próximo caractere

    if (c == EOF)
    {
        return -1; // Retorna -1 para EOF
    }

    if (isalpha(c)) // Verifica se começa com letra (possível ID ou palavra-chave)
    {
        int i = 0;
        lexeme[i++] = c; // Armazena o primeiro caractere no lexeme
        // Continua lendo caracteres alfanuméricos até o limite
        while (isalnum(c = getc(source)) && i < MAXIDLEN)
        {
            lexeme[i++] = c;
        }
        lexeme[i] = '\0'; // Termina a string

        if (c != EOF)
            ungetc(c, source); // Retorna o caractere não alfanumérico ao stream

        // Verifica se o lexeme corresponde a uma palavra-chave
        if (strcmp(lexeme, "program") == 0)
            return PROGRAM;
        if (strcmp(lexeme, "var") == 0)
            return VAR;
        if (strcmp(lexeme, "procedure") == 0)
            return PROCEDURE;
        if (strcmp(lexeme, "function") == 0)
            return FUNCTION;
        if (strcmp(lexeme, "integer") == 0)
            return INTEGER;
        if (strcmp(lexeme, "real") == 0)
            return REAL;
        if (strcmp(lexeme, "double") == 0)
            return DOUBLE;
        if (strcmp(lexeme, "boolean") == 0)
            return BOOLEAN;
        if (strcmp(lexeme, "if") == 0)
            return IF;
        if (strcmp(lexeme, "then") == 0)
            return THEN;
        if (strcmp(lexeme, "else") == 0)
            return ELSE;
        if (strcmp(lexeme, "begin") == 0)
            return BEGIN;
        if (strcmp(lexeme, "end") == 0)
            return END;
        if (strcmp(lexeme, "while") == 0)
            return WHILE;
        if (strcmp(lexeme, "do") == 0)
            return DO;
        if (strcmp(lexeme, "repeat") == 0)
            return REPEAT; // Reconhece 'repeat'
        if (strcmp(lexeme, "until") == 0)
            return UNTIL; // Reconhece 'until'

        return ID; // Retorna ID se não for palavra-chave
    }
    else if (isdigit(c)) // Verifica se começa com dígito (número)
    {
        ungetc(c, source); // Retorna o dígito ao stream
        isNumber(source);  // Processa o número
        return DEC;        // Retorna DEC para número válido
    }
    else
    {
        // Processa outros símbolos
        switch (c)
        {
        case '<':
            if ((c = fgetc(source)) == '=')
                return RELOP_LE; // Retorna token de operador <=
            if (c == '>')
                return RELOP_NE; // Retorna token de operador <>
            ungetc(c, source);   // Retorna o caractere se não for '=' ou '>'
            return '<';          // Retorna '<' como token
        case '>':
            if ((c = fgetc(source)) == '=')
                return RELOP_GE; // Retorna token de operador >=
            ungetc(c, source);   // Retorna o caractere se não for '='
            return '>';          // Retorna '>' como token
        case ':':
            if ((c = fgetc(source)) == '=')
                return ASGN;   // Retorna token de atribuição ':='
            ungetc(c, source); // Retorna o caractere se não for '='
            return ':';        // Retorna ':' como token
        case '.':
            return '.'; // Retorna '.' como token
        default:
            return c; // Retorna o caractere como token padrão
        }
    }
}

// Função para processar números em diferentes bases (decimal, octal, hexadecimal)
void isNumber(FILE *source)
{
    int c = getc(source); // Obtém o primeiro caractere do número
    int num_base = 10;    // Assume base decimal por padrão

    if (c == '0') // Verifica se o número começa com '0' (possível octal ou hexadecimal)
    {
        c = getc(source);         // Obtém o próximo caractere
        if (c == 'x' || c == 'X') // Verifica se é hexadecimal
        {
            num_base = 16;
            c = getc(source); // Consome o 'x' ou 'X'
        }
        else // Caso contrário, assume octal
        {
            num_base = 8;
            ungetc(c, source); // Retorna o caractere ao stream
        }
    }

    // Lê os caracteres numéricos de acordo com a base
    while (isdigit(c) || (num_base == 16 && isxdigit(c)))
    {
        c = getc(source); // Consome o próximo caractere
    }

    ungetc(c, source); // Retorna o caractere não numérico ao stream
}
