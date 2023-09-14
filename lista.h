#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Structs:

// Struct de indices secundarios
typedef struct sec
{
    char titulo_original[56];
    int proximo;
    int posicao_primario;
} indice_sec;

// Struct de primarios
typedef struct prim
{
    char id_prim[6];
    int prox;
} primarios_lista;

// Funcoes basicas
void inserir_secundario(int *head, int *ppl, indice_sec *lista, char *titulo, int posicao_prim);
int buscar_lista_sec(int head, indice_sec *lista_sec, char *titulo);
int buscar_lista_secundaria_para_operacao(int head, indice_sec *lista_sec, primarios_lista *lista_invertida, char *titulo);
int busca_sec_cod_prim(int head, indice_sec *lista_sec, int index);
void remocao_lista_invertida(indice_sec *lista_sec, primarios_lista *lista_invertida, char *primario_indice, int *head);

#endif