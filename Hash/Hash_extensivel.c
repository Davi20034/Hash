#include <stdlib.h>
#include <stdio.h>
#include "hash_extensivel.h"

static int capacidade_bucket_global;

/**
 * Função hash simples (usa os bits menos significativos)
 */
static int hash(int chave, int profundidade) {
    return chave & ((1 << profundidade) - 1);
}

/**
 * Cria um novo bucket
 */
static Bucket* criar_bucket(int profundidade) {
    Bucket *b = malloc(sizeof(Bucket));
    b->profundidade_local = profundidade;
    b->quantidade = 0;
    b->registros = malloc(sizeof(Registro) * capacidade_bucket_global);
    return b;
}

/**
 * Cria diretório
 */
Diretorio* criar_diretorio(int profundidade_inicial, int capacidade_bucket) {
    Diretorio *dir = malloc(sizeof(Diretorio));

    capacidade_bucket_global = capacidade_bucket;

    dir->profundidade_global = profundidade_inicial;
    dir->tamanho = 1 << profundidade_inicial;
    dir->buckets = malloc(sizeof(Bucket*) * dir->tamanho);

    for (int i = 0; i < dir->tamanho; i++) {
        dir->buckets[i] = criar_bucket(profundidade_inicial);
    }

    return dir;
}

/**
 * Divide bucket quando cheio
 */
static void dividir_bucket(Diretorio *dir, int indice) {
    Bucket *b = dir->buckets[indice];
    int nova_prof = b->profundidade_local + 1;

    // Se precisar aumentar diretório
    if (nova_prof > dir->profundidade_global) {
        int antigo_tam = dir->tamanho;

        dir->tamanho *= 2;
        dir->buckets = realloc(dir->buckets, sizeof(Bucket*) * dir->tamanho);

        for (int i = 0; i < antigo_tam; i++) {
            dir->buckets[i + antigo_tam] = dir->buckets[i];
        }

        dir->profundidade_global++;
    }

    Bucket *novo = criar_bucket(nova_prof);
    b->profundidade_local++;

    // Redistribuir ponteiros no diretório
    for (int i = 0; i < dir->tamanho; i++) {
        if (dir->buckets[i] == b) {
            if ((i >> (b->profundidade_local - 1)) & 1) {
                dir->buckets[i] = novo;
            }
        }
    }

    // Rehash dos registros
    Registro temp[capacidade_bucket_global];
    int qtd = b->quantidade;

    for (int i = 0; i < qtd; i++) {
        temp[i] = b->registros[i];
    }

    b->quantidade = 0;
    novo->quantidade = 0;

    for (int i = 0; i < qtd; i++) {
        inserir(dir, temp[i].chave, temp[i].posicao_arquivo);
    }
}

/**
 * Inserção
 */
int inserir(Diretorio *dir, int chave, long posicao) {
    int idx = hash(chave, dir->profundidade_global);
    Bucket *b = dir->buckets[idx];

    // Inserção simples
    if (b->quantidade < capacidade_bucket_global) {
        b->registros[b->quantidade].chave = chave;
        b->registros[b->quantidade].posicao_arquivo = posicao;
        b->quantidade++;
        return 1;
    }

    // Bucket cheio → dividir
    dividir_bucket(dir, idx);
    return inserir(dir, chave, posicao);
}

/**
 * Busca
 */
long buscar(Diretorio *dir, int chave) {
    int idx = hash(chave, dir->profundidade_global);
    Bucket *b = dir->buckets[idx];

    for (int i = 0; i < b->quantidade; i++) {
        if (b->registros[i].chave == chave) {
            return b->registros[i].posicao_arquivo;
        }
    }

    return -1;
}

/**
 * DESTRUIR DIRETÓRIO (CORRIGIDO)
 */
void destruir_diretorio(Diretorio *dir) {
    int tamanho = dir->tamanho;

    for (int i = 0; i < tamanho; i++) {
        if (dir->buckets[i] != NULL) {

            Bucket *b = dir->buckets[i];

            // Marca todos os ponteiros iguais como NULL
            for (int j = i; j < tamanho; j++) {
                if (dir->buckets[j] == b) {
                    dir->buckets[j] = NULL;
                }
            }

            free(b->registros);
            free(b);
        }
    }

    free(dir->buckets);
    free(dir);
}
