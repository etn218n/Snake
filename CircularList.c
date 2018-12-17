/*
File: CircularList.c
Author:	Vu Dinh Pham Duy
Description: implementation of circularly linked list.
*/
#include "CircularList.h"
/*-----------------------------------------------------------------------------*/
CircularList* createCircularList()
{
	CircularList *list = (CircularList*)malloc(sizeof(CircularList));

	list->head = NULL;
	list->tail = NULL;
	list->length = 0;

	return list;
}

Node* createNode(void *data)
{
	Node *node = (Node*)malloc(sizeof(Node));

	node->next = NULL;
	node->previous = NULL;
	node->data = data;

	return node;
}
/*-----------------------------------------------------------------------------*/
void add(CircularList *list, Node *newnode)
{
	if (list == NULL || newnode == NULL)
		return;

	if (list->length == 0)
	{
		list->head = newnode;
		list->tail = newnode;
		list->length = 1;
		return;
	}

	newnode->previous = list->tail;
	list->tail->next = newnode;

	newnode->next = list->head;
	list->head->previous = newnode;

	list->tail = newnode;
	list->length += 1;
}
/*-----------------------------------------------------------------------------*/
void traverse(CircularList *list, void (*fnc)(void *data))
{
	Node *current = list->head;

	for (int i = 0; i < list->length; i++)
	{
		fnc(current->data);
		current = current->next;
	}
}
/*-----------------------------------------------------------------------------*/
void deleteNode(Node *node)
{
	free(node->data);
	free(node);
}

void deleteAll(CircularList *list)
{
	Node *current = list->head;

	while (current != list->tail)
	{
		Node *temp = current->next;

		deleteNode(current);

		current = temp;
	}

	deleteNode(list->tail);

	list = NULL;
}
/*-----------------------------------------------------------------------------*/