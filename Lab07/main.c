/*
Laborat�rio 07
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
    //TODO - Campo para a pr�ordem da c�pia de seguran�a.
    //TODO - Campo in-ordem para a c�pia de seguran�a.
} Arvore;

//A fun��o retorna um ponteiro para a raiz da nova �rvore.
No* reconstruir(int preordem [], int inordem[], int tamanho) {
	if (tamanho == 1) {
        /*Caso base: a pr�-ordem e a in-ordem s�o iguais e consistem de um �nico n�,
        que vai ser a raiz. Ent�o criamos esse tal n� e retornamos ele, ele � a raiz
        da nossa �rvore.*/
        No* raiz = criarNo(/*passamos os par�metros desejados para a cria��o do n� aqui e pronto*/);
        return raiz;
	}
    //Quando n�o � o caso base, precisamos considerar sub�rvores.
    //Dada a pr�-ordem, seu primeiro n� � a raiz.
    int valorRaiz = preordem[0];
    //Ent�o n�s procuramos esse valor na inordem, porque � esquerda vai estar a
    //sub�rvore esquerda e � direita vai estar a sub�rvore direita.
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
    //TODO - Leitura das sementes geradoras in-ordem e preordem e salv�-las como c�pia de seguran�a inicial.
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
