#ifndef HASH_EXTENSIVEL_H_INCLUDED
#define HASH_EXTENSIVEL_H_INCLUDED

#include <stdio.h>

/* Tipos opacos */
typedef struct re *Registro;
typedef struct bu *Bucket;
typedef struct dir *Diretorio;

/* ==========================
   CRIA플O
========================== */
Diretorio* criar_diretorio(int profundidade_inicial, int capacidade_bucket);

/* ==========================
   OPERA합ES
========================== */
int inserir_arquivo(Diretorio *dir, int chave, long posicao);
long buscar_arquivo(Diretorio *dir, int chave);

/* ==========================
   NOVAS FUN합ES
========================== */

/* Verifica se chave existe */
int existe_chave(Diretorio *dir, int chave);

/* Remove uma chave */
int remover_arquivo(Diretorio *dir, int chave);

/* Atualiza valor */
int atualizar_arquivo(Diretorio *dir, int chave, long nova_posicao);

/* Quantidade total de registros */
int quantidade_registros(Diretorio *dir);

/* Debug */
void imprimir_diretorio(Diretorio *dir);

/* ==========================
   FINALIZA플O
========================== */
void destruir_arquivo(Diretorio *dir);
void fechar_deretorio(Diretorio *dir);

#endif
