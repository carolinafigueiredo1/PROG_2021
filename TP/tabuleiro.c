//
// Created by carol on 6/4/2021.
//
#include "main.h"
#include "tabuleiro.h"
#include "utils.h"


pTabuleiro inicializaTabuleiro(){
    pTabuleiro pt;

    pt = malloc(sizeof(tabuleiro));

    if(pt == NULL){
        puts("Erro na alocacao de memoria");
        return NULL;
    }

    initRandom();

    pt->lin = pt->col = intUniformRnd(MININICIAL,MAXINICIAL);

    celula **c = malloc(sizeof(celula*) * pt->lin);
    if(c == NULL){
        puts("Erro na alocacao de memoria");
        return NULL;
    }

    for(int i = 0; i < pt->lin; i++) {
        c[i] = malloc( sizeof(celula) * pt->col);
        if(c[i] == NULL){
            puts("Erro na alocacao de memoria");
            return NULL;
        }
    }

    pt->celulas = c;

    for (int i = 0; i < pt->lin; i++) {
        for (int j = 0; j < pt->col; j++) {
            pt->celulas[i][j].lin = i;
            pt->celulas[i][j].col = j;
            pt->celulas[i][j].peca = '_';

        }
    }

    pt->prox = NULL;

    return pt;
}

int verificaDiagonal(pTabuleiro tabuleiro){
    int i = 0;
    int j = 0;
    char peca = 0;
    int x = 0;
    if(tabuleiro->lin != tabuleiro->col)
        return 0;

    //Diagonal esquerda(cima) -> direita(baixo) meio para baixo (inclusive)
    while(i < tabuleiro->lin) {
        if (tabuleiro->celulas[i][i].peca == '_' || tabuleiro->celulas[i][i].peca == 'o' || (tabuleiro->celulas[i][i].peca != peca && peca != 0) ) {
            break;
        } else if (peca == 0) {
            peca = tabuleiro->celulas[i][i].peca;
            i++; x++;
        } else if (tabuleiro->celulas[i][i].peca == peca) {
            i++;
            x++;
        }

            if (x == tabuleiro->lin)
            return 1;
    }

    i = tabuleiro->lin - 1;
    j = 0;
    peca = 0;
    x = 0;

    //Diagonal esquerda(baixo) -> direita(cima) meio para cima(inclusive)
    while(i >= 0) {
        if (tabuleiro->celulas[i][j].peca == '_' || tabuleiro->celulas[i][j].peca == 'o' || (tabuleiro->celulas[i][j].peca != peca && peca != 0)) {
            break;
        }
        else if (peca == 0) {
            peca = tabuleiro->celulas[i][j].peca;
            i--; j++; x++;
        } else if (tabuleiro->celulas[i][j].peca == peca) {
            i--;j++;x++;
        }
        if (x == tabuleiro->lin)
            return 1;

    }

    return 0;
}

int verificaLinha(pTabuleiro tabuleiro){
    int i = 0, j;
    char peca = 0;
    int x = 0;

   do{
        for(j = 0; j < tabuleiro->col; j++){
            if(tabuleiro->celulas[i][j].peca == '_' || tabuleiro->celulas[i][j].peca == 'o') {
                x = 0;
                peca = 0;
                i++;
                break;
            } else if(peca != 0  && tabuleiro->celulas[i][j].peca != peca) {
                x = 0;
                peca = 0;
                i++;
                break;
            }
            else if(peca == 0) {
                peca = tabuleiro->celulas[i][j].peca;
                x++;
            }
            else if(peca == tabuleiro->celulas[i][j].peca) {
                x++;
            }

            if(x == tabuleiro->col)
                return 1;
        }
    } while(i < tabuleiro->lin);
    return 0;
}

int verificaColuna(pTabuleiro tabuleiro){
    int i = 0, j;
    char peca = 0;
    int x = 0;

    while(i < tabuleiro->col){
        for(j = 0; j < tabuleiro->lin; j++){
            if(tabuleiro->celulas[j][i].peca == '_' || tabuleiro->celulas[j][i].peca == 'o' || (peca != 0  && tabuleiro->celulas[j][i].peca != peca)) {
                x = 0;
                peca = 0;
                i++;
                break;
            } else if(peca == 0) {
                peca = tabuleiro->celulas[j][i].peca;
                x++;
            }
            else if(peca == tabuleiro->celulas[j][i].peca) {
                x++;
            }
            if(x == tabuleiro->lin)
                return 1;
        }
    }
    return 0;
}

