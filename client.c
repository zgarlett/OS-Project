#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main(){

    int clientSocket, ret;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0){
        printf("Error while connecting to server.\n");
        exit(1);
    }
    printf("Client socket has been opened.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        printf("Error when connecting to server (server may not be running?)\n");
        exit(1);
    }
    printf("Connected to server successfully, please input whether this client is a buyer or seller.\n");
    printf("You may type 'exit' to end the connection at any time.\n");

    //Send and receive loop
    while(1){
        printf("Input: \t");
        scanf("%s", &buffer[0]);
        send(clientSocket, buffer, strlen(buffer), 0);

        //When we send out exit, we go ahead and close the socket and exit, server will respond and close its connection with the client as well.
        if(strcmp(buffer, "exit") == 0){
            close(clientSocket);
            printf("Disconnected from server.\n");
            exit(1);
        }
        if(recv(clientSocket, buffer, 1024, 0) < 0){
            printf("Error in receiving data.\n");
        }
        else
            {
            printf("Server: \t%s\n", buffer);
        }
    }

    return 0;
}