#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
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

//Essa pilha será usada na operação 7 (imprimir todos os programas)
typedef struct Pilha {
    No** pilha;
    int qtd;
    int limite;
} Pilha;

int empilhar(Pilha* p, No* no) {
    if(p->qtd < p->limite) {
        p->pilha[p->qtd] = no;
        p->qtd++;
        return 1;
    }
    return 0;
}

char* obterPastaParaFilho(No* pai, char* tipo) {
    if (pai != NULL) {
        char* nome = malloc(30 * sizeof(char));
        strcpy(nome, pai->programa);
        strcat(nome, tipo);
        return nome;
    } else {
        return "";
    }
}

No* desempilhar(Pilha* p) {
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

//Percurso em pós-ordem para liberar a árvore.
// Visita subárvores recursivamente, liberando cada um dos nós até que
//as chamadas recursivas "voltem" para a raiz e a liberem.
void liberarMemoriaArvore(No* raiz) {
    if (raiz) {
        liberarMemoriaArvore(raiz->esq);
        liberarMemoriaArvore(raiz->dir);
        free(raiz);
    }
}

//A função retorna um ponteiro para a raiz da árvore criada a partir das sementes.
No* reconstruir(char* preordem [], char* inordem [], int tamanho, char* nomePastaRaiz) {
    //Trata-se de uma árvore vazia, então a raiz dela será NULL.
    if (tamanho == 0) {
        return NULL;
    }
    char* valorRaiz = malloc(30 * sizeof(char));
    //Dada a pré-ordem, seu primeiro nó é a raiz.
    strcpy(valorRaiz, preordem[0]);

    No* raiz = criarNo(valorRaiz, nomePastaRaiz);
    //Trata-se de uma árvore com apenas um nó, sendo ele, portanto, a raiz.
    if (tamanho == 1) {
        return raiz;
    }
    //Caso não seja nenhum dos casos bases acima, procuramos a raiz na in-ordem, porque à esquerda vai estar a
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

    //De 0 até o índice da raiz, temos a in-ordem da subárvore esquerda. De índice da raiz + 1 até o fim, temos a in-ordem
    // da subárvore direita.
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

    //As subárvores esquerda e direita são reconstruídas recursivamente. No final, teremos a raiz da árvore inteira.
    raiz->esq = reconstruir(preordemSubarvoreEsquerda, inordemSubarvoreEsquerda, comprimentoSubarvoreEsquerda, obterPastaParaFilho(raiz, "_esq"));
    raiz->dir = reconstruir(preordemSubarvoreDireita, inordemSubarvoreDireita, comprimentoSubarvoreDireita, obterPastaParaFilho(raiz, "_dir"));
    free(valorRaiz);
    return raiz;
}

//Essa função sempre retorna a raiz para a nova árvore.
No* insereRec(No* raiz, No** novo){
    if(raiz == NULL){ //Se não há raiz, ou seja, a árvore está vazia, o novo nó será a nova raiz.
        return *novo;
    }
    //Comparamos os programas. Se o programa a ser inserido é menor que o que temos na raiz
    //ele deve ficar à esquerda.
    if(strcmp((*novo)->programa, raiz->programa) < 0){
        raiz->esq = insereRec(raiz->esq, novo);
        if(raiz->esq == *novo){
            //Adequamos o nomes da pasta caso o novo nó tenha sido colocado imediatamente à esquerda da raiz.
            strcpy(raiz->esq->pasta, raiz->programa);
            strcat(raiz->esq->pasta, "_esq");
        }
    }else{ //Nesse caso, o programa a ser inserido é maior que o que temos na raiz e deve ficar à direita.
        raiz->dir = insereRec(raiz->dir, novo);
        if(raiz->dir == *novo){
            strcpy(raiz->dir->pasta, raiz->programa);
            strcat(raiz->dir->pasta, "_dir");
        }
    }
    return raiz;
}

void instalar(Arvore *arvore) {
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
    //Se aquele nó não possui pai, significa que estamos removendo a raiz
    if(pai == NULL){
        //A raiz não tem filhos, então a árvore tem apenas um nó
        if(no->dir == NULL && no->esq == NULL){
            free(no);
            no = NULL;
        }
        else{
            //A raiz possui dois filhos
            if(no->dir != NULL && no->esq != NULL){
                //Se tem os dois filhos, troca com a folha mais à direita da subárvore esquerda
                //e remove essa folha
                char nomeAux[30];
                No* aux = no->esq;
                pai = no;

                while(aux->dir != NULL){
                    pai = aux;
                    aux = aux->dir;
                }
                //Troca a informação com o no mais a direita da subarvore da esquerda
                strcpy(nomeAux, no->programa);
                strcpy(no->programa, aux->programa);
                strcpy(aux->programa, nomeAux);
                //Atualiza o nome da pasta
                strcpy(no->esq->pasta, obterPastaParaFilho(no, "_esq"));
                strcpy(no->dir->pasta, obterPastaParaFilho(no, "_dir"));

                retiraNo(aux, pai);
            }
            else{//É a raiz e tem apenas um filho
                //tem apenas o filho esquerdo
                if(no->esq != NULL){
                    strcpy(no->esq->pasta, "raiz");
                    no = no->esq;
                }
                else{ //tem apenas o filho direito
                    strcpy(no->dir->pasta, "raiz");
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
            //é necessário também alterar o nome da pasta do neto.
            if (no->esq == NULL && no->dir != NULL) { //tem apenas o filho direito
                if (pai->dir == no) {
                    strcpy(no->dir->pasta, obterPastaParaFilho(pai, "_dir"));
                    pai->dir = no->dir;
                } else {
                    strcpy(no->dir->pasta, obterPastaParaFilho(pai, "_esq"));
                    pai->esq = no->dir;
                }
                no = NULL;
                free(no);
            } else if (no->dir == NULL && no->esq != NULL) { //tem apenas o filho esquerdo
                if (pai->dir == no) {
                    strcpy(no->esq->pasta, obterPastaParaFilho(pai, "_dir"));
                    pai->dir = no->esq;
                } else {
                    strcpy(no->esq->pasta, obterPastaParaFilho(pai, "_esq"));
                    pai->esq = no->esq;
                }

                no = NULL;
                free(no);
            } else {
                //se tem os dois filhos, troca com a folha mais a direita da subarvore da esquerda
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

void desinstalar(Arvore* arvore) {
    char programa[30];
    int removeu = 0;
    scanf("%s", &programa);

    removeRec(arvore->raiz, programa, &removeu);
    if(removeu){
        printf("[UNINSTALL] Programa %s.exe desinstalado com sucesso\n", programa);
    }else{ //não encontrou o programa
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

void testarVelocidade(Arvore arvore) {
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

int contarNos(No* raiz) {
    if (raiz == NULL)
        return 0;
    return 1 + contarNos(raiz->esq) + contarNos(raiz->dir);
}

void sementeInordem(No* raiz, char** inordem, int* indice) {
    if (raiz) {
        sementeInordem(raiz->esq, inordem, indice);
        strcpy(inordem[*indice], raiz->programa);
        *indice = *indice + 1;
        sementeInordem(raiz->dir, inordem, indice);
    }
}

//Encontramos a raiz a partir do índice no qual está a mediana e balanceamos recursivamente as subárvores.
No* balancear(No* vetorNos [], int indiceInicio, int indiceFim, char* nomePastaRaiz) {
    if (indiceInicio <= indiceFim) {
        int indiceMediana = (indiceInicio + indiceFim)/2;
        No* raiz = vetorNos[indiceMediana];
        strcpy(raiz->pasta, nomePastaRaiz);

        raiz->esq = balancear(vetorNos, indiceInicio, indiceMediana - 1, obterPastaParaFilho(raiz, "_esq"));
        raiz->dir = balancear(vetorNos, indiceMediana + 1, indiceFim, obterPastaParaFilho(raiz, "_dir"));
        return raiz;
    } else {
        return NULL;
    }
}

//O percurso in-ordem fornece a ordem crescente. Essa função faz um percurso in-ordem e guarda os próprios nós no vetor.
//É usada para o balanceamento.
void obterOrdemCrescente(No* raiz, No* vetorNos [], int* indice) {
    if (raiz != NULL) {
        obterOrdemCrescente(raiz->esq, vetorNos, indice);
        vetorNos[*indice] = raiz;
        *indice = *indice + 1;
        obterOrdemCrescente(raiz->dir, vetorNos, indice);
    }
}

void otimizar(Arvore* arvore) {
    int quantos = contarNos(arvore->raiz);
    No* ordemCrescenteDosNos [quantos];
    int indice = 0;
    obterOrdemCrescente(arvore->raiz, ordemCrescenteDosNos, &indice);
    arvore->raiz = balancear(ordemCrescenteDosNos, 0, quantos - 1, "raiz");
    printf("[OPTIMIZE] O sistema de acesso a programas foi otimizado\n");
}

void sementePreordem(No* raiz, char** preordem, int* indice) {
    if (raiz) {
        strcpy(preordem[*indice], raiz->programa);
        *indice = *indice + 1;
        sementePreordem(raiz->esq, preordem, indice);
        sementePreordem(raiz->dir, preordem, indice);
    }
}

void criarCopias(Arvore* arvore) {
    int quantos = contarNos(arvore->raiz);
    arvore->inordemCopiaDeSeguranca = malloc(quantos * sizeof(char*));
    arvore->preordemCopiaDeSeguranca = malloc(quantos * sizeof(char*));
    for (int i = 0; i < quantos; i++) {
        arvore->inordemCopiaDeSeguranca[i] = malloc(30 * sizeof(char));
        arvore->preordemCopiaDeSeguranca[i] = malloc(30 * sizeof(char));
    }

    int indice = 0;
    sementePreordem(arvore->raiz, arvore->preordemCopiaDeSeguranca, &indice);
    indice = 0;
    sementeInordem(arvore->raiz, arvore->inordemCopiaDeSeguranca, &indice);

    arvore->quantosNosCopiaDeSeguranca = quantos;

    printf("[BACKUP] Configuracao atual do sistema salva com sucesso\n");
}

//Para restaurar uma cópia de segurança, basta construir a árvore a partir das sementes de segurança.
void restaurar(Arvore* arvore) {
    No* novaRaiz = reconstruir(arvore->preordemCopiaDeSeguranca, arvore->inordemCopiaDeSeguranca, arvore->quantosNosCopiaDeSeguranca, "raiz");
    arvore->raiz = novaRaiz;

    printf("[RESTORE] Sistema restaurado para a versao do backup\n");
}


//Utilizamos uma pilha para guardar o caminho inteiro desde a raiz até o elemento proriamente dito
// se desce um nível então guarda o elemento atual na pilha, pois será pai do próximo elemento da recursão
//se volta da recursão, então tira da pilha, pois o topo é o elemento atual
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
        inordem[i] = malloc(30 * sizeof(char));
        strcpy(inordem[i], nomePrograma);
    }

    for (int i = 0; i < quantidadePresenteAoIniciar; i++) {
        scanf("%s", &nomePrograma);
        preordem[i] = malloc(30 * sizeof(char));
        strcpy(preordem[i], nomePrograma);
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
                instalar(&arvore);
                break;

            case 2:
                desinstalar(&arvore);
                break;

            case 3:
                testarVelocidade(arvore);
                break;

            case 4:
                otimizar(&arvore);
                break;

            case 5:
                criarCopias(&arvore);
                break;

            case 6:
                restaurar(&arvore);
                break;

            case 7:
                p.qtd = 0;
                p.limite = contarNos(arvore.raiz);
                p.pilha = malloc(p.limite*sizeof(No*));

                printf("[PATHS]\n");
                imprimirTodosOsProgramas(arvore.raiz, &p);

                free(p.pilha);
                break;
        }
    }
    liberarMemoriaArvore(arvore.raiz);
    for (int i = 0; i < arvore.quantosNosCopiaDeSeguranca; i++) {
        free(arvore.preordemCopiaDeSeguranca[i]);
        free(arvore.inordemCopiaDeSeguranca[i]);
    }
    //free(arvore.preordemCopiaDeSeguranca);
    //free(arvore.inordemCopiaDeSeguranca);
}
