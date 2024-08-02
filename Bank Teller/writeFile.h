#ifndef WRITEFILE_H
#define WRITEFILE_H

void writeCustomerToQueue(FILE* fp,pthread_mutex_t mutex, int cNum, char sType, char* aTime);
void writeCustomerToTeller(FILE* fp,pthread_mutex_t mutex2,char* tellerID, int cNum, char* aTime, char* inTime);

#endif

