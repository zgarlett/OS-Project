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

//Begins a server that accepts bids and other commands.ß
void* serverStart(void *vargs);

//Checks if a char array is numbers or not
int isNumeric (const char * s);

//Other functions that will be needed but are not implemented quite yet.
//Processing a bid using the item name and bid amount, return statement will indicate success or fail.
int processBid(char itemName[16], float bidAmount);

//Process an item removal, may need to take in seller ID as well?
//return statement will indicate success or fail
int processRemoval(char itemName[16]);

//Send receipt to all clients, not sure what arguments this will need yet.
void sendReceipt();

