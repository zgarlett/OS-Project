
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

#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

/*Starkey's change list
* Added ItemID to biditem to find specific items
* Added buyerId to SoldItem to keep track of who buys what
* commented everything here
* all the includes etc
*/

/*Struct BidItem: Struct to contain all the information relative to items being put up for bidding
*@params
* int bidID - this is the ID of the user with the current highest bid
* int itemID - this is the ID of the item 
* int sellerID - this is the ID of the user who is selling the item
* char itemName[50] - this is the name of the item
* int itemQuantity - this is unused but can hold the amount of that item
* float startingBid - this is the starting bid amount
* float currentBid - this is the current highest bid
* int bidEndDate - this is an int value that represents a future moment in time
* char merchantInformation[50] - this is any information the merchant chose to include
*/ 
typedef struct{
    int bidID;
	int itemID;
	int sellerID;
    char itemName[50];
	//probs dont need quantity
    int itemQuantity;
    float startingBid;
    float currentBid;
	//how tf do we deal with bid end date?
    int bidEndDate;
    char merchantInformation[50];
}BidItem;

/*Struct SoldItem: Struct to contain all the information relative to an item that has already been soldID
* @params
* int soldID - 
* int itemID - this is the ID of the item
* int buyerID - this is the userID of the user who won the auction for this item
* char itemName[50] - Name of the item
* char buyInformation[50] - Any excess information that might be relavent after the purchase
* float buyingPrice - the amount that the item was sold for
* int center - which server the winner is located on 
* char purchaseDate[50] - when the item was purchased
*/
typedef struct{
    int soldID;
	int itemID;
	int buyerID;
	char itemName[50];
    char buyInformation[50];
    float buyingPrice;
    int center;
    char purchaseDate[50];
}SoldItem;

//Reads all items from the fileA and returns a single item. (Iteratable)
BidItem readfa(int ID);
//same thing as readfa except for file B 
SoldItem readfb(int ID);
//writes to file A
void writefa(BidItem bi);
//writes to file B
void writefb(SoldItem item);
//no idea
void testCreate();

//starkeys new methods change list
//gets items by their itemID
BidItem bget_item_by_id(int itemID);
SoldItem Sget_item_by_id(int itemID);
//gets items by buyer ID;
SoldItem Sget_item_by_buyerID(int buyerID);
//gets number of bid items
int get_bid_item_num();
//gets the number of available items to buyInformation
int get_user_buy_item_num(int userID);
//remove item from FILE B
void Sremove_item(int itemID);
//remove item from FILE A
void Bremove_item(int itemID);
//runs through File A and find's what user is selling for removal
int Bget_item_by_sellerID(int sellerID);
//Get an ID from a specified name
int getID_by_name(char itemName[16]);
#endif /*__FILEMANAGER_H__*/
