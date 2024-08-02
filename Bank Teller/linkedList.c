/*
19817082 | Jacob Arvino
LinkedList struct used to simulate the FIFO queue (c_queue)
*/
#include <stdlib.h>
#include <stdio.h>
#include "linkedList.h"

/*Initialise empty linked list*/
linkedList* createLinkedList()
{
    linkedList* list;

    list = (linkedList*)malloc(sizeof(linkedList));

    list->head = NULL;
    list->tail = NULL;
    list->count = 0;

    return list;
}

/*Inserts a customer into the front of the linkedList struct*/
void insertFirst(linkedList* list, int inNumber, char inType, char* inArrivalTime)
{
    linkedListNode* newNode;

    newNode = (linkedListNode*)malloc(sizeof(linkedListNode));

    newNode->customerNumber = inNumber;
    newNode->serviceType = inType;
    newNode->arrivalTime = inArrivalTime;
    newNode->next = NULL;
    newNode->prev = NULL;

    if(isEmpty(list) == 1)
    {
        list->head = newNode;
        list->tail = newNode;
        list->count = list->count + 1;
    }
    else
    {
        list->head->prev = newNode;
        newNode->next = list->head;
        
        
        list->head = newNode;
        list->count = list->count + 1;
    }

}

/*Removes a customer from the end of the linkedList struct*/
void removeLast(linkedList* list)
{
    linkedListNode* tempNode;
    if (list->tail == NULL)
    {
            return;
    }

    tempNode = list->tail;
    list->tail = list->tail->prev;
    if (list->tail == NULL)
    {
        list->head = NULL;
    }
    else
    {
        list->tail->next = NULL;
    }
    free(tempNode);
}

/*Checks if the linkedList struct is empty*/
int isEmpty(linkedList* list)
{
    int empty;

    empty = 0;

    if(list->head == NULL)
    {
        empty = 1;
    }

    return empty;
}

/*Deallocates the linkedList memory*/
void freeList(linkedList* list)
{
    linkedListNode* currentNode, *prevNode;
    currentNode = list->tail;

    while(currentNode != NULL)
    {
        prevNode = currentNode->prev;
        free(currentNode);
        if (prevNode != NULL)
        {
            prevNode->next = NULL;
        }

        currentNode = prevNode;
    }
    list->head = NULL;
    list->tail = NULL;
}
