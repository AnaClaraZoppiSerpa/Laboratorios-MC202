/*
Laboratório 07
MC202 - Estruturas de Dados
Ana Clara Zoppi Serpa - RA 165880
Gabriel Oliveira dos Santos - RA
*/

typedef struct No {
    struct No* esq;
    struct No* dir;
    char[30] pasta;
    char[30] programa;
} No;

typedef struct Arvore {
    No* raiz;
    //TODO - Campo para a préordem da cópia de segurança.
    //TODO - Campo in-ordem para a cópia de segurança.
} Arvore;

//A função retorna um ponteiro para a raiz da nova árvore.
No* reconstruir(int preordem [], int inordem[], int tamanho) {
	if (tamanho == 1) {
        /*Caso base: a pré-ordem e a in-ordem são iguais e consistem de um único nó,
        que vai ser a raiz. Então criamos esse tal nó e retornamos ele, ele é a raiz
        da nossa árvore.*/
        No* raiz = criarNo(/*passamos os parâmetros desejados para a criação do nó aqui e pronto*/);
        return raiz;
	}
    //Quando não é o caso base, precisamos considerar subárvores.
    //Dada a pré-ordem, seu primeiro nó é a raiz.
    int valorRaiz = preordem[0];
    //Então nós procuramos esse valor na inordem, porque à esquerda vai estar a
    //subárvore esquerda e à direita vai estar a subárvore direita.
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
