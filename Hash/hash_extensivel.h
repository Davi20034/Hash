#ifndef HASH_EXTENSIVEL_H_INCLUDED
#define HASH_EXTENSIVEL_H_INCLUDED

#include <stdio.h>
/**
 * @brief Estrutura de um registro armazenado no hash.
 */
typedef struct {
    int chave;
    long posicao_arquivo; // posição no arquivo de dados
} Registro;

/**
 * @brief Estrutura de bucket do hash extensível.
 */
typedef struct {
    int profundidade_local;
    int quantidade;
    Registro *registros;
} Bucket;

/**
 * @brief Estrutura do diretório do hash extensível.
 */
typedef struct {
    int profundidade_global;
    int tamanho;
    Bucket **buckets;
} Diretorio;

/**
 * @brief Inicializa o diretório do hash.
 *
 * @param profundidade_inicial profundidade global inicial
 * @param capacidade_bucket número máximo de registros por bucket
 * @return Diretorio* ponteiro para o diretório criado
 */
Diretorio* criar_diretorio(int profundidade_inicial, int capacidade_bucket);

/**
 * @brief Insere um registro no hash extensível.
 *
 * @param dir diretório do hash
 * @param chave chave a ser inserida
 * @param posicao posição no arquivo
 * @return int 1 sucesso, 0 erro
 */
int inserir(Diretorio *dir, int chave, long posicao);

/**
 * @brief Busca um registro pela chave.
 *
 * @param dir diretório do hash
 * @param chave chave buscada
 * @return long posição no arquivo ou -1 se não encontrado
 */
long buscar(Diretorio *dir, int chave);

/**
 * @brief Libera memória do diretório.
 */
void destruir_diretorio(Diretorio *dir);

#endif // HASH_EXTENSIVEL_H_INCLUDED
