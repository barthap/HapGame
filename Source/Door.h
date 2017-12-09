
struct Door : Game::Door
{

	Str doorType;

	virtual void create(Game::ObjParams &obj);
	virtual UInt drawPrepare();
};