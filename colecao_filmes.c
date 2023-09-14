#include "avl.c"
#include "lista.c"
#include "huffman.c"
#define MAX 100

// Declaração de structs

typedef struct film
{
    char codigo[6];
    char titulo_original[56];
    char titulo_pt[58];
    char diretor[40];
    char ano[5];
    char pais[21];
    int nota;
    char padding[2]; // Para o que sobrou, preenche com @, pois não dava 192 bytes
} filme;

// ==================================================================================================

// Verificações:

/**
 * Gera codigo primario a partir do nome do diretor e o ano. (3 ultimos caracteres do nome e 2 ultimos do ano)
 * Recebe tambem a string que ele vai modificar (por tipos incompativeis, ele  nao retorna char*)
 */
void gerar_codigo(char *ano, char *nome_dir, char *cod)
{
    int i;
    int tam = strlen(nome_dir);
    int k = 0;

    for (i = 0; i < 3; i++)
    {
        cod[k] = toupper(nome_dir[i]);
        k++;
    }

    cod[3] = ano[2];
    cod[4] = ano[3];
}

/**
 * Verifica a integridade do arquivo se ele existir -> os arquivos de indice
 * Verifica se tem 0 ou 1 no cabeçalho
 */
bool verifica_integridade(FILE *arquivo)
{
    char linha[2];
    if (fgets(linha, sizeof(linha), arquivo))
    {
        int num = atoi(linha); // Transforma o char lido em int
        if (num)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        printf("\033[31m\t\tNAO INTEGRO\033[0m\n");
        return false;
    }
}

// ================================================================================================


// Funções de pegar dados do usuário:

/**
 * Funcao que pega os dados do usuario para inserir no arquivo e chama a funcao para gerar codigo
 * Devolve (return) a posicao para colocar no offset
 * recebe o char*indice_prim para modifica-lo dentro da funcao
 */
int escreve_arq_filme(FILE *filmes, char *indice_prim, char *titulo)
{
    filme f;
    printf("\n\033[95m*****************************************************\33[0m");
    printf("\n\t\t\033[32mInsira os dados: \n");

    printf("\33[95mTitulo pt: \33[0m");
    scanf(" %58[^\n]s", f.titulo_pt);
    while (getchar() != '\n')
        ; // Limpar buffer de entrada
    printf("\33[95mTitulo original: \33[0m");
    scanf(" %56[^\n]s", f.titulo_original);
    while (getchar() != '\n')
        ; // Limpar buffer de entrada
    strcpy(titulo, f.titulo_original);
    printf("\33[95mDiretor (sobrenome, nome): \33[0m");
    scanf(" %40[^\n]s", f.diretor);
    while (getchar() != '\n')
        ; // Limpar buffer de entrada
    printf("\33[95mDigite o ano: \33[0m");
    scanf(" %s", f.ano);
    printf("\33[95mDigite o país: \33[0m");
    scanf(" %20[^\n]s", f.pais);
    while (getchar() != '\n')
        ; // Limpar buffer de entrada
    printf("\33[95mDigite a nota: \33[0m");
    scanf("%d", &f.nota);
    if (f.nota < 0 || f.nota > 9)
    {
        do
        {
            printf("\33[95mDigite uma nota valida: \33[0m");
            scanf("%d", &f.nota);
        } while (f.nota < 0 || f.nota > 9);
    }
    gerar_codigo(f.ano, f.diretor, f.codigo);
    strcpy(indice_prim, f.codigo);
    fseek(filmes, 0, SEEK_END);
    int posicao = ftell(filmes);
    fprintf(filmes, "%s@%s@%s@%s@%s@%s@%d@",
            f.codigo, f.titulo_pt, f.titulo_original, f.diretor, f.ano, f.pais, f.nota); // coloca os dados no  arquivo

    int tamanho_campos_texto = strlen(f.codigo) + strlen(f.titulo_pt) + strlen(f.titulo_original) +
                               strlen(f.diretor) + strlen(f.pais) + 7 + strlen(f.ano); // 7 caracteres @

    // int tamanho_fixo =  sizeof(f.nota);

    int tamanho_total = tamanho_campos_texto + 1;

    int qtd_hashtag = TAMANHO_REGISTRO - tamanho_total; // Achar quantos # precisam ser colocadas

    for (int i = 0; i < qtd_hashtag; i++)
    {
        fprintf(filmes, "#");
    }

    return posicao;
}

// ==================================================================================

