cmake_minimum_required(VERSION 3.17)
project(TP C)

set(CMAKE_C_STANDARD 99)

add_executable(TP main.c tabuleiro.c tabuleiro.h utils.c utils.h main.h dados.c dados.h jogo.c jogo.h)