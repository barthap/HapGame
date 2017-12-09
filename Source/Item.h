/******************************************************************************/
struct Item : Game::Item //Extend items to include item Icons, and parameters
{
   Char   name[32];
   Byte   type;
   Byte   type2;

   Flt    power;
   Flt    defence;
   Flt    weight;
   Flt    cost;

   Bool   usable;

   Image  *icon;
   Particles particles;

   //Manage
   void create(Game::ObjParams &obj);
   void use();

   //Draw
   UInt drawPrepare();
   virtual void drawPalette ();
   virtual void drawPalette1();
   virtual void drawBlend   ();
   void drawIcon(Vec2 pos);
   void moved();
   Bool update();
   //IO
   void save(File &f);
   Bool load(File &f);

   Item();
	~Item(){if(loot_gui)loot_gui->unlink();}
	
	//DO LOOTA
	   Memx<Game::ObjParams> Loot; //container for sub_obj
   //zmiana
   //Zmienne do operacji na przedmiotach w skrzyniach, manipulacja przedmiotami
   /*********************************************************************************/
   Memx<Item> loot; //Container that have items added as sub_obj in World Edytor

   Memx<Game::Obj>* itemContainer(){Memx<Game::Obj> &items=loot; return &items;}
   
   Reference<Item> slots[SLOT_NUM]; // these are references to items which are placed at certain slots
   LOOTGUI   *loot_gui; // pointer to InventoryGui linked with Inventory

   Bool slotCanBePutTo   (Int src,Int dest); // test if slot 'src' can be put to 'dest' slot , 'src dest' are slot indexes (SLOTS)
   Bool slotsCanBeSwapped(Int a  ,Int b   ); // test if slot 'a' can be swapped with slot 'b', 'a b'      are slot indexes (SLOTS)

	void itemRemoved(Game::Item &item); // called when an item is being removed
   void itemRemoved(                ); // called when an item has been removed
	void itemAdded  (Game::Item &item); // called when an item has been added

   void setGui     (                ); // update visual gui components

   void l_update(Game::Chr &owner); // handle moving items with mouse and setting equipped item matrixes
};
/******************************************************************************/
