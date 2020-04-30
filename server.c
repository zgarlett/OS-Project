//Operating Systems
//Group K
//3/30

#include "server.h"
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
#include <ctype.h>

#define QUEUE_SERVER1   "/server1"
#define QUEUE_SERVER2   "/server2"
#define QUEUE_SERVER3   "/server3"
#define QUEUE_SERVER4   "/server4"
#define PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10
#define MAX 9000
//Not sure what these are for at the moment, these I had to add to fix compiler errors.
#define FILE_WRITTEN 1
#define AUCTION_END 1

//Function to check if a char array or pointer is a number.
int isNumeric (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
        return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}

void* serverStart(void *vargs)
{
    int serverSocket, newSocket;
    char buffer[1024];
	int port = *(int *) vargs;
	struct sockaddr_in serverAddr, cli_addr;
	pid_t childpid;
    socklen_t sin_size;
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
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
    //MAIN SERVER LOOP
	while(1){
        newSocket = accept(serverSocket, (struct sockaddr*)&cli_addr, &sin_size);
        if(newSocket < 0){
            exit(1);
        }
        printf("Connection accepted from new client!\n");
		//~~is this issue? {}
        if((childpid = fork()) == 0){
            close(serverSocket);
			//buyer or seller flag: buyer = 0 | seller = 1 | 2 if not set
			int buyer_or_seller = 2;
			
			//gets id and checks if it exists returns only if ID found atm
			//int userID = checkID(buyer_or_seller);
			
			//gets the client associated with the ID
			//Client client = findID(userID);
		
			//Flags used for accepting client input properly, below is set when preparing to read a numeric input (bid amount) from the client
			int bidFlag = 0;
			//This flag is used when preparing to accept the item name to bid on.
			int bidName = 0;
			
			//this is the ItemID of the item we are currently dealing with
			int itemID = 0;

			//Temporary storage variables
			char itemName[16];
			float bidAmount;
			while(1){
				if(buyer_or_seller == 2){
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
						buyer_or_seller = 0;
						strcpy(buffer, "Thank you for connecting to the server, you have indicated you are a buyer, please type 'seller' if you would like to change type");
						send(newSocket, buffer, strlen(buffer), 0);
						//Clean out buffer
						bzero(buffer, sizeof(buffer));

						//Then we can go ahead and see what the buyer wants
					}
					//When server receives seller, we can mark the client down as a seller, able to be changed.
					if(strcmp(buffer, "seller") == 0)
					{
						buyer_or_seller = 1;
						strcpy(buffer, "Thank you for connecting to the server, you have indicated you are a seller, please type 'buyer' if you would like to change type");
						send(newSocket, buffer, strlen(buffer), 0);
						bzero(buffer, sizeof(buffer));
					}
					
					else
					{
						printf("Client: %s\n", buffer);
						strcpy(buffer, "Please indicate either buyer or seller");
						send(newSocket, buffer, strlen(buffer), 0);
						bzero(buffer, sizeof(buffer));
					}
				}
				
				//~~~~~~~~~~~~~~~~~BUYER AREA~~~~~~~~~~~~~~~~~~~
				//When bid flag is enabled, next input will be registered as bid amount
				if(buyer_or_seller == 0){
					recv(newSocket, buffer, 1024, 0);
					if(isNumeric(buffer) && bidFlag == 1)
					{
						char tempString[180];
						bidAmount = (float)atof(buffer);
						printf("Bid accepted, item name was: %s, bid amount is %f\n", itemName, bidAmount);
						//TODO does this work? do we need \n?
						sprintf(tempString, "Bid accepted, item name was: %s, bid amount is %f\n", itemName, bidAmount);
						strcpy(buffer, tempString);
						send(newSocket, buffer,strlen(buffer), 0);
						bzero(buffer, sizeof(buffer));
						//TODO send above to user as well
						bidFlag = 0;

						//Then we would want to pass off the data to a bidding function that will check for closing time, and register the bid.
					}
					else if(strcmp(buffer, "buy") == 0)
					{
						//TODO 
						//number of items to buy
						int buynum = get_user_buy_item_num(userID);
						if(buynum > 0){
							for(int i= 0; i< buynum;i++){
								
							}
						}else{
							strcpy(buffer, "There are no items for you to buy!");
							send(newSocket, buffer,strlen(buffer), 0);
							bzero(buffer, sizeof(buffer));
						}
					}

					//When bid name flag has been set, server will attempt to read in the item name, and save it temporarily.
					else if(bidName == 1)
					{
						//TODO we need to send out a list of items to them then accept an input. Might be better to do this based on item ID for ease of use and checking
						//Now we would need to check if item name exists here, for now we will simply accept the input
						strcpy(itemName, buffer);
						//Then unset the flag, and set the bidFlag to check for next argument (amount)
						bidName = 0;
						bidFlag = 1;

						printf("Item name accepted, checking for next argument (amount) \n");
					}

					else if(strcmp(buffer, "bid") == 0)
					{
						//Server has picked up the bid argument, it will expect some more arguments (the item ID) to arrive as well, it will see if it picks up, and if not
						//alert the user that they did not enter their item id
						printf("Received bid, checking for next argument (name)\n.");
						strcpy(buffer, "Here are all the items available for bidding:");
						send(newSocket,buffer,strlen(buffer),0);
						bzero(buffer, sizeof(buffer));
						print_current_items();
						
						strcpy(buffer, "What is the item ID of what you would like to bid on");
						send(newSocket,buffer,strlen(buffer),0);
						bzero(buffer, sizeof(buffer));
						recv(newSocket, buffer, 1024, 0);
						itemID = atoi(buffer);
						
						//Set flag to look for item name next.
						bidName = 1;
					}
					else if(strcmp(buffer, "list") == 0)
					{
						//TODO send statement here waiting on other dude
						//"Commands: exit, bid"
						strcpy(buffer,"Commands: list, exit, bid");
						send(newSocket, buffer,strlen(buffer),0);
						bzero(buffer, sizeof(buffer));
					}
					else
					{
						strcpy(buffer,"I didn't recognize that. Commands are: list, exit, bid");
						send(newSocket, buffer,strlen(buffer),0);
						bzero(buffer, sizeof(buffer));
					}
				}
				//~~~~~~~~~~~~~~SELLER AREA~~~~~~~~~~~~~~~~~
				//Seller function, to request the removal of an item, we will need a function to process this.
				if(buyer_or_seller == 1){
					recv(newSocket, buffer, 1024, 0);
					if(strcmp(buffer, "remove") == 0)
					{
						//Need method that will process the input, will need to implement another flag to get the input as well probably.
						///~!~~~~~~~~~~~~~~~~~heere~~~~~~~~~~~~~~~~
					}
					if(strcmp(buffer, "sell") == 0)
					{
						//add item
						create_item(userID, newSocket);
						
					}
					if(strcmp(buffer, "list") == 0)
					{
						//TODO send statement here waiting on other dude
						//"Commands: exit, bid"
						strcpy(buffer,"Commands: list, exit, sell, remove");
						send(newSocket, buffer,strlen(buffer),0);
						bzero(buffer, sizeof(buffer));
					}
					else
					{
						strcpy(buffer,"I didn't recognize that. Commands are: list, exit, sell, remove");
						send(newSocket, buffer,strlen(buffer),0);
						bzero(buffer, sizeof(buffer));
					}
				}
			}
		}
		else if (childpid > 0){
			//Listen for IPC based on port passed in methdod serverStart(void * port)
			//To send a message to each server use the method sendMessageToServers(char * message)
			//Depending on the message the server will read FILE_A and FILE_B again, or send messages to all clients
			//How are we going to keep track of all the clients???
			listenMessageServer((void *) port);
		}
	}
	//close(newSocket);
}
void print_current_items(){
	BidItem item;
	char buffer[1024];
	int num = get_bid_item_num();
	for(int i = 0;i < num; i++){
		item = readfa(i);
		char tempString[128];
		sprintf(tempString, "Item ID: %d", item.itemID);
		strcpy(buffer, tempString);
		send(newSocket,buffer,strlen(buffer),0);
		bzero(buffer, sizeof(buffer));
		
		sprintf(tempString, "Item Name: %s", item.itemName);
		strcpy(buffer, tempString);
		send(newSocket,buffer,strlen(buffer),0);
		bzero(buffer, sizeof(buffer));

		sprintf(tempString, "Starting Bid: %f", item.startingBid);
		strcpy(buffer, tempString);
		send(newSocket,buffer,strlen(buffer),0);
		bzero(buffer, sizeof(buffer));
		
		sprintf(tempString, "Bid End Date: %s", item.bidEndDate);
		strcpy(buffer, tempString);
		send(newSocket,buffer,strlen(buffer),0);
		bzero(buffer, sizeof(buffer));
	}
}
void print_item(){
	SoldItem item;
	char buffer[1024];
	////~!~~~~~~~~~~~~~~~~~heere~~~~~~~~~~~~~~~~
	
}

