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

int ports[4] = {8181,8182,8183,8184};

int main(int argc, char const *argv[]){


    //Creating thread for server
    pthread_t thread[4];


    for(int i = 0; i < 4; i++){
      pthread_create (&thread[i], NULL, serverStart, (void * ) &ports[i]);
      printf("Port Number %d\n", ports[i]);

    }

    for(int i = 0; i < 4; i++){
      pthread_join(thread[i], NULL);
    }

    return 0;
}
