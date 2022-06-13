//
// Created by carol on 6/4/2021.
//

#ifndef TP_TABULEIRO_H
#define TP_TABULEIRO_H

pTabuleiro inicializaTabuleiro();

void mostraTabuleiro(pTabuleiro tabuleiro);

int verificaDiagonal(pTabuleiro tabuleiro);

int verificaLinha(pTabuleiro tabuleiro);

int verificaColuna(pTabuleiro tabuleiro);

pTabuleiro acrescentaColuna(pTabuleiro tabuleiro);

pTabuleiro acrescentaLinha(pTabuleiro tabuleiro);

pTabuleiro acrescentarTabuleiro(pTabuleiro tab);

pTabuleiro copiaCelulas(pTabuleiro tabuleiro, pTabuleiro aux);

pTabuleiro dadosTabuleiro(pTabuleiro novo, pTabuleiro ultimo);

int verificaMov(pTabuleiro tabuleiro, int lin, int col, pCelula celula);

int tabuleiroCheio(pTabuleiro tab);

void cleanTabuleiro(pTabuleiro tab);

#endif //TP_TABULEIRO_H
