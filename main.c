//Operating Systems
//Group K
//3/30
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "server.h"



int main()
{

    //Creating thread for server
    pthread_t thread;
    pthread_create (&thread, NULL, serverStart, NULL);
    pthread_join(thread, NULL);


    return 0;
}
