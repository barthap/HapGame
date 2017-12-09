/******************************************************************************/
enum SLOTS
{
   SLOT_TEMP,     //Temporary slot (current item moved with mouse cursor)
   SLOT_HEAD,     //Head slot
   SLOT_NECK,     //Neck slot
   SLOT_ARM_L,    //Left arm slot
   SLOT_ARM_R,    //Right arm slot
   SLOT_BODY,     //Body slot
   SLOT_HAND_L,   //Left hand slot
   SLOT_HAND_R,   //Right hand slot
   SLOT_BOOT,     //Boot slot
   SLOT_PANTS,    //Pants slot
   SLOT_GAUNTLET, //Gauntlet slot
   SLOT_NUM,      //Number of slots
};
/******************************************************************************/
struct Inventory
{
   Memx<Item> items;               //All character items are stored in this container
   Reference<Item> slot[SLOT_NUM]; //These are references to items which are placed at certain slots
   InventoryGui *inv_gui;            //Pointer to InventoryGui linked with Inventory
   Flt inventoryWeight;

   //Get
   Bool slotCanBePutTo(Int src, Int dest); //Test if slot 'src' can be put to 'dest' slot , 'src dest' are slot indexes (SLOTS)
   Bool slotsCanBeSwapped(Int a, Int b);   //Test if slot 'a' can be swapped with slot 'b', 'a b'      are slot indexes (SLOTS)
   Bool itemCanBePickUp(Item &item);

   //Operations
   void itemRemoved(Game::Item &item);         //Called when an item is being removed
   void itemRemoved();                         //Called when an item has been removed
   void itemAdded(Game::Item &item);           //Called when an item has been added
   void setGui();                              //Update visual gui components
   void setWeight(Game::Item &item, bool add); //Update weight
   void addWeight(Flt weight);                 //Add weight
   void subtractWeight(Flt weight);            //Subtract weight
   Bool canAddWeight(Flt weight);              //Check if weight can be add

   //Update
   void update(Game::Chr &owner); //Handle moving items with mouse and setting equipped item matrixes

   //Draw
   void drawPrepare(Game::Chr &owner); //Draw equipped items in 3d world
   void drawShadow(Game::Chr &owner);  //Draw equipped items in 3d world
   void drawBlend();
   void drawPalette();

   //IO
   void save(File &f);
   Bool load(File &f);

  ~Inventory();
   Inventory() { inventoryWeight = 0; inv_gui = NULL; }
};
/******************************************************************************/
