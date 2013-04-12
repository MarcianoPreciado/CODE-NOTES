#include "soapList.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int soapList_isEmpty(struct soapList *header)
{
  if (header->next == NULL)
    return 1;
  return 0;
}

struct soapList *soapList_insert(struct soapList *header,
                                 struct soapList *parent, const char *name, const char *data)
{
  struct soapList *node = malloc(sizeof(struct soapList));
  if (name != NULL)
    node->elementName = strdup(name); /* strdup分配字符串空间保存，防止SOAP析构后原字符串被free */
  else
    node->elementName = NULL;

  if (data != NULL)
    node->elementData = strdup(data);
  else
    node->elementData = NULL;

  node->parent = parent;
  node->next = NULL;
  struct soapList *insertNode = header;
  while (insertNode->next != NULL)
    insertNode = insertNode->next;
  insertNode->next = node;
  return node;
}

struct soapList *soapList_findByName(struct soapList *header,const char *name)
{
  struct soapList *node = header;
  while (node->next != NULL && strcmp(node->next->elementName, name) != 0)
    node = node->next;
  return node->next;
}

void soapList_emptyList(struct soapList *header)
{
  struct soapList *node = header;
  struct soapList *nextNode;
  while (node != NULL) {
    nextNode = node->next;
    if (node->elementName != NULL)
      free(node->elementName);
    if (node->elementData != NULL)
      free(node->elementData);
    free(node);
    node = nextNode;
  }
}

void soapList_printAll(struct soapList *header)
{
  struct soapList *node = header->next;
  while(node != NULL) {
    printf("%s=%s\t",
           node->elementName ? node->elementName : "NULL",
           node->elementData ? node->elementData : "NULL");
    if (node->parent != NULL && node->parent->elementName != NULL)
      printf("parentname=%s", node->parent->elementName);
    printf("\n");
    node = node->next;
  }
}
