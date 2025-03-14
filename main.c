#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Uso: %s <nome_arquivo_entrada>.txt <nome_arquivo_saida>.txt\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    int tamanho = strlen(argv[1]);

    // Verifica se a extensão do arquivo é .txt
    if (tamanho < 4 || strcmp(argv[1] + tamanho - 4, ".txt") != 0)
    {
        printf("O arquivo não possui a extensão .txt\n");
        return 1;
    }

    //Tratamento das entrada/saída
    FILE* entrada = fopen(argv[1], "r");
    if (!entrada)
    {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(1);
    }
    FILE* saida = fopen(argv[2], "w");
    if (!saida)
    {
        printf("Erro ao abrir o arquivo de saída.\n");
        exit(1);
    }

    //Lendo o arquivo de entrada
    Grafo* grafo = processarArquivo(entrada);

    //Executando Dijkstra
    dijkstra(grafo, saida);

    destroiGrafo(grafo);
    fclose(entrada);
    fclose(saida);
    return 0;
}
