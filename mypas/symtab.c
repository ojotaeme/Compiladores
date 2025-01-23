#include <string.h>
#include <symtab.h>
#include "keywords.h"

#define MAXSYMTABSIZE 16384 // Define o tamanho máximo da tabela de símbolos

// Declaração da tabela de símbolos como um array de estruturas _symtab_
// Cada entrada na tabela representa um símbolo (variável, função, procedimento, etc.)
struct _symtab_ symtab[MAXSYMTABSIZE];

// Variável que indica a próxima entrada disponível na tabela de símbolos
int symtab_next_entry = 1;

// Variável que rastreia o nível léxico atual (escopo atual)
int current_lexlevel = 0;

/**
 * Esta função percorre a tabela de símbolos para encontrar um símbolo que corresponda
 * ao nome fornecido (`query`) e que esteja no nível léxico especificado (`lexlevel`).
 * A busca começa no nível léxico fornecido e vai descendo até o nível global (0).
 */
int symtab_lookup(const char *query, int lexlevel)
{
    // Percorre os níveis léxicos do atual até o nível global
    for (int level = lexlevel; level >= 0; level--)
    {
        // Percorre todas as entradas da tabela de símbolos
        for (int i = 0; i < MAXSYMTABSIZE; i++)
        {
            // Verifica se a entrada está ocupada (nome não vazio),
            // se o nome corresponde ao `query` e se está no nível léxico atual
            if (symtab[i].name[0] != '\0' && strcmp(symtab[i].name, query) == 0 && symtab[i].lexlevel == level)
            {
                return i; // Retorna o índice do símbolo encontrado
            }
        }
    }
    return -1; // Retorna -1 se o símbolo não for encontrado em nenhum nível léxico
}

/**
 * Esta função tenta adicionar um símbolo com o nome fornecido (`name`), tipo de objeto (`objtype`),
 * tipo de dado (`type`), nível léxico (`lexlevel`) e flag de parâmetro (`parmflag`).
 * Antes de adicionar, verifica se o símbolo já existe no nível léxico especificado.
 */
int symtab_append(const char *name, int objtype, int type, int lexlevel, int parmflag)
{
    // Verifica se o símbolo já existe no nível léxico especificado
    if (symtab_lookup(name, lexlevel) >= 0)
    {
        // Imprime mensagem de erro se o símbolo já foi definido no escopo
        fprintf(stderr, "Erro semântico: o símbolo '%s' já foi definido no nível léxico %d.\n", name, lexlevel);
        return -2; // Retorna -2 indicando que o símbolo já existe no escopo
    }

    // Percorre a tabela de símbolos para encontrar uma entrada livre
    for (int i = 0; i < MAXSYMTABSIZE; i++)
    {
        // Verifica se a entrada está livre (nome vazio)
        if (symtab[i].name[0] == '\0')
        {
            // Copia o nome do símbolo para a tabela, garantindo a terminação da string
            strncpy(symtab[i].name, name, sizeof(symtab[i].name) - 1);
            symtab[i].name[sizeof(symtab[i].name) - 1] = '\0'; // Garante a terminação com '\0'

            // Preenche os demais campos da estrutura com as informações fornecidas
            symtab[i].objtype = objtype;
            symtab[i].type = type;
            symtab[i].lexlevel = lexlevel; // Registra o nível léxico do símbolo
            symtab[i].parmflag = parmflag; // Indica se é um parâmetro

            return 0; // Retorna 0 indicando sucesso na adição
        }
    }

    // Se não encontrou uma posição livre, a tabela está cheia
    fprintf(stderr, "Erro: tabela de símbolos cheia.\n");
    return -1; // Retorna -1 indicando que a tabela está cheia
}

/**
 * Esta função é chamada quando se sai de um escopo (nível léxico) para remover símbolos
 * que não são mais válidos no escopo atual.
 */
void symtab_release(int lexlevel)
{
    // Percorre todas as entradas da tabela de símbolos
    for (int i = 0; i < MAXSYMTABSIZE; i++)
    {
        // Verifica se a entrada está no nível léxico a ser liberado
        if (symtab[i].lexlevel == lexlevel)
        {
            // Marca a entrada como livre definindo o primeiro caractere do nome como '\0'
            symtab[i].name[0] = '\0';
            // Opcionalmente, você pode limpar os demais campos para evitar resíduos
            symtab[i].objtype = 0;
            symtab[i].type = 0;
            symtab[i].lexlevel = 0;
            symtab[i].parmflag = 0;
        }
    }
}
