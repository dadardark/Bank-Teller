/*
19817082 | Jacob Arvino
Function which converts the systems current time into the format HH:MM:SS
*/
#include <time.h>
#include <stdlib.h>

char* currentTime(time_t t)
{
    char* buf;
    buf = (char*)malloc(sizeof(char));
    
    strftime(buf, sizeof(buf), "%T", localtime(&t));
    
    return buf;
}
