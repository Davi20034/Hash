#ifndef HASH_EXTENSIVEL_H_INCLUDED
#define HASH_EXTENSIVEL_H_INCLUDED

#include <stdio.h>

typedef struct {
    int chave;
    long posicao_arquivo;
} Registro;


typedef struct {
    int profundidade_local;
    int quantidade;
    Registro *registros;
} Bucket;


typedef struct {
    int profundidade_global;
    int tamanho;
    Bucket **buckets;
} Diretorio;

Diretorio* criar_diretorio(int profundidade_inicial, int capacidade_bucket);

int inserir(Diretorio *dir, int chave, long posicao);

long buscar(Diretorio *dir, int chave);

void destruir_diretorio(Diretorio *dir);

#endif // HASH_EXTENSIVEL_H_INCLUDED
