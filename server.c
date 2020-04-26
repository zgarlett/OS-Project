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
	int port = *(int *) vargs;
	struct sockaddr_in serverAddr, cli_addr;
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
        newSocket = accept(serverSocket, (struct sockaddr*)&cli_addr, &addr_size);
        if(newSocket < 0){
            exit(1);
        }
        printf("Connection accepted from new client!\n");

        if((childpid = fork()) == 0){
            close(serverSocket);

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
                //Seller function, to request the removal of an item, we will need a function to process this.
                if(strcmp(buffer, "remove") == 0)
                {
                    //Need method that will process the input, will need to implement another flag to get the input as well probably.
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

	/*~~~~things to do/note here~~~~
	* We cant do this iteratively for every client we will need to probably fork here for each client. Forking doesn't work well on my vm will ask others
	* Might need to restructure for forkage. 
	* I'll write out here the code each client will use but forking will need to be discussed between us
	*/

//gets whether he a buyer or seller (could make this a method)
//buyer = 0 | seller = 1 
	int buyer_or_seller = 0;
	int bORs_test = 0;
	while(bORs_test == 0){
		
		printf("Are you a buyer or seller? (type 0 for buyer, 1 for seller)\n");
		scanf("%d",&buyer_or_seller);
		if(buyer_or_seller == 0	|| buyer_or_seller == 1){
			bORs_test = 1;			
		}else{
		printf("I know reading is hard but I know children under 5 who listen better than you\n");
		printf("please try again...\n");
		}
	}
//going to make most stuff a method from here on. Threading could give incorrect results though need to be careful here. 
	//gets id and checks if it exists returns only if ID fount atm
	int userID = checkID(buyer_or_seller);
	//gets the client associated with the ID
	Client client = findID(userID);
	//if buyer
	if(buyer_or_seller == 0){
		//checks if buying or bidding: 0 = buy | 1 = bid 
		int buy_or_bid = check_buy_or_bid();
		//if buy
		if(buy_or_bid == 0){
			printf("retrieving list of items you have won the bid for\n");
			//TODO write code for this. (need other parts done for this)
			//cant method this out since we could be returning an array	
		}
		//if bid
		if(buy_or_bid == 1){
			printf("retrieving list of items currently open for bidding\n");
			//TODO write code for this. (need other parts done for this)
			//cant method this out since we could be returning an array
		}
	}
	//if seller
	if(buyer_or_seller == 1){
		int add_or_remove = check_add_or_remove();
		//if add
		if(add_or_remove == 0){
			Item item = create_item(userID);
			//TODO add to file. (need other parts done for this)	
			//make a method for this
			//make sure to increase num_items_selling		
		}
		//if remove
		if(add_or_remove == 1){
			//find item
			//TODO case of more than one item
			//make sure to decrease num_items_selling
		}
	}
	
        //Close connection with client.
        close(clientfd);
    }
    close(serverSocket);
    return NULL;
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
Item create_item(int userID){
	int itemID = 0;
	char name[20];
	double startprice = 0;
	//not putting checks here cause thats work and we can polish later
	//get item id
	printf("What is the ID of your item?\n");
	scanf("%d",&itemID);
	//get name of item
	printf("What is the name of your item?\n");
	scanf("%s",name);
	//get start price
	printf("What is the start price of the item?\n");
	scanf("%lf",&startprice);
	Item item;
	item.itemID = itemID;
	item.name = &name;
	item.startprice = startprice;
	item.sellerID = userID;
	return item;
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
