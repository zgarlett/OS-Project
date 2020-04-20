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
#include "client.h"
#include "fileManager.h"



int main()
{

    //Creating thread for server
    pthread_t thread;
    pthread_t thread2;

    pthread_create (&thread, NULL, serverStart, NULL);
    pthread_create (&thread2, NULL, mainclient, NULL);
    pthread_join(thread, NULL);
    pthread_join(thread2, NULL);


    return 0;
}
