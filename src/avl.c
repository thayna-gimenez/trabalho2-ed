#include <stdio.h>
#include <stdlib.h>
#include "../include/libavl.h"

// função pra retornar o maior de dois números
int max(int a, int b) {
    return a > b ? a : b;
}

// passa o nó raíz de uma (sub)árvore pra calcular sua altura
int altura(tNode *node) {
    int ret;

    if (node == NULL) // se a árvore estiver vazia
        ret = -1;
    else
        ret = node->h; // retorna a altura da árvore

    return ret;
}

// função pra percorrer à esquerda de uma (sub)árvore a partir de um nó dado
tNode ** percorre_esq(tNode **pnode) {
    tNode *aux = *pnode;

    if (aux->esq == NULL){
        return pnode;
    }

    else {
        while (aux->esq->esq != NULL){
            aux = aux->esq;
        }

        return &(aux->esq);
    } 
}

// passa o nó raíz de uma (sub)árvore e rotaciona à direita
void rotacao_dir(tNode **pnode) {
    tNode *y = *pnode;
    tNode *x = y->esq;
    tNode *A = x->esq;
    tNode *B = x->dir;
    tNode *C = y->dir;

    y->esq = B;
    x->dir = y;
    *pnode = x;

    if (B != NULL)
        B->pai = y;

    x->pai = y->pai;
    y->pai = x;

    y->h = max(altura(B), altura(C)) + 1;
    x->h = max(altura(A), altura(y));
}

// passa o nó raíz de uma (sub)árvore e rotaciona à esquerda
void rotacao_esq(tNode **pnode) {
    tNode *x = *pnode;
    tNode *y = x->dir;
    tNode *A = x->esq;
    tNode *B = y->esq;
    tNode *C = y->dir;

    x->dir = B;
    y->esq = x;
    *pnode = y;

    if (B != NULL)
        B->pai = x;

    y->pai = x->pai;
    x->pai = y;

    x->h = max(altura(A), altura(B));
    y->h = max(altura(x), altura(C));
}

// função baseada no livro de Cormem
tNode ** sucessor(tNode **pnode) {
    if (*pnode == NULL) // se for nulo, não tem sucessor
        return NULL;

    // se a subárvore da direita não for vazia, então o sucessor do nó está na extrema esquerda da subárvore direita do nó fornecido
    if ((*pnode)->dir != NULL) 
        return percorre_esq(&(*pnode)->dir);

    tNode *aux = (*pnode)->pai;
    
    // subindo a árvore desde o nó fornecido até encontrar um nó que seja o filho à esquerda de seu pai
    while (aux != NULL && *pnode == aux->dir) {
        *pnode = aux;
        aux = aux->pai; 
    }

    return aux; // aux é o sucessor do nó fornecido
}

void construir_avl(tArv *arv, double (*cmp)(void *, void *)) {
    arv->raiz = NULL;
    arv->cmp = cmp;
}

// função pra inserir nó na árvore
void inserir_avl_node(tArv *arv, tNode **pnode, tNode *pai, void *item, int cod_ibge) {
    if (*pnode == NULL) { // se a árvore estiver vazia
        *pnode = (tNode *) malloc(sizeof(tNode)); // cria a raíz
 
        (*pnode)->dir = NULL;
        (*pnode)->esq = NULL;
        (*pnode)->pai = pai;
        (*pnode)->h = 0;

        (*pnode)->lista_enc = NULL;
        inserir_lista(&(*pnode)->lista_enc, item, cod_ibge); // insere o item na lista encadeada do nó
    }

    else if ((*pnode)->lista_enc->item - item > 0) { // se o item a ser inserido for menor que o item do nó da árvore
        inserir_avl_node(arv, &(*pnode)->esq, *pnode, item, cod_ibge); // vai para o lado esquerdo da árvore
    }
    
    else if ((*pnode)->lista_enc->item - item < 0) { // se o item a ser inserido for maior que o item do nó da árvore
        inserir_avl_node(arv, &(*pnode)->dir, *pnode, item, cod_ibge); // vai para o lado direito da árvore
    }

    else { // se for igual ao item do nó, insere na lista encadeada do nó
        inserir_lista(&(*pnode)->lista_enc, item, cod_ibge);
    }

    (*pnode)->h = max(altura((*pnode)->esq), altura((*pnode)->dir)) + 1; // aumenta em 1 a altura da árvore
    rebalancear_avl(pnode);
}

