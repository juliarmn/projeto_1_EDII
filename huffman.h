#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define TAMANHO_REGISTRO 192
#define MAX_CHAR 256

typedef struct freq
{
    char caracter;
    int freq;
} char_frequencia;

typedef struct no
{
    unsigned char caracter;
    int frequencia;
    struct no *esq;
    struct no *dir;
    struct no *prox;
} node;

typedef struct lista_huffman
{
    node *inicio;
    int tamanho;
} list;

void inicializar_freq(unsigned int tabela[]);
void preenche_tabela(FILE *filmes, unsigned int tabela[]);
void imprimir_tabela_freq(unsigned int tabela[]);
void inicializar_lista(list **lista);
void inserir_ordenado(list **lista, node *no);
void preencher_lista(unsigned int tabela[], list *lista);
void imprimir_lista(list *lista);
node *remover_no_inicio(list *lista);
node *montar_arvore_huffman(list *lista);
void imprimir_arvore(node *raiz, int tamanho);
int altura_da_árvore(node *raiz);
char **alocar_dicionario(int coluna);
void gerar_dicionario(char **dicionario, node *raiz, char *caminho, int colunas);
void imprime_dicionario(char **dicionario);
int calcula_tamanho_string(char **dicionario, unsigned char *texto);
char *codificar_huffman(char **dicionario, unsigned char *texto);
char *decodificar(unsigned char *texto, node *raiz);
bool compactar_arquivo_huffman(FILE **filmes, char **dicionario, int colunas, node *raiz);


#endif