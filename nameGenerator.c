#include <stdio.h>
#include <time.h>
//NameGenerator by Tyler Porter

//Global Variables-----
    //File Pointer
    FILE *filepointer;
    //Array to hold names
    //4945 names in names.txt of length < 40 each
    char firstNameArray[4945][40];
    char lastNameArray[20000][40];

//---------------------

//Declaring methods
    void intake();
    void generate();
//-----------------
//numberOfNames tell how many names to generate
void generate(int numberOfNames)
{
    //Uses time as a seed to randomize names each run
    srand(time(0));

    //iterates numberOfNames times and prints to console
    //TODO: make names populate sql table
    for (int i = 0; i < numberOfNames; i++)
    {
        printf("%s, %s\n", firstNameArray[rand()%4945], lastNameArray[rand()%20000]);
    }
}

//intakes both first and last names
void intake()
{
    FILE *filepointer;
    filepointer = fopen("firstnames.txt", "r");
    if (!filepointer)
    {
        printf("Error opening first name file");
    }

    int i = 0;
    while(fgets(firstNameArray[i], 40, filepointer)!=NULL)
    {
        //replaces newline character at the end of names with null character
        firstNameArray[i][strlen(firstNameArray[i]) - 1] = '\0';
        i++;
    }
    fclose(filepointer);

    filepointer = fopen("lastnames.txt", "r");
    if (!filepointer)
    {
        printf("Error opening last name file");
    }

    i = 0;
    while(fgets(lastNameArray[i], 40, filepointer)!=NULL)
    {
        //replaces newline character at the end of names with null character
        lastNameArray[i][strlen(lastNameArray[i]) - 1] = '\0';
        i++;
    }
    fclose(filepointer);
}
void main()
{
    intake();

    //generate x names
    generate(10);

}
