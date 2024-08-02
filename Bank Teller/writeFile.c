/*
19817082 | Jacob Arvino
Unused writefile function, refer to documentation for further explanation
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void writeCustomerToQueue(FILE* fp, pthread_mutex_t mutex2, int cNum, char sType, char* aTime)
{
    pthread_mutex_lock(&mutex2);
    fprintf(fp, "\n--------------------------\n Customer number %d : %c  \n Arrival time: %s  \n--------------------------\n",cNum,sType,aTime);
    pthread_mutex_unlock(&mutex2);
    
}

void writeCustomerToTeller(FILE* fp,pthread_mutex_t mutex2, int writeType,char* tellerID, int cNum, char* aTime, char* inTime)
{
    pthread_mutex_lock(&mutex2);
    fprintf(fp, "\n\t--------------------------\n\t%s\n\tCustomer Number: %d\n\tArrival time: %s\n\tResponse time: %s\n\t--------------------------\n\t",tellerID,cNum,aTime,inTime);
    pthread_mutex_unlock(&mutex2);
}



