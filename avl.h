#ifndef AVL_H
#define AVL_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct primario
{
    char codigo[6];
    int offset;
    int altura;
    struct primario *esquerda;
    struct primario *direita;
} indice_primario;

int maior(int a, int b);
indice_primario *novo_no(int offset, char *cod);
int altura(indice_primario *no);
int diferenca_altura(indice_primario *no);
indice_primario *menor_no(indice_primario *no);
indice_primario *rotacao_dir(indice_primario *no);
indice_primario *rotacao_esq(indice_primario *no);
indice_primario *rotacao_esq_dir(indice_primario *no);
indice_primario *rotacao_dir_esq(indice_primario *no);
indice_primario *inserir_avl(indice_primario *raiz, char *cod, int offset);
indice_primario *menor_direita(indice_primario *raiz);
indice_primario *remover_indice(indice_primario *raiz, char *cod, FILE *filmes);
indice_primario *busca_avl(indice_primario *raiz, char *info);
void imprimir_em_ordem(indice_primario *raiz, FILE *pri_index);
indice_primario *destruir_avl(indice_primario *raiz);


#endif