// Funções de interação
/*
1. Inserir um novo filme no catalogo.
2. Remover um filme a partir da chave primaria.
3. Modificar o campo nota de um filme a partir da chave primaria.
4. Buscar filmes a partir da (a) chave prim ́aria ou do (b) t ́ıtulo em portuguˆes.
5. Listar todos os filmes no cat ́alogo.
*/

/**
 * Funcao que exibe o menu na tela e devolve a opcao que o usuario escolher
 */
int menu()
{
    int op;

    do
    {
        printf("\n\033[95m*\033[36m-\033[95m*\033[36m-\033[95m*\033[36m-\033[95m*\033[36m-\033[95m*\033[36m-\033[95m*\033[36m-\033[95m*\033[36m-\033[95m*\033[36m-\033[0m   LOCADORA   \033[95m*\033[36m-\033[95m*\033[36m-\033[95m*\033[36m-\033[95m*\033[36m-\033[95m*\033[36m-\033[95m*\033[36m-\033[95m*\033[36m-\033[95m*\033[36m-\n");
        printf("\033[95m-------------------------------------------------------\n\n");
        printf("\033[35m1)\033[0m  Inserir novo filme no catalogo\n");
        printf("\033[35m2)\033[0m  Remover filme a partir de chave primaria\n");
        printf("\033[35m3)\033[0m  Modificar a nota de um filme a partir da chave primaria\n");
        printf("\033[35m4)\033[0m  Buscar filme por chave primaria\n");
        printf("\033[35m5)\033[0m  Buscar filme por chave secundaria\n");
        printf("\033[35m6)\033[0m  Listar todos os filmes do catalogo\n");
        printf("\033[35m7)\033[0m  Compactar\n");
        printf("\033[35m0)\033[0m  Encerrar\n");
        printf("\033[32mDigite sua opcao: \33[0m");
        scanf("%d", &op);

        if (op < 0 || op > 7)
        {
            printf("\t\t  \033[31mOpcao invalida\033[0m\n");
        }
    } while (op < 0 || op > 7);

    return op;
}

// ===================================================================================

// 1)  Inserção

/**
 * Chama a funcao para escrever no arquivo de filmes e insere os dados na AVL e lista invertida
 */
indice_primario *cadastro_filmes(indice_primario *raiz, indice_sec *lista_sec, int *head, int *ppl, FILE *filmes, primarios_lista *lista)
{
    char indice_prim[6];
    char titulo[58];
    int posicao = escreve_arq_filme(filmes, indice_prim, titulo);
    indice_primario *aux = inserir_avl(raiz, indice_prim, posicao);

    if (aux == NULL)
    {
        printf("\t\t\033[31mErro ao inserir indice primario\033[0m\n");
    }
    int i;
    for (i = 0; lista[i].id_prim[0] != '\0'; i++)
        ;
    strcpy(lista[i].id_prim, indice_prim);

    int posicao_sec = buscar_lista_sec(*head, lista_sec, titulo); // Para verificar se ja existe esse indice secundario ou nao

    // INsere na lista invertida de acordo com o resultado da posicao_sec
    if (posicao_sec == -1)
    {
        inserir_secundario(head, ppl, lista_sec, titulo, i);
        lista[i].prox = -1;
    }
    else
    {
        lista[i].prox = lista_sec[posicao_sec].posicao_primario;
        lista_sec[posicao_sec].posicao_primario = i;
    }
    return aux;
}

// ===================================================================================

// Remoção

/**
 * Remove os indices da AVL e da lista secundaria
 * Coloca o separador para remocao do  arquivo
 * retorna a nova raiz modificada da arvore
 */
indice_primario *remover_filme(indice_sec *lista_sec, primarios_lista *lista_invertida, int *head, indice_primario *raiz, char *chave_primaria, FILE *filmes)
{
    indice_primario *aux = busca_avl(raiz, chave_primaria);
    if (!aux)
    {
        printf("\033[31m \t\tFILME NAO ENCONTRADO\n\033[0m");
        return raiz;
    }
    int offset = aux->offset;
    indice_primario *nova_raiz = remover_indice(raiz, chave_primaria, filmes);
    remocao_lista_invertida(lista_sec, lista_invertida, chave_primaria, head);

    fseek(filmes, offset, SEEK_SET);
    fprintf(filmes, "*|");
    printf("\033[31m \t\tFILME REMOVIDO <3\n\033[0m");
    return nova_raiz;
}

// ===================================================================================

// Buscas

