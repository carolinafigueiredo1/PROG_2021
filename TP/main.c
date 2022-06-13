#include <stdio.h>
#include <unistd.h>
#include "main.h"
#include "jogo.h"
#include "tabuleiro.h"
#include "dados.h"


void menu(){
    printf( "\n\n--------------------------------------------------------------------------------------------\n"
            "                ____   ______   ______   ______    ___                                     \n"
            "               |__  | |  __  | |  ____| |  __  |  |   |                                    \n"
            "                  | | | |  | | | |  __  | |  | |  | O |                                    \n"
            "               _  | | | |  | | | | |  | | |  | |  | O |                                    \n"
            "              | |_| | | |__| | | |__| | | |__| |  | O |                                    \n"
            "              |_____| |______| |______| |______|  |___|                                    \n"
            "                                                                                           \n"
            "       As jogadas válidas relativas à colocação de peças são as seguintes:                 \n"
            "           1. Colocar peca Verde numa célula vazia                                         \n"
            "           2. Trocar peça Verde por uma Amarela                                            \n"
            "           3. Trocar peça Amarela por uma Vermelha                                         \n"
            "                                                                                           \n"
            "       Existem duas jogadas adicionais que podem ser efetuadas pelos jogadores:            \n"
            "           4. Colocar uma pedra numa célula vazia(max. 1 p/Jogador)                        \n"
            "           5. Adicionar uma linha ou uma coluna ao final do tabuleiro(max. 2 p/Jogador)    \n"
            "                                                                                           \n"
            "--------------------------------------------------------------------------------------------\n"
            );
}


int scanfInteiro(){
    int inteiro, fim=0;
    char tmp;
    while (fim < 1) {
        if(scanf(" %d", &inteiro)== 1)
            fim++;
        scanf("%c", &tmp);
    }
    return inteiro;
}

pJogo verificaJogoInacabado() {
    FILE *f;
    int op;
    pJogo j = malloc(sizeof(jogo));

    if (j == NULL) {
        puts("\nErro ao alocar memoria!");
        return NULL;
    }

    pJogada jogadaX = malloc(sizeof(jogada));

    if (jogadaX == NULL) {
        puts("\nErro ao alocar memoria!");
        return NULL;
    };

    pTabuleiro tab = malloc(sizeof(tabuleiro));

    if (tab == NULL) {
        puts("\nErro ao alocar memoria!");
        return NULL;
    }

    celula **c;

    if(!access(FichJogoInacabado, F_OK )){
        printf("O ficheiro %s existe\n", FichJogoInacabado);
        printf("\nPretende jogar o jogo inacabado?  \n"
               "    1.Sim;                          \n"
               "    2.Nao                           \n");
        do {
            printf(">>Opcao: ");
            op = scanfInteiro();
        }while(op < 1 || op > 2);
        switch(op){
            case 1:
                break;
            case 2:
                printf("O ficheiro %s será apagado\n", FichJogoInacabado);
                remove(FichJogoInacabado);
                return NULL;
        }

    }else
        return NULL;

    f = fopen(FichJogoInacabado, "rb");
    if (f == NULL) {
        puts("\nNao foi possivel abrir ficheiro\n");
        return NULL;
    }

    if (fread(&j->nJogadas, sizeof(int), 1, f) == 0) {
        puts("nJogadas invalido");
        fclose(f);
        return NULL;
    }

    if(fread(&j->tipo, sizeof(int), 1, f) == 0){
        puts("tipo de jogo invalido");
        fclose(f);
        return NULL;
    }

    j->tabuleiro = tab;


    for(int i = 0; i < j->nJogadas + 1; i++) {
        if(fread(&tab->lin, sizeof(int), 1, f)== 0){
            puts("lin invalido");
            fclose(f);
            return NULL;
        }
        if(fread(&tab->col, sizeof(int), 1, f)== 0){
            puts("col invalido");
            fclose(f);
            return NULL;
        }

        c = malloc(sizeof(celula*) * tab->lin);
        if(c == NULL){
            puts("Erro na alocacao de memoria");
            return NULL;
        }
        tab->celulas = c;
        for(int n = 0; n < tab->lin; n++) {
            c[n] = malloc( sizeof(celula) * tab->col);
            if(c[n] == NULL){
                puts("Erro na alocacao de memoria");
                return NULL;
            }
            for(int k = 0; k < tab->col; k++) {
                fread(&tab->celulas[n][k].lin, sizeof(int), 1, f);
                fread(&tab->celulas[n][k].col, sizeof(int), 1, f);
                fread(&tab->celulas[n][k].peca, sizeof(char), 1, f);
            }
        }
        if(i < j->nJogadas) {
            tab->prox = malloc(sizeof(tabuleiro));
            if (tab == NULL) {
                puts("\nErro ao alocar memoria!");
                return NULL;
            }
            tab = tab->prox;
        }
        else
            tab->prox=NULL;
    }

    j->jogadores = malloc(sizeof(jogador)*2);
    if(j->jogadores == NULL){
        puts("Erro na alocacao de memoria");
        return NULL;
    }

    for(int i = 0; i < 2; i++) {
        int n;
        fread(&n, sizeof(int), 1, f);
        fread(j->jogadores[i].nome , sizeof(char), n, f);
        fread(&j->jogadores[i].numero, sizeof(int), 1, f);
        fread(&j->jogadores[i].nMovAdicionais, sizeof(int), 1, f);
        fread(&j->jogadores[i].iter, sizeof(int), 1, f);
        fread(&j->jogadores[i].pedra, sizeof(int), 1, f);
        fread(&j->jogadores[i].tipo, sizeof(int), 1, f);
    }

    tab = j->tabuleiro->prox;



    j->jogadas = jogadaX;

    for(int i = 0; i < j->nJogadas; i++) {
        int nJogador;

        fread(&jogadaX->movimentolen, sizeof(int), 1, f);

        jogadaX->movimento = malloc(sizeof(char)*jogadaX->movimentolen);

        fread(jogadaX->movimento, sizeof(char), jogadaX->movimentolen, f);
        fread(&jogadaX->turno, sizeof(int), 1,f);
        fread(&nJogador, sizeof(int), 1, f);

        jogadaX->tabuleiro = tab;

        if(tab->prox != NULL)
            tab = tab->prox;

        if(nJogador == 0){
            jogadaX->jogadorAtual = &j->jogadores[0];
        }else
            jogadaX->jogadorAtual = &j->jogadores[1];
        if(i < j->nJogadas - 1) {
            jogadaX->prox = malloc(sizeof(jogada));
            jogadaX = jogadaX->prox;
            if (jogadaX == NULL) {
                puts("\nErro ao alocar memoria!");
                return NULL;
            }
        }
        else
            jogadaX->prox = NULL;
    }


    remove(FichJogoInacabado);
    fclose(f);
    return j;
}