pTabuleiro acrescentarTabuleiro(pTabuleiro tab) {
    pTabuleiro novo, aux;

    novo = malloc(sizeof(tabuleiro));
    if (novo == NULL) {
        puts("Erro na alocacao de memória\n");
        return tab;
    }

    novo = dadosTabuleiro(novo, tab);

    tab->prox = novo;

    return novo;
}

pTabuleiro copiaCelulas(pTabuleiro tabuleiro, pTabuleiro aux){
    for (int i = 0; i < tabuleiro->lin; i++) {
        for (int j = 0; j < tabuleiro->col; j++) {
            tabuleiro->celulas[i][j].peca = aux->celulas[i][j].peca;
            tabuleiro->celulas[i][j].col = aux->celulas[i][j].col;
            tabuleiro->celulas[i][j].lin = aux->celulas[i][j].lin;
        }
    }
    return tabuleiro;
}

pTabuleiro acrescentaColuna(pTabuleiro tabuleiro){
    celula **c = tabuleiro->celulas;
    for(int i = 0; i < tabuleiro->lin; i++) {
        c[i] = realloc(c[i], sizeof(celula) * tabuleiro->col);
        if(c[i] == NULL){
            puts("Erro na alocacao de memoria");
            return tabuleiro;
        }
    }

    int i = tabuleiro->col - 1;
    for(int j = 0; j < tabuleiro->lin; j++){
        c[j][i].col = i;
        c[j][i].lin = j;
        c[j][i].peca = '_';
    }
    tabuleiro->celulas = c;

    return tabuleiro;
}

pTabuleiro acrescentaLinha(pTabuleiro tabuleiro){
    celula **c = realloc(tabuleiro->celulas,sizeof(celula*) * tabuleiro->lin);
    if(c == NULL){
        puts("Erro na alocacao de memoria");
        return tabuleiro;
    }

    tabuleiro->celulas = c;

    int i = tabuleiro->lin - 1;

    c[i] = malloc(sizeof(celula) * tabuleiro->col);

    if(c[i] == NULL) {
        puts("Erro na alocacao de memoria");

        return tabuleiro;
    }

    for(int j = 0; j < tabuleiro->col; j++){
        c[i][j].lin = i;
        c[i][j].col = j;
        c[i][j].peca = '_';
    }
    return tabuleiro;
}

pTabuleiro dadosTabuleiro(pTabuleiro novo, pTabuleiro ultimo){
    novo->lin = ultimo->lin;
    novo->col = ultimo->col;


    celula **c = malloc(sizeof(celula*) * novo->lin);
    if(c == NULL){
        puts("Erro na alocacao de memoria");
        return ultimo;
    }

    for(int i = 0; i < novo->lin; i++) {
        c[i] = malloc(sizeof(celula) * novo->col);
        if(c[i] == NULL){
            puts("Erro na alocacao de memoria");
            return ultimo;
        }
    }


    novo->celulas = c;
    novo = copiaCelulas(novo, ultimo);


    novo->prox = NULL;
    return novo;
}

pTabuleiro getUltimoTabuleiro(pTabuleiro tabuleiro) {
    pTabuleiro aux;

    aux = tabuleiro;
    while (aux->prox != NULL)
        aux = aux->prox;
    return aux;
}

int verificaMov(pTabuleiro tabuleiro, int lin, int col, pCelula celula){
    if(lin < 0 || lin > tabuleiro->lin - 1 || col < 0 || col > tabuleiro->col - 1) {
        printf("[Jogada Invalida] Fora dos limites!");
        return 0;
    }
    if (tabuleiro->celulas[lin][col].peca != 'R' && tabuleiro->celulas[lin][col].peca != 'o') {
        *celula = tabuleiro->celulas[lin][col];
        return 1;
    }
    printf("[Jogada Invalida] Pedra ou 'R' na celula!");
    return 0;

}

void mostraTabuleiro(pTabuleiro tabuleiro) {
    printf("\t   %2d ", 1);
    for(int i = 1; i < tabuleiro->col; i++)
        printf("%2d ", i + 1);
    printf("\n");
    for (int i = 0; i < tabuleiro->lin; i++) {
        printf("\t%2d ", i + 1);
        for (int j = 0; j < tabuleiro->col; j++) {
            printf("%2c ", tabuleiro->celulas[i][j].peca);

        }
        printf("\n");
    }
}

