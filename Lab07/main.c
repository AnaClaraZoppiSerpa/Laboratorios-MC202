#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Laborat�rio 07
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
    //TODO - Campo para a pr�ordem da c�pia de seguran�a.
    //TODO - Campo in-ordem para a c�pia de seguran�a.
} Arvore;

////A fun��o retorna um ponteiro para a raiz da nova �rvore.
//No* reconstruir(int preordem [], int inordem[], int tamanho) {
//	if (tamanho == 1) {
//        /*Caso base: a pr�-ordem e a in-ordem s�o iguais e consistem de um �nico n�,
//        que vai ser a raiz. Ent�o criamos esse tal n� e retornamos ele, ele � a raiz
//        da nossa �rvore.*/
//        No* raiz = criarNo(/*passamos os par�metros desejados para a cria��o do n� aqui e pronto*/);
//        return raiz;
//	}
//    //Quando n�o � o caso base, precisamos considerar sub�rvores.
//    //Dada a pr�-ordem, seu primeiro n� � a raiz.
//    int valorRaiz = preordem[0];
//    //Ent�o n�s procuramos esse valor na inordem, porque � esquerda vai estar a
//    //sub�rvore esquerda e � direita vai estar a sub�rvore direita.
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
    //TODO - Leitura das sementes geradoras in-ordem e preordem e salv�-las como c�pia de seguran�a inicial.
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
