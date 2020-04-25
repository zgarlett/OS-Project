#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
void connectToServer();

int main(int argc, char const *argv[]){

    connectToServer();

    return 0;
}


void connectToServer(){

    int clientSocket, ret;
    struct sockaddr_in serverAddr;
    char buffer[1024];
	char answer[1024];
	int ports[4] = {8081,8182,8183,8184};
	int portPosition = 0;

	//Create socket for the client
    if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Client socket creation failure.\n");
		exit(1);
	}
    printf("Client socket has been opened successfully.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(ports[portPosition]);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
	{
		serv_addr.sin_port = htons(ports[++portPosition]);
		printf("Trying next service center\n");
		if(portPosition > 3)
		{
			printf("Failed to find service center to connect to.");
			exit(1);
		}
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
	
	    //Take in Item info and List
	    //Will send Item struct to be written to file
        if(strcmp((recv(clientSocket, buffer, 1024, 0)), "list") == 0){

            char tempchar[50];

            printf("Input bidID of item.\n");
            scanf("%s", tempchar);
            send(clientSocket, tempchar, strlen(tempchar), 0);

            printf("Input name of item.\n");
            scanf("%s", tempchar);
            send(clientSocket, tempchar, strlen(tempchar), 0);

            printf("Input quantity of item.\n");
            scanf("%s", tempchar);
            send(clientSocket, tempchar, strlen(tempchar), 0);

            printf("Input starting bid of item.\n");
            scanf("%s", tempchar);
            send(clientSocket, tempchar, strlen(tempchar), 0);

            printf("Input the end date of bidding for item. MM/DD/YYYY\n");
            scanf("%s", tempchar);
            send(clientSocket, tempchar, strlen(tempchar), 0);

            printf("Input the merchant onformation for item.\n");
            scanf("%s", tempchar);
            send(clientSocket, tempchar, strlen(tempchar), 0);
        }
	    
        else
            {
            printf("Server: \t%s\n", buffer);
        }
    }

    return 0;
}
