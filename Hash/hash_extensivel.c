/**
 * ============================================================
 *  HASH EXTENSÍVEL EM C
 * ============================================================
 *
 * Implementação de uma tabela hash extensível baseada em:
 * - Diretório com profundidade global
 * - Buckets com profundidade local
 * - Divisão dinâmica (split)
 *
 * Características:
 * ✔ Crescimento dinâmico
 * ✔ Evita colisões excessivas
 * ✔ Estrutura semelhante a bancos de dados
 *
 * ============================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include "hash_extensivel.h"

/* ==========================
   STRUCTS INTERNAS
========================== */

/**
 * Representa um registro armazenado no hash
 */
struct re {
    int chave;
    long posicao;
};

/**
 * Bucket: armazena vários registros
 */
struct bu {
    int profundidade_local;
    int quantidade;
    Registro *registros;
};

/**
 * Diretório principal
 */
struct dir {
    int profundidade_global;
    int tamanho;
    int capacidade;
    int total_registros;
    Bucket *buckets;
};

/* ==========================
   FUNÇÕES AUXILIARES
========================== */

/**
 * Função hash usando bits menos significativos
 */
static int hash(int chave, int prof) {
    return chave & ((1 << prof) - 1);
}

/**
 * Cria um novo registro
 */
static Registro criar_registro(int chave, long pos) {
    Registro r = malloc(sizeof(struct re));
    r->chave = chave;
    r->posicao = pos;
    return r;
}

/**
 * Cria um novo bucket
 */
static Bucket criar_bucket(int prof, int cap) {
    Bucket b = malloc(sizeof(struct bu));
    b->profundidade_local = prof;
    b->quantidade = 0;
    b->registros = malloc(sizeof(Registro) * cap);
    return b;
}

/* ==========================
   CRIAÇÃO
========================== */

Diretorio* criar_diretorio(int prof, int cap) {
    if (prof < 0 || cap <= 0) return NULL;

    Diretorio *dir = malloc(sizeof(Diretorio));
    *dir = malloc(sizeof(struct dir));

    (*dir)->profundidade_global = prof;
    (*dir)->tamanho = 1 << prof;
    (*dir)->capacidade = cap;
    (*dir)->total_registros = 0;

    (*dir)->buckets = malloc(sizeof(Bucket) * (*dir)->tamanho);

    for (int i = 0; i < (*dir)->tamanho; i++)
        (*dir)->buckets[i] = criar_bucket(prof, cap);

    return dir;
}

/* ==========================
   SPLIT (divisão de bucket)
========================== */

static void dividir_bucket(Diretorio *dir, int idx) {
    Bucket b = (*dir)->buckets[idx];
    int nova = b->profundidade_local + 1;

    // Cresce o diretório se necessário
    if (nova > (*dir)->profundidade_global) {
        int antigo = (*dir)->tamanho;

        (*dir)->tamanho *= 2;
        (*dir)->buckets = realloc((*dir)->buckets, sizeof(Bucket) * (*dir)->tamanho);

        for (int i = 0; i < antigo; i++)
            (*dir)->buckets[i + antigo] = (*dir)->buckets[i];

        (*dir)->profundidade_global++;
    }

    Bucket novo = criar_bucket(nova, (*dir)->capacidade);
    b->profundidade_local++;

    // Redistribui ponteiros
    for (int i = 0; i < (*dir)->tamanho; i++) {
        if ((*dir)->buckets[i] == b &&
            ((i >> (b->profundidade_local - 1)) & 1)) {
            (*dir)->buckets[i] = novo;
        }
    }

    // Rehash dos registros
    int qtd = b->quantidade;
    Registro *temp = malloc(sizeof(Registro) * qtd);

    for (int i = 0; i < qtd; i++)
        temp[i] = b->registros[i];

    b->quantidade = 0;
    novo->quantidade = 0;

    for (int i = 0; i < qtd; i++) {
        inserir_arquivo(dir, temp[i]->chave, temp[i]->posicao);
        free(temp[i]);
    }

    free(temp);
}

