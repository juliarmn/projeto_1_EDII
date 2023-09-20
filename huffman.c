#include "huffman.h"

// INicializa a tabela de frequencias para todo valor ser 0
void inicializar_freq(unsigned int tabela[])
{
    for (int i = 0; i < MAX_CHAR; i++)
    {
        tabela[i] = 0;
    }
}

/**
 * Preenche as frequencias de cada letra no arquivo em sua devida posicao na tabela baseado em ASCII
 */
void preenche_tabela(FILE *filmes, unsigned int tabela[])
{
    int i;
    char buffer[TAMANHO_REGISTRO + 1];
    fseek(filmes, 0, SEEK_SET);
    while (!feof(filmes))
    {
        if (fgets(buffer, TAMANHO_REGISTRO + 1, filmes) == NULL)
            return;

        i = 0;

        while (buffer[i] != '\0')
        {
            tabela[buffer[i]]++; // Incrementa o valor da posicao da letra encontrada
            i++;
        }
    }
}

void imprimir_tabela_freq(unsigned int tabela[])
{
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (tabela[i] != 0)
            printf("%c: %u\n", i, tabela[i]);
    }
}

/**
 * Inicializa a lista como vazia
 */
void inicializar_lista(list **lista)
{
    (*lista)->inicio = NULL;
    (*lista)->tamanho = 0;
}

/**
 * Faz insercao ordenada na lista de acordo com a frequencia
 */
void inserir_ordenado(list **lista, node *no)
{
    if (!(*lista)->inicio) // Primeiro elemento
    {
        (*lista)->inicio = no;
        (*lista)->tamanho++;
        return;
    }

    if (no->frequencia < (*lista)->inicio->frequencia) // Atualizar cabeca da lista
    {
        no->prox = (*lista)->inicio;
        (*lista)->inicio = no;
        (*lista)->tamanho++;
        return;
    }

    node *aux = (*lista)->inicio;

    while (aux->prox && (aux->prox->frequencia < no->frequencia)) // Anda a arvore ate achar a posicao para inserir o no
    {
        aux = aux->prox;
    }

    if (aux->prox) // insere o proximo se estiver no meio
    {
        no->prox = aux->prox;
        aux->prox = no;
        (*lista)->tamanho++;
        return;
    }

    aux->prox = no; // insere se estiver no final
    (*lista)->tamanho++;

    no->prox = NULL;
}

/**
 * Preenche a lista com a tabela -> letra e frequencia
 */
void preencher_lista(unsigned int tabela[], list *lista)
{
    node *novo;
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (tabela[i] > 0)
        {   // Preenche a lista de acordo com as frequencias -> ordena por frequencia
            novo = malloc(sizeof(node));
            if (!novo)
            {
                printf("\033[31mErro ao criar novo no\033[0m\n");
                return;
            }
            novo->caracter = i;
            novo->frequencia = tabela[i];
            novo->dir = novo->esq = novo->prox = NULL;
            inserir_ordenado(&lista, novo);
        }
    }
}

void imprimir_lista(list *lista)
{
    node *aux = lista->inicio;
    while (aux)
    {
        printf("Caracter: %c | frequencia: %u\n", aux->caracter, aux->frequencia);
        aux = aux->prox;
    }
}

// Menor freq sempre no inicio
/**
 * Remove sempre o primeiro no da lista
 */
node *remover_no_inicio(list *lista)
{ // Remove sempre o inicioi por ser o de menor  frequencia
    node *aux = NULL;
    if (lista->inicio)
    {
        aux = lista->inicio;
        lista->inicio = aux->prox;
        aux->prox = NULL;
        lista->tamanho--;
    }
    return aux;
}

// Árvore de Huffman
/**
 * Remove o no de menor frequencia da lista e une sempre os dois com menores frequencias, fazendo um no com frequencia resultante
 * equivalente a soma entre a frequencia deles
 * Faz isso ate unnir em 1
 */
node *montar_arvore_huffman(list *lista)
{
    node *prim, *seg, *novo;
    while (lista->tamanho > 1)
    {
        prim = remover_no_inicio(lista); // elementos removidos que virarao novo no  da arvore com frequencia somada
        seg = remover_no_inicio(lista);
        novo = malloc(sizeof(node));
        if (!novo)
        {
            printf("\033[31mErro ao criar novo nó\033[0m\n");
            return NULL;
        }
        novo->caracter = ' ';
        novo->frequencia = prim->frequencia + seg->frequencia;
        novo->esq = prim;
        novo->dir = seg;
        novo->prox = NULL;
        inserir_ordenado(&lista, novo);
    }

    return lista->inicio;
}

void imprimir_arvore(node *raiz, int tamanho)
{
    if (!raiz->esq && !raiz->dir)
    {
        printf("Folha: %c | altura: %d\n", raiz->caracter, tamanho);
    }
    else
    {
        imprimir_arvore(raiz->esq, tamanho++);
        imprimir_arvore(raiz->dir, tamanho++);
    }
}

// dicionário

int altura_da_árvore(node *raiz)
{
    if (!raiz)
    {
        return -1;
    }
    int esq, dir;
    esq = altura_da_árvore(raiz->esq) + 1;
    dir = altura_da_árvore(raiz->dir) + 1;
    if (esq > dir)
    {
        return esq;
    }
    return dir;
}

