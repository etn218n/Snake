#ifndef CIRCULARLIST
#define CIRCULARLIST
/*-----------------------------------------------------------------------------*/
#include <stdlib.h>
/*-----------------------------------------------------------------------------*/
typedef struct node{
	void *data;
	struct node *next;
	struct node *previous;
} Node;

typedef struct circularlist{
	struct node *head;
	struct node *tail;
	int length;
} CircularList;
/*-----------------------------------------------------------------------------*/
CircularList* createCircularList();
Node* createNode(void *data);

void add(CircularList *list, Node *node); // add new node to the end of the list
void traverse(CircularList *list, void (*fnc)(void *data));
void deleteNode(Node *node);
void deleteAll(CircularList *list);
/*-----------------------------------------------------------------------------*/
#endif