void sair(pJogo j){
    //guardar num ficheiro.bin o jogo nao acabado
    pJogada jogadaX = j->jogadas;
    pTabuleiro tab = j->tabuleiro;

    if(j == NULL)
        return;

    FILE *f;

    if(!access(FichJogoInacabado, F_OK )){
        remove(FichJogoInacabado);
    }

    f = fopen (FichJogoInacabado, "wb");

    if(f == NULL)
    {
        puts( "\nNao foi possivel abrir ficheiro\n");
        return;
    }


    fwrite(&j->nJogadas, sizeof(int), 1, f);
    fwrite(&j->tipo, sizeof(int), 1, f);

    while(tab != NULL){
        fwrite(&tab->lin, sizeof(int), 1, f);
        fwrite(&tab->col, sizeof(int), 1, f);
        for (int i = 0; i < tab->lin; i++)
            for (int k = 0; k < tab->col; k++) {
                fwrite(&tab->celulas[i][k].lin, sizeof(int), 1, f);
                fwrite(&tab->celulas[i][k].col, sizeof(int), 1, f);
                fwrite(&tab->celulas[i][k].peca, sizeof(char), 1, f);
            }
        tab = tab->prox;
    }

    for(int i = 0; i < 2; i++) {
        int n = (int) strlen(j->jogadores[i].nome);
        fwrite(&n, sizeof(int), 1, f);
        fwrite(j->jogadores[i].nome , sizeof(char), n, f);
        fwrite(&j->jogadores[i].numero, sizeof(int), 1, f);
        fwrite(&j->jogadores[i].nMovAdicionais, sizeof(int), 1, f);
        fwrite(&j->jogadores[i].iter, sizeof(int), 1, f);
        fwrite(&j->jogadores[i].pedra, sizeof(int), 1, f);
        fwrite(&j->jogadores[i].tipo, sizeof(int), 1, f);
    }


    while(jogadaX != NULL){
        fwrite(&jogadaX->movimentolen, sizeof(int), 1,f);
        fwrite(jogadaX->movimento, sizeof(char), jogadaX->movimentolen, f);
        fwrite(&jogadaX->turno, sizeof(int), 1,f);
        fwrite(&jogadaX->jogadorAtual->numero, sizeof(int), 1, f);
        jogadaX = jogadaX->prox;
    }



    fclose (f);
}

