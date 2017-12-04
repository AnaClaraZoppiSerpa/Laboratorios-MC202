/*
 * Laboratório 08 - MC202
 * Ana Clara Zoppi Serpa - RA 165880
 * Gabriel Oliveira dos Santos - RA 197460*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct Ilha {
    char nome [30];
    int poderMilitar;
    int distancia;
    int conquistaRed;
    int conquistaBlue;
} Ilha;

//Nosso grafo será implementado com Listas de Adjacência.
typedef struct No {
    Ilha ilha;
    struct No* prox;
} No;

typedef struct Grafo {
    int quantasIlhasGreen;
    No** ilhas; //Vetor de listas para guardar os vizinhos de cada vértice do grafo.
} Grafo;

//Heap binário de mínimo para usar no algoritmo de Dijkstra.
typedef struct Item {
    Ilha vertice;
    int prioridade;
} Item;

typedef struct Heap {
    Item* vetor;
    int tamanhoAtual;
    int tamanhoMaximo;
} Heap;

//Estrutura para guardar a árvore de caminhos mínimos e o custo para chegar do vértice inicial ao vértice em questão.
typedef struct Informacao {
    int pai;
    int custoTotal;
} Informacao;

void inicializarHeap(Heap* heap, int tamanhoMaximo) {
    heap->tamanhoMaximo = tamanhoMaximo;
    heap->tamanhoAtual = 0;
    heap->vetor = malloc(tamanhoMaximo * sizeof(Item));
}

//Função para reorganizar o heap quando necessário e manter suas propriedades.
//O pai é sempre menor que os filhos (heap de mínimo).
//O filho esquerdo de um elemento em i está em 2i + 1.
//O filho direito, em 2i + 2.
//O pai, em (i-1)/2.
void subirNoHeap(Heap* heap, int posicao) {
    int pai = (posicao - 1) / 2;
    if (heap->vetor[pai].prioridade > heap->vetor[posicao].prioridade) {
        Item aux = heap->vetor[pai];
        heap->vetor[pai] = heap->vetor[posicao];
        heap->vetor[posicao] = aux;
        subirNoHeap(heap, pai);
    }
}

void inserirNoHeap(Heap* heap, Ilha ilha, int prioridade) {
    if (heap->tamanhoAtual < heap->tamanhoMaximo) {
        Item novo;
        novo.vertice = ilha;
        novo.prioridade = prioridade;
        heap->vetor[heap->tamanhoAtual] = novo;
        heap->tamanhoAtual++;
        subirNoHeap(heap, heap->tamanhoAtual-1);
    }
}

int vazio(Heap* heap) {
    return heap->tamanhoAtual == 0;
}

//Novamente uma função para reorganizar o heap e manter suas propriedades.
void descerNoHeap(Heap* heap, int posicao) {
    int filho = 2 * posicao + 1;
    if (filho < heap->tamanhoAtual) {
        if (filho < heap->tamanhoAtual - 1 && heap->vetor[filho].prioridade > heap->vetor[filho + 1].prioridade)
            filho++;
        if (heap->vetor[posicao].prioridade > heap->vetor[filho].prioridade) {
            Item aux = heap->vetor[posicao];
            heap->vetor[posicao] = heap->vetor[filho];
            heap->vetor[filho] = aux;
            descerNoHeap(heap, filho);
        }
    }
}

Item extrairMinimo(Heap* heap) {
    Item aux = heap->vetor[0];
    heap->vetor[0] = heap->vetor[heap->tamanhoAtual - 1];
    heap->vetor[heap->tamanhoAtual - 1] = aux;
    aux = heap->vetor[heap->tamanhoAtual - 1];
    heap->tamanhoAtual--;
    descerNoHeap(heap, 0);
    return aux;
}

void diminuirPrioridade(Heap* heap, Ilha ilha, int novaPrioridade) {
    int i;
    for (i = 0; i < heap->tamanhoAtual; i++) {
        if (strcmp(heap->vetor[i].vertice.nome, ilha.nome)==0) {
            heap->vetor[i].prioridade = novaPrioridade;
            break;
        }
    }
    subirNoHeap(heap, i);
}

int encontrarPrioridade(Heap heap, Ilha ilha) {
    for (int i = 0; i < heap.tamanhoAtual; i++) {
        if (strcmp(heap.vetor[i].vertice.nome, ilha.nome)==0) {
            return heap.vetor[i].prioridade;
        }
    }
    return -1;
}

int pesoAresta(Ilha i) {
    return i.poderMilitar + i.distancia;
}

void liberarMemoriaHeap(Heap* heap) {
    heap->tamanhoMaximo = 0;
    heap->tamanhoAtual = 0;
    free(heap->vetor);
}

void liberarLista(No* lista) {
    if (lista!=NULL) {
        liberarLista(lista->prox);
        free(lista);
    }
}

void liberarMemoriaGrafo(Grafo* grafo){
    for (int i = 0; i < grafo->quantasIlhasGreen + 2; i++) {
        liberarLista(grafo->ilhas[i]);
    }
    free(grafo->ilhas);
    grafo->quantasIlhasGreen = 0;
}

//Função auxiliar para encontrar o índice de uma ilha na lista de adjacências do grafo.
int indiceNoVetorDeListas(Grafo g, char nomeIlha [30]) {
    for (int i = 0; i < g.quantasIlhasGreen + 2; i++)
        if (strcmp(g.ilhas[i]->ilha.nome, nomeIlha) == 0)
            return i;

    return -1;
}

No* inserirNaLista(No* lista, Ilha nova) {
    No* novo = malloc(sizeof(No));
    novo->ilha = nova;
    novo->prox = lista;
    return novo;
}

Informacao* dijkstra(Grafo g, Ilha inicio, int evitarRed, int evitarBlue) {
    Heap heap;
    inicializarHeap(&heap, g.quantasIlhasGreen + 2);

    Informacao* pai = malloc((g.quantasIlhasGreen + 2) * sizeof(Informacao));

    for (int i = 0; i < g.quantasIlhasGreen + 2; i++) {
        //Inicialmente não sabemos quem é o "pai" de cada vértice e representamos isso com -1.
        pai[i].pai = -1;
        //Inicialmente o custo é "infinito" e, caso não exista caminho de um império até uma ilha, esse custo se mantem
        //"infinito". Isso facilita a etapa de comparação de custos realizada mais adiante.
        pai[i].custoTotal = INT_MAX;
        //Colocamos todos os vértices no heap de mínimo.
        inserirNoHeap(&heap, g.ilhas[i]->ilha, INT_MAX);
    }
    //Diminuindo a prioridade do vértice inicial, ele se tornará o mínimo do heap.
    diminuirPrioridade(&heap, inicio, 0);
    //O pai do vértice inicial é ele mesmo.
    pai[indiceNoVetorDeListas(g, inicio.nome)].pai = indiceNoVetorDeListas(g, inicio.nome);
    //O custo total para chegar do inicial até o inicial é 0.
    pai[indiceNoVetorDeListas(g, inicio.nome)].custoTotal = 0;

    while (!vazio(&heap)) {
        Item minimo = extrairMinimo(&heap);
        //Como o primeiro nó de cada lista é o vértice cujos vizinhos serão apresentados na lista, o primeiro vizinho é
        //o segundo nó da lista.
        No* vizinho = g.ilhas[indiceNoVetorDeListas(g, minimo.vertice.nome)]->prox;
        //Vértice já visitado, afinal, sua prioridade foi atualizada e não é mais INT_MAX.
        if (minimo.prioridade != INT_MAX) {
            //Verificamos os vértices adjacentes.
            while (vizinho != NULL) {
                //Se o custo é minimizado, atualizamos o pai e o custo total na árvore de caminhos mínimos, assim como a
                //prioridade no heap.
                if (minimo.prioridade + pesoAresta(vizinho->ilha) < encontrarPrioridade(heap, vizinho->ilha)) {
                    if (!(minimo.vertice.conquistaBlue && evitarBlue) && !(minimo.vertice.conquistaRed && evitarRed)) {
                        pai[indiceNoVetorDeListas(g, vizinho->ilha.nome)].pai = indiceNoVetorDeListas(g, minimo.vertice.nome);
                        pai[indiceNoVetorDeListas(g, vizinho->ilha.nome)].custoTotal = minimo.prioridade + pesoAresta(vizinho->ilha);
                        diminuirPrioridade(&heap, vizinho->ilha, minimo.prioridade + pesoAresta(vizinho->ilha));
                    }
                }
                vizinho = vizinho->prox;
            }
        }
    }
    liberarMemoriaHeap(&heap);
    return pai;
}

int main() {
    int limiteDeRecursosRed;
    int limiteDeRecursosBlue;
    int quantasIlhasGreen;

    scanf("%d %d %d", &limiteDeRecursosRed, &limiteDeRecursosBlue, &quantasIlhasGreen);

    Grafo grafo;
    grafo.quantasIlhasGreen = quantasIlhasGreen;
    //Teremos, além das listas para as ilhas de Green, listas para as ilhas de Blue e Red, guardando seus vizinhos.
    grafo.ilhas = malloc((quantasIlhasGreen + 2) * sizeof(No*));

    //Leitura e inserção das ilhas do império Green no grafo.
    for (int i = 0; i < quantasIlhasGreen; i++) {
        grafo.ilhas[i] = NULL;

        Ilha ilha;
        scanf("%s", &ilha.nome);
        scanf("%d", &ilha.poderMilitar);
        ilha.distancia = 0;
        ilha.conquistaRed = 0;
        ilha.conquistaBlue = 0;

        grafo.ilhas[i] = inserirNaLista(grafo.ilhas[i], ilha);
    }
    //Inserção do porto do império Red e do porto do império Blue, ambos como ilhas.
    grafo.ilhas[quantasIlhasGreen] = NULL;
    grafo.ilhas[quantasIlhasGreen+1] = NULL;

    Ilha red;
    strcpy(red.nome, "Red");
    red.distancia = 0;
    red.poderMilitar = 0;
    red.conquistaRed = 0;
    red.conquistaBlue = 0;

    Ilha blue;
    strcpy(blue.nome, "Blue");
    blue.distancia = 0;
    blue.poderMilitar = 0;
    blue.conquistaRed = 0;
    blue.conquistaBlue = 0;

    grafo.ilhas[quantasIlhasGreen] = inserirNaLista(grafo.ilhas[quantasIlhasGreen], red);
    grafo.ilhas[quantasIlhasGreen + 1] = inserirNaLista(grafo.ilhas[quantasIlhasGreen + 1], blue);

    char ilha1 [30];
    char ilha2 [30];
    int distancia;
    //Leitura de distâncias entre ilhas e inserção no grafo.
    while (scanf("%s %s %d", &ilha1, &ilha2, &distancia) != EOF && distancia != -1) {
        int indiceIlha1 = indiceNoVetorDeListas(grafo, ilha1);
        int indiceIlha2 = indiceNoVetorDeListas(grafo, ilha2);

        //Colocamos na lista de vizinhos da ilha 1 a ilha 2.
        Ilha i2;
        strcpy(i2.nome, ilha2);
        i2.distancia = distancia;
        i2.poderMilitar = 0;

        i2.poderMilitar = grafo.ilhas[indiceIlha2]->ilha.poderMilitar;

        //O primeiro nó de cada lista do vetor "ilhasGreen" é a ilha que possui os vizinhos.
        //Por isso, inserimos sempre após esse nó, ou seja, em seu próximo.
        //Teremos algo como: G1 (ilha que tem os vizinhos) -> G2 (ilha vizinha de G1) -> G3 (outra ilha vizinha de G1).
        grafo.ilhas[indiceIlha1]->prox = inserirNaLista(grafo.ilhas[indiceIlha1]->prox, i2);

        //Colocamos na lista de vizinhos da ilha 2 a ilha 1.
        Ilha i1;
        strcpy(i1.nome, ilha1);
        i1.distancia = distancia;
        i1.poderMilitar = 0;

        i1.poderMilitar = grafo.ilhas[indiceIlha1]->ilha.poderMilitar;

        //Como antes, o primeiro nó de cada lista do vetor de ilhas do império Green é a ilha que possui
        //os vizinhos que se seguirão na lista.
        //Assim, para não perder esse informação, a inserção ocorre sempre no segundo nó da lista.
        grafo.ilhas[indiceIlha2]->prox = inserirNaLista(grafo.ilhas[indiceIlha2]->prox, i1);
    }
    //Todas as informações foram lidas e o grafo está com seus vértices e arestas.
    //Agora resta determinar a situação de cada ilha do império Green e imprimir.

    Informacao* arvRed = NULL;
    Informacao* arvBlue = NULL;
    //Árvore de caminhos mínimos a partir de Red
    arvRed = dijkstra(grafo, grafo.ilhas[quantasIlhasGreen]->ilha, 0, 0);
    //Árvore de caminhos mínimos a partir de Blue
    arvBlue = dijkstra(grafo, grafo.ilhas[quantasIlhasGreen + 1]->ilha, 0, 0);

    //Percorremos o vetor de listas do grafo para verificar a situação de cada ilha do império Green.
    for (int i = 0; i < grafo.quantasIlhasGreen; i++) {
        printf("%s: ", grafo.ilhas[i]->ilha.nome);
        if (limiteDeRecursosRed >= arvRed[i].custoTotal && limiteDeRecursosBlue >= arvBlue[i].custoTotal) {
            //Tanto Red quanto Blue são capazes, então aquele cujo custo para conquistar a ilha for menor deve conquistar a ilha.
            if (arvRed[i].custoTotal < arvBlue[i].custoTotal) {
                printf("Conquistado por Red (%d)\n", arvRed[i].custoTotal);
                grafo.ilhas[i]->ilha.conquistaRed = 1;
                arvBlue = dijkstra(grafo, grafo.ilhas[quantasIlhasGreen + 1]->ilha, 1, 0);
            } else if (arvRed[i].custoTotal > arvBlue[i].custoTotal) {
                printf("Conquistado por Blue (%d)\n", arvBlue[i].custoTotal);
                grafo.ilhas[i]->ilha.conquistaBlue = 1;
                arvRed = dijkstra(grafo, grafo.ilhas[quantasIlhasGreen]->ilha, 0, 1);
            } else {
                //Os dois impérios podem conquistar a ilha com o mesmo custo. Então aquele com o maior limite de recursos a conquista.
                if (limiteDeRecursosRed > limiteDeRecursosBlue) {
                    printf("Conquistado por Red (%d)\n", arvRed[i].custoTotal);
                    grafo.ilhas[i]->ilha.conquistaRed = 1;
                    arvBlue = dijkstra(grafo, grafo.ilhas[quantasIlhasGreen + 1]->ilha, 1, 0);
                } else {
                    printf("Conquistado por Blue (%d)\n", arvBlue[i].custoTotal);
                    grafo.ilhas[i]->ilha.conquistaBlue = 1;
                    arvRed = dijkstra(grafo, grafo.ilhas[quantasIlhasGreen]->ilha, 0, 1);
                }
            }
        } else {
            //Só um deles pode conquistar aquela ilha e a conquistará sem disputa.
            if (limiteDeRecursosRed >= arvRed[i].custoTotal) {
                printf("Conquistado por Red (%d)\n", arvRed[i].custoTotal);
                grafo.ilhas[i]->ilha.conquistaRed = 1;
                arvBlue = dijkstra(grafo, grafo.ilhas[quantasIlhasGreen + 1]->ilha, 1, 0);
            } else if (limiteDeRecursosBlue >= arvBlue[i].custoTotal) {
                printf("Conquistado por Blue (%d)\n", arvBlue[i].custoTotal);
                grafo.ilhas[i]->ilha.conquistaBlue = 1;
                arvRed = dijkstra(grafo, grafo.ilhas[quantasIlhasGreen]->ilha, 0, 1);
            } else {
                //Nenhum pode conquistar aquela ilha, então ela será mantida por Green.
                printf("Mantido por Green\n");
            }
        }
    }
    free(arvBlue);
    free(arvRed);
    liberarMemoriaGrafo(&grafo);
}


