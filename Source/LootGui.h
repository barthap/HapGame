struct LOOTGUI
{
	Item *L_loot;
	//AI *Chr_loot;

	Str nazwa;
	Window L_window;
	Region L_region;
	//Item *L_item;
	List<Item> L_list;

	void create();

	void link(Item *L_loot);
	inline void unlink() { link(NULL); }

	//void link2(AI *Chr_loot);
	//inline void unlink2() { link2(NULL); }

	void setGui();
	void update(Game::Chr &owner); //Handle moving items with mouse
	void draw();
	void toggle(); //Toggle visibility

    LOOTGUI() { L_loot = NULL; /*Chr_loot = NULL;*/ }

} extern LootGui;