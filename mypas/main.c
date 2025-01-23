// main.c
#include "parser.h" // Inclui o cabeçalho do parser
#include <stdio.h>	// Inclui a biblioteca padrão de I/O
#include <stdlib.h> // Inclui a biblioteca padrão

// Definições de variáveis globais
int lookahead; // Token atual sendo analisado
FILE *src;	   // Ponteiro para o arquivo fonte

int main(int argc, char *argv[]) // Função principal do programa
{
	// Verifica se o arquivo fonte foi fornecido como argumento
	if (argc < 2)
	{
		fprintf(stderr, "Uso: %s <arquivo-fonte>\n", argv[0]);
		return 1; // Encerra o programa com código de erro
	}

	// Tenta abrir o arquivo fonte para leitura
	src = fopen(argv[1], "r");
	if (!src)
	{
		perror("Erro ao abrir o arquivo-fonte"); // Exibe mensagem de erro se a abertura falhar
		return 1;								 // Encerra o programa com código de erro
	}

	// Inicializa o lexer obtendo o primeiro token do arquivo fonte
	lookahead = gettoken(src);

	// Inicia o processo de parsing
	mypas();

	// Fecha o arquivo fonte após o parsing
	fclose(src);
	printf("Análise bem-sucedida.\n"); // Mensagem de sucesso

	return 0; // Encerra o programa com sucesso
}
