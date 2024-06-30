// funções genéricas da hash 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/libhash.h"
#include "../include/libstruct.h"

#define TAM_HASH 15877 // numero primo
#define SEED 0x12345678

// construindo a hash
int inicializar_hash(tHash *hash, int nbuckets, char * (*get_key)(void *)){
    hash->tabela = calloc(sizeof(void *), nbuckets);

    // se nbuckets = 0;
    if (hash->tabela == NULL)
        return EXIT_FAILURE;

    hash->max = nbuckets;
    hash->tam = 0;
    hash->deletado = (uintptr_t)&(hash->tam);
    hash->get_key = get_key; // puxa a chave do usuário

    return EXIT_SUCCESS;
}

// função hash
// soma de todos os caracteres da chave
uint32_t hashf(const char* str, uint32_t h){
    /* One-byte-at-a-time Murmur hash 
    Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

// função pra facilitar
int mod_chave(uint32_t chave, int tam_hash){
    return chave % tam_hash;
}

// hashing duplo
int hashing_duplo(const char *chave, int tentativas, int max_hash){
    int hash1 = mod_chave(hashf(chave, SEED), max_hash); // posição inicial do elemento
    int hash2 = mod_chave(hashf(chave, SEED), max_hash - 1) + 1; // calcular os deslocamentos em relação à posição inicial (no caso de uma colisão)
    
    return abs((hash1 + tentativas * hash2) % max_hash);
}

int inserir_hash(tHash *hash, void *bucket){
    int tentativas = 0;
    int pos = hashing_duplo(hash->get_key(bucket), tentativas, TAM_HASH);

    if(hash->max == (hash->tam) + 1){ // se estiver cheia, não dá pra adicionar
        free(bucket);
        return EXIT_FAILURE;
    } 
    
    else {
        while (hash->tabela[pos] != 0){ // até encontrar um bucket vazio
            if (hash->tabela[pos] == hash->deletado){ // se encontrar um bucket deletado, para de iterar, pode adicionar nele mesmo
                break;
            }

            pos = hashing_duplo(hash->get_key(bucket), ++tentativas, TAM_HASH);
        }

        hash->tabela[pos] = (uintptr_t) bucket;
        hash->tam += 1;
    }

    return EXIT_SUCCESS;
}

void * buscar_hash(tHash hash, const char *chave){
    int tentativas = 0;
    int pos = hashing_duplo(chave, tentativas, TAM_HASH);

    void *retorno = NULL;

    while(hash.tabela[pos] != 0 && retorno == NULL){
        if (strcmp(hash.get_key((void *) hash.tabela[pos]), chave) == 0){
            retorno = (void *) hash.tabela[pos];      
        }

        else {
            pos = hashing_duplo(chave, ++tentativas, TAM_HASH);
        }
    } 

    return retorno;
}

void * buscar_hash_nomes(tHash hash, const char *chave){
    int tentativas = 0;
    int pos = hashing_duplo(chave, tentativas, TAM_HASH);
    void **vetor = calloc(5, sizeof(void *));
    int i = 0;

    while(hash.tabela[pos] != 0){
        if (strcmp(hash.get_key((void *) hash.tabela[pos]), chave) == 0){
            vetor[i] = (void *) hash.tabela[pos];
            i++;

        }

        pos = hashing_duplo(chave, ++tentativas, TAM_HASH);
    }

    return vetor;
}

void apagar_hash(tHash *hash){
    free(hash->tabela);
    free(hash);
}