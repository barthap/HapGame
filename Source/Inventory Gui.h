/******************************************************************************/
#define PIXEL_SIZE 0.002 
/******************************************************************************/
struct InventoryGui
{
   Inventory *inv;                 //Inventory which InventoryGui is linked with

   Window window;                  //Inventory window
   Region region;                  //Region in which list of items is displayed
   List<Item> list;              //List of items
   GuiImage slot_img[SLOT_NUM][2]; //Slot images [slot index(SLOTS)] [0 = background, 1 = item]

   GuiImage goldImage;
   Text     goldText;
   GuiImage weightImage;
   Text     weightText;

   //Manage
   void create();

   //Operations
   void link(Inventory *inv);    //Link with 'inv' Inventory
   void unlink() { link(NULL); }

   void setGui(); //Set visual gui components
   void toggle(); //Toggle window

   //Update
   void update(Game::Chr &owner); //Handle moving items with mouse

   //Draw
   void draw();

   InventoryGui() { inv = NULL; }
} extern InvGui;
/******************************************************************************/
struct MENUGUI2
{
	Window   window;
	Text   title; // gui text
	Button start, // gui buttons
		load,
		wyjscie,
		opcje;

	void create(); // create gui controls
	void toggle(); // toggle visibility of options
	void update(); // set fps text counter
}extern
	PauseGui;