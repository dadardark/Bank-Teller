/*
19817082 | Jacob Arvino
 Main file of the program. Creates threads and calls functions to re-enacte the bank teller scenario
*/
void *teller(void *arg);
void* customer(void *Arg);

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <stdatomic.h>
#include "linkedList.h"
#include "readFile.h"
#include "writeFile.h"
#include "currentTime.h"
/*Variable Declaration*/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;


linkedList *readInQueue, *c_queue;
FILE* fp;
int queueArriveDelay, withdrawTime, depositTime, informationTime;
int customersServedArray[4];
char* tellerNameArray[4];
char* fileName;
atomic_int threadsDone = ATOMIC_VAR_INIT(0);

/*Reads in command-line arguments and passes them the required functions*/
int main(int argc, char *argv[])
{
    /*Variable delcaration*/
    int i, totalCustomers;
    
    pthread_t threads[4];
    char* threadNames[4] = {"Teller 1", "Teller 2", "Teller 3", "Teller 4"};
    totalCustomers = 0;
    
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_mutex_init(&mutex3, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&cond2, NULL);
    
    /*Check that the command-line arguments are sufficent for the program to run*/
    if(argc != 6)
    {
        printf("Invalid number of command line arguments: 'filename','Queue arrive delay','Withdraw time','Deposit time','Information time'");
        return(0);
    }
    
    /*Reads in the file declared on the command-line and casts the command-line arguments to ints*/
       /*fp = fopen(argv[0],"w"); // Unused writeFile code"*/
           
    fileName = argv[1];
    queueArriveDelay = atoi(argv[2]);
    withdrawTime = atoi(argv[3]);
    depositTime = atoi(argv[4]);
    informationTime = atoi(argv[5]);
             
    /*Creates 5 threads, with thread 0 running the customer function thread and the other 4 running the teller threads*/
    for(i = 0; i < 5; i++)
    {
        if( i == 0)
        {
            pthread_create(&threads[i], NULL, customer, NULL);
        }
        else
        {
            pthread_create(&threads[i], NULL, teller, (void *) threadNames[i-1]);
            pthread_setname_np(threadNames[i]);
        }
    }
    
    for (i = 0; i < 5; i++)
    {
        pthread_join(threads[i], NULL);
    }
    /*Prints to console how many customers each teller served and then the total number of customers served*/
    printf("\n Teller statistics:\n %s serves %d customers\n %s serves %d customers\n %s serves %d customers\n %s serves %d customers\n\n",tellerNameArray[0],customersServedArray[0],tellerNameArray[1],customersServedArray[1],tellerNameArray[2],customersServedArray[2],tellerNameArray[3],customersServedArray[3]);
    
    for(i = 0; i < 4;i++)
    {
        totalCustomers = totalCustomers + customersServedArray[i];
    }
    printf(" Total customers served: %d\n",totalCustomers);
    
    /*Memory deallocation and pthread destruction*/
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex2);
    pthread_mutex_destroy(&mutex3);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&cond2);
    freeList(readInQueue);
    freeList(c_queue);

    return 0;
}

void* customer(void *arg)
/*Function which moves customers periodically into the c_queue*/
{
    /*Variable decdaration*/
    char* arrivalTime;
    
    arrivalTime = (char*)malloc(sizeof(char));
    
    readInQueue = createLinkedList();
    c_queue = createLinkedList();
    
    /*Reads in the file from command-line and reads the contents*/
    readFile(readInQueue,fileName);
    
    /*While the linkedList read in from the file isn't empty, the customers are periodically moved into c_queue in a FIFO way*/
    while(readInQueue->head != NULL)
    {
        /*Acquire mutex, get the customers arrival time into c_queue, move customer from readInQueue -> c_queue and remove that customer from the readInQueue. The mutex is then released and the cond variable is signaled to other waiting threads*/
        pthread_mutex_lock(&mutex);
        
        arrivalTime = currentTime(time(0));
        
        insertFirst(c_queue, readInQueue->tail->customerNumber, readInQueue->tail->serviceType, arrivalTime);
        removeLast(readInQueue);
        
        printf("\n--------------------------\n Customer number %d : %c  \n Arrival time: %s  \n--------------------------\n",c_queue->head->customerNumber,c_queue->head->serviceType,c_queue->head->arrivalTime);
        
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        
       /* pthread_mutex_lock(&mutex2);
        
        writeCustomerToQueue(fp,mutex2,c_queue->head->customerNumber, c_queue->head->serviceType, c_queue->head->arrivalTime);
        printf("\n Yeah works\n");
                                                            // Unused writeFile code
        pthread_mutex_unlock(&mutex2);
        pthread_cond_signal(&cond2);*/
        
        sleep(queueArriveDelay);
        
    }
    
    /*Once all customers are moved in to c_queue and until c_queue is depleted, to prevent deadlock the function periodically signals that the mutex is free*/
    while(c_queue->head != NULL)
    {
        pthread_cond_signal(&cond);
    }
    pthread_exit(NULL);
}

