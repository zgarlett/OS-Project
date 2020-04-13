#include <stdio.h>
#include <stdlib.h>
//Tyler Porter
//Project
//CS 4323

//TODO:
//make a create() function to take input from user and write an item to file A
//make methods that control data movement from file A to file B once an item has been sold

//??Make a method to populate an array of bidItem structs for easier file management

//Global Variables
struct bidItem
{
    int bidID;
    char itemName[50];
    int itemQuantity;
    float startingBid;
    char bidEndDate[50];
    char merchantInformation[50];
};

struct soldItem
{
    int soldID;
    char buyInformation[50];
    float buyingPrice;
    int center;
    char purchaseDate[50];
};
//----------------

//Method Declarations
struct bidItem read(int ID);
void write(struct bidItem);
//-------------------

//File A will be tab delimited with info as such by line
//Item ID - Item Name - Item quantity - starting bid - bid end date - merchant info \n
//Given an ID, this method will return a struct of all info related to item
struct bidItem read(int ID)
{
    struct bidItem item;
    //str is a read in buffer
    char str[1000];
    FILE *fp;
    fopen("FILE_A", "r");
    if (fp == NULL)
    {
        printf("Error opening file from read()");
    }
    while (fgets(str , 1000, fp)!= NULL)
    {
        fscanf(fp, "%d\t%s\t%d\t%f\t%s\t%s\n", item.bidID, item.itemName, item.itemQuantity, item.startingBid, item.bidEndDate, item.merchantInformation);
        if (item.bidID == ID) return item;
    }



}
//Takes in, and prints a bidItem to FILE A
void write(struct bidItem item)
{
    FILE *fp;
    fopen("FILE_A", "w");
    if (fp == NULL)
    {
        printf("Error opening file from write()");
    }

    //Prints the bidItem struct to a standardized format in the text file
    fprintf(fp, "%d\t%s\t%d\t%f\t%s\t%s\n", item.bidID, item.itemName, item.itemQuantity, item.startingBid, item.bidEndDate, item.merchantInformation);
    fclose(fp);
}
int main()
{

}
