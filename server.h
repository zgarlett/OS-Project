//Operating Systems
//Group K
//3/30
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080


struct client{

	int index;
	int sockID;
	struct sockaddr_in clientAddr;
	int len;

};

void* serverStart(void *vargs);
