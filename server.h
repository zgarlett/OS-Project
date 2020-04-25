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

#define QUEUE_SERVER1   "/server1"
#define QUEUE_SERVER2   "/server2"
#define QUEUE_SERVER3   "/server3"
#define QUEUE_SERVER4   "/server4"
#define PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10
#define MAX 9000

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

#define QUEUE_SERVER1   "/server1"
#define QUEUE_SERVER2   "/server2"
#define QUEUE_SERVER3   "/server3"
#define QUEUE_SERVER4   "/server4"
#define PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10
#define MAX 9000
#ifndef __SERVER_H__
#define __SERVER_H__
  

void * serverStart(void *vargs);
void * listenMessageServer(void *vargs);
void * acceptConnections(void *vargs);
void sendMessageToServers(char * message);

void* serverStart(void *vargs){

    int serverSocket, val;
    int port = *(int *) vargs;
    setServerNumber(port);
    struct sockaddr_in serverAddr, cli_addr;
    socklen_t sin_size;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htons(INADDR_ANY);

    if((serverSocket = socket(PF_INET, SOCK_STREAM, 0)) == 0 ){
        perror("Error creating socket\n");

    } else {
        printf("Socket was made sucessfully for %d\n",port);
    }

    if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) < 0){
        perror("Error binding socket\n");

    } else {
        printf("Bind was sucessful for %d\n",port);
    }


    if (listen(serverSocket,10) < 0) {
        perror("Listen failed\n");
    } else {
        printf("Listening for connections on %d\n",port);
    }

    //Infinitely accept connections while running.
    pthread_t t1,t2;

    pthread_create(&t1,NULL,acceptConnections, (void *) &serverSocket);
    pthread_create(&t2,NULL,listenMessageServer,(void *) &port);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    return NULL;

}

void * acceptConnections(void * vargs){

  char * ask = "Input Test: ";
  char buffer[1024];
  int servSock = *(int*) vargs;

  printf("Waiting for connections on Server %d...\n", serverNumber);

  int clientfd,val;
  while (clientfd = accept(servSock,NULL,NULL)){


      //Accept incoming client connections.
      printf("Connected to Server %d!\n",serverNumber);


      send(clientfd, ask , strlen(ask), 0);
      val = recv(clientfd, buffer, 1024,0);
      printf("Messge from Client: %s\n", buffer);


      sendMessageToServers(buffer);
      bzero(buffer,1024);
  }
  close(servSock);
}

void * listenMessageServer(void * vargs){

    mqd_t server1,server2,server3,server4;
    int portNum = *(int *) vargs;
    char in_buffer[MSG_BUFFER_SIZE];

    if(portNum == 8181){
      server1 = mq_open(QUEUE_SERVER1, O_RDONLY);
      while(1){
        if(mq_receive (server1, in_buffer, MSG_BUFFER_SIZE, NULL) > 0){
          printf("Port %d recieved: %s\n", portNum, in_buffer);
        }
      }
    }
    if(portNum == 8182){
      server2 = mq_open(QUEUE_SERVER2, O_RDONLY);
      while(1){
        if(mq_receive (server2, in_buffer, MSG_BUFFER_SIZE, NULL) > 0){
          printf("Port %d recieved: %s\n", portNum, in_buffer);
        }
      }
    }
    if(portNum == 8183){
      server3 = mq_open(QUEUE_SERVER3, O_RDONLY);
      while(1){
        if(mq_receive (server3, in_buffer, MSG_BUFFER_SIZE, NULL) > 0){
          printf("Port %d recieved: %s\n", portNum, in_buffer);
        }
      }
    }
    if(portNum == 8184){
      server4 = mq_open(QUEUE_SERVER4, O_RDONLY);
      while(1){
        if(mq_receive (server4, in_buffer, MSG_BUFFER_SIZE, NULL) > 0){
          printf("Port %d recieved: %s\n", portNum, in_buffer);
        }
      }
    }
}

void sendMessageToServers(char * message){

  mqd_t server1,server2, server3,server4;
  struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MESSAGES;
  attr.mq_msgsize = MAX_MSG_SIZE;
  attr.mq_curmsgs = 0;

  server1 = mq_open (QUEUE_SERVER1, O_WRONLY | O_CREAT, PERMISSIONS, &attr);
  server2 = mq_open (QUEUE_SERVER2, O_WRONLY | O_CREAT, PERMISSIONS, &attr);
  server3 = mq_open (QUEUE_SERVER3, O_WRONLY | O_CREAT, PERMISSIONS, &attr);
  server4 = mq_open (QUEUE_SERVER4, O_WRONLY | O_CREAT, PERMISSIONS, &attr);

  mq_send (server1, message, strlen (message) + 1, 0);
  mq_send (server2, message, strlen (message) + 1, 0);
  mq_send (server3, message, strlen (message) + 1, 0);
  mq_send (server4, message, strlen (message) + 1, 0);

}
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

typedef struct{
	int itemID;
	int sellerID;
	char *name;
	double startprice;
}Item;
	
void* serverStart(void *vargs);

//checks for ID 
int checkID(int buyer_or_seller);
//actually finds ID
Client findID(int userID);
//checks if buying or bidding
int check_buy_or_bid();
//if seller check add or remove
int check_add_or_remove();
//adds item to list
Item create_item(int userID);
//finds item by itemID
Item find_item_by_itemID(int itemID);
//find item by userID
#endif /*__SERVER_H__*/

