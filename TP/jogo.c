//
// Created by carol on 6/4/2021.
//

#include "main.h"
#include "jogo.h"
#include "dados.h"
#include "tabuleiro.h"
#include "utils.h"


void menuInicial(){
    printf( "\n\n---------------------------------------------------\n"
            "      ____   ______   ______   ______    ___           \n"
            "     |__  | |  __  | |  ____| |  __  |  |   |          \n"
            "        | | | |  | | | |  __  | |  | |  | O |          \n"
            "     _  | | | |  | | | | |  | | |  | |  | O |          \n"
            "    | |_| | | |__| | | |__| | | |__| |  | O |          \n"
            "    |_____| |______| |______| |______|  |___|          \n"
            "                                                       \n"
            "           1.Humano vs Humano;                         \n"
            "           2.Humano vs Computador;                     \n"
            "                                                       \n"
            "---------------------------------------------------    \n"
            );
}

void menuPrincipal(){
    printf( "\n---------------------------------\n"
            "       1.Joga Peca;                \n"
            "       2.Acrescenta Linha;         \n"
            "       3.Acrescenta Coluna;        \n"
            "       4.Joga Pedra;               \n"
            "       Sair;                       \n"
            "---------------------------------\n");
}

void menuSecundario(){
    printf( "\n---------------------------------------------------\n"
            "       1.Mostra X tabuleiros;                        \n"
            "       2.Mostra Historico;                           \n"
            "       3.Meus Dados;                                 \n"
            "       4.Dados Jogo;                                 \n"
            "       Proxima Jogada;                               \n"
            "---------------------------------------------------  \n"
            );
}

int jogoTermina(pJogo jogo){
    pTabuleiro aux = jogo->tabuleiro;

    while(aux->prox != NULL)
        aux = aux->prox;

    if(verificaLinha(aux) || verificaColuna(aux) || verificaDiagonal(aux))
        return 1;
    else
        return 0;
}

pJogo inicializaJogo(){
    pJogo novo = malloc(sizeof(jogo));
    if(novo == NULL){
        puts("Erro na alocacao de memoria");
        return NULL;
    }

    novo->tabuleiro = malloc(sizeof(tabuleiro));
    if(novo->tabuleiro == NULL){
        puts("Erro na alocacao de memoria");
        return NULL;
    }

    novo->jogadas = malloc(sizeof(jogada));
    if(novo->jogadas == NULL){
        puts("Erro na alocacao de memoria");
        return NULL;
    }

    novo->nJogadas = 0;
    novo->jogadores = malloc(sizeof(jogador)*2);
    if(novo->jogadores == NULL){
        puts("Erro na alocacao de memoria");
        return NULL;
    }

    return novo;
}

pJogo iniciaJogo(){
    pJogo novo = malloc(sizeof(jogo));
    int op;
    if(novo == NULL){
        puts("Erro de alocacao de memoria\n");
        return NULL;
    }
    menuInicial();

    do {
        printf(">>Opcao: ");
        op = scanfInteiro();
    }while(op < 1 || op > 2);

    novo->tabuleiro = inicializaTabuleiro();
    novo->jogadas = NULL;
    novo->nJogadas = 0;
    novo->jogadores = malloc(sizeof(jogador)*2);
    if(novo->jogadores == NULL){
        puts("Erro na alocacao de memoria");
        return NULL;
    }

    switch(op){
        case 1:
            novo->tipo = 1;
            for(int i = 0; i < 2; i++) {
                novo->jogadores[i] = *adicionaHumano(i);
            }
            if(strcmp(novo->jogadores[1].nome,novo->jogadores[0].nome) == 0)
                    pedeNome(novo->jogadores[1].nome, 1);
            break;
        case 2:
            novo->tipo = 2;
            novo->jogadores[0] = *adicionaHumano(0);
            novo->jogadores[1] = *adicionaComputador();
            if(strcmp(novo->jogadores[1].nome,novo->jogadores[0].nome) == 0)
                pedeNome(novo->jogadores[0].nome, 0);
            break;
    }

    return novo;
}

