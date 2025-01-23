// parser.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "keywords.h"
#include "symtab.h"

// Define o número máximo de identificadores que podem ser armazenados
#define MAX_IDS 32

// Declaração das variáveis globais utilizadas pelo parser
extern int lookahead; // Token atual
extern FILE *src;     // Ponteiro para o arquivo fonte

// Arrays para armazenar nomes de identificadores e contagem
char idlist_names[MAX_IDS][MAXIDLEN]; // Lista de identificadores
int idlist_count = 0;                 // Contador de identificadores

/**
 * Função principal do parser que inicia a análise sintática.
 */
void mypas(void)
{
    match(PROGRAM);                   // Verifica o token PROGRAM
    match(ID);                        // Verifica um identificador
    match('(');                       // Verifica o '('
    idlist();                         // Processa a lista de identificadores
    match(')');                       // Verifica o ')'
    match(';');                       // Verifica o ';'
    block();                          // Processa o bloco de código
    match('.');                       // Verifica o '.' final
    symtab_release(current_lexlevel); // Libera símbolos do nível atual

    if (lookahead != EOF)
    {
        // Garante que após o '.' não haja mais tokens
        fprintf(stderr, "Erro de sintaxe: caracteres inesperados após o final do programa.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Processa um bloco que pode conter declarações e comandos.
 */
void block(void)
{
    if (lookahead == VAR)
    {
        vardef(); // Processa definições de variáveis
    }
    sbprgdef(); // Processa definições de procedimentos e funções
    beginend(); // Processa o bloco BEGIN ... END
}

/**
 * Processa definições de variáveis.
 */
void vardef(void)
{
    if (lookahead == VAR)
    {
        match(VAR); // Verifica o token VAR
        do
        {
            idlist();                 // Processa a lista de identificadores
            match(':');               // Verifica o ':'
            int var_type = lookahead; // Armazena o tipo da variável
            type();                   // Valida o tipo
            match(';');               // Verifica o ';'

            // Adiciona cada variável à tabela de símbolos
            for (int i = 0; i < idlist_count; i++)
            {
                if (symtab_append(idlist_names[i], VARIABLE, var_type, current_lexlevel, 0) < 0)
                {
                    fprintf(stderr, "Erro semântico: variável '%s' já definida.\n", idlist_names[i]);
                    exit(EXIT_FAILURE);
                }
            }
        } while (lookahead == ID); // Continua enquanto houver IDs
    }
}

/**
 * Processa definições de procedimentos e funções.
 */
void sbprgdef(void)
{
    while (lookahead == PROCEDURE || lookahead == FUNCTION)
    {
        // Determina se é procedimento ou função
        int objtype = (lookahead == PROCEDURE) ? PROCEDURE_OBJ : FUNCTION_OBJ;
        match(lookahead); // Verifica PROCEDURE ou FUNCTION
        char proc_func_name[MAXIDLEN];
        strcpy(proc_func_name, lexeme); // Armazena o nome
        match(ID);                      // Verifica o ID
        parmlist();                     // Processa a lista de parâmetros
        int return_type = 0;
        if (lookahead == ':')
        {
            match(':');              // Verifica o ':'
            return_type = lookahead; // Armazena o tipo de retorno
            type();                  // Valida o tipo
        }
        // Adiciona procedimento ou função à tabela de símbolos
        if (symtab_append(proc_func_name, objtype, return_type, current_lexlevel, 0) < 0)
        {
            fprintf(stderr, "Erro semântico: %s '%s' já definida.\n",
                    (objtype == PROCEDURE_OBJ ? "Procedimento" : "Função"),
                    proc_func_name);
            exit(EXIT_FAILURE);
        }
        match(';');                       // Verifica o ';'
        current_lexlevel++;               // Incrementa o nível léxico
        block();                          // Processa o bloco interno
        match(';');                       // Verifica o ';'
        symtab_release(current_lexlevel); // Libera símbolos do nível atual
        current_lexlevel--;               // Decrementa o nível léxico
    }
}

/**
 * Processa a lista de parâmetros em procedimentos ou funções.
 */
void parmlist(void)
{
    if (lookahead == '(')
    {
        match('('); // Verifica '('
        do
        {
            int is_var_param = 0;
            if (lookahead == VAR)
            {
                match(VAR); // Verifica VAR para parâmetros por referência
                is_var_param = 1;
            }
            idlist();                  // Processa a lista de identificadores
            match(':');                // Verifica ':'
            int parm_type = lookahead; // Armazena o tipo do parâmetro
            type();                    // Valida o tipo
            // Adiciona parâmetros à tabela de símbolos
            for (int i = 0; i < idlist_count; i++)
            {
                if (symtab_append(idlist_names[i], VARIABLE, parm_type, current_lexlevel, is_var_param) < 0)
                {
                    fprintf(stderr, "Erro semântico: parâmetro '%s' já definido.\n", idlist_names[i]);
                    exit(EXIT_FAILURE);
                }
            }
            if (lookahead == ';')
            {
                match(';'); // Verifica ';' entre parâmetros
            }
            else
            {
                break; // Finaliza a lista se não houver ';'
            }
        } while (1);
        match(')'); // Verifica ')'
    }
}

/**
 * Processa uma lista de identificadores separados por vírgulas.
 */
void idlist(void)
{
    idlist_count = 0; // Reinicia o contador de identificadores
    do
    {
        strcpy(idlist_names[idlist_count++], lexeme); // Armazena o identificador atual
        match(ID);                                    // Verifica o ID
        if (lookahead == ',')
        {
            match(','); // Verifica a vírgula
        }
        else
        {
            break; // Finaliza a lista se não houver vírgula
        }
    } while (1);
}

/**
 * Processa um bloco delimitado por BEGIN e END.
 */
void beginend(void)
{
    match(BEGIN); // Verifica BEGIN_TOKEN
    stmtlist();   // Processa a lista de comandos
    match(END);   // Verifica END_TOKEN
}

/**
 * Processa uma lista de comandos separados por ponto e vírgula.
 */
void stmtlist(void)
{
    do
    {
        stmt(); // Processa um comando
        if (lookahead == ';')
        {
            match(';'); // Verifica ';' entre comandos
        }
        else
        {
            break; // Finaliza a lista se não houver ';'
        }
    } while (1);
}

/**
 * Processa um único comando baseado no token atual.
 */
void stmt(void)
{
    switch (lookahead)
    {
    case ID:
        idstmt(); // Processa comando que começa com ID
        break;
    case IF:
        ifstmt(); // Processa comando IF
        break;
    case WHILE:
        whlstmt(); // Processa comando WHILE
        break;
    case REPEAT:
        repstmt(); // Processa comando REPEAT
        break;
    case BEGIN:
        beginend(); // Processa bloco BEGIN ... END
        break;
    default:
        // Erro caso o comando não seja reconhecido
        fprintf(stderr, "Erro de sintaxe: comando inesperado na linha %d\n", linenum);
        exit(EXIT_FAILURE);
    }
}

/**
 * Processa um comando IF ... THEN ... [ELSE ...].
 */
void ifstmt(void)
{
    match(IF);   // Verifica IF
    expr();      // Processa a expressão condicional
    match(THEN); // Verifica THEN
    stmt();      // Processa o comando no THEN
    if (lookahead == ELSE)
    {
        match(ELSE); // Verifica ELSE
        stmt();      // Processa o comando no ELSE
    }
}

/**
 * Processa um comando REPEAT ... UNTIL ....
 */
void repstmt(void)
{
    match(REPEAT); // Verifica REPEAT
    stmtlist();    // Processa a lista de comandos a serem repetidos
    match(UNTIL);  // Verifica UNTIL
    expr();        // Processa a condição de parada
}

/**
 * Processa um comando WHILE ... DO ....
 */
void whlstmt(void)
{
    match(WHILE); // Verifica WHILE
    expr();       // Processa a condição do loop
    match(DO);    // Verifica DO
    stmt();       // Processa o comando dentro do loop
}

/**
 * Processa comandos que começam com um identificador (atribuições ou chamadas).
 */
void idstmt(void)
{
    if (lookahead == ID)
    {
        int sym_index = symtab_lookup(lexeme, current_lexlevel); // Busca o símbolo na tabela
        if (sym_index < 0)
        {
            // Erro se o símbolo não estiver definido
            fprintf(stderr, "Erro semântico: variável ou função '%s' não definida.\n", lexeme);
            exit(EXIT_FAILURE);
        }
        int objtype = symtab[sym_index].objtype; // Obtém o tipo do objeto
        match(ID);                               // Verifica o ID
        if (lookahead == ASGN)
        {
            // Verifica se é uma variável ou função que pode receber atribuição
            if (objtype != VARIABLE && objtype != FUNCTION_OBJ)
            {
                fprintf(stderr, "Erro semântico: não é possível atribuir a '%s'.\n", lexeme);
                exit(EXIT_FAILURE);
            }
            match(ASGN); // Verifica ':='
            expr();      // Processa a expressão de atribuição
        }
        else
        {
            exprlist(); // Processa uma lista de expressões (possível chamada de procedimento)
        }
    }
}

/**
 * Processa uma lista de expressões separadas por vírgulas.
 */
void exprlist(void)
{
    if (lookahead == '(')
    {
        match('('); // Verifica '('
        do
        {
            expr(); // Processa uma expressão
            if (lookahead == ',')
            {
                match(','); // Verifica vírgula entre expressões
            }
            else
            {
                break; // Finaliza a lista se não houver vírgula
            }
        } while (1);
        match(')'); // Verifica ')'
    }
}

/**
 * Processa um tipo de dado (INTEGER, REAL, etc.).
 */
void type(void)
{
    switch (lookahead)
    {
    case INTEGER:
        match(INTEGER); // Verifica INTEGER
        break;
    case REAL:
        match(REAL); // Verifica REAL
        break;
    case DOUBLE:
        match(DOUBLE); // Verifica DOUBLE
        break;
    case BOOLEAN:
        match(BOOLEAN); // Verifica BOOLEAN
        break;
    default:
        // Erro caso o tipo não seja válido
        fprintf(stderr, "Erro: tipo inválido na linha %d.\n", linenum);
        exit(-1);
    }
}

/**
 * Processa uma expressão completa, possivelmente com operadores relacionais.
 */
void expr(void)
{
    smpexpr(); // Processa uma expressão simples

    // Verifica se há um operador relacional após a expressão simples
    if (lookahead == '<' || lookahead == '>' || lookahead == RELOP_LE ||
        lookahead == RELOP_GE || lookahead == RELOP_NE)
    {
        match(lookahead); // Consome o operador relacional
        smpexpr();        // Processa a próxima expressão simples
    }
}

/**
 * Processa uma expressão simples com operadores de soma e subtração.
 */
void smpexpr(void)
{
    term(); // Processa o primeiro termo

    // Continua processando enquanto houver operadores '+' ou '-'
    while (lookahead == '+' || lookahead == '-')
    {
        match(lookahead); // Consome o operador
        term();           // Processa o próximo termo
    }
}

/**
 * Processa um termo com operadores de multiplicação e divisão.
 */
void term(void)
{
    factor(); // Processa o primeiro fator

    // Continua processando enquanto houver operadores '*', '/' ou 'mod', 'div'
    while (lookahead == '*' || lookahead == '/' || lookahead == MOD || lookahead == DIV)
    {
        match(lookahead); // Consome o operador
        factor();         // Processa o próximo fator
    }
}

/**
 * Processa um fator que pode ser um identificador, número ou expressão entre parênteses.
 */
void factor(void)
{

    switch (lookahead)
    {
    case ID:
    {
        // Verifica se o identificador está definido na tabela de símbolos
        int sym_index = symtab_lookup(lexeme, current_lexlevel);
        if (sym_index < 0)
        {
            fprintf(stderr, "Erro semântico: símbolo '%s' não definido.\n", lexeme);
            exit(EXIT_FAILURE);
        }
        int objtype = symtab[sym_index].objtype; // Obtém o tipo do objeto
        match(ID);                               // Verifica o ID

        if (lookahead == '(')
        {
            // Processa parâmetros de função ou procedimento
            match('(');
            if (lookahead != ')')
            {
                expr();
                while (lookahead == ',')
                {
                    match(',');
                    expr();
                }
            }
            match(')'); // Verifica ')'
        }
        else if (objtype == FUNCTION_OBJ)
        {
            // Funções sem parâmetros não precisam de parênteses
            // Nenhuma ação necessária
        }
        break;
    }
    case DEC:
    case OCT:
    case HEX:
        match(lookahead); // Processa números (decimal, octal, hexadecimal)
        break;
    case '(':
        match('('); // Verifica '('
        expr();     // Processa a expressão dentro dos parênteses
        match(')'); // Verifica ')'
        break;
    default:
        // Erro caso o fator não seja válido
        fprintf(stderr, "Erro de sintaxe: fator inválido na linha %d, token: %d\n", linenum, lookahead);
        exit(EXIT_FAILURE);
    }
}

/**
 * Verifica se o token atual corresponde ao esperado e avança para o próximo token.
 *
 * token Token esperado.
 */
void match(int token)
{

    if (lookahead == token)
    {
        lookahead = gettoken(src); // Avança para o próximo token
    }
    else
    {
        // Caso especial para o ponto final do programa
        if (lookahead == '.' && token == '.')
        {
            return; // Sucesso, encontrado o final do programa
        }

        // Erro caso o token não corresponda ao esperado
        fprintf(stderr, "Erro de sintaxe: esperado %d, mas encontrado %d na linha %d\n", token, lookahead, linenum);
        exit(EXIT_FAILURE);
    }
}
