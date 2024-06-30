#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "../include/libhash.h"
#include "../include/libstruct.h"
#include "../include/libparser.h"
#include "../include/libavl.h"

// definindo que a chave do usuário é codigo_ibge
char *get_key(void *reg){
    return (*((tMunicipio *)reg)).codigo_ibge;
}

// funções de comparação para a estrutura avl
double cmp_str(void *a, void *b) {
    return strcmp((char *) a, (char *) b);
}

double cmp_double(void *a, void *b) {
    return ((float *) a) -  ((float *) b);
}

double cmp_int(void *a, void *b) {
    return ((int *) a) - ((int *) b);
}

// alocando um bucket cidade sem determinar os valores de seus atributos
void aloca_cidade(tMunicipio **cidade){
    *cidade = (tMunicipio*) malloc(sizeof(tMunicipio));
}

int main(){
    tHash hash_cod;
    tArv arv_nome, arv_ddd, arv_uf, arv_lat, arv_long;

    inicializar_hash(&hash_cod, 15877, get_key);
    construir_avl(&arv_nome, cmp_str);
    construir_avl(&arv_ddd, cmp_int);
    construir_avl(&arv_uf, cmp_int);
    construir_avl(&arv_lat, cmp_double);
    construir_avl(&arv_long, cmp_double);

    FILE *arquivo = fopen("municipios.json", "r");
    leitor(arquivo, &hash_cod);

}