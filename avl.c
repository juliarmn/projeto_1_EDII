#include "avl.h"

int maior(int a, int b) { return (a > b) ? a : b; }

// Faz alocacao  do  novo no
indice_primario *novo_no(int offset, char *cod)
{
    indice_primario *raiz = malloc(sizeof(indice_primario));

    if (!raiz)
    {
        printf("Sem espaço\n");
        return NULL;
    }
    int len = sizeof(raiz->codigo);
    strncpy(raiz->codigo, (const char *)cod, sizeof(raiz->codigo)); // Evitar buffer overflow
    raiz->codigo[len - 1] = '\0';

    raiz->offset = offset;
    raiz->direita = NULL;
    raiz->esquerda = NULL;
    raiz->altura = 0;
    return raiz;
}

// Calcula a leitura por recursao
int altura(indice_primario *no)
{ 
    if (!no)
        return 0;

    int esq = altura(no->esquerda);
    int dir = altura(no->direita);

    if (esq > dir)
        return esq + 1;
    return dir + 1;
}

// Verifica a diferenca de altura para balanceamento
int diferenca_altura(indice_primario *no)
{
    if (!no)
        return 0;
    int altura_esq = altura(no->esquerda);
    int altura_dir = altura(no->direita);
    return (altura_esq - altura_dir);
}

// Devolve o menor dos maiores para balanceamento
indice_primario *menor_no(indice_primario *no)
{
    indice_primario *aux = no;
    while (aux->esquerda)
    {
        aux = aux->esquerda;
    }
    return aux;
}

// Rotaciona caso tenhamos uma diagonal secundaria
indice_primario *rotacao_dir(indice_primario *no)
{
    indice_primario *esq = no->esquerda;
    indice_primario *dir = esq->direita;

    esq->direita = no;
    no->esquerda = dir;

    no->altura = (maior(altura(no->esquerda), altura(no->direita)) + 1);
    esq->altura = (maior(altura(esq->esquerda), altura(esq->direita)) + 1);

    return esq;
}

// Rotaciona no caso de ser uma rotacao em uma diagonal principal
indice_primario *rotacao_esq(indice_primario *no)
{
    indice_primario *dir = no->direita;
    indice_primario *esq = dir->esquerda;

    dir->esquerda = no;
    no->direita = esq;

    no->altura = (maior(altura(no->esquerda), altura(no->direita)) + 1);
    dir->altura = (maior(altura(dir->esquerda), altura(dir->direita)) + 1);

    return dir;
}

indice_primario *rotacao_esq_dir(indice_primario *no)
{
    no->esquerda = rotacao_esq(no);
    return (rotacao_dir(no));
}

indice_primario *rotacao_dir_esq(indice_primario *no)
{
    no->direita = rotacao_dir(no);
    return (rotacao_esq(no));
}

// Insercao AVL 
indice_primario *inserir_avl(indice_primario *raiz, char *cod, int offset)
{
    if (!raiz) // VVerifica se ja chegou no fim da arvore ou caso esteja vazia, insere
    {
        return novo_no(offset, cod);
    }

    if (strcmp(cod, raiz->codigo) < 0)
    {
        raiz->esquerda = inserir_avl(raiz->esquerda, cod, offset); // Percorre a arvore de acordo com o valor
    }
    else
    {
        raiz->direita = inserir_avl(raiz->direita, cod, offset);
    }

    raiz->altura = 1 + maior(altura(raiz->esquerda), altura(raiz->direita)); // Varifica a altura desse no

    int diferenca = diferenca_altura(raiz);
    // Diferenca de altura define o fator de balanceeamento
    if (diferenca > 1 && strcmp(cod, raiz->esquerda->codigo) < 0)
    {
        return rotacao_dir(raiz);
    }
    if (diferenca > 1 && strcmp(cod, raiz->esquerda->codigo) > 0)
    {
        raiz->esquerda = rotacao_esq(raiz->esquerda);
        return rotacao_dir(raiz);
    }
    if (diferenca < -1 && strcmp(cod, raiz->direita->codigo) > 0)
    {
        return rotacao_esq(raiz);
    }
    if (diferenca < -1 && strcmp(cod, raiz->direita->codigo) < 0)
    {
        raiz->direita = rotacao_dir(raiz->direita);
        return rotacao_esq(raiz);
    }

    return raiz;
}

// Busca o menor dos maiores pra balancear
indice_primario *menor_direita(indice_primario *raiz)
{
    indice_primario *busca = raiz;

    while (busca->esquerda)
        busca = busca->esquerda;

    return busca;
}

// Remove recursivamente um indice
indice_primario *remover_indice(indice_primario *raiz, char *cod, FILE *filmes)
{
    if (!raiz)
        return raiz;

    if (strcmp(cod, raiz->codigo) < 0)
        raiz->esquerda = remover_indice(raiz->esquerda, cod, filmes);

    else if (strcmp(cod, raiz->codigo) > 0)
        raiz->direita = remover_indice(raiz->direita, cod, filmes);

    else
    {
        if (!(raiz->esquerda) || !(raiz->direita))
        {
            indice_primario *aux = raiz->esquerda ? raiz->esquerda : raiz->direita;

            if (!aux)
            {
                aux = raiz;
                raiz = NULL;
            }
            else
            {
                *raiz = *aux;
            }
            free(aux);
        }
        else
        {
            indice_primario *aux = menor_direita(raiz->direita);
            strcpy(raiz->codigo, aux->codigo);
            raiz->offset = aux->offset;
            raiz->direita = remover_indice(raiz->direita, aux->codigo, filmes);
        }
    }

    if (!raiz)
        return raiz;

    raiz->altura = maior(altura(raiz->esquerda), altura(raiz->direita) + 1);

    int diferenca = diferenca_altura(raiz);

    if (diferenca < -1 && diferenca_altura(raiz->direita) <= 0)
        return rotacao_esq(raiz);
    if (diferenca > 1 && diferenca_altura(raiz->esquerda) >= 0)
        return rotacao_dir(raiz);

    if (diferenca > 1 && diferenca_altura(raiz->esquerda) < 0)
    {
        raiz->esquerda = rotacao_esq(raiz->esquerda);
        return rotacao_dir(raiz);
    }
    if (diferenca < -1 && diferenca_altura(raiz->direita) > 0)
    {
        raiz->direita = rotacao_dir(raiz->direita);
        return rotacao_esq(raiz);
    }

    return raiz;
}
// BUsca o elemento na AVL avaliando seu valor, que compara se e maior ou menor que o no atual e percorre a arvore
indice_primario *busca_avl(indice_primario *raiz, char *info)
{
    if (!raiz)
        return NULL;

    if (!strcmp(info, raiz->codigo))
        return raiz;

    if (strcmp(info, raiz->codigo) < 0)
        return busca_avl(raiz->esquerda, info);
    else
        return busca_avl(raiz->direita, info);
}

// Função para imprimir os nós da árvore em ordem
void imprimir_em_ordem(indice_primario *raiz, FILE *pri_index)
{
    if (raiz != NULL)
    {
        imprimir_em_ordem(raiz->esquerda, pri_index);
        fprintf(pri_index, "%s@%d\n", raiz->codigo, raiz->offset);
        imprimir_em_ordem(raiz->direita, pri_index);
    }
}

// Destroi a AVL para sua reconstrucao
indice_primario *destruir_avl(indice_primario *raiz)
{
    if (raiz)
    {
        raiz->esquerda = destruir_avl(raiz->esquerda);
        raiz->direita = destruir_avl(raiz->direita);
        free(raiz);
    }
    return NULL;
}