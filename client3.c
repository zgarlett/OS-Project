#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
//Define port to check for server connection.
#define PORT 8183

void connectToServer();

char buffer[1024];
char answer[1024];

int main(int argc, char const *argv[]){

    connectToServer();

    return 0;
}

void connectToServer(){
    //Initialize some variables and the message client replies back to server with.
    int sock = 0;
    int readVal;
    struct sockaddr_in serv_addr;


    //Create a socket for the client.
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
    }

    //Set some properties of the server address we are connecting to.
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address to binary.
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
    }

    //Connect to our server socket.
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
    }
    else
    {
        printf("\nConnection success.\n");
        readVal = read(sock, buffer, 1024);
        printf("%s\n",buffer);
        scanf("%s", answer);
        send(sock, answer ,strlen(answer),0);
    }
    exit(0);
}
