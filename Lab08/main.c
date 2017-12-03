/*
 * Laboratório 08 - MC202
 * Ana Clara Zoppi Serpa - RA 165880
 * Gabriel Oliveira dos Santos - RA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct Ilha {
    char nome [30];
    int poderMilitar;
    int distancia;
} Ilha;

typedef struct No {
    Ilha ilha;
    struct No* prox;
} No;

typedef struct Grafo {
    int quantasIlhasGreen;
    No** ilhas;
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

//Estrutura para guardar a árvore de caminhos mínimos
typedef struct Relacao {
    //Exemplo: o G1 é pai do G2.
    char pai [30]; //G1
    char filho [30]; //G2
} Relacao;

void inicializarHeap(Heap* heap, int tamanhoMaximo) {
    heap->tamanhoMaximo = tamanhoMaximo;
    heap->tamanhoAtual = 0;
    heap->vetor = malloc(tamanhoMaximo * sizeof(Item));
}

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

void imprimirHeap(Heap heap) {
    for (int i = 0; i < heap.tamanhoAtual; i++) {
        printf("Ilha %s, prioridade %d\n", heap.vetor[i].vertice.nome, heap.vetor[i].prioridade);
    }
}

int pesoAresta(Ilha i) {
    return i.poderMilitar + i.distancia;
}

void liberarMemoriaHeap(Heap* heap) {
    //TODO
}

void liberarMemoria(Grafo* grafo){
    //TODO
}

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

void imprimirLista(No* lista) {
    No* atual = lista;
    while (atual != NULL) {
        printf("%s, ", atual->ilha.nome);
        atual = atual->prox;
    }
}

int* dijkstra(Grafo g, Ilha inicio) {
    Heap heap;
    inicializarHeap(&heap, g.quantasIlhasGreen + 1);

    int* pai = malloc(heap.tamanhoMaximo * sizeof(int));

    for (int i = 0; i < heap.tamanhoMaximo; i++) {
        pai[i] = -1;
    }

    for (int i = 0; i < heap.tamanhoMaximo - 1; i++) {
        inserirNoHeap(&heap, g.ilhas[i]->ilha, INT_MAX);
    }
    inserirNoHeap(&heap, inicio, 0);
    pai[indiceNoVetorDeListas(g, inicio.nome)] = indiceNoVetorDeListas(g, inicio.nome);
    while (!vazio(&heap)) {
        Item minimo = extrairMinimo(&heap);
        No* vizinho = g.ilhas[indiceNoVetorDeListas(g, minimo.vertice.nome)]->prox;

        if (minimo.prioridade != INT_MAX) {
            while (vizinho != NULL) {
                if (minimo.prioridade + pesoAresta(vizinho->ilha) < encontrarPrioridade(heap, vizinho->ilha)) {
                    //atualizar pai e tals
                    pai[indiceNoVetorDeListas(g, vizinho->ilha.nome)] = indiceNoVetorDeListas(g, minimo.vertice.nome);
                    //pai[indiceNoVetorDeListas(g, vizinho->ilha.nome)] = minimo.prioridade + pesoAresta(vizinho->ilha);
                    diminuirPrioridade(&heap, vizinho->ilha, minimo.prioridade + pesoAresta(vizinho->ilha));
                }
                vizinho = vizinho->prox;
            }
        }
    }
    return pai;
}

int main() {
    int limiteDeRecursosRed;
    int limiteDeRecursosBlue;
    int quantasIlhasGreen;

    scanf("%d %d %d", &limiteDeRecursosRed, &limiteDeRecursosBlue, &quantasIlhasGreen);

    Grafo grafo;
    grafo.quantasIlhasGreen = quantasIlhasGreen;
    grafo.ilhas = malloc((quantasIlhasGreen + 2) * sizeof(No*));

    for (int i = 0; i < quantasIlhasGreen; i++) {
        grafo.ilhas[i] = NULL;

        Ilha ilha;
        scanf("%s", &ilha.nome);
        scanf("%d", &ilha.poderMilitar);
        ilha.distancia = 0;

        grafo.ilhas[i] = inserirNaLista(grafo.ilhas[i], ilha);
    }
    grafo.ilhas[quantasIlhasGreen] = NULL;
    grafo.ilhas[quantasIlhasGreen+1] = NULL;

    Ilha red;
    strcpy(red.nome, "Red");
    red.distancia = 0;
    red.poderMilitar = 0;

    Ilha blue;
    strcpy(blue.nome, "Blue");
    blue.distancia = 0;
    blue.poderMilitar = 0;

    grafo.ilhas[quantasIlhasGreen] = inserirNaLista(grafo.ilhas[quantasIlhasGreen], red);
    grafo.ilhas[quantasIlhasGreen + 1] = inserirNaLista(grafo.ilhas[quantasIlhasGreen + 1], blue);

    char ilha1 [30];
    char ilha2 [30];
    int distancia;
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
        //Teremos algo como: G1 (ilha que tem os vizinhos) -> G2 (ilha vizinha) -> G3 (outra ilha vizinha).
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
    printf("Leu tudo\n");

    int* arvRed;
    int* arvBlue;
    //Árvore de caminhos mínimos a partir de Red
    arvRed = dijkstra(grafo, grafo.ilhas[quantasIlhasGreen]->ilha);
    //Árvore de caminhos mínimos a partir de Blue
    arvBlue = dijkstra(grafo, grafo.ilhas[quantasIlhasGreen + 1]->ilha);

    printf("ArvRed\n");
    for (int i = 0; i < quantasIlhasGreen+1; i++) {
        printf("O pai de %d eh %d\n", i, arvRed[i]);
        if (arvRed[i] != -1)
            printf("%d = %s, %d = %s\n", i, grafo.ilhas[i]->ilha.nome, arvRed[i], grafo.ilhas[arvRed[i]]->ilha.nome);
    }
    printf("\nArvBlue\n");
    for (int i = 0; i < quantasIlhasGreen+1; i++) {
        printf("O pai de %d eh %d\n", i, arvBlue[i]);
        if (arvBlue[i] != -1)
            printf("%d = %s, %d = %s\n", i, grafo.ilhas[i]->ilha.nome, arvBlue[i], grafo.ilhas[arvBlue[i]]->ilha.nome);
    }

    for (int i = 0; i < grafo.quantasIlhasGreen + 2; i++) {
        printf("%s: ", grafo.ilhas[i]->ilha.nome);
        //Achar o custo mínimo para o império Red conquistar a ilha i.
        //Achar o custo mínimo para o império Blue conquistar a ilha i.
        //Comparar esses custos de acordo com as regras e exibir resultado.
        printf("\n");
    }
}