// função abstraída de inserção na árvore
void inserir_avl(tArv *arv, void *item, int cod_ibge) {
    inserir_avl_node(arv, &arv->raiz, NULL, item, cod_ibge); // manda a raíz da árvore como primeiro nó
}

// função para rebalancear uma (sub)árvore
void rebalancear_avl(tNode **pnode) {
    int fb, fbf;
    tNode *filho;

    fb = altura((*pnode)->esq) - altura((*pnode)->dir);  // diferença da altura entre a sub árvore da esquerda e da direita

    if (fb == -2) { // está desbalanceada pro lado direito
        filho = (*pnode)->dir;

        fbf = altura(filho->esq) - altura(filho->dir);

        // caso 1 -> ->
        if (fbf <= 0) {
            rotacao_esq(pnode);
        }
        
        // caso 2 -> <-
        else {
            rotacao_dir(&(*pnode)->dir);
            rotacao_esq(pnode);
        }
    }

    else if (fb == 2) { // está desbalanceada pro lado esquerdo
        filho = (*pnode)->esq;
        fbf = altura(filho->esq) - altura(filho->dir);

        // caso 3 <- <-
        if (fbf >= 0) {
            rotacao_dir(pnode);
        }

        // caso 4 <- ->
        else {
            rotacao_esq(&(*pnode)->dir);
            rotacao_dir(pnode);
        }
    }
}

// função pra remover item da árvore
void remover_avl_node(tArv *arv, tNode **pnode, void *item) {
    int cmp;
    tNode *aux;
    tNode **sucessor;

    if (*pnode != NULL){
        cmp = (*pnode)->lista_enc->item - item; // comparando o item que quer remover com o item do nó

        if (cmp > 0) // se o item for menor que o do nó
            remover_avl_node(arv, &(*pnode)->esq, item); // chama a função pra árvore da esquerda

        else if (cmp < 0) // se o item for maior que o do nó
            remover_avl_node(arv, &(*pnode)->dir, item); // chama a função pra árvore da direita

        else { // achou o item
            if ((*pnode)->dir == NULL && (*pnode)->esq == NULL){ // se é nó folha
                free(*pnode);
                *pnode = NULL;
            }

            else if ((*pnode)->dir == NULL || (*pnode)->esq == NULL) { // se tem um filho
                aux = *pnode;

                if ((*pnode)->esq == NULL)// tem filho à direita
                    *pnode = (*pnode)->dir; // substitui pelo filho

                else// tem filho à esquerda
                    *pnode = (*pnode)->esq; // substitui pelo filho

                free(aux); // apaga o nó do item
            }

            else { // tem dois filhos
                sucessor = percorre_esq(&(*pnode)->dir); // alocando endereço do nó sucessor na variável sucessor
                
                (*pnode)->lista_enc->item = (*sucessor)->lista_enc->item; // colocando o valor do sucessor no nó
                
                remover_avl_node(arv, &(*pnode)->dir, (*sucessor)->lista_enc->item); //entra na sub árvore da direita pra remover o sucessor, que ficou sobrando
            }

            if (*pnode != NULL) {
                (*pnode)->h = max(altura((*pnode)->dir), altura((*pnode)->esq)) + 1; // atualiza a altura da árvore
                avl_rebalancear(pnode);
            }
        }
    }
}

// função abstraída de remoção da árvore
void remover_avl(tArv *arv, void *item) {
    remover_avl_node(arv, &arv->raiz, item);
}


// função pra inserir item na lista encadeada
void inserir_lista(tLista **plista, void *item, int cod_ibge) {
    if (*plista == NULL) { // se a lista estiver vazia, cria uma nova lista
        *plista = (tLista *) malloc(sizeof(tLista));

        (*plista)->item = item;
        (*plista)->proximo = NULL;
        (*plista)->cod_ibge = cod_ibge;
    }

    else { // se não, vai até o final da lista e insere
        inserir_lista(&(*plista)->proximo, item, cod_ibge);
    }
}

