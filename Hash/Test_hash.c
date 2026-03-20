#include <stdio.h>
#include <assert.h>
#include "hash_extensivel.h"

void teste_insercao_busca() {
    Diretorio *dir = criar_diretorio(1, 2);

    inserir(dir, 10, 100);
    inserir(dir, 20, 200);
    inserir(dir, 30, 300);

    assert(buscar(dir, 10) == 100);
    assert(buscar(dir, 20) == 200);
    assert(buscar(dir, 30) == 300);
    assert(buscar(dir, 40) == -1);

    printf("Teste insercao/busca OK\n");

    destruir_diretorio(dir);
}

int main() {
    teste_insercao_busca();
    printf("Todos os testes passaram!\n");
    return 0;
}
