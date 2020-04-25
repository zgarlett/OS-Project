

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


struct bidItem readf(int ID);
void writef(struct bidItem);
void testcase();
