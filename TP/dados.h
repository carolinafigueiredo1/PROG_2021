//
// Created by carol on 6/4/2021.
//

#ifndef TP_DADOS_H
#define TP_DADOS_H

void mostraJogador(pJogador jogador);

void pedeNome(char *nome, int i);

pJogador iniciaJogador();

pJogada iniciaJogada(pJogada jogada);

void mostraJogada(pJogada jogada);

pJogada eliminarJogadaNova(pJogada jogada1, int nJogadas);

void cleanJogada(pJogada jog);

#endif //TP_DADOS_H
