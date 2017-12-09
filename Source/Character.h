
struct HandSwing // this covers the "swing triangle" between previous and current frames
{
	Vec base_pos,
		extended_prev,
		extended_new;

	Vec center() {return Avg(base_pos, extended_prev, extended_new);} // triangle center
	Tri tri   () {return Tri(base_pos, extended_prev, extended_new);} // triangle

	void    update(C OrientP &hand, Flt weapon_range);
	void debugDraw();
};

struct Chr : Game::Chr
{
   Flt health, defense, strength;
   Flt mana;
   
   Bool isPlayer, attackBreak, isWanted;

   Inventory inv;

   Reference<Horse> horse;

   Int uniqueID;

   Flt helpVar;

   Flt timeToEndWanted, timeToGiveUp;

   Motion attackAnim;

   Memb<Reference<AI>> subordinates;

   virtual void create(Game::ObjParams &obj);

   virtual void dismount();
   virtual void mount(Horse &horse);
           void alignToHorse();

           Bool alive     () { return health > 0; }
   virtual UInt bloodColor() { return 0xFF400000; }

   virtual void die(Chr *src);
   virtual void addDamage(Flt damage,Chr *src);

   Bool inAttackRange(Chr &chr, Flt dist=0.6);	//Check player is in range
   Bool inRange(Game::Obj &obj, Flt dist=0.6);
   Bool updateAttacks();
   Bool update();

   void toggleTorchiere(Torchiere &torchiere);

   Flt attackRange();

   virtual void animateStepOccured();

   virtual void animate();
   virtual void animateUpdateMatrix(Matrix &body_matrix);

   Bool collisions(Bool collisions); //Set character actor collisions and return previous settings
   void addBodyVel(Vec &vel);

   virtual void moveTo(Vec &pos);
   virtual UInt drawPrepare();

   Byte aiType;
   Str name;

   HandSwing handSwing;

   Chr();
};