pJogo escolheMovimento(pJogo jogo, int *sair, int *tabCheio){
    int op = 0;
    pJogada jogada = NULL, aux = NULL;

    jogada = proximaJogada(jogada, jogo);

    *tabCheio = tabuleiroCheio(jogada->tabuleiro);
    if(*tabCheio){
        printf("\nTabuleiro Cheio! Só pode realizar jogadas de mudança de linhas/colunas!\n");
        if(jogada->jogadorAtual->numero == 0 && jogada->jogadorAtual->nMovAdicionais == 2){
            printf("\nNao pode fazer mais movimentos!\n");
            if(jogo->jogadores[1].nMovAdicionais < 2) {
                printf("\nA vez passa para o proximo jogador!\n");
                jogada->jogadorAtual = &jogo->jogadores[1];
            }
            else{
                printf("\nNenhum dos jogadores tem mais movimentos!\n");
                printf("\nOcorreu um empate!\n");
            }
        }
        else if(jogada->jogadorAtual->numero == 1 && jogada->jogadorAtual->nMovAdicionais == 2){
            printf("\nNao pode fazer mais movimentos!\n");
            if(jogo->jogadores[0].nMovAdicionais < 2) {
                printf("\nA vez passa para o proximo jogador!\n");
                jogada->jogadorAtual = &jogo->jogadores[0];
            }
            else{
                printf("\nNenhum dos jogadores tem mais movimentos!\n");
                printf("Ocorreu um empate!\n");
            }
        }
    }

    printf("\n");
    mostraUltTabuleiro(jogo);
    menuPrincipal();

    printf("[Jogador %d] %s\n", jogada->jogadorAtual->tipo,jogada->jogadorAtual->nome);

    do {
        printf(">>Opcao: ");
        if(jogada->jogadorAtual->tipo == 1) {
            op = scanfInteiro();
        }
        else {
            op = intUniformRnd(1, 4);
            printf("%d\n", op);
        }

        printf("\n");
        mostraUltTabuleiro(jogo);

        switch (op) {
            case 1:
                if(!(*tabCheio))
                    aux = jogarPeca(jogada, jogo->tipo);
                else {
                    printf("\nNao pode fazer este movimento\n");
                    aux = NULL;
                }

                break;
            case 2:
                aux = acrescentaLin(jogada);
                *tabCheio = 0;
                if(aux == NULL)
                    printf("\n[Jogada Invalida] Ja adicionou linhas/colunas 2x!\n");
                break;
            case 3:
                aux = acrescentaCol(jogada);
                *tabCheio = 0;
                if(aux == NULL)
                    printf("\n[Jogada Invalida] Ja adicionou linhas/colunas 2x!\n");
                break;
            case 4:
                if(!(*tabCheio)) {
                    aux = jogaPedra(jogada, jogo->tipo);
                    if (aux == NULL)
                        printf("\n[Jogada Invalida] Ja jogou a pedra!\n");
                }
                else {
                    printf("\nNao pode fazer este movimento\n");
                    aux = NULL;
                }
                break;
            default:
                printf("\nPretende mesmo sair?    \n"
                       "    1-Sim;              \n"
                       "    2-Nao               \n");
                do{
                    printf(">>Opcao: ");
                    op = scanfInteiro();
                }while(op < 1 || op > 2);

                switch (op) {
                    case 1:
                        eliminarJogadaNova(jogo->jogadas, jogo->nJogadas);
                        *sair = 1;
                        break;
                    case 2:
                        menuPrincipal();
                        break;
                }
                break;
        }
    }while(aux == NULL && !(*sair));
    if(*sair != 1) {
        jogo->nJogadas++;
        jogada->jogadorAtual->iter++;
    }
    return jogo;
}

void verDados(pJogo jogo){
    int op, k, sair = 0;
    pJogada jogada = jogo->jogadas;

    while(jogada->prox != NULL)
        jogada = jogada->prox;

    do {
        menuSecundario();
        printf(">>Opcao: ");
        op = scanfInteiro();
        switch (op) {
            case 1:
                do {
                    printf("\nNumero de Tabuleiros: ");
                    k = scanfInteiro();
                } while (k > jogo->nJogadas);
                mostraXTabuleiro(jogo, k);
                break;
            case 2:
                mostraJogadas(jogo);
                break;
            case 3:
                printf( "\n\n---------------------------------\n");
                mostraJogador(jogada->jogadorAtual); //Dados Jogador Atual
                printf("---------------------------------\n");
                break;
            case 4:
                mostraJogo(jogo);
                break;
            default:
                sair = 1;
                break;
        }
    }while(!sair);
}

