#include "server.h"
#define PORT 8080

//Start up the server
void* serverStart(void* vargs)
{
    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[1024];
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("Error in connecting to server.\n");
        exit(1);
    }
    printf("Server socket has been opened.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        printf("Error in binding to port.\n");
        exit(1);
    }
    printf("Binded to port %d\n", 8080);

    if(listen(sockfd, 10) == 0){
        printf("Listening for new clients..\n");
    }else{
        printf("Error in binding to port.\n");
    }


    while(1){
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        if(newSocket < 0){
            exit(1);
        }
        printf("Connection accepted from new client!\n");

        if((childpid = fork()) == 0){
            close(sockfd);

            while(1){

                //Receive clients input here
                recv(newSocket, buffer, 1024, 0);

                //Then compare the input and perform the correct result
                //Disconnect from client when "exit" is received.
                if(strcmp(buffer, "exit") == 0){
                    printf("Disconnected from a client\n");
                    break;
                }
                //When server receives buyer, we can mark the client down as a buyer, able to be changed.
                if(strcmp(buffer, "buyer") == 0)
                {
                    strcpy(buffer, "Thank you for connecting to the server, you have indicated you are a buyer, please type 'seller' if you would like to change type");
                    send(newSocket, buffer, strlen(buffer), 0);
                    //Clean out buffer
                    bzero(buffer, sizeof(buffer));

                    //Then we can go ahead and see what the buyer wants
                }
                //When server receives seller, we can mark the client down as a seller, able to be changed.
                if(strcmp(buffer, "seller") == 0)
                {
                    strcpy(buffer, "Thank you for connecting to the server, you have indicated you are a seller, please type 'buyer' if you would like to change type");
                    send(newSocket, buffer, strlen(buffer), 0);
                    bzero(buffer, sizeof(buffer));
                }


                else
                {
                    printf("Client: %s\n", buffer);
                    send(newSocket, buffer, strlen(buffer), 0);
                    bzero(buffer, sizeof(buffer));
                }
            }
        }

    }

    close(newSocket);
}



int main(){
//Creating thread for server
    pthread_t thread;
    pthread_create (&thread, NULL, serverStart, NULL);
    pthread_join(thread, NULL);


    return 0;

}
