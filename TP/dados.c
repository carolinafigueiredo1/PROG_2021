//
// Created by carol on 6/4/2021.
//

#include "main.h"
#include "dados.h"


pJogador iniciaJogador(){
    pJogador novo = malloc(sizeof(jogador));
    if(novo == NULL){
        puts("Erro na alocacao de memoria\n");
        return NULL;
    }

    novo->iter = 0;
    novo->tipo = 0;
    strcpy(novo->nome, "");
    novo->pedra = 0;
    novo->numero = 0;
    novo->nMovAdicionais = 0;

    return novo;

}

void pedeNome(char *nome, int i) {
    printf("\nEscolha o nome:\n");
    printf("Jogador %d: ", i + 1);
    scanf("%s", nome);
}

pJogada iniciaJogada(pJogada j){
    pJogada novo = malloc(sizeof(jogada)), aux;

    if(novo == NULL){
        puts("Erro na alocacao de memoria\n");
        return j;
    }

    novo->jogadorAtual = iniciaJogador();
    novo->prox = NULL;
    novo->tabuleiro = NULL;


    if(j != NULL) {
        aux = j;
        while (aux->prox != NULL)
            aux = aux->prox;
        novo->jogadorAtual = aux->jogadorAtual;
        aux->prox = novo;
    }
    else
        j = novo;

    return j;
}

pJogada eliminarJogadaNova(pJogada jogada1, int nJogadas){
    if (jogada1 == NULL)
        return NULL;

    if (jogada1->prox == NULL) {
        free(jogada1);
        return NULL;
    }

    pJogada penultimo = jogada1;
    for(int i = 0; i < nJogadas; i++) {
        penultimo = penultimo->prox;
    }

    free(penultimo->prox);
    penultimo->prox = NULL;
    penultimo->tabuleiro->prox = NULL;

    return jogada1;
}

void mostraJogada(pJogada jogada){
    printf( "\n\tTurno %d\n"
            "\tJogador %d: %s %s\n"
            ,jogada->turno, jogada->jogadorAtual->numero + 1, jogada->jogadorAtual->nome, jogada->movimento);
}

void mostraJogador(pJogador jogador){
    if(jogador->tipo == 1){
        printf("\tJogador Humano %d: ", jogador->numero + 1);
    }
    else
        printf("\tComputador: ");
    printf( "%s\n"
            "\tNumero de iteracoes: %d\n"
            "\tNumero de pedras jogadas: %d\n"
            "\tNumero de movimentos adicionais: %d\n"
    , jogador->nome, jogador->iter, jogador->pedra, jogador->nMovAdicionais);
}

void cleanJogada(pJogada jog){
    pJogada aux = jog;
    while(aux->prox == NULL){
        jog = aux->prox;
        free(aux);
        aux = jog;
    }
}
