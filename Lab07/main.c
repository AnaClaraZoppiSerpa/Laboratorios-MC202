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
    char** preordemCopiaDeSeguranca;
    char** inordemCopiaDeSeguranca;
    int quantosNosCopiaDeSeguranca;
} Arvore;

typedef struct Pilha {
    No** pilha;
    int qtd;
    int limite;
} Pilha;


int empilhar(Pilha* p, No* no){
    if(p->qtd < p->limite){
        p->pilha[p->qtd] = no;
        p->qtd++;

        return 1;
    }

    return 0;
}

No* desempilhar(Pilha* p){
    if(p->qtd == 0)
        return NULL;

    p->qtd--;
    return p->pilha[p->qtd];
}

No* criarNo(char* nomePrograma, char* nomePasta) {
    No* no = malloc(sizeof(No));
    no->esq = NULL;
    no->dir = NULL;
    strcpy(no->programa, nomePrograma);
    strcpy(no->pasta, nomePasta);
    return no;
}

//A função retorna um ponteiro para a raiz da nova árvore.
No* reconstruir(char* preordem [], char* inordem [], int tamanho, char* nomePastaRaiz) {
    //Dada a pré-ordem, seu primeiro nó é a raiz.
    if (tamanho == 0) {
        return NULL;
    }

    char* valorRaiz = malloc(30 * sizeof(char));
    strcpy(valorRaiz, preordem[0]);

    No* raiz = criarNo(valorRaiz, nomePastaRaiz);
    if (tamanho == 1) {
        return raiz;
    }

    //Então nós procuramos esse valor na in-ordem, porque à esquerda vai estar a
    //subárvore esquerda e à direita vai estar a subárvore direita.
    int comprimentoSubarvoreEsquerda = 0, comprimentoSubarvoreDireita = 0;
    int indiceDaRaizNaInordem = 0;
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(inordem[i], valorRaiz) == 0) {
            indiceDaRaizNaInordem = i;
            //Das posições x tais que 0 <= x < i temos a inordem da subárvore esquerda.
            comprimentoSubarvoreEsquerda = i;
            //Das posições y tais que i + 1 <= y < tamanho temos a inordem da subárvore direita.
            comprimentoSubarvoreDireita = tamanho - i - 1;
            break;
        }
    }

    char* inordemSubarvoreEsquerda [comprimentoSubarvoreEsquerda];
    char* inordemSubarvoreDireita [comprimentoSubarvoreDireita];
    char* preordemSubarvoreDireita [comprimentoSubarvoreDireita];
    char* preordemSubarvoreEsquerda [comprimentoSubarvoreEsquerda];

    //A pré-ordem da subárvore esquerda é obtida percorrendo, a partir da posição 1 e até completar as unidades do comprimento, a pré-ordem fornecida por parâmetro.
    //A pré-ordem da subárvore direita começa logo depois, percorrendo, também, até completar as unidades do comprimento.
    int indicePreordem = 1;
    int indiceParaNovoVetor = 0;
    int contadorComprimento = 0;

    while (contadorComprimento < comprimentoSubarvoreEsquerda) {
        preordemSubarvoreEsquerda[indiceParaNovoVetor] = malloc(30 * sizeof(char));
        strcpy(preordemSubarvoreEsquerda[indiceParaNovoVetor], preordem[indicePreordem]);
        contadorComprimento++;
        indicePreordem++;
        indiceParaNovoVetor++;
    }

    contadorComprimento = 0;
    indiceParaNovoVetor = 0;

    while (contadorComprimento < comprimentoSubarvoreDireita) {
        preordemSubarvoreDireita[indiceParaNovoVetor] = malloc(30 * sizeof(char));
        strcpy(preordemSubarvoreDireita[indiceParaNovoVetor], preordem[indicePreordem]);
        contadorComprimento++;
        indicePreordem++;
        indiceParaNovoVetor++;
    }

    //De 0 até o índice da raiz, temos a in-ordem da subárvore esquerda. De índice da raiz + 1 até o fim, temos a in-ordem da subárvore direita.
    for (int i = 0; i < indiceDaRaizNaInordem; i++) {
        inordemSubarvoreEsquerda[i] = malloc(30 * sizeof(char));
        strcpy(inordemSubarvoreEsquerda[i], inordem[i]);
    }
    indiceParaNovoVetor = 0;
    for (int i = indiceDaRaizNaInordem + 1; i < tamanho; i++) {
        inordemSubarvoreDireita[indiceParaNovoVetor] = malloc(30 * sizeof(char));
        strcpy(inordemSubarvoreDireita[indiceParaNovoVetor], inordem[i]);
        indiceParaNovoVetor++;
    }

    char* pastaEsq = malloc(30 * sizeof(char));
    strcpy(pastaEsq, valorRaiz);
    strcat(pastaEsq, "_esq");

    char* pastaDir = malloc(30 * sizeof(char));
    strcpy(pastaDir, valorRaiz);
    strcat(pastaDir, "_dir");
    raiz->esq = reconstruir(preordemSubarvoreEsquerda, inordemSubarvoreEsquerda, comprimentoSubarvoreEsquerda, pastaEsq);
    raiz->dir = reconstruir(preordemSubarvoreDireita, inordemSubarvoreDireita, comprimentoSubarvoreDireita, pastaDir);
    return raiz;
}

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

        //só tem a raiz
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

                //troca a informação com o no mais a direita da subarvore da esquerda
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
        //é uma folha, simplesmente remove e faz o pai apontar para nulo
        if (no->esq == NULL && no->dir == NULL) {
            if (pai->dir == no) {
                pai->dir = NULL;
            } else {
                pai->esq = NULL;
            }

            no = NULL;
            free(no);
        } else {
            //se tem apenas um filho, faça pai apontar para o neto
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

                //troca a informação com o no mais a direita da subarvore da esquerda
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
    //árvore vazia
    if(raiz == NULL){
        *removeu = 0;
        return NULL;
    }

    if(strcmp(raiz->programa, programa) == 0){
        retiraNo(raiz, NULL);
        *removeu = 1;
    }else {

        //se o programa a ser removido é o filho esquerdo ou direito, remove-o
        if (raiz->esq != NULL && strcmp(raiz->esq->programa, programa) == 0) {
            retiraNo(raiz->esq, raiz);
            *removeu = 1;
        } else if (raiz->dir != NULL && strcmp(raiz->dir->programa, programa) == 0) {
            retiraNo(raiz->dir, raiz);
            *removeu = 1;
        } else {
            //se o programa a ser removido não é um filho, então busca na subarvore da esquerda
            //caso seja menor que a raiz, ou na direita, caso contrário
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

    //não encontrou o programa
    if(removeu){
        printf("[UNINSTALL] Programa %s.exe desinstalado com sucesso\n", programa);
    }else{
        printf("[UNINSTALL] Nao foi encontrado no sistema nenhum programa com nome %s\n", programa);
    }
}

int calculaAltura(No* raiz, char* programa){
    if(raiz == NULL || strcmp(raiz->programa, programa) == 0)
        return 0;

    if(strcmp(programa, raiz->programa) < 0)
        return 1+calculaAltura(raiz->esq, programa);

    return 1+calculaAltura(raiz->dir, programa);
}

void testarVelocidadeDeResposta(Arvore arvore) {
    int tempoObtido, tempoEsperado;
    char programa[30];

    scanf("%s", &programa);
    scanf("%d", &tempoEsperado);

    tempoObtido = calculaAltura(arvore.raiz, programa);

    if(tempoObtido > tempoEsperado){
        printf("[DELAY][FAIL] O acesso ao programa %s.exe ultrapassou o limite de %d segundo\n", programa, tempoEsperado);
    }else{
        printf("[DELAY][OK] O acesso ao programa %s.exe foi concluido em %d segundos\n", programa, tempoObtido);
    }
}

void otimizarCapacidadeDeResposta() {

}

int contarNos(No* raiz) {
    if (raiz == NULL)
        return 0;
    return 1 + contarNos(raiz->esq) + contarNos(raiz->dir);
}

void sementePreordem(No* raiz, char** preordem, int* indice) {
    if (raiz) {
        strcpy(preordem[*indice], raiz->programa);
        *indice = *indice + 1;
        sementePreordem(raiz->esq, preordem, indice);
        sementePreordem(raiz->dir, preordem, indice);
    }
}

void sementeInordem(No* raiz, char** inordem, int* indice) {
    if (raiz) {
        sementeInordem(raiz->esq, inordem, indice);
        strcpy(inordem[*indice], raiz->programa);
        *indice = *indice + 1;
        sementePreordem(raiz->dir, inordem, indice);
    }
}

void criarCopiasDeSeguranca(Arvore* arvore) {
    int quantos = contarNos(arvore->raiz);
    char* inordem [quantos];
    char* preordem [quantos];

    for (int i = 0; i < quantos; i++) {
        inordem[i] = malloc(30 * sizeof(char));
        preordem[i] = malloc(30 * sizeof(char));
    }

    int indice = 0;
    sementePreordem(arvore->raiz, preordem, &indice);
    indice = 0;
    sementeInordem(arvore->raiz, inordem, &indice);

    arvore->preordemCopiaDeSeguranca = preordem;
    arvore->inordemCopiaDeSeguranca = inordem;
    arvore->quantosNosCopiaDeSeguranca = quantos;

    printf("[BACKUP] Configuracao atual do sistema salva com sucesso\n");
}

//apenas pra teste, pode apagar dps
void pre(No* arv) {
    if (arv) {
        printf("Programa %s Pasta %s, ", arv->programa, arv->pasta);
        pre(arv->esq);
        pre(arv->dir);
    }
}

void restaurarCopiaDeSeguranca(Arvore* arvore) {
    No* novaRaiz = reconstruir(arvore->preordemCopiaDeSeguranca, arvore->inordemCopiaDeSeguranca, arvore->quantosNosCopiaDeSeguranca, "raiz");
    arvore->raiz = novaRaiz;
    printf("[RESTORE] Sistema restaurado para a versao do backup\n");
}


//nesse método utilizamos uma pilha para guardar todo o caminhp
//desde a raiz até o elemento proriamente dito

// se desce um nível então guarda o elemento atual na pilha,
// pois será pai do próximo elemento da recursão

//se volta da recursão, então tira da pilha, pois o topo
// é o elemento atual
void imprimirTodosOsProgramas(No* raiz, Pilha* p) {
    if(raiz != NULL){
        if(raiz->esq != NULL){
            empilhar(p, raiz);
            imprimirTodosOsProgramas(raiz->esq, p);
        }

        int i;
        printf("C:/");
        for(i = 0; i <  p->qtd; i++){
           printf("%s/", p->pilha[i]->pasta);
        }

        printf("%s/%s.exe\n", raiz->pasta, raiz->programa);

        if(raiz->dir != NULL){
            empilhar(p, raiz);
            imprimirTodosOsProgramas(raiz->dir, p);
        }

        //sobe um nível
        desempilhar(p);
    }
}

int main() {
    int quantidadePresenteAoIniciar;
    scanf("%d", &quantidadePresenteAoIniciar);

    //Leitura das sementes geradoras in-ordem e preordem.
    char* preordem [quantidadePresenteAoIniciar];
    char* inordem [quantidadePresenteAoIniciar];

    char nomePrograma [30];
    for (int i = 0; i < quantidadePresenteAoIniciar; i++) {
        scanf("%s", &nomePrograma);
        preordem[i] = malloc(30 * sizeof(char));
        strcpy(preordem[i], nomePrograma);
    }

    for (int i = 0; i < quantidadePresenteAoIniciar; i++) {
        scanf("%s", &nomePrograma);
        inordem[i] = malloc(30 * sizeof(char));
        strcpy(inordem[i], nomePrograma);
    }

    int operacao;

    //Reconstrução da árvore binária a partir dos percursos dados.
    No* raiz = reconstruir(preordem, inordem, quantidadePresenteAoIniciar, "raiz");

    //Armazenamento dos percursos dados como cópia de segurança atual.
    Arvore arvore;
    arvore.raiz = raiz;
    arvore.inordemCopiaDeSeguranca = inordem;
    arvore.preordemCopiaDeSeguranca = preordem;
    arvore.quantosNosCopiaDeSeguranca = quantidadePresenteAoIniciar;

    Pilha p;

    while (scanf("%d", &operacao) != EOF) {
        switch (operacao) {
            case 1:
                instalarNovoPrograma(&arvore);
                break;

            case 2:
                desinstalarPrograma(&arvore);
                break;

            case 3:
                testarVelocidadeDeResposta(arvore);
                break;

            case 4:
                otimizarCapacidadeDeResposta();
                break;

            case 5:
                criarCopiasDeSeguranca(&arvore);
                break;

            case 6:
                restaurarCopiaDeSeguranca(&arvore);
                break;

            case 7:
                p.qtd = 0;
                p.limite = contarNos(arvore.raiz);
                p.pilha = malloc(p.limite*sizeof(No*));

                imprimirTodosOsProgramas(arvore.raiz, &p); printf("\n");
                break;
        }
    }
}
