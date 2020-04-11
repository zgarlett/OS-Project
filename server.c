//Operating Systems
//Group K
//3/30
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


struct client{

	int index;
	int sockID;
	struct sockaddr_in clientAddr;
	int len;

};


void* serverStart(void *vargs)
{
    int serverSocket;

	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = htons(INADDR_ANY);

    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0 ){
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
    while

    return NULL;

}
