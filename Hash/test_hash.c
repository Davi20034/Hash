#include "unity.h"
#include "hash_extensivel.h"

Diretorio *dir;

void setUp(void) {
    dir = criar_diretorio(1, 2);
}

void tearDown(void) {
    destruir_arquivo(dir);
}

/* ========================== */

void test_inserir_buscar() {
    inserir_arquivo(dir, 10, 100);
    TEST_ASSERT_EQUAL(100, buscar_arquivo(dir, 10));
}

void test_existe() {
    inserir_arquivo(dir, 5, 50);
    TEST_ASSERT_TRUE(existe_chave(dir, 5));
    TEST_ASSERT_FALSE(existe_chave(dir, 99));
}

void test_atualizar() {
    inserir_arquivo(dir, 1, 10);
    atualizar_arquivo(dir, 1, 999);
    TEST_ASSERT_EQUAL(999, buscar_arquivo(dir, 1));
}

void test_remover() {
    inserir_arquivo(dir, 2, 20);
    remover_arquivo(dir, 2);
    TEST_ASSERT_EQUAL(-1, buscar_arquivo(dir, 2));
}

void test_quantidade() {
    inserir_arquivo(dir, 1, 10);
    inserir_arquivo(dir, 2, 20);
    TEST_ASSERT_EQUAL(2, quantidade_registros(dir));
}

void test_divisao() {
    for (int i = 0; i < 20; i++)
        inserir_arquivo(dir, i, i * 10);

    for (int i = 0; i < 20; i++)
        TEST_ASSERT_EQUAL(i * 10, buscar_arquivo(dir, i));
}

/* ========================== */

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_inserir_buscar);
    RUN_TEST(test_existe);
    RUN_TEST(test_atualizar);
    RUN_TEST(test_remover);
    RUN_TEST(test_quantidade);
    RUN_TEST(test_divisao);

    return UNITY_END();
}
