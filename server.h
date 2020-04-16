#include <stdio.h>
#ifndef __SERVER_H__
#define __SERVER_H__

typedef struct{
	
	int index;
	int buyer_or_seller;//- if buyer 0 if seller 1
	int clientID; 
	int sockID;
	//NOTICE: i commented out the below cause of errors being thrown
	//struct sockaddr_in clientAddr;
	int len;
	int num_items_selling;
	int num_items_buying;
}Client;

typedef struct{
	int itemID;
	int sellerID;
	char *name;
	double startprice;
}Item;
	
void* serverStart(void *vargs);

//checks for ID 
int checkID(int buyer_or_seller);
//actually finds ID
Client findID(int userID);
//checks if buying or bidding
int check_buy_or_bid();
//if seller check add or remove
int check_add_or_remove();
//adds item to list
Item create_item(int userID);
//finds item by itemID
Item find_item_by_itemID(int itemID);
//find item by userID


#endif /*__SERVER_H__*/
