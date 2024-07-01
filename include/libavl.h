#ifndef __AVL__
#define __AVL__

typedef struct _lista {
    void *item;
    int cod_ibge;
    struct _lista *proximo;
} tLista;

typedef struct _node {
    tLista *lista_enc;
    struct _node *pai;
    struct _node *esq;
    struct _node *dir;
    int h;
} tNode;

typedef struct {
    tNode *raiz;
    double (*cmp)(void *, void *);
} tArv;

int max(int a, int b);
int altura(tNode *node);
tNode ** percorre_esq(tNode **pnode);
void rotacao_dir(tNode **pnode);
void rotacao_esq(tNode **pnode);
tNode ** sucessor(tNode **pnode);
void construir_avl(tArv *arv, double (*cmp)(void *, void *));
void inserir_avl_node(tArv *arv, tNode **pnode, tNode *pai, void *item, int cod_ibge);
void inserir_avl(tArv *arv, void *item, int cod_ibge);
void rebalancear_avl(tNode **pnode);
void remover_avl_node(tArv *arv, tNode **pnode, void *item);
void remover_avl(tArv *arv, void *item);
void inserir_lista(tLista **plista, void *item, int cod_ibge);

#endif