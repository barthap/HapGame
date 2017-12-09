/******************************************************************************/
struct _Storage
{
	AI *Chr_loot;

	Str nazwa;
	Window L_window;
	Region L_region;
	List<Item> L_list;

	void create();

	void link(AI *Chr_loot);
	void unlink() { Chr_loot = NULL; }

	void setGui();
	void update(Game::Chr &owner);
	void draw();
	void toggle();

    _Storage() { Chr_loot = NULL; }

} extern Storage;
/******************************************************************************/