pJogada jogarPeca(pJogada jogada, int tipo) {
    int lin, col, movValido = 0;
    pCelula c = malloc(sizeof(celula));
    if(c == NULL){
        puts("Erro na alocacao de memoria");
        return NULL;
    }

    if(tipo == 2 && jogada->jogadorAtual->tipo == 2) {
        do {
            printf("\n>>Jogada(lin col): ");
            lin = intUniformRnd(1, jogada->tabuleiro->lin) - 1;
            col = intUniformRnd(1, jogada->tabuleiro->col) - 1;
            movValido = verificaMov(jogada->tabuleiro, lin, col, c);
        }while (!movValido);
        printf("%d %d\n", lin - 1, col - 1);

    }
    else {
        do {
            printf("\n>>Jogada(lin col): ");
            lin = scanfInteiro() - 1;
            col = scanfInteiro() - 1;
            movValido = verificaMov(jogada->tabuleiro, lin, col, c);
        } while (!movValido);
    }

    if (c->peca == '_')
        c->peca = 'V';
    else if (c->peca == 'V')
        c->peca = 'Y';
    else if(c->peca == 'Y')
        c->peca = 'R';

    jogada->movimento = "jogou peca";
    jogada->movimentolen = (int) strlen(jogada->movimento);
    jogada->tabuleiro->celulas[lin][col] = *c;

    printf("\n-->%s colocou %c na c [%d, %d]\n", jogada->jogadorAtual->nome, c->peca, lin + 1, col + 1);

    return jogada;
}

pJogada acrescentaLin(pJogada jogada){

    if (jogada->jogadorAtual->nMovAdicionais == 2)
            return NULL;

    jogada->tabuleiro->lin += 1;


    jogada->tabuleiro = acrescentaLinha(jogada->tabuleiro);


    jogada->movimento =  "acrescentou uma linha";
    jogada->movimentolen = (int) strlen(jogada->movimento);

    printf("\n-->%s acrescentou uma linha!\n", jogada->jogadorAtual->nome);

    jogada->jogadorAtual->nMovAdicionais++;

    return jogada;
}

pJogada acrescentaCol(pJogada jogada){
    if (jogada->jogadorAtual->nMovAdicionais == 2)
        return NULL;

    jogada->tabuleiro->col += 1;


    jogada->tabuleiro = acrescentaColuna(jogada->tabuleiro);

    jogada->movimento = "acrescentou uma coluna";
    jogada->movimentolen = (int) strlen(jogada->movimento);

    printf("\n-->%s acrescentou uma coluna!\n", jogada->jogadorAtual->nome);

    jogada->jogadorAtual->nMovAdicionais++;

    return jogada;
}

pJogada jogaPedra(pJogada jogada, int tipo) {
    int lin, col, movValido = 0;
    pCelula c = malloc(sizeof(celula));
    if(c == NULL){
        puts("Erro na alocacao de memoria");
        return NULL;
    }

    if (jogada->jogadorAtual->pedra == 1)
        return NULL;

    if(tipo == 2 && jogada->jogadorAtual->tipo == 2) {
        do {
            lin = intUniformRnd(0, jogada->tabuleiro->lin) - 1;
            col = intUniformRnd(0, jogada->tabuleiro->col) - 1;
            movValido = verificaMov(jogada->tabuleiro, lin, col, c);
            if(c->peca != '_')
                movValido = 0;

        }while (!movValido);
        printf("%d %d\n", lin - 1, col - 1);

    }
    else {
        do {
            printf("\n>>Jogada(lin col): ");
            lin = scanfInteiro() - 1;
            col = scanfInteiro() - 1;
            movValido = verificaMov(jogada->tabuleiro, lin, col, c);

            if(c->peca != '_') {
                movValido = 0;
                printf("\n[Jogada Invalida] já existe uma peca na c! Celula deve ser '_'\n");
            }

        } while (!movValido);
    }
    c->peca = 'o';
    jogada->movimento = "jogou pedra";
    jogada->movimentolen = (int) strlen(jogada->movimento);
    jogada->jogadorAtual->pedra = 1;
    jogada->tabuleiro->celulas[lin][col] = *c;

    printf("\n-->%s jogou pedra na c [%d, %d]\n", jogada->jogadorAtual->nome, lin, col);
    return jogada;
}

