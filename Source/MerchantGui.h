/******************************************************************************/
struct _MerchantGui
{
	AI *merchant;

	Int cost;

	//Inventory inv;
	//Inventory inv2;
	Memb<Reference<Item>> refInv;
	Memb<Reference<Item>> refInv2;

	Window window;

	Region mRegion;
	Region pRegion;

	List<Item> mList;
	List<Item> pList;

	Text textCost;
	Text textGold;

	Button buttonBuy;
	Button buttonResign;

	void create();

	void link(AI *merchant);
	void unlink() { merchant->nowTrading = false; merchant = NULL; cost = 0; }

	void setGui();
	void update(Game::Chr &owner);
	void draw();
	void toggle();

    _MerchantGui() { merchant = NULL; }

} extern MerchantGui;
/******************************************************************************/
void pto_buySelectedItems(Ptr);
void pto_Resign(Ptr);
/******************************************************************************/