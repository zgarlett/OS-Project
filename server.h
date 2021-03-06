
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/ipc.h> 	// Used by IPC maechanisms: messages, shared memory and semaphores
#include <sys/msg.h>	// for message queue structures
#include <string.h>		// for string operation: strlen
#include <sys/wait.h>	// for wait
#include <fcntl.h>		// for file control options
#include <errno.h>		// for system error numbers
#include <mqueue.h>
#include <time.h>
#include "fileManager.h"

#ifndef __SERVER_H__
#define __SERVER_H__

//using tylers bidItems struct from fileManager.H
//this is unused
typedef struct{
	
	int index;
	int buyer_or_seller;//- if buyer 0 if seller 1
	int clientID; 
	int sockID;
	//NOTICE: i commented out the below cause of errors being thrown
	//struct sockaddr_in clientAddr;
	int len;
	int num_items_selling;
	int num_items_buying;
}Client;

//using tylers SoldItems struct from fileManager.H
//this is unused
typedef struct{
	int itemID;
	int sellerID;
	char *name;
	double startprice;
}Item;

//zach's message passing	
void * serverStart(void *vargs);
void * listenMessageServer(void *vargs);
void * acceptConnections(void *vargs);
void sendMessageToServers(char * message);
void checkRecievedMessage(char * message);

//starkeys initial skeleton methods (most are no longer used)
//checks for ID 
int checkID(int buyer_or_seller);
//actually finds ID
Client findID(int userID);
//checks if buying or bidding
int check_buy_or_bid();
//if seller check add or remove
int check_add_or_remove();

//starkeys methods we are using 
//adds item to list
void create_item(int userID, int socket);
//finds item by itemID
Item find_item_by_itemID(int itemID);
//print current items
void print_current_items(int newSocket);
//print an item from the sellers list
void print_item(int itemID,int newSocket);

//kyles methods
//Processes bid data
int processBid(int itemID, float bidAmount, int clientID, int clientSocket);

#endif /*__SERVER_H__*/