pJogada proximaJogada(pJogada jogada, pJogo jogo) {
    int jogador = 0;
    pTabuleiro tabuleiro = NULL;

    if(jogo->jogadas == NULL) {
        jogo->jogadas = iniciaJogada(jogada);
        jogo->jogadas->tabuleiro = jogo->tabuleiro;
        jogo->jogadas->jogadorAtual = &jogo->jogadores[0];
        jogo->jogadas->turno = jogo->nJogadas + 1;
        jogada = jogo->jogadas;
        return jogada;
    }

    tabuleiro = jogo->tabuleiro;
    while(tabuleiro->prox != NULL)
        tabuleiro = tabuleiro->prox;

    tabuleiro = acrescentarTabuleiro(tabuleiro);

    jogada = jogo->jogadas;
    while (jogada->prox != NULL)
        jogada = jogada->prox;


    jogo->jogadas = iniciaJogada(jogo->jogadas);

    jogada = jogada->prox;


    if(jogada->jogadorAtual->numero == 1)
        jogador = 0;
    else
        jogador = 1;

    jogada->tabuleiro = tabuleiro;

    jogada->jogadorAtual = &jogo->jogadores[jogador];

    jogada->turno = jogo->nJogadas + 1;

    return jogada;
}

pJogador adicionaHumano(int i){
    pJogador novo = iniciaJogador();

    novo->tipo = 1;
    novo->numero = i;
    pedeNome(novo->nome, i);
    return novo;
}

pJogador adicionaComputador(){
    pJogador novo = iniciaJogador();

    novo->tipo = 2;
    novo->numero = 1;
    strcpy(novo->nome, "Computador");
    return novo;
}

void mostraJogadas(pJogo jogo) {
    pJogada jogada = jogo->jogadas;
    printf( "\n\n---------------------------------\n");
    for(int i = 0 ; i < jogo->nJogadas; i++) {
        mostraJogada(jogada);
        printf("\tTabuleiro: \n");
        mostraTabuleiro(jogada->tabuleiro);
        jogada = jogada->prox;
        printf("---------------------------------\n");
    }
}

void mostraJogo(pJogo jogo) {
    printf( "\n\n---------------------------------\n"
            "\tJogadas: %d\n"
            "\tJogador 1: %s\n"
            "\tJogador 2: %s\n"
            , jogo->nJogadas, jogo->jogadores[0].nome, jogo->jogadores[1].nome);

    if(jogo-> tipo == 1)
        printf("\tHumano vs Humano;\n");
    else
        printf("\tHumano vs Computador\n");

    printf("\tTabuleiro: \n");
    mostraUltTabuleiro(jogo);

    printf("---------------------------------\n");
}

void mostraXTabuleiro(pJogo jogo, int kx){
    pTabuleiro aux = jogo->tabuleiro;
    for(int i = 0; i < jogo->nJogadas-kx; i++)
        aux = aux->prox;
    printf( "\n\n---------------------------------\n");
    for(int i = 0; i < kx; i++){
        printf("Tabuleiro %d\n", jogo->nJogadas - kx + i + 1);
        mostraTabuleiro(aux);
        printf("---------------------------------\n");
        aux = aux->prox;
    }
}

void mostraUltTabuleiro(pJogo jogo){
    pTabuleiro t = jogo->tabuleiro;

    while(t->prox != NULL)
        t = t->prox;
    mostraTabuleiro(t);
}

void mostraStatusFinal(pJogo jogo){
    pJogada aux = jogo->jogadas;
    while(aux->prox != NULL)
        aux = aux->prox;
    printf( "\n---------------------------------\n"
            "Parabéns! \n"
            "%s venceu o jogo!\n"
            , aux->jogadorAtual->nome);
}

void mostraEmpate(pJogo jogo){
    printf( "\n\n---------------------------------\n"
            "Jogadas: %d\n"
            "Empate!\n"
            "Tabuleiro: \n"
            , jogo->nJogadas);
    mostraUltTabuleiro(jogo);
}

void cleanJogo(pJogo jogo){
    free(jogo->jogadores);
    cleanTabuleiro(jogo->tabuleiro);
    cleanJogada(jogo->jogadas);
}