/**
 * Buscar o indice primario na AVL e encontrar seu offset, visando buscar ele no arquivo e devolver o registro encontrado
 */
filme *buscar_prim(indice_primario *raiz, char *indice_prim, FILE *arquivo_filme)
{
    indice_primario *primario;
    primario = busca_avl(raiz, indice_prim);
    filme *registro_filme = malloc(sizeof(filme));

    if (primario == NULL)
    {
        printf("Erro\n");
        return NULL;
    }
    fseek(arquivo_filme, primario->offset, SEEK_SET);

    if (!feof(arquivo_filme))
    {
        fscanf(arquivo_filme, " %[^@^#]s", registro_filme->codigo);
        fscanf(arquivo_filme, "%c", &registro_filme->padding[0]);
        fscanf(arquivo_filme, " %[^@^#]s", registro_filme->titulo_pt);
        fscanf(arquivo_filme, "%c", &registro_filme->padding[0]);
        fscanf(arquivo_filme, " %[^@^#]s", registro_filme->titulo_original);
        fscanf(arquivo_filme, "%c", &registro_filme->padding[0]);
        fscanf(arquivo_filme, " %[^@^#]s", registro_filme->diretor);
        fscanf(arquivo_filme, "%c", &registro_filme->padding[0]);
        fscanf(arquivo_filme, "%[^@^#]s", registro_filme->ano);
        fscanf(arquivo_filme, "%c", &registro_filme->padding[0]);
        fscanf(arquivo_filme, " %[^@^#]s", registro_filme->pais);
        fscanf(arquivo_filme, "%c", &registro_filme->padding[0]);
        fscanf(arquivo_filme, "%d", &registro_filme->nota);

        char c;
        while (c = fgetc(arquivo_filme) == '#') // remover #
            ;

        return registro_filme;
    }
}

/**
 * Busca pelo indice secundario na lista secundario e acha seu link com a lista invertida, encontrando o offset e buscando por ele no arquivo
 */
void buscar_sec(indice_sec *lista_sec, primarios_lista *lista_invertida, int head, char *titulo, FILE *arquivo_filme, indice_primario *raiz)
{
    int posicao = buscar_lista_secundaria_para_operacao(head, lista_sec, lista_invertida, titulo);
    filme *f;
    int k;

    if (posicao != -1)
    {
        int i = posicao;

        do
        {
            f = buscar_prim(raiz, lista_invertida[i].id_prim, arquivo_filme);
            if (!f)
            {
                printf("lista invertida: %s \n", lista_invertida[i].id_prim);
                printf("\033[31m\t\tNão encontrado ou erro\n");
                return;
            }
            printf("\033[95m*************** Dados do filme: *************\n");
            printf("\033[95mCodigo: \033[0m%s\n", f->codigo);
            printf("\033[95mTitulo original: \033[0m%s\n", f->titulo_original);
            printf("\033[95mTitulo português:\033[0m %s\n", f->titulo_pt);
            printf("\033[95mDiretor: \033[0m%s\n", f->diretor);
            printf("\033[95mAno: \033[0m%s\n", f->ano);
            printf("\033[95mPais:\033[0m %s\n", f->pais);
            printf("\033[95mnota: \033[0m%d\n", f->nota);
            printf("\033[95m****************************************************************************\033[0m\n\n");
            i = lista_invertida[i].prox;
        } while (i != -1);
    }
    else
    {
        printf("Titulo invalido\n");
    }
}

// ===================================================================================

// Listagem de filmes do catálogo

/**
 * Mostra todos os filmes do catalogo, ignorando os com caractere de remocao e separando campo por campo a partir do @
 */
