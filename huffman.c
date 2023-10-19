// (C) First half of 2019 - Glener Lanes Pizzolato - glenerpizzolato@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef unsigned char byte;

typedef struct NodeTree
{
  int frequency;
  byte c;
  struct NodeTree *left;
  struct NodeTree *right;
} NodeTree;

typedef struct NodeList
{
  NodeTree *n;
  struct NodeList *next;
} NodeList;

typedef struct list
{
  NodeList *head;
  int elements;
} list;

NodeList *newListNode(NodeTree *nArv)
{

  NodeList *new;
  if ((new = malloc(sizeof(*new))) == NULL)
    return NULL;

  new->n = nArv;

  new->next = NULL;

  return new;
}

NodeTree *newTreeNode(byte c, int frequency, NodeTree *left, NodeTree *right)
{

  NodeTree *new;

  if ((new = malloc(sizeof(*new))) == NULL)
    return NULL;

  new->c = c;
  new->frequency = frequency;
  new->left = left;
  new->right = right;

  return new;
}

void insertList(NodeList *n, list *l)
{
  if (!l->head)
  {
    l->head = n;
  }

  else if (n->n->frequency < l->head->n->frequency)
  {
    n->next = l->head;
    l->head = n;
  }

  else
  {
    NodeList *aux = l->head->next;
    NodeList *aux2 = l->head;

    while (aux && aux->n->frequency <= n->n->frequency)
    {
      aux2 = aux;
      aux = aux2->next;
    }

    aux2->next = n;
    n->next = aux;
  }
  l->elements++;
}

NodeTree *popMinlist(list *l)
{

  NodeList *aux = l->head;
  NodeTree *aux2 = aux->n;

  l->head = aux->next;

  free(aux);
  aux = NULL;

  l->elements--;
  return aux2;
}

void frequencybyte(FILE *input, unsigned int *listBytes)
{
  byte c;

  while (fread(&c, 1, 1, input) >= 1)
  {
    listBytes[(byte)c]++;
  }
  rewind(input);
}

bool findCode(NodeTree *n, byte c, char *buffer, int size)
{

  if (!(n->left || n->right) && n->c == c)
  {
    buffer[size] = '\0';
    return true;
  }

  else
  {
    bool finded = false;

    if (n->left)
    {
      buffer[size] = '0';
      finded = findCode(n->left, c, buffer, size + 1);
    }

    if (!finded && n->right)
    {
      buffer[size] = '1';
      finded = findCode(n->right, c, buffer, size + 1);
    }

    if (!finded)
    {
      buffer[size] = '\0';
    }
    return finded;
  }
}

NodeTree *huffmanArv(unsigned *listBytes)
{

  list l = {NULL, 0};

  for (int i = 0; i < 256; i++)
  {
    if (listBytes[i])
    {
      insertList(newListNode(newTreeNode(i, listBytes[i], NULL, NULL)), &l);
    }
  }

  while (l.elements > 1)
  {
    NodeTree *leftNode = popMinlist(&l);
    NodeTree *rightNode = popMinlist(&l);
    NodeTree *sum = newTreeNode('#', leftNode->frequency + rightNode->frequency, leftNode, rightNode);

    insertList(newListNode(sum), &l);
  }

  return popMinlist(&l);
}

void freeHuffmanTree(NodeTree *n)
{

  if (!n)
    return;
  else
  {
    NodeTree *left = n->left;
    NodeTree *right = n->right;
    free(n);
    freeHuffmanTree(left);
    freeHuffmanTree(right);
  }
}

// Bitmasking, taken from: http://ellard.org/dan/www/CS50-95/s10.html
int generateBit(FILE *input, int position, byte *aux)
{
  (position % 8 == 0) ? fread(aux, 1, 1, input) : NULL == NULL;
  return !!((*aux) & (1 << (position % 8)));
}

void inputReadingError()
{
  printf("Error in reading file.\n");
  exit(0);
}

