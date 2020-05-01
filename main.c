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
#include <semaphore.h>

//semaphores for file a and b write access
sem_t filealock;
sem_t fileblock;

int ports[4] = {8181,8182,8183,8184};

int main(int argc, char const *argv[]){

    pthread_t thread[4];
    
    for(int i = 0; i < 4; i++)
    {
        pthread_create(&thread[i], NULL, serverStart, &ports[i]);
        printf("Port Number: %d", ports[i]);
    }
    
    //starts Client for testing
    clientMain();
    
    for(int i = 0; i < 4; i++)
    {
        pthread_join(thread[i], NULL);
    }

    /*
    if(fork() > 0){
        serverStart((void *) &ports[0]);
        if(fork() > 0){
            serverStart((void *) &ports[1]);
        } else {
            serverStart((void *) &ports[2]);
        }
    } else {
        serverStart((void *) &ports[3]);
    }
    */
    
    //Destroys Semaphores once done
    sem_destroy(&filealock);
    sem_destroy(&filealock);

    return 0;
}
