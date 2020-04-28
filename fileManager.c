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
struct bidItem readfa(int ID);
void writefa(struct bidItem);
void testCreate();
//-------------------

//File A will be tab delimited with info as such by line
//Item ID - Item Name - Item quantity - starting bid - bid end date - merchant info \n
//Given an ID, this method will return a struct of all info related to item
struct bidItem readfa(int ID)
{
    struct bidItem item[100];

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

struct soldItem readfb(int ID)
{

    struct soldItem item[100];

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
//Takes in, and prints a bidItem to FILE A
void writefa(struct bidItem item)
{
    struct bidItem temp = item;
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
//Takes a soldItem as a parameter and prints it to File B
void writefb(struct soldItem item)
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
void testCreate()
{

    //Creates, writes, then reads back a test bidItem
    struct bidItem test = {IDcheck, "TestName", 1, 1, "MM/DD/YYYY", "Merchant Information"};
    IDcheck++;

    writefa(test);
    //struct bidItem temp = read(IDcheck-1);
    //printf("Test: %d\t%s\t%d\t%f\t%s\t%s\n", temp.bidID, temp.itemName, temp.itemQuantity, temp.startingBid, temp.bidEndDate, temp.merchantInformation);

}

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
    struct bidItem temp;
    temp = readfa(20);
    printf("Test: %d\t%s\t%d\t%f\t%s\t%s\n", temp.bidID, temp.itemName, temp.itemQuantity, temp.startingBid, temp.bidEndDate, temp.merchantInformation);

}
