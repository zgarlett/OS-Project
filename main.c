//Operating Systems
//Group K
//3/30
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "server.h"

int main()
{

    //Creating 5 threads for servers
    pthread_t thread[5];
    for (int i = 0; i < 5; i++)
    {
        pthread_create (&thread[i], NULL, serverStart, NULL);
    }

    //Joining threads
    for (int i = 0; i < 5; i++)
    {
        pthread_join(thread[i], NULL);
    }
}
