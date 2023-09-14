# COLEÇÃO DE FILMES (LOCADORA)

## Sobre o código:

O código é dividido em partes, separando as estruturas de dados (ED's) para ordenar os índices e fazer compactações (compactação de HUffman).

**AVL:**

- Ordena índices primários em RAM
- Estrutura dinâmica escolhida pela sua complexidade de busca (O(logn))

**Lista secundária:**

- Usada para ordenar os índices secundários alfabeticamente e ligar os índices secundários aos primários correspondentes
- Foram utilizados vetores para tal
- A remoção e ordenação é feita logicamente (não física)
- São dois vetores, um de secundários e um com os primários

**Huffman:**

- Foi feita uma tentativa de HUffman, como seria muito trabalhoso descompactar o arquivo, por questões de tempo, decidi apenas deixar minha tentativa de compactar.
- Utiliza uma árvore de Huffman para gerar os códigos de acordo com as frequências e guarda esses novos códigos binários em um dicionário.
- Infelizmente, não fiz a descompactação, então ao retornar, o arquivo acaba sendo perdido.

## Compactação (extra):

A compactação é feita removendo os registros com indicador de remoção, salvando em um arquivo auxiliar sem eles e modificando o ponteiro do  arquivo original. 

## Sobre a separação do código:

Como dito anteriormente, por questões de organização, o códdigo foi separado em outros cabeçalhos (bibliotecas) e arquivos .c, assim, para importar aonde são feitas as interações com usuário e a main, faz-se a importação dos arquivos .c

## Qual material utilizou-se para Huffman:

Para o código de Huffman, usei essa fonte para aprender a fazer:

```
https://www.youtube.com/watch?v=o8UPZ_KDWdU&list=PLqJK4Oyr5WShtxF1Ch3Vq4b1Dzzb-WxbP

CANAL: Programe seu futuro
```

## Arquivo de filmes

O arquivo movies.dat que estou usando foi gerado pelo meu colega de sala e usei ele para testar o código, então envio o mesmo para correção do trabalho, pois foi com ele que fiz e corrigi todo o código.

## IMPORTANTE

Não foi usado acentuação pelo fato de C usar ASCII e mesmo importando a biblioteca para se trabalhar  com esses tipos de caracteres, a locale.h, houveram erros que não consegui corrigir, então ao inserir, evite caracteres de acentuação.

## Podia melhorar:

Algumas melhorias poderiam ser feitas, tais quais:

- Funções extras para trechos repetidos: não foi feito pelo tempo e porque tive medo de acabar dando algum erro de alguma forma
- Descompactar Huffman
