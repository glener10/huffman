//Autors: Glener Lanes Pizzolato, Alegrete-RS, brazil.
//First half of 2019


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>


typedef unsigned char byte;


typedef struct NoArv
{
    int frequencia;
    byte c;
    struct NoArv  *esquerda;
    struct NoArv  *direita;
} NoArv;



typedef struct NoLista
{
    NoArv *n;
    struct NoLista *proximo;
} NoLista;



typedef struct lista
{
    NoLista *head;
    int elementos;
} lista;

//---------------------------------------------------


NoLista *novoNoLista(NoArv *nArv)
{

    NoLista *novo;
    if ( (novo = malloc(sizeof(*novo))) == NULL ) return NULL;

    novo->n = nArv;

    novo->proximo = NULL;

    return novo;
}



NoArv *novoNoArv(byte c, int frequencia, NoArv *esquerda, NoArv *direita)
{

    NoArv *novo;

    if ( ( novo = malloc(sizeof(*novo)) ) == NULL ) return NULL;

    novo->c = c;
    novo->frequencia = frequencia;
    novo->esquerda = esquerda;
    novo->direita = direita;

    return novo;
}




//---------------------------------------------------



void insereLista(NoLista *n, lista *l)
{

    if (!l->head)
    {
        l->head = n;
    }


    else if (n->n->frequencia < l->head->n->frequencia)
    {
        n->proximo = l->head;
        l->head = n;
    }

    else
    {

        NoLista *aux = l->head->proximo;

        NoLista *aux2 = l->head;

        while (aux && aux->n->frequencia <= n->n->frequencia)
        {
            aux2 = aux;
            aux = aux2->proximo;
        }

        aux2->proximo = n;
        n->proximo = aux;
    }


    l->elementos++;
}




NoArv *popMinLista(lista *l)
{

    NoLista *aux = l->head;

    NoArv *aux2 = aux->n;


    l->head = aux->proximo;

    free(aux);
    aux = NULL;


    l->elementos--;

    return aux2;
}





void frequenciabyte(FILE *entrada, unsigned int *listaBytes)
{

    byte c;


    while (fread(&c, 1, 1, entrada) >= 1)
    {
        listaBytes[(byte)c]++;
    }
    rewind(entrada);

}




bool pegaCodigo(NoArv *n, byte c, char *buffer, int tamanho)
{


    if (!(n->esquerda || n->direita) && n->c == c)
    {
        buffer[tamanho] = '\0';
        return true;
    }


    else
    {
        bool encontrado = false;

        if (n->esquerda)
        {

            buffer[tamanho] = '0';

            encontrado = pegaCodigo(n->esquerda, c, buffer, tamanho + 1);
        }

        if (!encontrado && n->direita)
        {
            buffer[tamanho] = '1';
            encontrado = pegaCodigo(n->direita, c, buffer, tamanho + 1);
        }

        if (!encontrado)
        {
            buffer[tamanho] = '\0';
        }


        return encontrado;
    }

}




NoArv *huffmanArv(unsigned *listaBytes)
{

    lista l = {NULL, 0};


    for (int i = 0; i < 256; i++)
    {
        if (listaBytes[i])
        {

            insereLista(novoNoLista(novoNoArv(i, listaBytes[i], NULL, NULL)), &l);
        }
    }

    while (l.elementos > 1)
    {

        NoArv *nodeEsquerdo = popMinLista(&l);

        NoArv *nodeDireito = popMinLista(&l);


        NoArv *soma = novoNoArv('#',nodeEsquerdo->frequencia + nodeDireito->frequencia, nodeEsquerdo, nodeDireito);



        insereLista(novoNoLista(soma), &l);
    }

    return popMinLista(&l);
}



void liberaHuffman(NoArv *n)
{

    if (!n) return;
    else
    {
        NoArv *esquerda = n->esquerda;
        NoArv *direita = n->direita;
        free(n);
        liberaHuffman(esquerda);
        liberaHuffman(direita);
    }
}




// Bitmasking, taken from: http://ellard.org/dan/www/CS50-95/s10.html

int geraBit(FILE *entrada, int posicao, byte *aux )
{

    (posicao % 8 == 0) ? fread(aux, 1, 1, entrada) : NULL == NULL ;
    return !!((*aux) & (1 << (posicao % 8)));

}






void erroArquivo()
{
    printf("Arquivo nao encontrado\n");
    exit(0);
}






