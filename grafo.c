#include "grafo.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h> // Para INFINITY

struct tGrafo
{
    Node *inicial;
    Node **nos;
    int quantidade;
    int capacidade;
};

/**
 * @struct NoComDistancia
 * @brief Estrutura auxiliar para ordenação de nós por distância.
 *
 * Armazena o índice do nó e sua distância mínima calculada.
 */
typedef struct
{
    int indice;
    float distancia;
} NoComDistancia;

void imprimirResultadosDoDijkstra(Grafo *grafo, FILE *saida, int *anterior, float *distancias, NoComDistancia *nos_ordenados, int qtd);

Grafo *criaGrafo()
{
    Grafo *novo = (Grafo *)calloc(1, sizeof(Grafo));
    novo->inicial = NULL;
    novo->nos = NULL;
    novo->quantidade = 0;
    novo->capacidade = 0;
    return novo;
}

Grafo *processarArquivo(FILE *arquivo)
{
    Grafo *grafo = criaGrafo();
    char *linha = NULL;
    size_t tamanho_linha = 0;
    size_t bytes_lidos;
    int primeiro_no = 1; // Flag para controlar o primeiro nó

    while ((bytes_lidos = getline(&linha, &tamanho_linha, arquivo)) != -1)
    {
        // Remove newline e quebras
        linha[strcspn(linha, "\r\n")] = '\0';

        // Extrai número do nó
        char *token = strtok(linha, ",");
        if (!token)
            continue;

        int numero_node;
        if (sscanf(token, "node_%d", &numero_node) != 1)
        {
            fprintf(stderr, "Formato de nó inválido: %s\n", token);
            continue;
        }

        // Cria nó
        Node *no = criaNo(numero_node);
        if (primeiro_no)
        {
            atribuiInicial(grafo, no); // Atribui ao nó inicial

            primeiro_no = 0; // Desativa a flag
        }
        else
        {
            // Processa valores
            int pos = 0;
            while ((token = strtok(NULL, ",")) != NULL)
            {
                while (isspace(*token))
                    token++; // Remove espaços

                if (strcmp(token, "bomba") == 0 || atof(token) == 0)
                    insereDistancias(no, &pos, -1.0f);
                    /*aqui é passado o ponteiro de pos, pois há uma situação (quando o nó acha ele mesmo) que é necessário que a posição avance e a distância lida seja
                    posta no próximo nó (se o inicial for 50, quando ler a 50ª distância, a distância de 50 para 50 é definida como 0 e pos é incrementado para inserir a 50ª
                    no 51º nó, já que a distância do nó para ele mesmo não está contida na entrada)*/
                else
                {
                    float valor = atof(token);
                    insereDistancias(no, &pos, valor);
                }
                pos++;
            }
            adicionaNo(grafo, no); // Adiciona aos nós normais (conta na quantidade)
        }
    }
    free(linha); // Libera a memória alocada pelo getline
    arrumaUltimoNo(retornaNos(grafo)[retornaQtdNos(grafo) - 1]);
    return grafo;
}

void adicionaNo(Grafo *grafo, Node *no)
{
    // Verifica se o grafo ou nó são inválidos
    if (!grafo || !no)
    {
        fprintf(stderr, "Grafo ou nó inválido!\n");
        return;
    }

    // Verifica se precisa expandir o array
    if (grafo->quantidade >= grafo->capacidade)
    {
        // Calcula nova capacidade (dobra ou adiciona 200 se capacidade for zero)
        int nova_capacidade = (grafo->capacidade == 0) ? 200 : grafo->capacidade * 2;

        Node **novos_nos = (Node **)realloc(grafo->nos, nova_capacidade * sizeof(Node *));
        if (!novos_nos)
        {
            fprintf(stderr, "Falha ao realocar memória para nós!\n");
            return;
        }

        grafo->nos = novos_nos;
        grafo->capacidade = nova_capacidade;
    }

    // Adiciona o nó e atualiza a quantidade
    grafo->nos[grafo->quantidade] = no;
    grafo->quantidade++;

    // printf("Nó %d adicionado. Qtd: %d, Capacidade: %d\n", retornaNumero(no), grafo->quantidade, grafo->capacidade);
}

Node **retornaNos(Grafo *grafo)
{
    return grafo->nos;
}

int retornaQtdNos(Grafo *grafo)
{
    return grafo->quantidade;
}

void imprimeGrafo(Grafo *grafo)
{
    printf("Nó inicial: node_%d", retornaNumero(grafo->inicial));
    // printf("qtd de nós no grafo: %d", grafo -> quantidade);
    for (int i = 0; i < grafo->quantidade; i++)
        imprimeNo(grafo->nos[i]);
}

void atribuiInicial(Grafo *grafo, Node *inicial)
{
    grafo->inicial = inicial;
}

Node *retornaInicial(Grafo *grafo)
{
    return grafo->inicial;
}

