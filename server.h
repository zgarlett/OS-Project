//Operating Systems
//Group K
//3/30
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

struct client{

	int index;
	int sockID;
	struct sockaddr_in clientAddr;
	int len;

};

void* serverStart(void *vargs);
