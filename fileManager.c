#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "fileManager.h"
#include "main.h"


//Tyler Porter
//Project
//CS 4323

//TODO:
//make a create() function to take input from user and write an item to file A
//make methods that control data movement from file A to file B once an item has been sold

//??Make a method to populate an array of BidItem structs for easier file management

//~~~wut is dis?
//This is only necessary for testCreate.
//It auto increments test values to print to FILE_A
//int IDcheck;
//----------------
/*Starkey's change list
* Moved everything that needed to be in the .h file to that
* Created new method: basically same as readfa but takes item ID and returns the match
* Created new method: Also same as above but for readfb/sold items (uses soldID)
* Created new method: gets soldItems by buyerID
* Created new method: Gets total number of items in file A
* I made many other methods but stopped writing them down check filemanager.h for full list
*/

//Global Variables & Data

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
//gets the itemID number of the bid item
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

//gets the item ID by the name of the item
int getID_by_name(char itemName[16])
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
		if(strcmp(item.itemName, itemName) == 0){
			fclose(fp);
			return item.itemID;
		}
        i++;
    }

//really bad if it gets here will most likely give random answers ~~~~
    fclose(fp);

    return item.itemID;
}

//find an item in FILE A by itemID
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

//reads all the items in FILE_B and returns a specific item
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
//finds an item in file B by the buyerID
SoldItem Sget_item_by_buyerID(int buyerID){
//can only get one item at a time. DONT GIVE USER OPTION TO BUY ANYTHING BUT THE FIRST ITEM THAT COMES UP
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
    
    //CRITICAL SECTION WRITING TO FILE
    //sem wait will wait until filealock is available and grab it
    sem_wait(&filealock);
    FILE *fp;
    fp = fopen("FILE_A.bin", "a+");
    if (fp == NULL)
    {
        printf("Error opening file from writefa()");
    }

    fwrite(&item, sizeof(temp) + 1, 1, fp);


    //fprintf(fp, "%d\t%s\t%d\t%f\t%s\t%s\n", item.bidID, item.itemName, item.itemQuantity, item.startingBid, item.bidEndDate, item.merchantInformation);
    fclose(fp);
    
    //END CRITICAL SECTION
    //sem post releases the filealock
    sem_post(&filealock);
}
//Takes a SoldItem as a parameter and prints it to File B
void writefb(SoldItem item)
{
    
    //CRITICAL SECTION WRITING TO FILE
    //sem wait will wait until filealock is available and grab it
    sem_wait(&filealock);
    FILE *fp;
    fp = fopen("FILE_B.bin", "a+");
    if (fp == NULL)
    {
        printf("Error opening file from writefb()");
    }

    fwrite(&item, sizeof(item) + 1, 1, fp);


    fprintf(fp, "%d\t%s\t%f\t%d\t%s\n",  item.soldID, item.buyInformation, item.buyingPrice, item.center, item.purchaseDate);
    fclose(fp);
    
    //END CRITICAL SECTION
    //sem post releases the filealock
    sem_post(&fileblock);
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
		if(item->buyerID == userID){
			count++;
		}
        i++;
    }
    fclose(fp);

    return count;
}
//finds item with matching sellerID in FILEA returns itemID
int Bget_item_by_sellerID(int sellerID){
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
		if(item.sellerID == sellerID){
			fclose(fp);
			return item.sellerID;
		}
        i++;
    }

//really bad if it gets here will most likely give random answers ~~~~ 
    fclose(fp);
    return item.itemID;
}
//removes an item from FILE_B by its itemID
void Sremove_item(int itemID){
	SoldItem item[100];
	
//CRITICAL SECTION WRITING TO FILE
    //sem wait will wait until filealock is available and grab it
    sem_wait(&fileblock);
	
	FILE *fp;
	fp = fopen("FILE_B.bin", "ab+");
    FILE *fpreplace;
    fpreplace = fopen("FILE_B_Temporary.bin", "wb");
    int i = 0;
	//While reading through File A, If an ID matches the delete ID, dont copy it
    //Otherwise, copy file to file a temporary then delete File A and rename new file to file A
	while (!feof(fp))
    {
        fread(&item[i], sizeof(item[i]) + 1, 1, fp);

        if (itemID == item[i].itemID)
        {
            continue;
        }
        else
        {
            fwrite(&item[i], sizeof(item[i]) + 1, 1, fpreplace);
        }
        i++; 
    }
	//Delete old file
        if (remove("FILE_B.bin") == 0)
        {
            printf("Item deleted, and old FILE_B.bin deleted");
        }
        else
        {
                printf("Error deleting FILE_B.bin");

        }
        //Rename new file to match
        rename("FILE_B_Temporary.bin", "FILE_B.bin");
	
//END CRITICAL SECTION
    //sem post releases the filealock
    sem_post(&fileblock);
}
//removes an item by its itemID in FILE_A
void Bremove_item(int itemID){
	BidItem item[100];
	
//CRITICAL SECTION WRITING TO FILE
    //sem wait will wait until filealock is available and grab it
    sem_wait(&filealock);
	
	FILE *fp;
	fp = fopen("FILE_A.bin", "ab+");
    FILE *fpreplace;
    int i = 0;
    fpreplace = fopen("FILE_A_Temporary.bin", "wb");
	//While reading through File A, If an ID matches the delete ID, dont copy it
    //Otherwise, copy file to file a temporary then delete File A and rename new file to file A
	while (!feof(fp))
    {
        fread(&item[i], sizeof(item[i]) + 1, 1, fp);

        if (itemID == item[i].itemID)
        {
            continue;
        }
        else
        {
            fwrite(&item[i], sizeof(item[i]) + 1, 1, fpreplace);
        }
        i++; 
    }
	//Delete old file
        if (remove("FILE_A.bin") == 0)
        {
            printf("Item deleted, and old FILE_A.bin deleted");
        }
        else
        {
                printf("Error deleting FILE_A.bin");

        }
        //Rename new file to match
        rename("FILE_A_Temporary.bin", "FILE_A.bin");
	
	//END CRITICAL SECTION
    //sem post releases the filealock
    sem_post(&filealock);
}
/*
void testCreate()
{
    int IDcheck;
    //Creates, writes, then reads back a test BidItem
    BidItem test = {IDcheck, "TestName", 1, 1, "MM/DD/YYYY", "Merchant Information"};
    IDcheck++;

    writefa(test);
    //struct BidItem temp = read(IDcheck-1);
    //printf("Test: %d\t%s\t%d\t%f\t%s\t%s\n", temp.bidID, temp.itemName, temp.itemQuantity, temp.startingBid, temp.bidEndDate, temp.merchantInformation);

}*/
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
