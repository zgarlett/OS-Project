//Operating Systems
//Group K
//3/30
	
#include "server.h"
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

	
	
void* serverStart(void *vargs)
{
    int serverSocket;

	struct sockaddr_in serverAddr, cli_addr;
    socklen_t sin_size;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
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
    while (1)
    {
        //Socket for client
        int clientfd;
        struct sockaddr_in client_addr;
        int addrlen=sizeof(client_addr);

        //Accept incoming client connections.
        clientfd = accept(serverSocket, (struct sockaddr*)&client_addr,(socklen_t *) &addrlen);

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