int tabuleiroCheio(pTabuleiro tab){
    for(int i = 0; i < tab->lin; i++)
        for(int j = 0; j < tab->col; j++)
            if(tab->celulas[i][j].peca != 'R' && tab->celulas[i][j].peca != 'o')
                return 0;
    return 1;
}

void cleanTabuleiro(pTabuleiro tab){
    pTabuleiro aux = tab;
    while(aux->prox != NULL){
        tab = aux->prox;
        free(aux->celulas);
        aux = tab;
    }
}

/*int verificaDiagonal(pTabuleiro tabuleiro){
    int n = 0;
    int i = 0;
    int j = 0;
    char peca = 0;
    int x = 0;
    if(tabuleiro->lin != tabuleiro->col)
        return 0;

    //Diagonal esquerda(cima) -> direita(baixo) meio para baixo (inclusive)
    while(n < tabuleiro->lin) {
        if (tabuleiro->celulas[i][j].peca == '_' || tabuleiro->celulas[i][j].peca == 'o' || i == tabuleiro->lin ||j == tabuleiro->col) {
            i = ++n; j = 0; x = 0; peca = 0;
        } else if (peca == 0) {
            peca = tabuleiro->celulas[i][j].peca;
            i++; j++; x++;
        } else if (tabuleiro->celulas[i][j].peca == peca) {
            i++; j++; x++;
        } else if (x == tabuleiro->lin - n)
            return 1;
        else if(tabuleiro->celulas[i][j].peca != peca){
            i = ++n; j = 0; x = 0; peca = 0;
        }
    }

    n = 0;
    i = 0;
    j = 1;
    peca = 0;
    x = 0;

    //Diagonal esquerda(cima) -> direita(baixo) meio para cima
    while(n < tabuleiro->lin - 1) {
        if (tabuleiro->celulas[i][j].peca == '_' || tabuleiro->celulas[i][j].peca == 'o' || i == tabuleiro->lin ||j == tabuleiro->col) {
            i = 0; j = ++n; x = 0; peca = 0;
        } else if (peca == 0) {
            peca = tabuleiro->celulas[i][j].peca;
            i++; j++; x++;
        } else if (tabuleiro->celulas[i][j].peca == peca) {
            i++; j++; x++;
        } else if (x == tabuleiro->lin - n)
            return 1;
        else if(tabuleiro->celulas[i][j].peca != peca){
            i = 0; j = ++n; x = 0; peca = 0;
        }
    }

    n = 0;
    i = tabuleiro->lin - 1;
    j = 0;
    peca = 0;
    x = 0;

    //Diagonal esquerda(baixo) -> direita(cima) meio para cima(inclusive)
    while(n < tabuleiro->lin) {
        if (tabuleiro->celulas[i][j].peca == '_' || tabuleiro->celulas[i][j].peca == 'o' || i == -1 ||j == tabuleiro->col) {
            i = tabuleiro->lin - (++n); j = 0; x = 0; peca = 0;
        } else if (peca == 0) {
            peca = tabuleiro->celulas[i][j].peca;
            i--; j++; x++;
        } else if (tabuleiro->celulas[i][j].peca == peca) {
            i--; j++; x++;
        } else if (x == tabuleiro->lin - n)
            return 1;
        else if(tabuleiro->celulas[i][j].peca != peca){
            i = tabuleiro->lin - (++n); j = 0; x = 0; peca = 0;
        }
    }

    n = 0;
    i = tabuleiro->lin - 1;
    j = 1;
    peca = 0;
    x = 0;

    //Diagonal esquerda(baixo) -> direita(cima) meio para baixo
    while(n < tabuleiro->lin - 1) {
        if (tabuleiro->celulas[i][j].peca == '_' || tabuleiro->celulas[i][j].peca == 'o' || i == -1 ||j == tabuleiro->col) {
            i = tabuleiro->lin - 1; j = ++n; x = 0; peca = 0;
        } else if (peca == 0) {
            peca = tabuleiro->celulas[i][j].peca;
            i--; j++; x++;
        } else if (tabuleiro->celulas[i][j].peca == peca) {
            i--; j++; x++;
        } else if (x == tabuleiro->lin - n)
            return 1;
        else if(tabuleiro->celulas[i][j].peca != peca){
            i = tabuleiro->lin - 1; j = ++n; x = 0; peca = 0;
        }

    }

    return 0;
}*/