void construirCaminho(Grafo *grafo, Node *no, Node **nos, int *anterior, char **buffer, int *buffer_size)
{
    int qtd_nos = retornaQtdNos(grafo);
    int *caminho = malloc(qtd_nos * sizeof(int));
    int *visitados = calloc(qtd_nos, sizeof(int)); // Novo array para detectar ciclos
    int count = 0;
    int current = -1;

    // Encontra índice do nó destino
    for (int i = 0; i < qtd_nos; i++)
    {
        if (nos[i] == no)
        {
            current = i;
            break;
        }
    }

    if (current == -1)
    {
        free(caminho);
        return;
    }

    // Coleta o caminho com verificação de ciclo
    while (current != -1 && count < qtd_nos)
    {
        // Detecção de ciclo
        if (visitados[current])
        {
            fprintf(stderr, "Ciclo detectado no nó %d\n", current);
            break;
        }
        visitados[current] = 1;
        caminho[count++] = current;
        current = anterior[current];
    }

    // Calcula tamanho necessário
    int needed_size = 0;
    for (int i = 0; i < count; i++)
    {
        needed_size += snprintf(NULL, 0, "node_%d%s",
                                retornaNumero(nos[caminho[i]]),
                                (i < count - 1 ? " <- " : ""));
    }

    // Realoca se necessário
    if (*buffer_size < needed_size + 1)
    {
        *buffer_size = needed_size + 1;
        *buffer = realloc(*buffer, *buffer_size);
        if (!*buffer)
        {
            fprintf(stderr, "Falha ao realocar memória para caminho\n");
            exit(EXIT_FAILURE);
        }
    }

    // Constrói o caminho
    int offset = 0;
    for (int i = 0; i < count; i++)
    {
        offset += snprintf(*buffer + offset, *buffer_size - offset,
                           "node_%d%s",
                           retornaNumero(nos[caminho[i]]),
                           (i < count - 1 ? " <- " : ""));
    }
    free(visitados);
    free(caminho);
}

/**
 * @brief Função interna de comparação para ordenação de nós por distância.
 *
 * Utilizada pelo qsort para ordenar o array nos_ordenados.
 *
 * @param a Ponteiro para o primeiro NoComDistancia.
 * @param b Ponteiro para o segundo NoComDistancia.
 * @return int Resultado da comparação (1, -1 ou 0).
 */
int comparar_distancia(const void *a, const void *b)
{
    NoComDistancia *no_a = (NoComDistancia *)a;
    NoComDistancia *no_b = (NoComDistancia *)b;
    return (no_a->distancia > no_b->distancia) ? 1 : -1;
}

void dijkstra(Grafo *grafo, FILE *saida)
{
    int qtd = retornaQtdNos(grafo);
    Node **nos = retornaNos(grafo);
    float *distancias = (float *)malloc(qtd * sizeof(float));
    int *visitados = (int *)calloc(qtd, sizeof(int));
    int *anterior = (int *)malloc(qtd * sizeof(int));

    if (!distancias || !visitados || !anterior)
    {
        free(distancias);
        free(visitados);
        free(anterior);
        return;
    }

    // Inicialização
    for (int i = 0; i < qtd; i++)
    {
        distancias[i] = FLT_MAX;
        anterior[i] = -1;
    }
    // Encontrar índice do nó inicial
    int indice_inicial = retornaNumero(retornaInicial(grafo));
    // printf("indice inicial: %d\n", indice_inicial);
    if (indice_inicial == -1)
    {
        free(distancias);
        free(visitados);
        free(anterior);
        return;
    }
    distancias[indice_inicial] = 0.0f;

    for (int i = 0; i < qtd; i++)
    {
        // Encontra o nó não visitado com menor distância
        int u = -1;
        float min_dist = FLT_MAX;
        for (int j = 0; j < qtd; j++)
        {
            if (!visitados[j] && distancias[j] >= 0 && distancias[j] < min_dist)
            {
                min_dist = distancias[j];
                u = j;
            }
        }
        if (u == -1)
            break;
        visitados[u] = 1;

        // Atualiza distâncias para vizinhos
        for (int v = 0; v < qtd; v++)
        {
            float distancia = retornaDistancia(nos[u], v);
            if (distancia < 0 || distancias[v] < 0)
                continue;

            if (distancias[v] > distancias[u] + distancia)
            {
                distancias[v] = distancias[u] + distancia;
                anterior[v] = u;
            }
        }
    }

    NoComDistancia *nos_ordenados = malloc(qtd * sizeof(NoComDistancia));
    for (int i = 0; i < qtd; i++)
    {
        nos_ordenados[i].indice = i;
        nos_ordenados[i].distancia = distancias[i];
    }

    // Ordena os nós com base na distância
    qsort(nos_ordenados, qtd, sizeof(NoComDistancia), comparar_distancia);

    // Imprime os caminhos na ordem ordenada
    imprimirResultadosDoDijkstra(grafo, saida, anterior, distancias, nos_ordenados, qtd);

    free(nos_ordenados);
    free(distancias);
    free(visitados);
    free(anterior);
}

/**
 * @brief Função interna que imprime os resultados do algoritmo de Dijkstra em ordem crescente de distância.
 *
 * @param grafo Grafo contendo os nós e estrutura de dados.
 * @param saida Arquivo de saída onde os resultados serão impressos.
 * @param anterior Array de predecessores para cada nó no caminho mínimo.
 * @param distancias Array de distâncias mínimas calculadas pelo algoritmo.
 * @param nos_ordenados Array de nós ordenados por distância crescente.
 * @param qtd Quantidade de nós presentes no grafo.
 */
void imprimirResultadosDoDijkstra(Grafo *grafo, FILE *saida, int *anterior, float *distancias, NoComDistancia *nos_ordenados, int qtd)
{
    Node **nos = retornaNos(grafo);
    for (int j = 0; j < qtd; j++)
    {
        int i = nos_ordenados[j].indice; // Usa o índice ordenado
        char *caminho = NULL;
        int caminho_size = 0;

        construirCaminho(grafo, nos[i], nos, anterior,
                         &caminho, &caminho_size);

        fprintf(saida, "SHORTEST PATH TO node_%d: %s (Distance: %.2f)\n",
                retornaNumero(nos[i]), caminho ? caminho : "", distancias[i]);

        free(caminho);
    }
}

void destroiGrafo(Grafo *grafo)
{
    for (int i = 0; i < grafo->quantidade; i++)
    {
        destroiNo(grafo->nos[i]);
    }
    free(grafo->inicial);
    free(grafo->nos);
    free(grafo);
}