void compress(const char *inputFile, const char *outputFile)
{

  clock_t beginTime, finalTime;
  double timeOfExecution;
  beginTime = clock();

  unsigned listBytes[256] = {0};

  FILE *input = fopen(inputFile, "rb");
  if (inputFile == NULL)
  {
    inputReadingError();
  }

  FILE *output = fopen(outputFile, "wb");
  if (outputFile == NULL)
  {
    inputReadingError();
  }

  frequencybyte(input, listBytes);
  NodeTree *rootOfTree = huffmanArv(listBytes);
  fwrite(listBytes, 256, sizeof(listBytes[0]), output);
  fseek(output, sizeof(unsigned int), SEEK_CUR);
  byte c;
  unsigned size = 0;
  byte aux = 0;

  while (fread(&c, 1, 1, input) >= 1)
  {
    char buffer[1024] = {0};
    findCode(rootOfTree, c, buffer, 0);

    for (char *i = buffer; *i; i++)
    {
      if (*i == '1')
      {
        aux = aux | (1 << (size % 8));
      }

      size++;

      if (size % 8 == 0)
      {
        fwrite(&aux, 1, 1, output);
        aux = 0;
      }
    }
  }

  fwrite(&aux, 1, 1, output);

  fseek(output, 256 * sizeof(unsigned int), SEEK_SET);
  fwrite(&size, 1, sizeof(unsigned), output);

  finalTime = clock();
  timeOfExecution = (double)(finalTime - beginTime) / CLOCKS_PER_SEC;

  fseek(input, 0L, SEEK_END);
  double sizeinput = ftell(input);

  fseek(output, 0L, SEEK_END);
  double outputSize = ftell(output);

  freeHuffmanTree(rootOfTree);

  printf("%s: (%g Bytes)\n%s: (%g Bytes)\n\nTime: %g s", inputFile, sizeinput / 1000, outputFile, outputSize / 1000, timeOfExecution);
  printf("Compress: %d%%\n", (int)((100 * outputSize) / sizeinput));

  fclose(input);
  fclose(output);
}

void decompress(const char *inputFile, const char *outputFile)
{

  clock_t beginTime, finalTime;
  double timeOfExecution;
  beginTime = clock();

  unsigned listBytes[256] = {0};

  FILE *input = fopen(inputFile, "rb");
  if (inputFile == NULL)
  {
    inputReadingError();
  }

  FILE *output = fopen(outputFile, "wb");
  if (outputFile == NULL)
  {
    inputReadingError();
  }

  fread(listBytes, 256, sizeof(listBytes[0]), input);
  NodeTree *rootOfTree = huffmanArv(listBytes);

  unsigned size;
  fread(&size, 1, sizeof(size), input);

  unsigned position = 0;
  byte aux = 0;

  while (position < size)
  {

    NodeTree *nodeAtual = rootOfTree;
    while (nodeAtual->left || nodeAtual->right)
    {
      nodeAtual = generateBit(input, position++, &aux) ? nodeAtual->right : nodeAtual->left;
    }
    fwrite(&(nodeAtual->c), 1, 1, output);
  }

  freeHuffmanTree(rootOfTree);

  finalTime = clock();
  timeOfExecution = (double)(finalTime - beginTime) / CLOCKS_PER_SEC;

  fseek(input, 0L, SEEK_END);
  double sizeinput = ftell(input);

  fseek(output, 0L, SEEK_END);
  double outputSize = ftell(output);

  printf("%s (%g bytes)\n%s (%g bytes)\n\nTime: %g s\n", inputFile, sizeinput / 1000, outputFile, outputSize / 1000, timeOfExecution);
  printf("Uncompress: %d%%\n", (int)((100 * outputSize) / sizeinput));

  fclose(output);
  fclose(input);
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
    if (strstr(argv[3], ".hx"))
    {

      printf("\ncompressing ...");
      compress(argv[2], argv[3]);
    }
    else
    {
      printf("error: Wrong information, please read the README.md file");
      return 0;
    }
  }

  else if (strcmp("d", argv[1]) == 0)
  {
    if (strstr(argv[2], ".hx"))
    {
      printf("\nuncompressing ...");
      decompress(argv[2], argv[3]);
    }
    else
    {
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