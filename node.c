#include "node.h"
#include <stdlib.h>
#include <stdio.h>

struct tNode
{
    int numero;
    double* distancias;
    int capacidade;
    int qtd;
    struct Node* predecessor; // Adicionar campo predecessor
};

Node* criaNo(int numero)
{
    Node* novo = calloc (1, sizeof(Node));
    novo -> numero = numero;
    novo -> distancias = NULL;
    novo -> capacidade = 0;
    novo -> qtd = 0;
    return novo;
}

void insereDistancias(Node* no, int* posicao, double distancia)
{
    if (*posicao < 0)
        return;

    // Caso especial quando a posição é igual ao número do nó
    if (*posicao == no->numero)
    {
        if (no->distancias == NULL)
        {
            // Aloca espaço suficiente para *posicao + 1 elementos
            int nova_cap = *posicao + 1;
            no->distancias = (double*)calloc(nova_cap, sizeof(double));
            if (no->distancias == NULL) return;
            no->capacidade = nova_cap;
        }
        // Verifica se precisa expandir antes de acessar
        if (*posicao >= no->capacidade)
        {
            int nova_cap = *posicao + 200;
            double* temp = (double*)realloc(no->distancias, nova_cap * sizeof(double));
            if (!temp) return;
            no->distancias = temp;
            for (int i = no->capacidade; i < nova_cap; i++)
            {
                no->distancias[i] = -2.0;
            }
            no->capacidade = nova_cap;
        }
        no->distancias[*posicao] = 0.0;
        (*posicao)++; // Incrementa após a inserção
    }

    // Verifica se a posição atual excede a capacidade
    if (*posicao >= no->capacidade)
    {
        int nova_cap = *posicao + 200;
        double* temp = (double*)realloc(no->distancias, nova_cap * sizeof(double));
        if (!temp) return;
        no->distancias = temp;
        for (int i = no->capacidade; i < nova_cap; i++)
        {
            no->distancias[i] = INFINITY; // Usar INFINITY ao invés de -2.0
        }
        no->capacidade = nova_cap;
    }

    // Insere a distância na posição atual
    no->distancias[*posicao] = distancia;
    no->qtd++;
}

int retornaNumero(Node* no)
{
    return no -> numero;
}

void imprimeNo(Node* no)
{
    printf("node_%d\n", no -> numero);
    for (int i = 0; i < no -> qtd; i++)
        printf("%0.2lf, ", no -> distancias[i]);
    printf("\n");
}

void destroiNo(Node* no)
{
    free(no -> distancias);
    free(no);
}

void arrumaUltimoNo(Node* no)
{
    if (no -> distancias[no -> numero] == -2)
    {
        no -> distancias[no -> numero] = 0;
        no -> qtd++;
    }
}

float retornaDistancia(Node* no, int posicao)
{
    return no->distancias[posicao];
}