#ifndef __LIBHASH__
#define __LIBHASH__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// criando hash genérica
typedef struct {
    uintptr_t *tabela;
    int tam;
    int max;
    uintptr_t deletado;
    char * (*get_key)(void *);
} tHash;

// funções da hash
int inicializar_hash(tHash *hash, int nbuckets, char * (*get_key)(void *));
uint32_t hashf(const char* str, uint32_t h);
int mod_chave(uint32_t chave, int tam_hash);
int hashing_duplo(const char *chave, int tentativas, int tam_hash);
int inserir_hash(tHash *hash, void *bucket);
void * buscar_hash(tHash hash, const char *chave);
void * buscar_hash_nomes(tHash hash, const char *chave);
void apagar_hash(tHash *hash);

#endif