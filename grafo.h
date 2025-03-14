#ifndef GRAFO_H
#define GRAFO_H

#include "node.h"
#include <stdio.h>
typedef struct tGrafo Grafo;

/**
 * @brief Cria um novo grafo vazio.
 * 
 * Aloca memória para a estrutura do grafo e inicializa seus campos.
 * 
 * @return Grafo* Ponteiro para o grafo criado.
 */
Grafo* criaGrafo();

/**
 * @brief Função para se ler um arquivo de entrada e armazenar os nós com suas respectivas
 * distancias para os outros nós.
 * 
 * Lê o arquivo linha a linha, cria os nós e configura as distâncias entre eles.
 * 
 * @param arquivo Arquivo a ser processado.
 * @return Grafo* Ponteiro para o grafo construído.
 */
Grafo* processarArquivo(FILE* arquivo);

/**
 * @brief Adiciona um nó ao grafo.
 * 
 * Expande o array de nós se necessário e adiciona o nó ao final.
 * 
 * @param grafo Grafo ao qual o nó será adicionado.
 * @param no Nó a ser adicionado.
 */
void adicionaNo(Grafo* grafo, Node* no);

/**
 * @brief Retorna o array de nós do grafo.
 * 
 * @param grafo Grafo do qual recuperar os nós.
 * @return Node** Array de ponteiros para os nós do grafo.
 */
Node** retornaNos(Grafo* grafo);

/**
 * @brief Retorna a quantidade de nós no grafo.
 * 
 * @param grafo Grafo do qual recuperar a quantidade de nós.
 * @return int Quantidade de nós no grafo.
 */
int retornaQtdNos(Grafo* grafo);

/**
 * @brief Imprime todos os nós do grafo.
 * 
 * Exibe o nó inicial e todos os outros nós com seus detalhes, em ordem crescente relativo à posição do nó.
 * 
 * @param grafo Grafo a ser impresso.
 */
void imprimeGrafo(Grafo* grafo);

/**
 * @brief Define o nó inicial do grafo.
 * 
 * @param grafo Grafo ao qual atribuir o nó inicial.
 * @param inicial Nó a ser definido como inicial.
 */
void atribuiInicial(Grafo* grafo, Node* inicial);

/**
 * @brief Retorna o nó inicial do grafo.
 * 
 * @param grafo Grafo do qual recuperar o nó inicial.
 * @return Node* Ponteiro para o nó inicial.
 */
Node* retornaInicial(Grafo* grafo);

/**
 * @brief Constrói o caminho mais curto até um nó.
 * 
 * Utiliza o array 'anterior' para reconstruir o caminho e detectar ciclos.
 * 
 * @param grafo Grafo contendo os nós.
 * @param no Nó destino para o qual o caminho é construído.
 * @param nos Array de nós do grafo.
 * @param anterior Array que indica o predecessor de cada nó.
 * @param buffer Ponteiro para o buffer onde o caminho será armazenado.
 * @param buffer_size Ponteiro para o tamanho do buffer.
 */
void construirCaminho(Grafo* grafo, Node* no, Node** nos, int* anterior, char** buffer, int* buffer_size);


/**
 * @brief Executa o algoritmo de Dijkstra para encontrar os caminhos mais curtos.
 * 
 * Calcula as distâncias mínimas a partir do nó inicial e imprime os resultados ordenados.
 * 
 * @param grafo Grafo no qual executar o algoritmo.
 * @param inicial Nó inicial para o cálculo dos caminhos.
 * @param saida Arquivo onde os resultados serão impressos.
 */
void dijkstra(Grafo* grafo, Node* inicial, FILE* saida);

/**
 * @brief Define o nó inicial do grafo.
 * 
 * @param grafo Grafo ao qual atribuir o nó inicial.
 * @param inicial Nó a ser definido como inicial.
 */
void destroiGrafo(Grafo* grafo);

#endif