//this should be unused 
int buy_or_sell_test(){
	int buyer_or_seller = 0;
	int bORs_test = 0;
	while(bORs_test == 0){
		printf("Are you a buyer or seller? (type 0 for buyer, 1 for seller)\n");
		scanf("%d",&buyer_or_seller);
		if(buyer_or_seller == 0	|| buyer_or_seller == 1){
			bORs_test = 1;
			return buyer_or_seller;
		}else{
			printf("I know reading is hard but I know children under 5 who listen better than you\n");
			printf("please try again...\n");
		}
	}
}

//find item by userID
void find_item_by_userID(Client user){
	//TODO look below at file name
	/*gets file of users fix the below line when we know name*/
	FILE *infile = fopen("SOMETHINGABOUTITEMS.TEXT","r");
	if(infile == NULL){ printf("file wasn't loaded\n");}
	Item item;
	while(item.itemID != NULL){
		fread(&item, sizeof(item),1,infile);
		if(item.sellerID == user.clientID){
			printf("itemID:%d, itemName:%s, item StartPrice:%f\n",item.itemID,item.name,item.startprice);
		//TODO alot more needed here but I'm done for today
			printf("is this your item?\n");
		}
	}


}
//find item by ID
Item find_item_by_itemID(int itemID){
	//TODO this will probs be needed for confirming and stuff
}
//adds item to file
void create_item(int userID, int socket){
	char buffer[1024];
	int itemID = 0;
	char name[20];
	float startprice = 0;
	int endtime = 0;
	time_t now = time(0);
	char merchinfo[100];
	
	//get itemID
	strcpy(buffer,"Please enter the itemID");
	send(socket, buffer,strlen(buffer),0);
	bzero(buffer, sizeof(buffer));
	recv(socket, buffer, 1024, 0);
	itemID = atoi(buffer);
	bzero(buffer, sizeof(buffer));
	printf("ItemID: %d\n",itemID);
	
	//get name of item
	strcpy(buffer,"Please enter the item name");
	send(socket, buffer,strlen(buffer),0);
	bzero(buffer, sizeof(buffer));
	recv(socket, buffer, 1024, 0);
	strcpy = (name, buffer);
	bzero(buffer, sizeof(buffer));
	printf("Item Name: %s\n",name);
	
	//get start price
	strcpy(buffer,"Please enter the starting bid price");
	send(socket, buffer,strlen(buffer),0);
	bzero(buffer, sizeof(buffer));
	recv(socket, buffer, 1024, 0);
	startprice = atof(buffer);
	bzero(buffer, sizeof(buffer));
	printf("Item startprice: %f\n",startprice);
	
	//get end time
	strcpy(buffer,"Please enter the amount of time in minutes from now you want the bidding to end on this item");
	send(socket, buffer,strlen(buffer),0);
	bzero(buffer, sizeof(buffer));
	recv(socket, buffer, 1024, 0);
	endtime = atoi(buffer);
	endtime = now + (endtime * 60);
	bzero(buffer, sizeof(buffer));
	printf("item start time: %f\n",now);
	printf("Item endtime: %f\n",endtime);
	
	BidItem item;
	item.itemID = itemID;
	strcpy(item.itemName,name);
	item.startingBid = startprice;
	item.sellerID = userID;
	item.bidEndDate = endtime; 
	strcpy(item.merchantInformation, merchinfo);
	writefa(item);
}
//method for checking ID
//doesn't use the buyer_or_seller method but I included because it might be useful depending on how we save clients.
int checkID(int buyer_or_seller){
//this method assumes the user has an ID there is no exit without one
//TODO ~~~need to make exit without id~~~
	int userID = 0;
	int test = 0;
	//gets user id tests if its valid etc
	while(test == 0){
		printf("Please enter your ID \n");
		//gets userID
		scanf("%d",&userID);
		if(userID > 200){
			//method findID returns clientid == 0 if not found
			Client client = findID(userID);
			//checks if found
			if(client.clientID == 0){
				printf("userID wasn't found please try again\n");
			}else{return userID;}
		}else{
			printf("Please stop making my life difficult and enter ur id...\n");
		}
	}
}
//searches through file for ID returns client if found returns a client with ID == 0 if not
Client findID(int userID){
	/*gets file of users fix the below line when we know name*/
	FILE *infile = fopen("SOMETHINGHERE.TEXT","r");
	if(infile == NULL){ printf("file wasn't loaded\n");}

	//Client used for iterating
	Client client;
	//read each struct in from file and tests them against ID.
	int counter = 0;

	//~~~~~~~~~This test case will need to be changed this will cause segfault we just need to figure out how we will determine total amount of users~~~~~~~~~~~~~~!

	while(client.clientID != NULL){
		fread(&client, sizeof(Client),1,infile);
		counter++;
		if(client.clientID == userID){return client;}
	}
	//if clientID == 0 then wasn't found
	Client client1;
	client1.clientID = 0;
	return client1;
}
//I dont think anything below this is used
//checks if buying or bidding
int check_buy_or_bid(){
	int buy_or_bid = 10; //10 base val. 0 = buy | 1 = bid
	int test = 0;
	while(test == 0){
		printf("Do you want to buy or bid?(0 for buy, 1 for bid)\n");
		scanf("%d", &buy_or_bid);
		if(buy_or_bid == 1 || buy_or_bid == 0){
			return buy_or_bid;
		}else{
			printf("Stop trying to break me please I'm fragile\n");
		}
	}
}
//does what name says
int check_add_or_remove(){
	//option to add or remove items
	int add_or_remove = 10; //10 base, 0 = add | 1 = remove
		while(0){
			printf("would you like to add or remove items?(0 if add, 1 if remove)\n");
			scanf("%d",&add_or_remove);
			if(add_or_remove == 0 || add_or_remove == 1){
				return add_or_remove;
			}else{
				printf("STAHP ItS 0 Or 1 iTS nOt ThAt haRD\n");
				printf("try again...\n");
			}
		}
}

//Needed only if we are going to use threads
void * acceptConnections(void * vargs){

	char * ask = "Input Test: ";
  char buffer[1024];
  int servSock = *(int*) vargs;

  printf("Waiting for connections on Server ...\n");

  int clientfd,val;
  while (clientfd = accept(servSock,NULL,NULL)){


      //Accept incoming client connections.
      printf("Connected to Server!\n");


      send(clientfd, ask , strlen(ask), 0);
      val = recv(clientfd, buffer, 1024,0);
      printf("Messge from Client: %s\n", buffer);


      sendMessageToServers(buffer);
      bzero(buffer,1024);
  }
  close(servSock);
}

//Listen for messages on infinite loop
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


//Send message to all service centers, including self
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

//Use this message after writing to file, or other major processes that others
//service centers need to know about. Please use the constants at the top of the file
void checkRecievedMessage(char * message){
	//if the file has changed
	if(strcmp(message,FILE_WRITTEN) == 0){
		//read file again method
		//print to clients method

		//if auction has finished
	} else if(strcmp(message,AUCTION_END) == 0){
		//tell all clients method

	}
}