void listar_catalogo(FILE *arquivo_filmes)
{
    filme f;
    char c;
    char *campo;
    char *delim = "@";
    rewind(arquivo_filmes);
    char buffer[TAMANHO_REGISTRO + 1];
    printf("\n\033[95m******************\033[0m Lista de filmes \033[95m******************\033[0m\n");
    printf("%d", (int)ftell(arquivo_filmes));
    while (!feof(arquivo_filmes))
    {
        if (fgets(buffer, TAMANHO_REGISTRO + 1, arquivo_filmes) == NULL)
        {
            printf("\t\t       \033[31mFIM DO ARQUIVO");
            return;
        }

        if (buffer[0] == '*' && buffer[1] == '|')
        {
            continue;
        }

        // Inicializar os campos do struct
        memset(&f, 0, sizeof(filme));

        campo = strtok(buffer, delim); // Separa os campos a partir do @

        strcpy(f.codigo, campo);
        campo = strtok(NULL, delim);
        strcpy(f.titulo_pt, campo);
        campo = strtok(NULL, delim);
        strcpy(f.titulo_original, campo);
        campo = strtok(NULL, delim);
        strcpy(f.diretor, campo);
        campo = strtok(NULL, delim);
        strcpy(f.ano, campo);
        campo = strtok(NULL, delim);
        strcpy(f.pais, campo);
        campo = strtok(NULL, delim);
        f.nota = atoi(campo); // Transforma em int

        printf("\033[95mCodigo: \033[0m%s\n", f.codigo);
        printf("\033[95mTitulo original: \033[0m%s\n", f.titulo_original);
        printf("\033[95mTitulo portugues: \033[0m%s\n", f.titulo_pt);
        printf("\033[95mDiretor: \033[0m%s\n", f.diretor);
        printf("\033[95mAno: \033[0m%s\n", f.ano);
        printf("\033[95mPais: \033[0m%s\n", f.pais);
        printf("\033[95mNota: \033[0m%d\n", f.nota);
        printf("\033[95m*****************************************************\33[0m\n");
    }
}

// ===================================================================================

// Modificar a nota por chave primaria

/**
 * Modifica a nota do registro correspondente, buscando por chave primaria na AVL
 * Com offset, encontra o registro, conta 6 @ e acha a notta e muda
 */
bool modificar_nota(FILE *arquivo_filmes, char *primario, int nova_nota, indice_primario *raiz)
{
    indice_primario *indice = busca_avl(raiz, primario);
    filme f;
    int count = 0;
    char caractere;

    if (indice != NULL)
    {
        fseek(arquivo_filmes, indice->offset, SEEK_SET);
        while ((caractere = fgetc(arquivo_filmes)) != EOF && count < 6)
        {
            if (caractere == '@')
            {
                count++;

                if (count == 6)
                {
                    fprintf(arquivo_filmes, "%d", nova_nota);
                    fflush(arquivo_filmes);
                    return true;
                }
            }
        }
    }
    else
    {
        printf("Índice não encontrado\n");
        return false;
    }
}

// ===================================================================================

// Encerramento

/**
 * Encerra os arquivos primario e secundario salvando dados a AVL e lista invertida
 */
void encerrar_programa(FILE *primario, FILE *secundario, indice_sec *lista_sec, int *head, primarios_lista *lista_invertida, indice_primario *raiz)
{
    fclose(secundario);
    fclose(primario);
    primario = fopen("iprimary.idx", "w");
    secundario = fopen("ititle.idx", "w");
    fprintf(primario, "1\n");
    fprintf(secundario, "1\n");
    int secundario_impresso = 0;

    imprimir_em_ordem(raiz, primario);

    int j = *head;
    int k;
    if (*head == -1)
    {
        return;
    }

    do
    {
        k = lista_sec[j].posicao_primario;
        if (lista_invertida[k].id_prim[0] != '\0')
        {
            fprintf(secundario, "%s", lista_sec[j].titulo_original);
            secundario_impresso = 1;
        }
        do
        {
            if (lista_invertida[k].id_prim[0] == '\0')
            {
                break;
            }

            fprintf(secundario, "@%s", lista_invertida[k].id_prim);
            k = lista_invertida[k].prox;
        } while (k != -1);
        if (secundario_impresso)
            fprintf(secundario, "\n");
        j = lista_sec[j].proximo;
        secundario_impresso = 0;
    } while (j != -1);
}

// ===================================================================================

// Funções arquivo

/**
 * Abertuta dos arquivos de indices, verificando se ja existem ou nao para definir o tipo de abertura
 */
FILE *abrir_arquivo_indices(char *nome, bool *flag)
{
    FILE *arquivo = fopen(nome, "r+a");

    if (arquivo == NULL)
    {
        arquivo = fopen(nome, "w+");
    }
    else
    {
        *flag = verifica_integridade(arquivo);
    }
    return arquivo;
}

/**
 * Le o arquivo de indice primario e salva seus dados na AVL se ele ja existir e nao estiver integro
 */
