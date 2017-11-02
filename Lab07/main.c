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

//sempre retorna a raiz
No* insereRec(No* raiz, No** novo){
    if(raiz == NULL){
        return *novo;
    }

    if(strcmp((*novo)->programa, raiz->programa) < 0){
        raiz->esq = insereRec(raiz->esq, novo);
        if(raiz->esq == *novo){
            strcpy(raiz->esq->pasta, raiz->programa);
            strcat(raiz->esq->pasta, "_esq");
        }
    }else{
        raiz->dir = insereRec(raiz->dir, novo);
        if(raiz->dir == *novo){
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

    No* aux = insereRec(arvore->raiz, &novo);
    if(arvore->raiz == NULL){
        strcpy(aux->pasta, "raiz");
        arvore->raiz = aux;
    }

    printf("[INSTALL] Programa %s.exe instalado com sucesso na pasta %s\n", novo->programa, novo->pasta);
}

void retiraNo(No* no, No* pai){

    //retirar a raiz
    if(pai == NULL){

        //s� tem a raiz
        if(no->dir == NULL && no->esq == NULL){
            free(no);
            no = NULL;
        }
        else{

            //se tem dois filhos
            if(no->dir != NULL && no->esq != NULL){
                //se tem os dois filhos, troca com a folha mais a direita da subarvore da direita
                //e remove essa folha
                char nomeAux[30];
                No* aux = no->esq;
                pai = no;

                while(aux->dir != NULL){
                    pai = aux;
                    aux = aux->dir;
                }

                //troca a informa��o com o no mais a direita da subarvore da esquerda
                strcpy(nomeAux, no->programa);
                strcpy(no->programa, aux->programa);
                strcpy(aux->programa, nomeAux);

                strcpy(nomeAux, no->pasta);
                strcpy(no->pasta, aux->pasta);
                strcpy(aux->pasta, nomeAux);

                retiraNo(aux, pai);
            }
            else{//tem apenas um filho

                //tem apenas o filho esquerdo
                if(no->esq != NULL){
                    no = no->esq;
                }
                else{ //tem apenas o filho direito
                    no = no->dir;
                }
            }
        }
    }else {
        //� uma folha, simplesmente remove e faz o pai apontar para nulo
        if (no->esq == NULL && no->dir == NULL) {
            if (pai->dir == no) {
                pai->dir = NULL;
            } else {
                pai->esq = NULL;
            }

            no = NULL;
            free(no);
        } else {
            //se tem apenas um filho, fa�a pai apontar para o neto
            if (no->esq == NULL && no->dir != NULL) { //tem apenas o filho direito
                if (pai->dir == no) {
                    pai->dir = no->dir;
                } else {
                    pai->esq = no->dir;
                }

                no = NULL;
                free(no);
            } else if (no->dir == NULL && no->esq != NULL) { //tem apenas o filho esquerdo
                if (pai->dir == no) {
                    pai->dir = no->esq;
                } else {
                    pai->esq = no->esq;
                }

                no = NULL;
                free(no);
            } else {

                //se tem os dois filhos, troca com a folha mais a direita da subarvore da direita
                //e remove essa folha
                char nomeAux[30];
                No *aux = no->esq;
                pai = no;

                while (aux->dir != NULL) {
                    pai = aux;
                    aux = aux->dir;
                }

                //troca a informa��o com o no mais a direita da subarvore da esquerda
                strcpy(nomeAux, no->programa);
                strcpy(no->programa, aux->programa);
                strcpy(aux->programa, nomeAux);

                strcpy(nomeAux, no->pasta);
                strcpy(no->pasta, aux->pasta);
                strcpy(aux->pasta, nomeAux);

                retiraNo(aux, pai);
            }
        }
    }
}

No* removeRec(No* raiz, char* programa, int* removeu){
    //�rvore vazia
    if(raiz == NULL){
        *removeu = 0;
        return NULL;
    }

    if(strcmp(raiz->programa, programa) == 0){
        retiraNo(raiz, NULL);
        *removeu = 1;
    }else {

        //se o programa a ser removido � o filho esquerdo ou direito, remove-o
        if (raiz->esq != NULL && strcmp(raiz->esq->programa, programa) == 0) {
            retiraNo(raiz->esq, raiz);
            *removeu = 1;
        } else if (raiz->dir != NULL && strcmp(raiz->dir->programa, programa) == 0) {
            retiraNo(raiz->dir, raiz);
            *removeu = 1;
        } else {
            //se o programa a ser removido n�o � um filho, ent�o busca na subarvore da esquerda
            //caso seja menor que a raiz, ou na direita, caso contr�rio
            if (strcmp(programa, raiz->programa) < 0)
                raiz->esq = removeRec(raiz->esq, programa, removeu);
            else {
                raiz->dir = removeRec(raiz->dir, programa, removeu);
            }
        }
    }

    return raiz;
}

void desinstalarPrograma(Arvore* arvore) {
    char programa[30];
    int removeu = 0;
    scanf("%s", &programa);

    No* aux = removeRec(arvore->raiz, programa, &removeu);

    //n�o encontrou o programa
    if(removeu){
        printf("[UNINSTALL] Programa %s.exe desinstalado com sucesso\n", programa);
    }else{
        printf("[UNINSTALL] Nao foi encontrado no sistema nenhum programa com nome %s\n", programa);
    }
}

void testarVelocidadeDeResposta() {

}

void otimizarCapacidadeDeResposta() {

}

void criarCopiasDeSeguranca() {

}

void restaurarCopiaDeSeguranca() {

}

//TODO fazer essa fun��o como no enunciado, pois aqui � apenas um in-ordem para teste
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
                desinstalarPrograma(&arvore);
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
                imprimirTodosOsProgramas(arvore.raiz); printf("\n");
                break;

        }

    }
}