/* ==========================
   INSERÇÃO
========================== */

int inserir_arquivo(Diretorio *dir, int chave, long pos) {
    if (!dir) return -1;

    int idx = hash(chave, (*dir)->profundidade_global);
    Bucket b = (*dir)->buckets[idx];

    // Atualiza se já existir
    for (int i = 0; i < b->quantidade; i++) {
        if (b->registros[i]->chave == chave) {
            b->registros[i]->posicao = pos;
            return 0;
        }
    }

    if (b->quantidade < (*dir)->capacidade) {
        b->registros[b->quantidade++] = criar_registro(chave, pos);
        (*dir)->total_registros++;
        return 0;
    }

    dividir_bucket(dir, idx);
    return inserir_arquivo(dir, chave, pos);
}

/* ==========================
   BUSCA
========================== */

long buscar_arquivo(Diretorio *dir, int chave) {
    if (!dir) return -1;

    int idx = hash(chave, (*dir)->profundidade_global);
    Bucket b = (*dir)->buckets[idx];

    for (int i = 0; i < b->quantidade; i++)
        if (b->registros[i]->chave == chave)
            return b->registros[i]->posicao;

    return -1;
}

/* ==========================
   OUTRAS FUNÇÕES
========================== */

int existe_chave(Diretorio *dir, int chave) {
    return buscar_arquivo(dir, chave) != -1;
}

int atualizar_arquivo(Diretorio *dir, int chave, long nova_posicao) {
    if (!dir) return -1;

    int idx = hash(chave, (*dir)->profundidade_global);
    Bucket b = (*dir)->buckets[idx];

    for (int i = 0; i < b->quantidade; i++) {
        if (b->registros[i]->chave == chave) {
            b->registros[i]->posicao = nova_posicao;
            return 0;
        }
    }
    return -1;
}

int remover_arquivo(Diretorio *dir, int chave) {
    if (!dir) return -1;

    int idx = hash(chave, (*dir)->profundidade_global);
    Bucket b = (*dir)->buckets[idx];

    for (int i = 0; i < b->quantidade; i++) {
        if (b->registros[i]->chave == chave) {
            free(b->registros[i]);

            for (int j = i; j < b->quantidade - 1; j++)
                b->registros[j] = b->registros[j + 1];

            b->quantidade--;
            (*dir)->total_registros--;
            return 0;
        }
    }
    return -1;
}

int quantidade_registros(Diretorio *dir) {
    return dir ? (*dir)->total_registros : -1;
}

/* ==========================
   DEBUG
========================== */

void imprimir_diretorio(Diretorio *dir) {
    if (!dir) return;

    printf("\n=== HASH EXTENSÍVEL ===\n");
    printf("Profundidade global: %d\n", (*dir)->profundidade_global);

    for (int i = 0; i < (*dir)->tamanho; i++) {
        Bucket b = (*dir)->buckets[i];

        printf("[%d] (prof=%d): ", i, b->profundidade_local);

        for (int j = 0; j < b->quantidade; j++)
            printf("(%d,%ld) ", b->registros[j]->chave, b->registros[j]->posicao);

        printf("\n");
    }
}

/* ==========================
   DESTRUIÇÃO
========================== */

void destruir_arquivo(Diretorio *dir) {
    if (!dir) return;

    int tam = (*dir)->tamanho;

    for (int i = 0; i < tam; i++) {
        if ((*dir)->buckets[i]) {

            Bucket b = (*dir)->buckets[i];

            for (int j = i; j < tam; j++)
                if ((*dir)->buckets[j] == b)
                    (*dir)->buckets[j] = NULL;

            for (int k = 0; k < b->quantidade; k++)
                free(b->registros[k]);

            free(b->registros);
            free(b);
        }
    }

    free((*dir)->buckets);
    free(*dir);
    free(dir);
}

void fechar_deretorio(Diretorio *dir) {
    destruir_arquivo(dir);
}