void le_e_grava_indice(indice_primario **raiz, FILE *pri_index, FILE *filmes)
{
    int count = 0;
    filme f;
    char buffer[TAMANHO_REGISTRO + 1];
    char *campo;
    char *delim = "@";
    int i = 0;
    fseek(filmes, 0, SEEK_SET);
    fprintf(pri_index, "1\n");
    while (!feof(filmes))
    {
        i = ftell(filmes);
        if (fgets(buffer, TAMANHO_REGISTRO + 1, filmes) == NULL)
        {
            printf("\t\t       \033[31mFIM DO ARQUIVO");
        }
        else
        {
            if (buffer[0] == '*' && buffer[1] == '|')
            {
                continue;
            }

            // Inicializar os campos do struct
            memset(&f, 0, sizeof(filme));

            campo = strtok(buffer, delim);

            strcpy(f.codigo, campo);
            campo = strtok(NULL, delim);
            strcpy(f.titulo_pt, campo);
            campo = strtok(NULL, delim);
            strcpy(f.titulo_original, campo);
            campo = strtok(NULL, delim);
            strcpy(f.diretor, campo);
            campo = strtok(NULL, delim);
            strcpy(f.ano, campo);
            campo = strtok(NULL, delim);
            strcpy(f.pais, campo);
            campo = strtok(NULL, delim);
            f.nota = atoi(campo);

            *raiz = inserir_avl(*raiz, f.codigo, i);
            count++;
        }
    }
    printf("%d", count);
    imprimir_em_ordem(*raiz, pri_index);
}

/**
 * Se o arquivo de indices secundarios ja existir e nao estiver integro, salvar na lista invertida seus dados a partir do arquivo de filmes
 */
void grava_indice_sec(indice_sec *lista_sec, primarios_lista *lista_invertida, FILE *indice_sec, FILE *filmes, int *head, int *ppl)
{
    filme f;
    char *campo;
    char *delim = "@";
    char buffer[TAMANHO_REGISTRO + 1];

    fseek(filmes, 0, SEEK_SET);
    fprintf(indice_sec, "1\n");

    while (!feof(filmes))
    {
        if (fgets(buffer, TAMANHO_REGISTRO + 1, filmes) == NULL)
        {
            printf("\t\t       \033[31mFIM DO ARQUIVO");
            return;
        }

        if (buffer[0] == '*' && buffer[1] == '|')
        {
            continue;
        }

        // Inicializar os campos do struct
        memset(&f, 0, sizeof(filme));

        campo = strtok(buffer, delim);

        strcpy(f.codigo, campo);
        campo = strtok(NULL, delim);
        strcpy(f.titulo_pt, campo);
        campo = strtok(NULL, delim);
        strcpy(f.titulo_original, campo);
        campo = strtok(NULL, delim);
        strcpy(f.diretor, campo);
        campo = strtok(NULL, delim);
        strcpy(f.ano, campo);
        campo = strtok(NULL, delim);
        strcpy(f.pais, campo);
        campo = strtok(NULL, delim);
        f.nota = atoi(campo);
        int i;
        for (i = 0; lista_invertida[i].id_prim[0] != '\0'; i++)
            ;
        strcpy(lista_invertida[i].id_prim, f.codigo);

        int posicao_sec = buscar_lista_sec(*head, lista_sec, f.titulo_original);

        if (posicao_sec == -1)
        {
            inserir_secundario(head, ppl, lista_sec, f.titulo_original, i);
            lista_invertida[i].prox = -1;
        }
        else
        {
            lista_invertida[i].prox = lista_sec[posicao_sec].posicao_primario;
            lista_sec[posicao_sec].posicao_primario = i;
        }
    }

    int j = *head; // arrumar
    int k = lista_sec[j].posicao_primario;

    do
    {
        fprintf(indice_sec, "%s|", lista_sec[j].titulo_original);
        k = lista_sec[j].posicao_primario;
        do
        {
            fprintf(indice_sec, "%s|", lista_invertida[k].id_prim);
            k = lista_invertida[k].prox;
        } while (lista_invertida[k].prox != -1);
        fprintf(indice_sec, "\n");
        j = lista_sec[j].proximo;
    } while (lista_sec[j].proximo != -1);
}

/**
 * Refaz a lista secundaria e invertida caso esteja integra, salvando os dados na estrutura
 */
