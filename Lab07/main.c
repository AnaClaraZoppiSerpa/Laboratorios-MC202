/*
Laboratório 07
MC202 - Estruturas de Dados
Ana Clara Zoppi Serpa - RA 165880
Gabriel Oliveira dos Santos - RA
*/

typedef struct No {
    struct No* esq;
    struct No* dir;
    int valor;
    //char[30] pasta;
    //char[30] programa;
} No;

typedef struct Arvore {
    No* raiz;
    //TODO - Campo para a préordem da cópia de segurança.
    //TODO - Campo in-ordem para a cópia de segurança.
} Arvore;

No* criarNo(int valor) {
    No* no = malloc(sizeof(No));
    no->valor = valor;
    no->esq = NULL;
    no->dir = NULL;
    return no;
}

//A função retorna um ponteiro para a raiz da nova árvore.
No* reconstruir(int preordem [], int inicioPreordem, int fimPreordem, int inordem[], int inicioInordem, int fimInordem) {
    //Dada a pré-ordem, seu primeiro nó é a raiz.
    int valorRaiz = preordem[inicioPreordem];
    No* raiz = criarNo(valorRaiz);
    if (inicioPreordem == fimPreordem && inicioInordem == fimInordem) {
        return raiz;
    }
    //Então nós procuramos esse valor na in-ordem, porque à esquerda vai estar a
    //subárvore esquerda e à direita vai estar a subárvore direita.
    int inicioInordemSubarvoreEsquerda;
    int fimInordemSubarvoreEsquerda;
    int inicioInordemSubarvoreDireita;
    int fimInordemSubarvoreDireita;
    for (int i = inicioInordem; i <= fimInordem; i++) {
        if (inordem[i] == valorRaiz) {
            //Das posições inicioInordem até i - 1 temos a subárvore esquerda.
            inicioInordemSubarvoreEsquerda = inicioInordem;
            fimInordemSubarvoreEsquerda = i - 1;
            //Das posições i + 1 até fimInordem temos a subárvore direita.
            inicioInordemSubarvoreDireita = i + 1;
            fimInordemSubarvoreDireita = fimInordem;
        }
    }
    int comprimentoSubarvoreEsquerda = fimInordemSubarvoreEsquerda - inicioInordemSubarvoreEsquerda + 1;
    int comprimentoSubarvoreDireita = fimInordemSubarvoreDireita - inicioInordemSubarvoreDireita + 1;
    //Agora, sabendo os comprimentos, vamos determinar os índices de início e fim das pré-ordens.
    int inicioPreordemSubarvoreEsquerda = 1;
    int fimPreordemSubarvoreEsquerda = comprimentoSubarvoreEsquerda;
    int inicioPreordemSubarvoreDireita = fimPreordemSubarvoreEsquerda + 1;
    int fimPreordemSubarvoreDireita = fimPreordem;
    raiz->esq = reconstruir(preordem, inicioPreordemSubarvoreEsquerda, fimPreordemSubarvoreEsquerda, inordem, inicioInordemSubarvoreEsquerda, fimInordemSubarvoreEsquerda);
    raiz->dir = reconstruir(preordem, inicioPreordemSubarvoreDireita, fimPreordemSubarvoreDireita, inordem, inicioInordemSubarvoreDireita, fimInordemSubarvoreDireita);
}

void instalarNovoPrograma() {

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

void imprimirTodosOsProgramas() {

}

int main() {
    int quantidadePresenteAoIniciar;
    scanf("%d", &quantidadePresenteAoIniciar);
    //TODO - Leitura das sementes geradoras in-ordem e preordem e salvá-las como cópia de segurança inicial.
    int operacao;
    while (scanf("%d", &operacao) != EOF) {
        switch (operacao) {
            case 1: {
                instalarNovoPrograma();
                break;
            }
            case 2: {
                desinstalarPrograma();
                break;
            }
            case 3: {
                testarVelocidadeDeResposta();
                break;
            }
            case 4: {
                otimizarCapacidadeDeResposta();
                break;
            }
            case 5: {
                criarCopiasDeSeguranca();
                break;
            }
            case 6: {
                restaurarCopiaDeSeguranca();
                break;
            }
            case 7: {
                imprimirTodosOsProgramas();
                break;
            }
        }
    }
}