void *teller(void *arg)
/*Function which moves serves customers from c_queue*/
{
    /*Variable declaration*/
    char* inTime, *outTime, *sType, *startTime, *endTime, *aTime;
    int* cNum;
    int customerServed;
    
    cNum = (int*)malloc(sizeof(int));
    sType = (char*)malloc(sizeof(char));
    startTime = (char*)malloc(sizeof(char));
    endTime = (char*)malloc(sizeof(char));
    
    customerServed = 0;
    startTime = currentTime(time(0));
    /*While nobody is in the c_queue, the tellers sleep*/
    while(c_queue->head == NULL)
    {
        usleep(25000);
    }
    /*Once customersa are in the c_queue, tellers are able to acquire the mutex and 'serve' the customers */
    pthread_mutex_lock(&mutex);
    while(c_queue->head != NULL)
    {
        /*Checks that no other thread is exeucting the critical section, if not takes a customer from the front of c_queue, gets their information and releases the lock*/
        pthread_cond_wait(&cond,&mutex);
        
        *cNum = c_queue->tail->customerNumber;
        *sType = c_queue->tail->serviceType;
        aTime = c_queue->tail->arrivalTime;
        
        removeLast(c_queue);
       
        pthread_mutex_unlock(&mutex);
        
        inTime = currentTime(time(0));
        
       /* pthread_mutex_lock(&mutex2);
        pthread_cond_wait(&cond2,&mutex2);
                                                    //Unused writeFile code
        writeCustomerToTeller(fp,mutex2,arg,*cNum,aTime,inTime);
        
        pthread_mutex_unlock(&mutex2);*/
        
        printf("\n\t--------------------------\n\t%s\n\tCustomer Number: %d\n\tArrival time: %s\n\tResponse time: %s\n\t--------------------------\n\t",(char*)arg,*cNum,aTime,inTime);
        
        if(*sType == 'I')
        {
            sleep(informationTime);
        }
        else if (*sType == 'W')
        {
            sleep(withdrawTime);
        }
        else if (*sType == 'D')
        {
            sleep(depositTime);
        }
        outTime = currentTime(time(0));
        
        printf("\n\t\t--------------------------\n\t\t%s:\n\t\tCustomer Number: %d\n\t\tArrival time: %s\n\t\tCompletion time: %s\n\t\t--------------------------\n\t\t",(char*)arg,*cNum,aTime,outTime);
        customerServed++;
        
    }
    /*Once c_queue is empty and a thread has finished with their last customer, prints out the tellers termination information*/
        endTime = currentTime(time(0));
    
        printf("\n\t\t\t--------------------------\n\t\t\tTermination: %s\n\t\t\tServed: %d customers\n\t\t\tStart time: %s\n\t\t\tFinish time: %s \n\t\t\t--------------------------\n\t\t\t",(char*)arg, customerServed,startTime,endTime);
    
        /*Each teller acquires the mutex lock and stores the number of customers served as well as the teller number in the array*/
        pthread_mutex_lock(&mutex3);
    
        tellerNameArray[threadsDone] = (char*)arg;
        customersServedArray[threadsDone] = customerServed;
        threadsDone++;
    
        pthread_mutex_unlock(&mutex3);
    
    free(cNum);
    free(sType);
    free(startTime);
    free(endTime);
    free(inTime);
    free(outTime);
    free(aTime);
    
    pthread_exit(NULL);
}







