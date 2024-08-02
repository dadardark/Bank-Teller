/*
19817082 | Jacob Arvino
Static linkedList struct for housing customer nodes
*/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct linkedListNode
{
    int customerNumber;
    char serviceType;
    char* arrivalTime;
    struct linkedListNode* next;
    struct linkedListNode* prev;
}
linkedListNode;

typedef struct linkedList
{
    linkedListNode* head;
    linkedListNode* tail;
    int count;
}
linkedList;

linkedList* createLinkedList();
void insertFirst(linkedList* list, int inNumber, char inType, char* arrivalTime);
void removeLast(linkedList* list);
int isEmpty(linkedList* list);
void freeList(linkedList* list);


#endif