/**
 * Aloca dicionario vazio
 */
char **alocar_dicionario(int coluna)
{
    char **dicionario;

    dicionario = malloc(sizeof(char *) * MAX_CHAR);
    for (int i = 0; i < MAX_CHAR; i++)
    {
        dicionario[i] = calloc(coluna, sizeof(char));
    }
    return dicionario;
}

/**
 * De acordo com a posicao na arvore, cria o dicionario, que e a representacao binaria nova que cada letra tera
 * Isso e feito precorrendo a arvore
 */
void gerar_dicionario(char **dicionario, node *raiz, char *caminho, int colunas)
{
    char esq[colunas], dir[colunas];

    if (!raiz->esq && !raiz->dir)
    {
        strcpy(dicionario[raiz->caracter], caminho);
    }
    else
    {
        strcpy(esq, caminho);
        strcpy(dir, caminho);
        strcat(esq, "0"); // defini esquerda 0 
        strcat(dir, "1"); // direita 1

        gerar_dicionario(dicionario, raiz->esq, esq, colunas);
        gerar_dicionario(dicionario, raiz->dir, dir, colunas);
    }
}

void imprime_dicionario(char **dicionario)
{
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (strlen(dicionario[i]) > 0)
            printf("%3d: %s", i, dicionario[i]);
    }
}

/**
 * Calcula o tamanho da string do dicionario -> tamanho da string de binarios
 */
int calcula_tamanho_string(char **dicionario, unsigned char *texto)
{
    int i = 0, tam = 0;
    while (texto[i] != '\0')
    {
        tam = tam + strlen(dicionario[texto[i]]);
        i++;
    }
    return tam + 1;
}

/**
 * Faz a codificacao do codigo de cada letra de acordo com dicionario
 */
char *codificar_huffman(char **dicionario, unsigned char *texto)
{
    int tam = calcula_tamanho_string(dicionario, texto);
    int i = 0;
    char *codigo = calloc(MAX_CHAR, sizeof(char) * tam);

    while (texto[i] != '\0')
    {
        strcat(codigo, dicionario[texto[i]]); // Gera o binario a partir do  dicionario
        i++;
    }
    codigo[i] = '\0';

    return codigo;
}

char *decodificar(unsigned char *texto, node *raiz)
{
    int i = 0;
    char temp[2];
    char *decodificado = calloc(strlen(texto), sizeof(char));
    node *aux = raiz;
    while (texto[i] != '\0')
    {
        if (texto[i] == '0')
        {
            aux = aux->esq;
        }
        else
        {
            aux = aux->dir;
        }

        if (!raiz->esq && !raiz->dir)
        {
            temp[0] = aux->caracter;
            temp[1] = '\0';
            strcat(decodificado, temp);
            aux = raiz;
        }

        i++;
    }
}

/**
 * Faz a compactacao de acordo com o dicionario, movendo bit a bit para formar uma letra
 */
bool compactar_arquivo_huffman(FILE **filmes, char **dicionario, int colunas, node *raiz)
{
    FILE *aux_file = fopen("aux.dat", "wb");

    if (!aux_file)
        return false;

    unsigned char buffer[TAMANHO_REGISTRO + 1];
    unsigned char byte = 0; // Inicializa o byte
    int bit_count = 0;      // Contador de bits no byte atual
    char *texto_codificado;

    fseek(*filmes, 0, SEEK_SET);

    gerar_dicionario(dicionario, raiz, "", colunas);
    imprime_dicionario(dicionario);
    while (fgets(buffer, TAMANHO_REGISTRO + 1, *filmes) != NULL)
    {
        // Codificar a linha usando Huffman
        texto_codificado = codificar_huffman(dicionario, buffer);

        // Percorrer os bits codificados
        for (int i = 0; texto_codificado[i] != '\0'; i++)
        {

            int bit = texto_codificado[i] - '0';
            // printf("\nbit: %d", bit);

            // Adicionar o bit ao byte atual
            byte = (byte << 1) | bit;
            bit_count++;

            // Se tem 8 bits no byte, escreve no arquivo
            if (bit_count == 8)
            {
                printf("\nletra: %c", byte);
                fwrite(&byte, sizeof(unsigned char), 1, aux_file);
                byte = 0;
                bit_count = 0;
            }
        }
    }

    // Escrever bits restantes
    if (bit_count > 0)
    {
        fwrite(&byte, sizeof(unsigned char), 1, aux_file);
    }

    fclose(aux_file);

    FILE *novo_filmes = freopen("movies.dat", "wb", *filmes);

    if (!novo_filmes)
    {
        return false;
    }

    *filmes = novo_filmes; // Atualiza o ponteiro original de filmes

    aux_file = fopen("aux.dat", "rb");

    if (!aux_file)
    {
        return false;
    }

    int c;
    while ((c = fgetc(aux_file)) != EOF)
    {
        fputc(c, *filmes);
    }

    fclose(aux_file);

    if (remove("aux.dat"))
    {
        return false;
    }

    return true;
}
