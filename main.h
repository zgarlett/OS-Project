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


int main(int argc, char const *argv[]);
