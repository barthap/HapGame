/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Player::Player()
{
   InvGui.link(&inv); //When creating a player automatically link him with InvGui

   L_loot = NULL;
   loot_gui = NULL;

   strength = 10;
   dexterity = 5;
   intelligence = 5;
   charisma = 5;

   gold = 0;

   lvl = 1;
   exp = 0;

   health = 100;
   mana = 100;

   wood = 0;
   stone = 0;
   iron = 0;

   isPlayer = true;
   isWanted = false;

	usedAvadaKedavra = false;
	timeToAvadaKedavraEnd = 0;

   timeToAction = 0;
   timeToGiveUp = 4;
}
void Player::create(Game::ObjParams &obj)
{
	__super::create(obj);
		avadaKedavraParticle.load(File("Particle/fireball/avadaKedavra.particle"));
}

void Player::itemDestroy(Item &item)
{
   if(inv.items.contains(&item)) // can only destroy items from own inventory
   {
      itemRemoved(item); // notify that 'item' is being removed
      inv.items.removeData(&item); // remove 'item'
      itemRemoved(); // notify that 'item' has been removed
   }
}
/******************************************************************************/
// UPDATE
/******************************************************************************/
void Player::updateSpells()
{
	if(Spell==SPELL_ATTACK)
	{
		if(mana >= 15)
      {
			animations.New().set(cskel, CastSpellNormal[Random(Elms(CastSpellNormal))]);
            Game::ObjParams op;
			op.type(true, "OBJ_FIREBALL");

			Game::World.objCreate(op, Matrix(cskel.getPoint("HandR").pos));
			Game::Obj* fireBallObj = &Fireballs[Fireballs.elms() - 1];

			if(Fireball *fb = CAST(Fireball, fireBallObj))
			{
				mana -= 20;
				fb->type = FB_FIRE_ATTACK;
            fb->vel = cskel.getPoint("head").dir;
            fb->caster = this;
            fb->power = RandomF(18, 22)/* * magic_power*/;

            Vec targetVec;

            Vec pos, dir;

				ScreenToPosDir(Vec2(0, 0), pos, dir);
				targetVec = pos + dir * D.viewRange();

            fb->vel = targetVec - fb->particles.matrix.pos;
            fb->vel.setLength(20);
			}
		}
	}
	if(Spell==SPELL_HEALTH)
	{
		 if(mana >= 30)
         {
			animations.New().set(cskel, CastSpellNormal[Random(Elms(CastSpellNormal))]);
            Game::ObjParams op;
			op.type(true, "OBJ_FIREBALL");

			//Game::Obj* fireBallObj = Game::World.objCreateNear(op, Matrix(hand_r.pos));
			Game::World.objCreate(op, Matrix(cskel.getPoint("HandR").pos));
			Game::Obj* fireBallObj = &Fireballs[Fireballs.elms() - 1];

            if(Fireball *fb = CAST(Fireball, fireBallObj))
            {
               mana -= 30;
					fb->type = FB_FIRE_HEALTH;
					fb->particles.color = Color(0,0,190,255);
               fb->vel = cskel.getPoint("head").dir;
               fb->caster = this;
               fb->power = RandomF(18, 22)/* * magic_power*/;

               Vec targetVec;

               /*if(Cur.valid)
			   {
				   targetVec = Cur.pos;
			   }
			   else
               {*/
                     Vec pos, dir;

					 ScreenToPosDir(Vec2(0, 0), pos, dir);
					 targetVec = pos + dir * D.viewRange();
               /*}*/

               fb->vel = targetVec - fb->particles.matrix.pos;
               fb->vel.setLength(20);
          }
	   }
	}
	if(Spell==SPELL_USE_FORCE)
	{
		 if(mana >= 15)
         {
			animations.New().set(cskel, CastSpellNormal[Random(Elms(CastSpellNormal))]);
            Game::ObjParams op;
			op.type(true, "OBJ_FIREBALL");

			//Game::Obj* fireBallObj = Game::World.objCreateNear(op, Matrix(hand_r.pos));
			Game::World.objCreate(op, Matrix(cskel.getPoint("HandR").pos));
			Game::Obj* fireBallObj = &Fireballs[Fireballs.elms() - 1];

            if(Fireball *fb = CAST(Fireball, fireBallObj))
            {
               mana -= 15;
			   fb->type = FB_F_USE_FORCE;
               fb->vel = cskel.getPoint("head").dir;
               fb->caster = this;
               fb->power = RandomF(18, 22)/* * magic_power*/;

               Vec targetVec;

               /*if(Cur.valid)
			   {
				   targetVec = Cur.pos;
			   }
			   else
               {*/
                     Vec pos, dir;

					 ScreenToPosDir(Vec2(0, 0), pos, dir);
					 targetVec = pos + dir * D.viewRange();
               /*}*/

               fb->vel = targetVec - fb->particles.matrix.pos;
               fb->vel.setLength(20);
          }
	   }
	}
	if(Spell==SPELL_HEALTH_SELF)
	{
		if(mana >= 20)
		{
			animations.New().set(cskel, CastSpellSelf[Random(Elms(CastSpellSelf))]);

			mana-=25;
			health+=RandomF(30,35);
		}
	}
	if(Spell==SPELL_AVADA_KEDAVRA)
	{
		if(Lit && mana >=95) //If hase highlighted object and mouse button pressed
		{
			if(Chr *chr = CAST(Chr, Lit))
			{
				mana-=95;
				avadaKedavraParticle.source(&chr->cskel, true);
				avadaKedavraParticle.reborn=true;
				avadaKedavraParticle.matrix.pos = chr->pos();
				usedAvadaKedavra = true;
				timeToAvadaKedavraEnd = 3;
				//This isn't nice version, but it enable good ragdoll
				chr->addDamage(chr->health+1, this);
			}
		}
	}
}
void Player::Attack()
{

	Reference<Item> &weapon=inv.slot[SLOT_ARM_R];

	   if(!attackAnim.is())
		{
			attackBreak = false;

			if(weapon.valid())
			{
				attackAnim.set(cskel, AttackRightHand[Random(Elms(AttackRightHand))]);
				
			}//if weapon.valid
			else
			{
				attackAnim.set(cskel,"anim/swing/r-d.anim");
			}//weapon.valid else
		}
}//attack
/******************************************************************************/
void Player::animate()
{
   __super::animate();

   cskel.animate(giveUp, true);
  // cskel.animate(spellAnim, true);
	REPA(animations)cskel.animate(animations[i],true);
}
Bool Player::update()
{
	if(Kb.bp(KB_R))giveUp.set(cskel, "anim/give-up.anim");

   //Level and experience system
   if(exp >= GetExp(lvl + 1))
	{
		lvl++;
		AddMessage(S+"Awansowa³eœ na poziom " + (Int)lvl + "!");

		strength = lvl+10;
		defense = lvl+5;
	}

	if(timeToAvadaKedavraEnd <= 0 && usedAvadaKedavra)
	{
		timeToAvadaKedavraEnd = 0;
		usedAvadaKedavra=false;
		avadaKedavraParticle.reborn=false;
	}
	else if(timeToAvadaKedavraEnd > 0)
		timeToAvadaKedavraEnd-=Time.d();

	avadaKedavraParticle.update();

   if(alive())
   {

	   if(Kb.bp(KB_F))
	   {
		   if(horse.valid())dismount();else
		   {
			  if(Lit)
				   if(Horse *horse=CAST(Horse,Lit))
					   if(inRange(*horse, 2))
						   mount(*horse);
		   }
	   }



	   if(horse.valid())
       {
         //Mouse turn
         if(!Ms.visible())
         {
            Flt max = DegToRad(900) * Time.d(),
                dy = Ms.d().y * 1.7;// * Ms.inv();
            angle.y += Mid(dy, -max, max);
         }
       }
	   else
	   {

		   if(action && !CameraMode)
		   {
			  if(Kb.b(KB_W) || Kb.b(KB_S) || Kb.b(KB_A) || Kb.b(KB_D) || Kb.b(KB_Q) || Kb.b(KB_E)) actionBreak();
		   }

			//spellAnim.updateAuto(3, 3, 1);
			REPA(animations)
			{
				Motion &motion = animations[i];
				if(!motion.updateAuto(3, 3, 1)) animations.remove(i); //If finished playing then remove
			}
			giveUp.updateAuto(3,3, 0.8);

			if(timeToGiveUp <= 3)
				timeToGiveUp -= Time.d();


		   if(!action && !Dialogue.now)
		   {
				if(!CameraMode)
				{
					// turn & move
					if(!Ms.visible())
					{
						input.turn.x=Kb.b(KB_Q)-Kb.b(KB_E);
						input.turn.y=Kb.b(KB_T)-Kb.b(KB_G);
						
					}
					else
					{
						input.turn.x=(Ps3.Axis(AXIS_X_MINUS, false, 0.3f)-Ps3.Axis(AXIS_X_PLUS, false, 0.3f));
						input.turn.y=(Ps3.Axis(AXIS_Y_PLUS, false, 0.5f)-Ps3.Axis(AXIS_Y_MINUS, false, 0.5f));
					}
					

					input.move.x=Kb.b(KB_D)-Kb.b(KB_A);
					input.move.z=Kb.b(KB_W)-Kb.b(KB_S);
					//input.move.x=Ps3.Axis(AXIS_X_PLUS)-Ps3.Axis(AXIS_X_MINUS);
					//input.move.z=Ps3.Axis(AXIS_Y_PLUS)-Ps3.Axis(AXIS_Y_MINUS);
					input.move.y=Kb.b(KB_SPACE)-Kb.b(KB_LSHIFT);
			
					// dodge, crouch, walk, jump
					input.dodge = Kb.bd(KB_D)-Kb.bd(KB_A);
					input.crouch= Kb.b (KB_LSHIFT);
					//input.walk  = Kb.b (KB_LCTRL );
					input.walk = Ps3.Btn(BTN_GREEN_TRIANGLE);
					input.jump  =(Kb.bp(KB_SPACE ) ? 3.5f : 0);

				  //Flying
				  if(Ps3.PointOfViewButtonPressed(POV_TOP, false) || Kb.bp(KB_N)) ctrl.flying(!ctrl.flying());
				  turn_speed = ((ctrl.flying() && !input.walk && input.move.z == 1) ? 1 : 3.5);

					}
				  // mouse turn
				  if(!Ms.visible())
				  {
					  if(!CameraMode)
					  {
						  Flt max = DegToRad(900) * Time.d(),
							  dx = Ms.d().x * 1.7,
							  dy = Ms.d().y * 1.7 ;//* Ms.inv();
						  angle.x -= Mid(dx, -max, max);
						  angle.y += Mid(dy, -max, max);
					  }

					  if(Forces.tObject.valid())
					  {
						  Vec vector = Vec(Cos(angle.y) * Forces.distanceToObj + pos().x, Forces.tObject().pos().y, Sin(angle.y) * Forces.distanceToObj + pos().z);
						  Vec vector2 = Forces.tObject().pos();
						  Forces.tObject().pos(vector);
					  }
				}
			  // ready stance change
			  //ready ^= Kb.bp(KB_R); //Ready jest usuniête z silnika
		   }
	   }
   }

   if(__super::update())
	{
		if(Dialogue.now && Dialogue.scriptPointer != NULL)
		{
			//Rotate face to player
			actionBreak();

			Flt angle_delta = AngleDelta(angle.x + PI_2, Angle(Dialogue.scriptPointer->pos().xz() - pos().xz()));
			Flt	max_frame_delta = Time.d() * DegToRad(270);

			Clamp(angle_delta, -max_frame_delta, max_frame_delta);
			angle.x += angle_delta;

			return true;
		}

		if(alive())
		{
			timeToAction -= Time.d();

			//Detect if the player wants to pickup an item
			if(Lit && (Ps3.Btn(2, false) || Kb.bp(KB_E)) && Ms.hidden()) //If hase highlighted object and mouse button pressed
		  {
				if(Item *item = CAST(Item, Lit)) //If the object is an item
				{
					if(inRange(*item, 1))
					{

						if(item->type == ITEM_CONTAINER)
						{
							link(item);
							LootGui.link(L_loot);
							L_loot->setGui();
							LootGui.nazwa = item->name;
							LootGui.L_window.text = item->name;
							LootGui.toggle();
							InvGui.window.visibleToggle();
						}
						else if(item->type == ITEM_PLANK)
						{
							wood += item->weight * 10;
							Items.removeData(item);
						}
						else
						{
							if(inv.itemCanBePickUp(*item))
							{
								AddMessage(S + "Podniesiono " + item->name);
								itemPickUp(*item); //Pick it up
							}
							else
							{
								AddMessage("Nie mo¿esz podnieœæ wiecej przedmiotów");
							}
						}
					}
					else // inRange
						AddMessage("Nie mo¿esz dosiêgn¹æ przedmiotu, jesteœ zbyt daleko!");
				}
				else if(Door *door = CAST(Door, Lit))
				{
					if(Contains(S+door->doorType, "prison"))
					{
						bool result = false;
						FREP(inv.items.elms())
						{
							if(Contains(inv.items[i].name, "Prison key"))
							{
								result = true;
							}
						}

						if(result)door->toggle();
						else AddMessage("Nie masz klucza");
					}
					else
						door->toggle();
				}
				else if(Torchiere *torchiere=CAST(Torchiere,Lit)) // toggle torchiere
				{
					if(inRange(*torchiere))toggleTorchiere(*torchiere);
				}
				else if(AI *obj = CAST(AI, Lit))
				{
					if(inAttackRange(*obj, 1))
					{
						if(obj->alive())
						{
							if(obj->dialoguePath.is() && (obj->target.valid() == false))
							{
								Dialogue.scriptPointer = obj;
								obj->isTalking = true;
								Dialogue.init((Char *)obj->dialoguePath());
							}
							else if(obj->aiType == AI_MERCHANT_1)
							{
								MerchantGui.link(obj);
								MerchantGui.setGui();
								MerchantGui.window.visible(true);
							}
						}
						else
						{
							Storage.link(obj);
							Storage.setGui();
							Storage.nazwa = obj->name;
							Storage.L_window.visible(true);
							InvGui.window.visible(true);
						}
					}
				}
			}

		  //Detect if player wants to attack or interact with some objects nad can do it
		  if((Ps3.LR2(false) || Ms.b(0)) && timeToAction <= 0 && Ms.hidden())
		  {
			 //Detect type of object which player want to interact
			 if(Lit)
			 {
				 if(Tree *interactObject = CAST(Tree, Lit))/* if(Ms.b(0))*/ //If the object is OBJ_TREE
				 {
					 if(inRange(*interactObject, 0.6))
					 {
						  //Player want to get some wood
						 //Check that he has right tool
						 if(inv.slot[SLOT_ARM_R].valid())
						 {
							 if(inv.slot[SLOT_ARM_R]().type == ITEM_WEAPON && inv.slot[SLOT_ARM_R]().type2 == WEAPON_AXE)
							 {
								 Int woodToAdd;

								 woodToAdd = (Int)RandomF(inv.slot[SLOT_ARM_R]().power - 3, inv.slot[SLOT_ARM_R]().power + 3);

								 if(inv.canAddWeight(woodToAdd * 0.1))
								 {
									wood += woodToAdd;
									inv.addWeight(woodToAdd * 0.1);
									AddMessage(S + "Drewno +" + woodToAdd);
								 }
								 else
								 {
									 AddMessage(S + "Nie masz wiêcej miejsca");
								 }
							  }//end if(inv.slot[SLOT_ARM_R]().type == ...
							 else
							 {
								 AddMessage(S + "Musisz mieæ za³o¿on¹ siekierê aby móc r¹baæ drewno");
							 }
						 } //end if(inv.slot[SLOT_ARM_R].valid())
						 else
						 {
							 AddMessage(S + "Musisz mieæ za³o¿on¹ siekierê aby móc r¹baæ drewno");
						 }

						 timeToAction = 0.5;
					}//inRange
				 }//end if(Tree * interactObject = CAST...
				 else Attack();
			 }//Lit
			 else Attack();
		  }//end if(Ms.b(0) && timeToAction <= 0) 

		}//end if(alive())

		//Set playing animations
		Flt blend = Lerp(1.0f, 0.8f, anim.stop_move * anim.walk_run);

		if(Kb.bp(KB_C)) updateSpells();

		inv.update(T);     //Update the inventory
		if(L_loot)
			L_loot->l_update(T);
		if(loot_gui)
			loot_gui->update(T);

		Storage.update(T);
		MerchantGui.update(T);

		Flt tempDef = 0;
		FREPA(inv.slot)
		{
			if(inv.slot[i].valid())
				tempDef += inv.slot[i]().defence;
		}
		T.defense = (tempDef / 3);

		StatsGui.update(); //Update stats

		if(Kb.bp(KB_TAB) && inv.inv_gui && !inv.slot[SLOT_TEMP].valid())
		{
			inv.setGui();
			inv.inv_gui->window.visibleToggle();


			if(LootGui.L_window.visible())
				LootGui.toggle();
			if(Storage.L_window.visible())
				Storage.L_window.visible(false);

			setMouseVisibility();
		}
		
		if(alive()) ctrl_pos = ctrl.actor.pos();
		else ctrl_pos = cskel.pos();

		if(mana < 0) mana = 0;
		if(health < 0) health = 0;

		if(alive()) mana += Time.d();
		MIN(mana, 100);
		if(alive()) health += Time.d() / 3;
		MIN(health, 100);

		return true;
	}

	return false;
}
/******************************************************************************/
// DRAW
/******************************************************************************/
UInt Player::drawPrepare()
{
   inv.drawPrepare(T);
	avadaKedavraParticle.draw();

   return __super::drawPrepare();
}
void Player::drawShadow()
{
   inv.drawShadow(T);

   return __super::drawShadow();
}
/******************************************************************************/
// IO
/******************************************************************************/
void Player::save(File &f)
{
   __super::save(f);

   inv.save(f);

	f.putFlt(health);
	f.putInt(gold);
	f.putFlt(mana);
   //f << health << gold << mana;
	f.putInt(strength);
	f.putInt(dexterity);
	f.putInt(intelligence);
	f.putInt(charisma);
   //f << strenght << dexterity << intelligence << charisma; //Stats
	f.putInt(wood);
	f.putInt(stone);
	f.putInt(iron);
   //f << wood << stone << iron; //Resources
	f.putFlt(exp);
	f.putInt(lvl);
   //f << exp << lvl; //experience, level
	f.putFlt(defense);
}
Bool Player::load(File &f)
{
   if(__super::load(f))
   {
      inv.load(f);

		health = f.getFlt();
		gold = f.getInt();
		mana = f.getFlt();

		strength = f.getInt();
		dexterity = f.getInt();
		intelligence = f.getInt();
		charisma = f.getInt();

		wood = f.getInt();
		stone = f.getInt();
		iron = f.getInt();

		exp = f.getFlt();
		lvl = f.getInt();
		defense = f.getFlt();

	  return true;
   }

   return false;
}
/******************************************************************************/
void Player::link(Item *item)
{
	L_loot = item;
}
/*void Player::link2(AI *Chr_loot)
{
	T.Chr_loot = Chr_loot;
	//loot_gui = &LootGui;
}*/