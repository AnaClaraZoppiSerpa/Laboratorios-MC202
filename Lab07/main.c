#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Laboratório 07
MC202 - Estruturas de Dados
Ana Clara Zoppi Serpa - RA 165880
Gabriel Oliveira dos Santos - RA 197460
*/

typedef struct No {
    struct No* esq;
    struct No* dir;
    char pasta[30];
    char programa[30];
} No;

typedef struct Arvore {
    No* raiz;
    //TODO - Campo para a préordem da cópia de segurança.
    //TODO - Campo in-ordem para a cópia de segurança.
} Arvore;

////A função retorna um ponteiro para a raiz da nova árvore.
//No* reconstruir(int preordem [], int inordem[], int tamanho) {
//	if (tamanho == 1) {
//        /*Caso base: a pré-ordem e a in-ordem são iguais e consistem de um único nó,
//        que vai ser a raiz. Então criamos esse tal nó e retornamos ele, ele é a raiz
//        da nossa árvore.*/
//        No* raiz = criarNo(/*passamos os parâmetros desejados para a criação do nó aqui e pronto*/);
//        return raiz;
//	}
//    //Quando não é o caso base, precisamos considerar subárvores.
//    //Dada a pré-ordem, seu primeiro nó é a raiz.
//    int valorRaiz = preordem[0];
//    //Então nós procuramos esse valor na inordem, porque à esquerda vai estar a
//    //subárvore esquerda e à direita vai estar a subárvore direita.
//}

No* insereRec(No* raiz, No* novo){
    if(raiz == NULL){
        return novo;
    }

    if(strcmp(novo->programa, raiz->programa) < 0){
        raiz->esq = insereRec(raiz->esq, novo);
        if(raiz->esq == novo){
            strcpy(raiz->esq->pasta, raiz->programa);
            strcat(raiz->esq->pasta, "_esq");
        }
    }else{
        raiz->dir = insereRec(raiz->dir, novo);
        if(raiz->dir == novo){
            strcpy(raiz->dir->pasta, raiz->programa);
            strcat(raiz->dir->pasta, "_dir");
        }
    }

    return raiz;
}

void instalarNovoPrograma(Arvore *arvore) {
    No* novo = malloc(sizeof(No));

    scanf("%s", &novo->programa);
    novo->esq = NULL;
    novo->dir = NULL;

    No* aux = insereRec(arvore->raiz, novo);
    if(arvore->raiz == NULL){
        strcpy(aux->pasta, "raiz");
        arvore->raiz = aux;
    }
}

void desinstalarPrograma() {

}

void testarVelocidadeDeResposta() {

}

void otimizarCapacidadeDeResposta() {

}

void criarCopiasDeSeguranca() {

}

void restaurarCopiaDeSeguranca() {

}

void imprimirTodosOsProgramas(No* raiz) {
    if(raiz != NULL){
        printf("(");
        imprimirTodosOsProgramas(raiz->esq);
        printf(")");

        printf("%s [%s]", raiz->programa, raiz->pasta);

        printf("(");
        imprimirTodosOsProgramas(raiz->dir);
        printf(")");
    }
}

int main() {
    int quantidadePresenteAoIniciar;
    scanf("%d", &quantidadePresenteAoIniciar);
    //TODO - Leitura das sementes geradoras in-ordem e preordem e salvá-las como cópia de segurança inicial.
    int operacao;

    Arvore arvore;
    arvore.raiz = NULL;

    while (scanf("%d", &operacao) != EOF) {
        switch (operacao) {
            case 1:
                instalarNovoPrograma(&arvore);
                break;

            case 2:
                desinstalarPrograma();
                break;

            case 3:
                testarVelocidadeDeResposta();
                break;

            case 4:
                otimizarCapacidadeDeResposta();
                break;

            case 5:
                criarCopiasDeSeguranca();
                break;

            case 6:
                restaurarCopiaDeSeguranca();
                break;

            case 7:
                imprimirTodosOsProgramas(arvore.raiz);
                break;

        }

    }
}
