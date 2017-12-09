/******************************************************************************/
extern State StateGame;
/******************************************************************************/
extern Game::ObjMemx<Game::Static>   Statics;
extern Game::ObjMemx<Tree>           Trees;
extern Game::ObjMemx<Item>         Items;
extern Game::ObjMemx<Player>         Players;
extern Game::ObjMemx<AI>             Chrs;
extern Game::ObjMemx<Fireball>       Fireballs;
extern Game::ObjMemx<Teleport1>      Teleports1;
extern Game::ObjMemx<Horse>          Horses;
extern Game::ObjMemx<DestructibleEC> Destructs;
extern Game::ObjMemx<Trigger>        Triggers;

/******************************************************************************/
struct ObjectiveMarker
{
	Decal decal;
	Vec4 color;
	bool isSet;
	bool _drawDecal;

	void Set(Vec4 color, Vec pos, bool drawDecal = false)
	{
		isSet = true;
		// set decal data
		T.color = color;
		_drawDecal = drawDecal;
		decal.color=color;
		decal.material(Materials.ptrRequire("Decal/star.mtrl"));
		decal.matrix.setPosDir(pos,Vec(0,1,0));
	}
	void Update()
	{
		decal.matrix.rotateZVec(Time.d());
	}
	void Draw()
	{
		if(!isSet)return;

		//if(Renderer()==RM_BLEND)decal.drawStatic();

		Vec2 screen;
		if(PosToScreen(decal.matrix.pos, screen))
		{
			int dist = Round(Vec(Players[0].pos() - decal.matrix.pos).length());
			D.dot(color, screen);
			D.text(screen+ Vec2(0, 0.05), S+ dist + " m");
		}
	}

	//in RM_BLEND mode
	void drawDecal()
	{
		if(_drawDecal)
			decal.drawStatic();
	}


}extern objectiveMarker;
/******************************************************************************/
void Render();

void getObjectUnderCursor();
void DetectTeleport();

void DrawCrosshair();
void DrawHud();

void setMouseVisibility();

Int countSigns(Str sign, Str string);
Bool split(Str delimeter, Str string, Bool removeSpaces, Memb<Str> &container);

void initMods();

AI* getChrByID(Flt id);
/******************************************************************************/

enum SPELL_TYPE
{
   SPELL_HEALTH,
   SPELL_ATTACK,
   SPELL_HEALTH_SELF,
   SPELL_USE_FORCE,
   SPELL_AVADA_KEDAVRA,
};
extern SPELL_TYPE Spell;
extern bool CameraMode;