void salva_lista_secundaria(indice_sec *lista_sec, primarios_lista *lista_invertida, int *ppl, int *head, FILE *sec_index)
{
    char nome[56];
    char titulo[56];
    char buffer[TAMANHO_REGISTRO + 1];
    char *cod;
    char prim[6];
    int pos;
    char *delim = "@";
    fgetc(sec_index);

    while (fgets(buffer, TAMANHO_REGISTRO + 1, sec_index) != NULL)
    {

        if (strcmp(buffer, "\n") != 0)
        {
            char *posicao_arroba = strchr(buffer, '@');

            cod = strtok(buffer, delim);
            char *titulo = strdup(cod);
            cod = strtok(NULL, delim);
            while (cod)
            {
                strcpy(prim, cod);
                if (prim[strlen(prim) - 1] == '\n')
                    prim[strlen(prim) - 1] = '\0';
                int i;
                for (i = 0; lista_invertida[i].id_prim[0] != '\0'; i++)
                    ;
                strcpy(lista_invertida[i].id_prim, prim);

                int posicao_sec = buscar_lista_sec(*head, lista_sec, titulo);

                if (posicao_sec == -1)
                {
                    inserir_secundario(head, ppl, lista_sec, titulo, i);
                    lista_invertida[i].prox = -1;
                }
                else
                {
                    lista_invertida[i].prox = lista_sec[posicao_sec].posicao_primario;
                    lista_sec[posicao_sec].posicao_primario = i;
                }
                cod = strtok(NULL, delim);
            }
        }
    }
}

/**
 * Funcao para abrir arquivo de filmes e verificar se ele existe ou nao, dependendo desse dado, ele faz um modo de abertura diferente
 */
FILE *abrir_arquivo_filme(char *nome, indice_primario *raiz, FILE *pri_index, FILE *sec_index, int *head, int *ppl, primarios_lista *lista_invertida, indice_sec *lista_sec, bool *flag)
{
    FILE *arquivo;
    arquivo = fopen(nome, "r+a");

    if (!arquivo)
    {
        arquivo = fopen(nome, "w+");
        fclose(arquivo);
        arquivo = fopen(nome, "r+");
        return arquivo;
    }
    *flag = true;
    return arquivo;
}

//====================================================================================

// Função de compactar

/**
 * Compactacao de arquivo, removendo os registros com o indicador de remocao
 */
FILE *compactacao_arquivo(FILE *filmes, FILE *pri_index, indice_primario **raiz)
{
    FILE *aux = fopen("aux.dat", "w"); // Abra o arquivo "aux"
    filme f;
    char *campo;
    char *delim = "@";
    char buffer[TAMANHO_REGISTRO + 1];

    fseek(filmes, 0, SEEK_SET);

    if (!aux)
    {
        return false;
    }

    while (!feof(filmes))
    {
        if (fgets(buffer, TAMANHO_REGISTRO + 1, filmes) == NULL)
        {
            break;
        }

        if (buffer[0] == '*' && buffer[1] == '|')
        {
            continue;
        }

        fprintf(aux, "%s", buffer);
    }

    fclose(aux);
    fclose(filmes);

    remove("movies.dat");
    rename("aux.dat", "movies.dat");

    aux = fopen("movies.dat", "r+a");

    *raiz = destruir_avl(*raiz);
    le_e_grava_indice(raiz, pri_index, aux);

    return aux;
}

// main



