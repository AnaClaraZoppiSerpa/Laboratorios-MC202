/*
 * Laboratório 08 - MC202
 * Ana Clara Zoppi Serpa - RA 165880
 * Gabriel Oliveira dos Santos - RA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    No** ilhasGreen;
    No* vizinhosRed;
    No* vizinhosBlue;
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

void inicializarHeap(Heap* heap, int tamanhoMaximo) {
    heap->tamanhoMaximo = tamanhoMaximo;
    heap->tamanhoAtual = 0;
    heap->vetor = malloc(tamanhoMaximo * sizeof(Item));
}

void inserirNoHeap(Heap* heap, Ilha ilha, int prioridade) {
    if (heap->tamanhoAtual < heap->tamanhoMaximo) {
        Item novo;
        novo.vertice = ilha;
        novo.prioridade = prioridade;
        heap->vetor[heap->tamanhoAtual] = novo;
        heap->tamanhoAtual++;
    }
}

void mudarPrioridade(Heap* heap, char nomeIlha [30], int novaPrioridade) {
    for (int i = 0; i < heap->tamanhoAtual; i++) {
        if (strcmp(heap->vetor[i].vertice.nome, nomeIlha)==0) {
            heap->vetor[i].prioridade = novaPrioridade;
            break;
        }
    }
}

int vazio(Heap* heap) {
    return heap->tamanhoAtual == 0;
}

void extrairMinimo(Heap* heap) {
}

void liberarMemoriaHeap(Heap* heap) {
    //TODO
}

void liberarMemoria(Grafo* grafo){
    //TODO
}

int indiceNoVetorDeListas(Grafo g, char nomeIlha [30]) {
    for (int i = 0; i < g.quantasIlhasGreen; i++)
        if (strcmp(g.ilhasGreen[i]->ilha.nome, nomeIlha) == 0)
            return i;

    return -1;
}

////Essa função retorna o custo caso as ilhas sejam vizinhas e 0 caso não sejam vizinhas.
//int vizinhas(Grafo g, char ilha1 [30], char ilha2 [30]) {
//    No* atual = NULL;
//    if (strcmp(ilha1, "Red")==0) {
//        atual = g.vizinhosRed;
//    } else if (strcmp(ilha1, "Blue")==0) {
//        atual = g.vizinhosBlue;
//    } else {
//        atual = g.ilhasGreen[indiceNoVetorDeListas(g, ilha1)]->prox;
//    }
//    while (atual != NULL) {
//        if (strcmp(atual->ilha.nome, ilha2)==0)
//            return atual->ilha.poderMilitar + atual->ilha.distancia;
//        atual = atual->prox;
//    }
//    return 0;
//}

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

void imprimirGrafo(Grafo g) {
    for (int i = 0; i < g.quantasIlhasGreen; i++) {
        printf("%d: ", i);
        imprimirLista(g.ilhasGreen[i]);
        printf("\n");
    }
    printf("Lista Blue: ");
    imprimirLista(g.vizinhosBlue);
    printf("\nLista Red:");
    imprimirLista(g.vizinhosRed);
    printf("\n");
}

int main() {
    int limiteDeRecursosRed;
    int limiteDeRecursosBlue;
    int quantasIlhasGreen;

    scanf("%d %d %d", &limiteDeRecursosRed, &limiteDeRecursosBlue, &quantasIlhasGreen);

    Grafo grafo;
    grafo.quantasIlhasGreen = quantasIlhasGreen;
    grafo.ilhasGreen = malloc(quantasIlhasGreen * sizeof(No*));

    for (int i = 0; i < quantasIlhasGreen; i++) {
        grafo.ilhasGreen[i] = NULL;

        Ilha ilha;
        scanf("%s", &ilha.nome);
        scanf("%d", &ilha.poderMilitar);
        ilha.distancia = 0;

        grafo.ilhasGreen[i] = inserirNaLista(grafo.ilhasGreen[i], ilha);
    }

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

        if (indiceIlha2 != -1) {
            i2.poderMilitar = grafo.ilhasGreen[indiceIlha2]->ilha.poderMilitar;
        }

        if (strcmp(ilha1, "Red")==0) {
            grafo.vizinhosRed = inserirNaLista(grafo.vizinhosRed, i2);
        } else if (strcmp(ilha1, "Blue")==0) {
            grafo.vizinhosBlue = inserirNaLista(grafo.vizinhosBlue, i2);
        } else {
            //O primeiro nó de cada lista do vetor "ilhasGreen" é a ilha que possui os vizinhos.
            //Por isso, inserimos sempre após esse nó, ou seja, em seu próximo.
            //Teremos algo como: G1 (ilha que tem os vizinhos) -> G2 (ilha vizinha) -> G3 (outra ilha vizinha).
            grafo.ilhasGreen[indiceIlha1]->prox = inserirNaLista(grafo.ilhasGreen[indiceIlha1]->prox, i2);
        }

        //Colocamos na lista de vizinhos da ilha 2 a ilha 1.
        Ilha i1;
        strcpy(i1.nome, ilha1);
        i1.distancia = distancia;
        i1.poderMilitar = 0;

        if (indiceIlha1 != -1) {
            i1.poderMilitar = grafo.ilhasGreen[indiceIlha1]->ilha.poderMilitar;
        }

        if (strcmp(ilha2, "Red")==0) {
            grafo.vizinhosRed = inserirNaLista(grafo.vizinhosRed, i1);
        } else if (strcmp(ilha2, "Blue")==0) {
            grafo.vizinhosBlue = inserirNaLista(grafo.vizinhosBlue, i1);
        } else {
            //Como antes, o primeiro nó de cada lista do vetor de ilhas do império Green é a ilha que possui
            //os vizinhos que se seguirão na lista.
            //Assim, para não perder esse informação, a inserção ocorre sempre no segundo nó da lista.
            grafo.ilhasGreen[indiceIlha2]->prox = inserirNaLista(grafo.ilhasGreen[indiceIlha2]->prox, i1);
        }
    }
    //Todas as informações foram lidas e o grafo está com seus vértices e arestas.
    //Agora resta determinar a situação de cada ilha do império Green e imprimir.
    printf("Leu tudo\n");
    for (int i = 0; i < grafo.quantasIlhasGreen; i++) {
        printf("%s: ", grafo.ilhasGreen[i]->ilha.nome);
        //Achar o custo mínimo para o império Red conquistar a ilha i.
        //Achar o custo mínimo para o império Blue conquistar a ilha i.
        //Comparar esses custos de acordo com as regras e exibir resultado.
        printf("\n");
    }
}


