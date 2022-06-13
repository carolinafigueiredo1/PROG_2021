//
// Created by carol on 6/4/2021.
//

#ifndef TP_MAIN_H
#define TP_MAIN_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAXINICIAL 5
#define MININICIAL 3
#define FichJogoInacabado "jogo.bin"


typedef struct celula celula, *pCelula;

struct celula{
    int lin, col;
    char peca;
};

typedef struct jogador jogador, *pJogador;

struct jogador{
    char nome[100];
    int iter, tipo, pedra, numero, nMovAdicionais;
};

typedef struct tabuleiro tabuleiro, *pTabuleiro;

struct tabuleiro{
    int lin,col;
    pCelula *celulas;
    pTabuleiro prox;
};

typedef struct jogada jogada, *pJogada;

struct jogada{
    pJogador jogadorAtual;
    pTabuleiro tabuleiro;
    char *movimento;
    int movimentolen;
    int turno;
    pJogada prox;
};

typedef struct jogo jogo, *pJogo;

struct jogo{
    pJogador jogadores;
    pJogada jogadas;
    pTabuleiro tabuleiro;
    int nJogadas;
    int tipo; //humano vs humano - 1; humano vs PC - 2;
};

int scanfInteiro();

#endif //TP_MAIN_H
