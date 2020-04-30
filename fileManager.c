#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileManager.h"

//Tyler Porter
//Project
//CS 4323

//TODO:
//make a create() function to take input from user and write an item to file A
//make methods that control data movement from file A to file B once an item has been sold

//??Make a method to populate an array of BidItem structs for easier file management

//~~~wut is dis?
//int IDcheck;
//----------------
/*Starkey's change list
* Moved everything that needed to be in the .h file to that
* Created new method: basically same as readfa but takes item ID and returns the match
* Created new method: Also same as above but for readfb/sold items (uses soldID) 
* Created new method: gets soldItems by buyerID
* Created new method: Gets total number of items in file A
*/

//Item ID - Item Name - Item quantity - starting bid - bid end date - merchant info \n
//Given an ID, this method will return a struct of all info related to item
//TODO this might need to take in socket info so that the bid info can be sent to the user?????
BidItem readfa(int ID)
{
    BidItem item[100];

    FILE *fp;
    fp = fopen("FILE_A.bin", "rb");
    if (fp == NULL)
    {
        printf("Error opening file from read()\n");
    }


    int i = 0;

    while (!feof(fp))
    {
        fread(&item[i], sizeof(item[i]) + 1, 1, fp);
        i++;
    }


    fclose(fp);

    return item[ID];
}

int get_bid_item_num()
{
    BidItem item[100];

    FILE *fp;
    fp = fopen("FILE_A.bin", "rb");
    if (fp == NULL)
    {
        printf("Error opening file from read()\n");
    }


    int i = 0;

    while (!feof(fp))
    {
        fread(&item[i], sizeof(item[i]) + 1, 1, fp);
        i++;
    }


    fclose(fp);

    return i;


}
//find by itemID bid
BidItem bget_item_by_id(int itemID)
{
	BidItem item;

    FILE *fp;
    fp = fopen("FILE_A.bin", "rb");
    if (fp == NULL)
    {
        printf("Error opening file from read()\n");
    }


    int i = 0;

    while (!feof(fp))
    {
		
        fread(&item, sizeof(item) + 1, 1, fp);
		if(item.itemID == itemID){
			fclose(fp);
			return item;
		}
        i++;
    }

//really bad if it gets here will most likely give random answers ~~~~ 
    fclose(fp);

    return item;
}

SoldItem readfb(int ID)
{

    SoldItem item[100];

    FILE *fp;
    fp = fopen("FILE_B.bin", "rb");
    if (fp == NULL)
    {
        printf("Error opening file from read()\n");
    }


    int i = 0;

    while (!feof(fp))
    {
        fread(&item[i], sizeof(item) +1, 1, fp);
        i++;
    }


    fclose(fp);

    return item[ID];


}
//gets sold items by their item ID
SoldItem Sget_item_by_id(int itemID){

    SoldItem item;

    FILE *fp;
    fp = fopen("FILE_B.bin", "rb");
    if (fp == NULL)
    {
        printf("Error opening file from read()\n");
    }


    int i = 0;

    while (!feof(fp))
    {
        fread(&item, sizeof(item) +1, 1, fp);
		if(item.soldID == itemID){
			fclose(fp);
			return item;
		}
        i++;
    }

//just like above really bad if it hits this since there are no catches. 
    fclose(fp);

    return item;
}
//can only get one item at a time. DONT GIVE USER OPTION TO BUY ANYTHING BUT THE FIRST ITEM THAT COMES UP
SoldItem Sget_item_by_buyerID(int buyerID){

    SoldItem item;

    FILE *fp;
    fp = fopen("FILE_B.bin", "rb");
    if (fp == NULL)
    {
        printf("Error opening file from read()\n");
    }


    int i = 0;

    while (!feof(fp))
    {
        fread(&item, sizeof(item) +1, 1, fp);
		if(item.buyerID == buyerID){
			fclose(fp);
			return item;
		}
        i++;
    }

//just like above really bad if it hits this since there are no catches. 
    fclose(fp);

    return item;
}
//Takes in, and prints a BidItem to FILE A
void writefa(BidItem item)
{
    BidItem temp = item;
    FILE *fp;
    fp = fopen("FILE_A.bin", "a+");
    if (fp == NULL)
    {
        printf("Error opening file from writefa()");
    }

    fwrite(&item, sizeof(temp) + 1, 1, fp);


    //fprintf(fp, "%d\t%s\t%d\t%f\t%s\t%s\n", item.bidID, item.itemName, item.itemQuantity, item.startingBid, item.bidEndDate, item.merchantInformation);
    fclose(fp);
}
//Takes a SoldItem as a parameter and prints it to File B
void writefb(SoldItem item)
{
    FILE *fp;
    fp = fopen("FILE_B.bin", "a+");
    if (fp == NULL)
    {
        printf("Error opening file from writefb()");
    }

    fwrite(&item, sizeof(item) + 1, 1, fp);


    fprintf(fp, "%d\t%s\t%f\t%d\t%s\n",  item.soldID, item.buyInformation, item.buyingPrice, item.center, item.purchaseDate);
    fclose(fp);
}
//method to find # of items a user can buyInformation
int get_user_buy_item_num(int userID)
{
	SoldItem item[100];

    FILE *fp;
    fp = fopen("FILE_B.bin", "rb");
    if (fp == NULL)
    {
        printf("Error opening file from read()\n");
    }
    int i = 0;
	int count = 0;
    while (!feof(fp))
    {
        fread(&item[i], sizeof(item[i]) + 1, 1, fp);
		if(item.buyerID == userID){
			count++;
		}
        i++;
    }


    fclose(fp);

    return count;
}
void testCreate()
{
    int IDcheck;
    //Creates, writes, then reads back a test BidItem
    BidItem test = {IDcheck, "TestName", 1, 1, "MM/DD/YYYY", "Merchant Information"};
    IDcheck++;

    writefa(test);
    //struct BidItem temp = read(IDcheck-1);
    //printf("Test: %d\t%s\t%d\t%f\t%s\t%s\n", temp.bidID, temp.itemName, temp.itemQuantity, temp.startingBid, temp.bidEndDate, temp.merchantInformation);

}
/*
int main()
{
    //This opens and closes file to ensure westart with a blank file everytime
    FILE *fp;
    fp = fopen("FILE_A.bin", "w");
    fclose(fp);



    //ID check generates sequential ID's for items made in testCreate
    IDcheck = 1;
    for (int i = 0; i < 30; i++)
    {
        testCreate();
    }

    //More testing to try and read something from file
    struct BidItem temp;
    temp = readfa(20);
    printf("Test: %d\t%s\t%d\t%f\t%s\t%s\n", temp.bidID, temp.itemName, temp.itemQuantity, temp.startingBid, temp.bidEndDate, temp.merchantInformation);

}*/
