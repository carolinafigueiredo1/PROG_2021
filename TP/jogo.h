//
// Created by carol on 6/4/2021.
//

#ifndef TP_JOGO_H
#define TP_JOGO_H

void menuInicial();

void menuPrincipal();

pJogo inicializaJogo();

pJogo iniciaJogo();

pJogo escolheMovimento(pJogo jogo, int *sair, int *tabCheio);

void verDados(pJogo jogo);

pJogada jogarPeca(pJogada jogada, int tipo);

pJogada acrescentaLin(pJogada jogada);

pJogada acrescentaCol(pJogada jogada);

pJogada jogaPedra(pJogada jogada, int tipo);

pJogador adicionaHumano(int i);

pJogador adicionaComputador();

pJogada proximaJogada(pJogada pJogada, pJogo jogo);

int jogoTermina(pJogo jogo);

void mostraUltTabuleiro(pJogo jogo);

void mostraXTabuleiro(pJogo jogo, int kx);

void mostraJogo(pJogo pJogo);

void mostraJogadas(pJogo jogo);

void mostraStatusFinal(pJogo jogo);

void mostraEmpate(pJogo jogo);

void cleanJogo(pJogo jogo);





#endif //TP_JOGO_H
