#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Tyler Porter
//Project
//CS 4323

//TODO:
//make a create() function to take input from user and write an item to file A
//make methods that control data movement from file A to file B once an item has been sold

//??Make a method to populate an array of bidItem structs for easier file management

//Global Variables
typedef struct bidItem
{
    int bidID;
    char itemName[50];
    int itemQuantity;
    float startingBid;
    char bidEndDate[50];
    char merchantInformation[50];
};

typedef struct soldItem
{
    int soldID;
    char buyInformation[50];
    float buyingPrice;
    int center;
    char purchaseDate[50];
};


int IDcheck;
//----------------

//Method Declarations
struct bidItem readf(int ID);
void writef(struct bidItem);
void testCreate();
//-------------------

//File A will be tab delimited with info as such by line
//Item ID - Item Name - Item quantity - starting bid - bid end date - merchant info \n
//Given an ID, this method will return a struct of all info related to item
struct bidItem readf(int ID)
{
    int found = 0;
    struct bidItem item;

    FILE *fp;
    fp = fopen("FILE_A.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file from read()\n");
    }


    fseek(fp, sizeof(struct bidItem)*ID, SEEK_SET);
    fread(&item, sizeof(struct bidItem), 1, fp);

    return item;
    /*while(1)
    {
        fread(&item, sizeof(struct bidItem), 1, fp);
        printf("Browsing %d to %d\n", item.bidID, ID);

        if (item.bidID == ID)
        {
            //Print test
            //printf("Found item %d\n", item.bidID);
            return item;
            found = 1;
            break;
        }
        if(item.bidID > ID) break;
    }
    fclose(fp);

    if(found == 0)
    {
        //If item not found
        printf("Item: %d not found.\n", ID);
    }*/




}
//Takes in, and prints a bidItem to FILE A
void writef(struct bidItem item)
{
    FILE *fp;
    fp = fopen("FILE_A.txt", "a+");
    if (fp == NULL)
    {
        printf("Error opening file from write()");
    }

    fwrite(&item, sizeof(struct bidItem), 1, fp);
    printf("Successfully wrote item %d\n", item.bidID);
    //Prints the bidItem struct to a standardized format in the text file
    //printf("Input: %d\t%s\t%d\t%f\t%s\t%s\n", item.bidID, item.itemName, item.itemQuantity, item.startingBid, item.bidEndDate, item.merchantInformation);
    fclose(fp);
}
void testCreate()
{

    //Creates, writes, then reads back a test bidItem
    struct bidItem test = { IDcheck, "TestName", 1, 1, "MM/DD/YYYY", "Merchant Information"};
    printf("%d\n", IDcheck);
    IDcheck++;






    writef(test);
    //struct bidItem temp = read(IDcheck-1);
    //printf("Test: %d\t%s\t%d\t%f\t%s\t%s\n", temp.bidID, temp.itemName, temp.itemQuantity, temp.startingBid, temp.bidEndDate, temp.merchantInformation);

}
int mainfileManager()
{
    FILE *fp;
    fp = fopen("FILE_A.txt", "w");
    fclose(fp);
    IDcheck = 1;
    for (int i = 0; i < 30; i++)
    {
        testCreate();
    }

}