void comprime(const char *arquivoEntrada, const char *arquivoSaida)
{

    clock_t inicio, final;
    double tempoGasto;
    inicio = clock();

    unsigned listaBytes[256] = {0};

    FILE *entrada = fopen(arquivoEntrada, "rb");
    if(arquivoEntrada == NULL){
        erroArquivo();
    }

    FILE *saida = fopen(arquivoSaida, "wb");
    if(arquivoSaida == NULL){
        erroArquivo();
    }



    frequenciabyte(entrada, listaBytes);


    NoArv *raiz = huffmanArv(listaBytes);


    fwrite(listaBytes, 256, sizeof(listaBytes[0]), saida);


    fseek(saida, sizeof(unsigned int), SEEK_CUR);

    byte c;
    unsigned tamanho = 0;
    byte aux = 0;



    while (fread(&c, 1, 1, entrada) >= 1)
    {


        char buffer[1024] = {0};


        pegaCodigo(raiz, c, buffer, 0);


        for (char *i = buffer; *i; i++)
        {

            if (*i == '1')
            {

                aux = aux | (1 << (tamanho % 8));
            }

            tamanho++;


            if (tamanho % 8 == 0)
            {
                fwrite(&aux, 1, 1, saida);
                aux = 0;
            }
        }
    }


    fwrite(&aux, 1, 1, saida);

    fseek(saida, 256 * sizeof(unsigned int), SEEK_SET);
    fwrite(&tamanho, 1, sizeof(unsigned), saida);

    final = clock();
    tempoGasto = (double)(final - inicio) / CLOCKS_PER_SEC;

    fseek(entrada, 0L, SEEK_END);
    double tamanhoEntrada = ftell(entrada);

    fseek(saida, 0L, SEEK_END);
    double tamanhoSaida = ftell(saida);

    liberaHuffman(raiz);

    printf("%s: (%g Bytes)\n%s: (%g Bytes)\n\nTime: %g s",arquivoEntrada,tamanhoEntrada/1000,arquivoSaida,tamanhoSaida/1000,tempoGasto);
    printf("Compress: %d%%\n", (int)((100 * tamanhoSaida) / tamanhoEntrada));

    fclose(entrada);
    fclose(saida);

}




void descomprime(const char *arquivoEntrada, const char *arquivoSaida)
{

    clock_t inicio, final;
    double tempoGasto;
    inicio = clock();

    unsigned listaBytes[256] = {0};

    FILE *entrada = fopen(arquivoEntrada, "rb");
    if (arquivoEntrada == NULL){
        erroArquivo();
    }

    FILE *saida = fopen(arquivoSaida, "wb");
    if(arquivoSaida == NULL){
        erroArquivo();
    }



    fread(listaBytes, 256, sizeof(listaBytes[0]), entrada);


    NoArv *raiz = huffmanArv(listaBytes);


    unsigned tamanho;
    fread(&tamanho, 1, sizeof(tamanho), entrada);

    unsigned posicao = 0;
    byte aux = 0;


    while (posicao < tamanho)
    {

        NoArv *nodeAtual = raiz;


        while ( nodeAtual->esquerda || nodeAtual->direita )
        {
            nodeAtual = geraBit(entrada, posicao++, &aux) ? nodeAtual->direita : nodeAtual->esquerda;
        }

        fwrite(&(nodeAtual->c), 1, 1, saida);
    }

    liberaHuffman(raiz);

    final = clock();
    tempoGasto = (double)(final - inicio) / CLOCKS_PER_SEC;

    fseek(entrada, 0L, SEEK_END);
    double tamanhoEntrada = ftell(entrada);

    fseek(saida, 0L, SEEK_END);
    double tamanhoSaida = ftell(saida);

    printf("%s (%g bytes)\n%s (%g bytes)\n\nTime: %g s\n", arquivoEntrada, tamanhoEntrada / 1000, arquivoSaida, tamanhoSaida / 1000, tempoGasto);
    printf("Uncompress: %d%%\n", (int)((100 * tamanhoSaida) / tamanhoEntrada));


    fclose(saida);
    fclose(entrada);
}



int main(int argc, char *argv[])
{

    if (argc < 4)
    {

        printf("error: Wrong information, please read the README.md file");
        return 0;
    }

    if (strcmp("c", argv[1]) == 0)
    {
	if (strstr (argv[3], ".hx")){

		printf("\ncompressing ...");
        	comprime(argv[2], argv[3]);
	}
	else{
		printf("error: Wrong information, please read the README.md file");
        	return 0;
	}

    }


    else if (strcmp("d", argv[1]) == 0)
    {
	if( strstr(argv[2], ".hx")){
		printf("\nuncompressing ...");
        	descomprime(argv[2], argv[3]);
	}
	else{
		printf("error: Wrong information, please read the README.md file");
        	return 0;
	}


    }

    else
    {
        printf("error: Wrong information, please read the README.md file");
        return 0;
    }

    return 0;
}
