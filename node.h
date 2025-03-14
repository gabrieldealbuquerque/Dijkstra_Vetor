#ifndef NODE_H
#define NODE_H

#include <float.h>
#define INFINITY FLT_MAX

/**
 * @struct tNode
 * @brief Estrutura que representa um nó do grafo.
 * 
 * Contém informações sobre o número do nó, distâncias para outros nós e metadados.
 */
typedef struct tNode Node;

/**
 * @brief Cria um novo nó com o número especificado.
 * 
 * Aloca memória para o nó e inicializa seus campos.
 * 
 * @param numero Identificador do nó.
 * @return Node* Ponteiro para o nó criado.
 */
Node* criaNo(int numero);

/**
 * @brief Insere uma distância no array de distâncias do nó.
 * 
 * Gerencia alocação dinâmica e expansão do array conforme necessário.
 * 
 * @param no Nó que receberá a distância.
 * @param posicao Posição (índice) onde a distância será inserida.
 * @param distancia Valor da distância a ser armazenado.
 */
void insereDistancias(Node* no, int* posicao, double distancia);

/**
 * @brief Libera toda a memória alocada para o nó.
 * 
 * @param no Nó a ser destruído.
 */
void destroiNo(Node* no);

/**
 * @brief Retorna o número identificador do nó.
 * 
 * @param no Nó a ser consultado.
 * @return int Número do nó.
 */
int retornaNumero(Node* no);

/**
 * @brief Imprime os dados do nó no terminal.
 * Essa função serviu mais como uma forma de debugar o código,
 * ela não é realmente usada para a saída em si.
 * 
 * Formato: "node_X\n dist1, dist2, ...\n".
 * 
 * @param no Nó a ser impresso.
 */
void imprimeNo(Node* no);

/**
 * @brief Ajusta o último elemento do array de distâncias.
 * 
 * Usado para corrigir o caso em que o último nó não tinha distância 0 a ele 
 * mesmo quando ele era a origem. Por exemplo, se o grafo tinha apenas 99 nós
 * e a origem do dijkstra era o nó 98, o nó 98 não tinha distância 0 para ele mesmo,
 * essa função serve justamente para arrumar esse erro.
 * 
 * @param no Nó a ser ajustado.
 */
void arrumaUltimoNo(Node* no);

/**
 * @brief Retorna a distância do nó para outro baseado na posição do outro nó,
 * que é o seu índice nessa implementação.
 * 
 * @param no Nó de origem.
 * @param posicao Índice do nó destino.
 * @return float Distância armazenada.
 */
float retornaDistancia(Node* no, int posicao);

#endif