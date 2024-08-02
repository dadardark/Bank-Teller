/*
19817082 | Jacob Arvino
Function which reads in a file (.txt) and inserts each entry into a linkedListNode
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "linkedList.h"

linkedList* readFile(linkedList* c_queue, char* fileName)
{
    /*Variable declaration*/
    FILE * fp;
    char * line = NULL;
    char * splitLine;
    size_t len = 0;
    ssize_t read;
    int splitCount, customerNumber;
    char serviceType;

    fp = fopen(fileName, "r");
    /*Reads in line of a file and seperates each arguments before inserting it into the linkedList and returning the full linkedList*/
    while ((read = getline(&line, &len, fp)) != -1)
    {
        splitLine = strtok(line, " ");
        splitCount = 0;
        while(splitLine != NULL )
        {
            if(splitCount == 0)
            {
                customerNumber = atoi(splitLine);
                splitLine = strtok(NULL, " ");
                splitCount++;
            }
            else if (splitCount == 1)
            {
                serviceType = *splitLine;
                insertFirst(c_queue,customerNumber, serviceType, NULL);
                splitLine = strtok(NULL, " ");
                splitCount = 0 ;
            }
        }
    }

    fclose(fp);
    return c_queue;
}
