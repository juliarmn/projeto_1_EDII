#include "lista.h"

void inserir_secundario(int *head, int *ppl, indice_sec *lista, char *titulo, int posicao_prim)
{
    strcpy(lista[*ppl].titulo_original, titulo);
    lista[*ppl].posicao_primario = posicao_prim;

    if (*head == -1)
    {
        (*head) = *ppl;
        lista[*ppl].proximo = -1;
        (*ppl)++;

        return;
    }

    if (strcmp(titulo, lista[*head].titulo_original) < 0)
    {
        lista[*ppl].proximo = *head;
        *head = *ppl;
        (*ppl)++;
        return;
    }

    int atual = *head;
    int anterior = -1;

    while (atual != -1 && strcmp(titulo, lista[atual].titulo_original) > 0)
    {
        anterior = atual;
        atual = lista[atual].proximo;
    }

    lista[*ppl].proximo = atual;

    if (anterior != -1)
    {
        lista[anterior].proximo = *ppl;
    }
    else
    {
        *head = *ppl;
    }

    (*ppl)++;
}

/**
 * Busca na lista secundaria o titulo e retorna a posicao dele na lista
 */
int buscar_lista_sec(int head, indice_sec *lista_sec, char *titulo)
{
    int i = head;
    if (head == -1)
        return -1;

    do
    {
        if (strcmp(lista_sec[i].titulo_original, titulo) == 0)
            return i;

        i = lista_sec[i].proximo;
    } while (i != -1);

    return -1;
}

/**
 * Busca o nome na lista secundaira, para verificar se ele ja existe  ou  nao
 */
int buscar_lista_secundaria_para_operacao(int head, indice_sec *lista_sec, primarios_lista *lista_invertida, char *titulo)
{
    int i = head;
    if (head == -1)
        return -1;

    do
    {
        if (strcmp(lista_sec[i].titulo_original, titulo) == 0 && lista_invertida[lista_sec[i].posicao_primario].id_prim[0] != '\0')
            return lista_sec[i].posicao_primario;

        i = lista_sec[i].proximo;

    } while (i != -1);

    return -1;
}

/**
 * Busca pela posicao do rpimario na lsita invertida
 */
int busca_sec_cod_prim(int head, indice_sec *lista_sec, int index)
{
    int i = head;

    if (head == -1)
    {
        return -1;
    }

    do
    {
        if (lista_sec[i].posicao_primario == index)
            return i;
        i = lista_sec[i].proximo;
    } while (lista_sec[i].proximo != -1);

    return -1;
}

/**
 * Remove da lista invertida o indice primario que foi retirado, atualizando seu head se necessario
 */
void remocao_lista_invertida(indice_sec *lista_sec, primarios_lista *lista_invertida, char *primario_indice, int *head)
{
    int i;
    for (i = 0; i < 1000 && strcmp(lista_invertida[i].id_prim, primario_indice); i++)
        ;
    int aux;
    int indice_sec;
    if (i < 1000)
    {
        if (lista_invertida[i].prox != -1)
        {
            aux = lista_invertida[i].prox;
            strcpy(lista_invertida[i].id_prim, lista_invertida[aux].id_prim);
            lista_invertida[aux].id_prim[0] = '\0';
            lista_invertida[i].prox = lista_invertida[aux].prox;
            lista_invertida[aux].prox = -1;
        }
        else
        {
            for (aux = 0; (aux < 1000 && lista_invertida[aux].prox != i); aux++)
                ;

            if (aux < 1000)
            {
                lista_invertida[i].id_prim[0] = '\0';
                lista_invertida[aux].prox = -1;
            }
            else
            {
                indice_sec = busca_sec_cod_prim(*head, lista_sec, i);
                lista_invertida[i].id_prim[0] = '\0'; // O '\0' vai indicar se foi ou nao removido
                lista_invertida[i].prox = -1;
                lista_sec[indice_sec].posicao_primario = -1;
            }
        }
    }
}
