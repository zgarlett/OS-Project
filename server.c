#include "server.h"
#include "ctype.h"
#define PORT 8080


int isNumeric (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
        return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}

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

            //Flags used for accepting client input properly, below is set when preparing to read a numeric input (bid amount) from the client
            int bidFlag = 0;
            //This flag is used when preparing to accept the item name to bid on.
            int bidName = 0;

            //Temporary storage variables
            char itemName[16];
            float bidAmount;
            while(1){

                //Receive clients input here
                recv(newSocket, buffer, 1024, 0);
                //Then compare the input and perform the correct result
                //Disconnect from client when "exit" is received.
                if(strcmp(buffer, "exit") == 0){
                    printf("Disconnected from a client\n");
                    break;
                }
                //When bid flag is enabled, next input will be registered as bid amount
                if(isNumeric(buffer) && bidFlag == 1)
                {
                    bidAmount = (float)atof(buffer);
                    printf("Bid accepted, item name was: %s, bid amount is %f\n", itemName, bidAmount);
                    bidFlag = 0;

                    //Then we would want to pass off the data to a bidding function that will check for closing time, and register the bid.
                }

                //When bid name flag has been set, server will attempt to read in the item name, and save it temporarily.
                if(bidName == 1)
                {
                    //Now we would need to check if item name exists here, for now we will simply accept the input
                    strcpy(itemName, buffer);
                    //Then unset the flag, and set the bidFlag to check for next argument (amount)
                    bidName = 0;
                    bidFlag = 1;

                    printf("Item name accepted, checking for next argument (amount) \n");
                }

                if(strcmp(buffer, "bid") == 0)
                {
                    //Server has picked up the bid argument, it will expect some more arguments (the item ID) to arrive as well, it will see if it picks up, and if not
                    //alert the user that they did not enter their item id
                    printf("Received bid, checking for next argument (name)\n.");

                    //Set flag to look for item name next.
                    bidName = 1;
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