int main()
{
    bool flag1 = false, flag2 = false, flag_filmes = false; // Verificam se os arquivos estavam integros, ou se o arquivo de filmes ja existia
    indice_primario *raiz = NULL;                           // Raiz da AVL
    primarios_lista lista_invertida[1000];                  // Declaracao da lista invertida
    indice_sec lista_sec[900];                              // Declaracao da lista secundaria que linka com a invertida
    filme *f;                                               // Registro de filmes
    char chave_prim[6], chave_sec[56];                      // Nossas chaves primarias e secundarias para buscas
    int ppl = 0;                                            // primeira posicao livre da lista invertida -> vai sendo atualizado
    int head = -1;                                          // Cabeca da lista invertida
    int op_final;                                           // Opcao final para huffman ou nao huffman
    node *no;                                               // no da arvore de huffman
    unsigned int tabela[MAX_CHAR];                          // tabela para huffman -> salva frequencias de letras
    char **dicionario;                                      // Dicionario de huffman -> salva as strings de cada letra (sequencia nova de 1 ou 0)
    list *lista_huffman = malloc(sizeof(list));             // LIsta de hhuffman para linkar os elementos
    int altura_arvore;                                      // Altura da arvore de huffman -> saber o numero de colunas do dicionario (max)

    FILE *pri_index = abrir_arquivo_indices("iprimary.idx", &flag1);
    FILE *sec_index = abrir_arquivo_indices("ititle.idx", &flag2);
    FILE *filmes = abrir_arquivo_filme("movies.dat", raiz, pri_index, sec_index, &head, &ppl, lista_invertida, lista_sec, &flag_filmes);

    if (!filmes)
    {
        printf("\t\t\033[31mErro ao abrir o arquivo de filmes!\033[0m\n");
        return 1;
    }

    if (!pri_index)
    {
        printf("\t\t\033[31mErro ao abrir o arquivo de indice primario!\033[0m\n");
        return 1;
    }

    if (!sec_index)
    {
        printf("\t\t\033[31mErro ao abrir o arquivo de indice secundario!\033[0m\n");
        return 1;
    }

    if (flag1) // Se ja existe e esta integro, apenas insere na AVL (o indice prim)
    {
        indice_primario *aux = malloc(sizeof(indice_primario));
        fgetc(pri_index);
        char linha[50];
        char caractere;

        while (fgets(linha, sizeof(linha), pri_index) != NULL)
        {
            if (strcmp(linha, "\n") != 0)
            {
                if (sscanf(linha, "%[^@]@%d", aux->codigo, &aux->offset) == 2)
                {
                    raiz = inserir_avl(raiz, aux->codigo, aux->offset);
                    // printf("Codigo: %s, Offset: %d\n", aux->codigo, aux->offset);
                }
            }
        }
    }
    else if (!flag1 && flag_filmes) // Se existir e nao esta integro, e o arquivo de filmes existir, chama a funcao para corrigir isso
    {
        le_e_grava_indice(&raiz, pri_index, filmes);
    }

    if (flag2) // Se existir e estiver integro, apenas passa os dados para lista invertida
    {
        salva_lista_secundaria(lista_sec, lista_invertida, &ppl, &head, sec_index);
    }
    else if (!flag2 && flag_filmes) // Caso esteja nao integro e exista arquivo de filmes previo
    {
        grava_indice_sec(lista_sec, lista_invertida, sec_index, filmes, &head, &ppl);
    }

    int op;
    if (!flag_filmes) // Caso nao exista filmes previamente, incializa a secundaria
    {
        lista_sec[0].proximo = -1;
    }

    do
    {
        op = menu();
        switch (op)
        {

        case 1:
        {
            if (!raiz)
            {
                fseek(pri_index, 0, SEEK_SET);
                fprintf(pri_index, "0");
                fseek(sec_index, 0, SEEK_SET);
                fprintf(sec_index, "0");
            }
            raiz = cadastro_filmes(raiz, lista_sec, &head, &ppl, filmes, lista_invertida);
            fseek(pri_index, 0, SEEK_SET);
            fseek(sec_index, 0, SEEK_SET);

            int flag_pri, flag_sec;
            fscanf(pri_index, "%d", &flag_pri);
            fscanf(sec_index, "%d", &flag_sec);

            if (flag_pri == 1)
            {
                fseek(pri_index, 0, SEEK_SET);
                fprintf(pri_index, "0");
            }

            if (flag_sec == 1)
            {
                fseek(sec_index, 0, SEEK_SET);
                fprintf(sec_index, "0");
            }
            break;
        }

        case 2:
        {
            printf("\n\033[95m*****************************************************\n");

            printf("\033[33mDigite a chave primaria para remover: \33[0m");
            scanf("%s", chave_prim);
            for (int i = 0; i < 3; i++)
                chave_prim[i] = toupper(chave_prim[i]);
            raiz = remover_filme(lista_sec, lista_invertida, &head, raiz, chave_prim, filmes);

            fseek(pri_index, 0, SEEK_SET);
            fseek(sec_index, 0, SEEK_SET);

            int flag_pri, flag_sec;
            fscanf(pri_index, "%d", &flag_pri);
            fscanf(sec_index, "%d", &flag_sec);

            if (flag_pri == 1)
            {
                fseek(pri_index, 0, SEEK_SET);
                fprintf(pri_index, "0");
            }

            if (flag_sec == 1)
            {
                fseek(sec_index, 0, SEEK_SET);
                fprintf(sec_index, "0");
            }
            break;
        }

        case 3:
        {
            printf("\033[33mDigite a chave primaria: \033[0m");
            scanf(" %s", chave_prim);
            for (int i = 0; i < 3; i++)
                chave_prim[i] = toupper(chave_prim[i]);
            printf("\033[35m-------------------------------------\n");
            int nota;
            do
            {
                printf("\033[33mDigite a nova nota: \033[0m");
                scanf(" %d", &nota);
                if (nota < 0 || nota > 9)
                {
                    printf("\033[31mDIGITE NOTA VALIDA\033[0m");
                }
            } while (nota < 0 || nota > 9);

            if (modificar_nota(filmes, chave_prim, nota, raiz))
            {
                printf("\033[33m ------->  Nota modificada com sucesso! <-------\033[0m\n");
            }
            else
            {
                printf("\033[31m\t\tErro ao modificar a nota\033[0m\n");
            }
            break;
        }

        case 4:
        {
            printf("\033[33mDigite a chave primaria: \033[0m");
            scanf("%s", chave_prim);
            for (int i = 0; i < 3; i++)
                chave_prim[i] = toupper(chave_prim[i]);
            f = buscar_prim(raiz, chave_prim, filmes);
            if (!f)
            {
                break;
            }
            printf("\033[95m*************** Dados do filme: *************\n");
            printf("\033[95mCodigo: \033[0m%s\n", f->codigo);
            printf("\033[95mTitulo original: \033[0m%s\n", f->titulo_original);
            printf("\033[95mTitulo português: \033[0m%s\n", f->titulo_pt);
            printf("\033[95mDiretor: \033[0m%s\n", f->diretor);
            printf("\033[95mAno: \033[0m%s\n", f->ano);
            printf("\033[95mPais: \033[0m%s\n", f->pais);
            printf("\033[95mNota:\033[0m %d\n", f->nota);
            printf("\033[95m*******************************************************\033[0m\n\n");

            break;
        }

        case 5:
        {
            printf("\033[33mDigite o titulo: \033[0m");
            scanf(" %[^\n]s", chave_sec);
            buscar_sec(lista_sec, lista_invertida, head, chave_sec, filmes, raiz);
            break;
        }

        case 6:
        {
            listar_catalogo(filmes);
            break;
        }

        case 7:
        {
            printf("\033[33m\n\t\tInicio da compactacao, aguarde .... \n\033[0m");
            if (filmes = compactacao_arquivo(filmes, pri_index, &raiz))
            {
                printf("\033[95m\n******************   SUCESSO   ********************\033[0m\n");
            }
            else
            {
                printf("\033[31m\n#############   ERRO DE COMPACTACAO   #############\033[0m\n");
            }
            break;
        }

        case 0:
        {
            do
            {
                printf("\033[95mFaça uma escolha: \033[0m\n");
                printf("\033[95m0)\033[0m Testar o Huffman (vai perder o arquivo)\n");
                printf("\033[95m1)\033[0m Nao testar e manter o arquivo\n");
                scanf("%d", &op_final);
                if (op_final < 0 || op_final > 1)
                {
                    printf("\n\033[31m ESCOLHA UMA OPCAO VALIDA\033[31m\n\n");
                }
            } while (op_final < 0 || op_final > 1);
            if (op_final)
            {
                encerrar_programa(pri_index, sec_index, lista_sec, &head, lista_invertida, raiz);
                printf("\t\t\033[95mAte a proxima\n\n");
                printf(" \033[33m\t\t    /\\_/\\  \n");
                printf("\t\t   ( o.o ) \n");
                printf("\t\t    > ^ <  \n");
            }
            else
            {
                encerrar_programa(pri_index, sec_index, lista_sec, &head, lista_invertida, raiz);
                inicializar_freq(tabela);
                preenche_tabela(filmes, tabela);
                imprimir_tabela_freq(tabela);
                inicializar_lista(&lista_huffman);
                preencher_lista(tabela, lista_huffman);
                imprimir_lista(lista_huffman);
                no = montar_arvore_huffman(lista_huffman);
                altura_arvore = altura_da_árvore(no);
                dicionario = alocar_dicionario(altura_arvore);
                if (compactar_arquivo_huffman(&filmes, dicionario, altura_arvore, no))
                    printf("\n\033[95m------------------   Arquivo compactado com huffman ------------------\033[0m\n\n");
                else
                    printf("\n\033[31m##################  ERRO PARA COMPACTAR HUFFMAN #####################\033[0m\n\n");

                printf("\t\t\033[95mADEUS PARA SEMPRE\n\n");
                printf(" \033[33m\t\t    /\\_/\\  \n");
                printf("\t\t   ( o.o ) \n");
                printf("\t\t    > ^ <  \n");
            }
            break;
        }
        }
    } while (op != 0);

    fclose(pri_index);
    fclose(sec_index);
    fclose(filmes);
    return 0;
}