void guardarJogo(pJogo jogo){
    FILE *f;
    int exist, op;
    pJogada jogadaX = NULL;
    char aux[] = "..\\jogosGuardados\\";

    char *nomeFichTxt = malloc(sizeof(char) * 100);
    if (nomeFichTxt == NULL) {
        puts("\nErro ao alocar memoria!");
        return;
    }

    do {
        printf(">>Nome do Ficheiro: ");
        scanf("%s", nomeFichTxt);

        strcat(nomeFichTxt,".txt");
        strcat(aux,nomeFichTxt);
        strcpy(nomeFichTxt, aux);

        if(!access(nomeFichTxt, F_OK )){
            printf("O ficheiro %s ja existe\n",nomeFichTxt);
            printf("\nPretende escrever por cima do ficheiro existente?  \n"
                   "    1.Sim;                          \n"
                   "    2.Nao                           \n");
            do {
                printf(">>Opcao: ");
                op = scanfInteiro();
            }while(op < 1 || op > 2);
            switch(op){
                case 1:
                    exist = 0;
                    remove(nomeFichTxt);
                    break;
                case 2:
                    exist = 1;
                    break;
            }

        }else
            exist = 0;

    }while(exist);

    f = fopen(nomeFichTxt, "wt");

    if(f==NULL){
        puts("\nNao foi possivel abrir ficheiro\n");
        return ;
    }

    fprintf(f,"------------Historico------------\n");
    jogadaX = jogo->jogadas;

    while(jogadaX->prox != NULL){
        fprintf(f,  "\n\tTurno %d\n"
                    "\tJogador %d: %s\n"
                    "\tTabuleiro: \n"
                ,jogadaX->turno, jogadaX->jogadorAtual->numero + 1, jogadaX->jogadorAtual->nome);

        fprintf(f, "\t\t\t   %2d ", 1);
        for(int i = 1; i < jogadaX->tabuleiro->col; i++)
            fprintf(f, "%2d ", i + 1);
        fprintf(f, "\n");
        for (int i = 0; i < jogadaX->tabuleiro->lin; i++) {
            fprintf(f,"\t\t\t%2d ", i + 1);
            for (int j = 0; j < jogadaX->tabuleiro->col; j++) {
                fprintf(f,"%2c ", jogadaX->tabuleiro->celulas[i][j].peca);

            }
            fprintf(f,"\n");
        }
        jogadaX = jogadaX->prox;
    }

    fprintf(f,  "\n\tTurno %d\n"
                "\tJogador: %s\n"
                "\tTabuleiro: \n"
            , jogadaX->turno, jogadaX->jogadorAtual->nome);

    fprintf(f, "\t\t\t   %2d ", 1);
    for(int i = 1; i < jogadaX->tabuleiro->col; i++)
        fprintf(f, "%2d ", i + 1);
    fprintf(f, "\n");
    for (int i = 0; i < jogadaX->tabuleiro->lin; i++) {
        fprintf(f,"\t\t\t%2d ", i);
        for (int j = 0; j < jogadaX->tabuleiro->col; j++) {
            fprintf(f,"%2c ", jogadaX->tabuleiro->celulas[i][j].peca);

        }
        fprintf(f,"\n");
    }

    fprintf(f, "\n\tVencedor: %s!\n", jogadaX->jogadorAtual->nome);

    fclose(f);
}


int main() {
    pJogo jogo = NULL;
    int op, saiu = 0, tabCheio = 0;
    menu();


    jogo = verificaJogoInacabado(jogo);

    if(jogo == NULL)
        jogo = iniciaJogo();

    do {
        if(jogo->nJogadas > 0) {
            printf( "\nVer dados de jogos:            \n"
                    "   1.Sim;                        \n"
                    "   2.Nao;                        \n");
            do{
                printf("\n>>Opcao: ");
                op = scanfInteiro();
            }while(op < 1|| op > 2);
            switch (op) {
                case 1:
                    verDados(jogo);
                    break;
                case 2:
                    break;
            }
        }
        jogo = escolheMovimento(jogo,&saiu,&tabCheio);
    }while(!jogoTermina(jogo) && !saiu );


    if(saiu) {
        printf( "\nAinda nao terminou o jogo! Pretende guardar jogo inacabado?\n"
                "       1.Sim;                    \n"
                "       2.Nao;                    \n");
        do {
            printf("\n>>Opcao: ");
            op = scanfInteiro();
        }while(op < 1 || op > 2);
        switch (op) {
            case 1:
                sair(jogo);
                break;
            case 2:
                break;
        }

    }
    if(jogoTermina(jogo)) {
        printf( "\nTerminou o jogo! Pretende guardar o jogo?\n"
                "       1.Sim;                    \n"
                "       2.Nao;                    \n");

        do {
            printf("\n>>Opcao: ");
            op = scanfInteiro();
        }while(op < 1 || op > 2);
        switch (op) {
            case 1:
                guardarJogo(jogo);
                break;
            case 2:
                break;
        }

        mostraStatusFinal(jogo);

    }

    if(tabCheio)
        mostraEmpate(jogo);

    cleanJogo(jogo);
    free(jogo);
    return 0;
}


