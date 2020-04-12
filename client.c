#include "client.h"

void connectToServer()
{
    //Initialize some variables and the message client replies back to server with.
    int sock = 0;
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
        }
    exit(0);
}

int main(int argc, char const *argv[])
{
    connectToServer();

    return 0;
}