STRUCT(Torchiere , Game::Static)
	//{
	Memx<Item> items;   virtual Memx<Game::Obj>* itemContainer() {Memx<Game::Obj> &items=T.items; return &items;}

// manage
virtual void create(Game::ObjParams &obj);

// operations
virtual void itemRemoved(Game::Obj &item);
virtual void itemAdded  (Game::Obj &item);

// update
virtual Bool update();

// draw
virtual UInt drawPrepare();
virtual void drawShadow ();
virtual void drawPalette();
virtual void drawBlend  ();

// io
virtual Bool load(File &f);
};