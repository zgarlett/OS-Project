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


    if(fork() > 0){
        serverStart((void *) ports[0]);
        if(fork() > 0){
            serverStart((void *) ports[1]);
        } else {
            serverStart((void *) ports[2]);
        }
    } else {
        serverStart((void *) ports[3]);
    }

    return 0;
}
