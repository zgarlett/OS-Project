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

void * serverStart2(void *vargs);
void * listenMessageServer2(void *vargs);
void * acceptConnections(void *vargs);


int main(){


  //Creating thread for server
  pthread_t thread;
  pthread_create(&thread, NULL, serverStart2, NULL);
  pthread_join(thread,NULL);

}

void* serverStart2(void *vargs){

    int serverSocket, val;

    struct sockaddr_in serverAddr, cli_addr;
    socklen_t sin_size;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8182);
    serverAddr.sin_addr.s_addr = htons(INADDR_ANY);

    if((serverSocket = socket(PF_INET, SOCK_STREAM, 0)) == 0 ){
        perror("Error creating socket\n");

    } else {
        printf("Socket was made sucessfully\n");
    }

    if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) < 0){
        perror("Error binding socket\n");

    } else {
        printf("Bind was sucessful\n");
    }


    if (listen(serverSocket,10) < 0) {
        perror("Listen failed\n");
    } else {
        printf("Listening for connections\n");
    }

    //Infinitely accept connections while running.
    pthread_t t1,t2;

    pthread_create(&t1,NULL,acceptConnections, (void *) &serverSocket);
    pthread_create(&t2,NULL,listenMessageServer2,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    return NULL;

}

void * acceptConnections(void * vargs){

  char * ask = "Input Test: ";
  char buffer[1024];
  int servSock = *(int*) vargs;

  mqd_t server1,server3,server4;
  struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MESSAGES;
  attr.mq_msgsize = MAX_MSG_SIZE;
  attr.mq_curmsgs = 0;

  server1 = mq_open (QUEUE_SERVER1, O_WRONLY | O_CREAT, PERMISSIONS, &attr);
  server3 = mq_open (QUEUE_SERVER3, O_WRONLY | O_CREAT, PERMISSIONS, &attr);
  server4 = mq_open (QUEUE_SERVER4, O_WRONLY | O_CREAT, PERMISSIONS, &attr);


  int clientfd,val;
  while (clientfd = accept(servSock,NULL,NULL)){
      //Socket for client
      printf("Waiting for connections on Server 1...\n");

      //Accept incoming client connections.
      printf("Connected to Server 1!\n");


      send(clientfd, ask , strlen(ask), 0);
      val = recv(clientfd, buffer, 1024,0);
      printf("Messge from Client: %s\n", buffer);


      mq_send (server1, buffer, strlen (buffer) + 1, 0);
      mq_send (server3, buffer, strlen (buffer) + 1, 0);
      mq_send (server4, buffer, strlen (buffer) + 1, 0);
      bzero(buffer,1024);



  }

  close(servSock);
}

void * listenMessageServer2(void * vargs){

    mqd_t server2;

    char in_buffer[MSG_BUFFER_SIZE];
    server2 = mq_open(QUEUE_SERVER2, O_RDONLY);
    while(1){
      if(mq_receive (server2, in_buffer, MSG_BUFFER_SIZE, NULL) > 0){
        printf("Server 2 recieved: %s\n", in_buffer);
      }


    }


}
