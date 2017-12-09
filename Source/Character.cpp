/******************************************************************************/
#include "stdafx.h"
#include "Main.h"


void HandSwing::update(C OrientP &hand, Flt weapon_range)
{
	base_pos     =hand.pos;
	extended_prev=extended_new;

	extended_new=hand.pos+hand.dir*weapon_range;
}
void HandSwing::debugDraw()
{
	tri().draw(WHITE);
}


/******************************************************************************/
Chr::Chr()
{
   health = 100;
   mana = 100;

   defense = 0;
   isWanted = false;
   timeToEndWanted = 10;
}
/******************************************************************************/
void Chr::create(Game::ObjParams &obj)
{
	__super::create(obj);

	if(Param *p = obj.findParam("uniqueID")) uniqueID = p->asInt();
}
/******************************************************************************/
bool Chr::collisions(Bool collisions)
{
   bool old = ctrl.actor.collision();

   ctrl.actor.collision(collisions);

   return old;
}
/******************************************************************************/
// OPERATIONS
/******************************************************************************/
void Chr::addBodyVel(Vec &vel)
{
   // assumes that ragdoll blending is already enabled

   if(ragdoll.bones())
	{
                                                           ragdoll.bone(0).actor.addVel(vel);
                  if(Ragdoll::Bone *bone=ragdoll.findBone("BodyU"))bone  ->actor.addVel(vel);
      if(!alive())if(Ragdoll::Bone *bone=ragdoll.findBone("Head" ))bone  ->actor.addVel(vel);
   }
}

/******************************************************************************/
// OPERATIONS
/******************************************************************************/
void Chr::die(Chr *src)
{
    FREPA(Chrs)
	{
		if(Chrs[i].target.valid())
		{
			if(Chrs[i].target == this) Chrs[i].target.clear();
		}
	}

    ragdollEnable();

	//REPA(inv.items)
	//	itemDropDown(inv.items[i]);
    if(inv.slot[SLOT_ARM_L].valid()) itemDropDown(inv.slot[SLOT_ARM_L]());
	if(inv.slot[SLOT_ARM_R].valid()) itemDropDown(inv.slot[SLOT_ARM_R]());
}
/******************************************************************************/
void Chr::addDamage(Flt damage, Chr *src)
{
	damage = ( damage - RandomF( RandomF(0,1), T.defense) );

	if(defense > damage)damage = 0;

   if(alive())
   {
         health -= damage;
      if(health <= 0)
      {
         die(src);
		// Flt random_scale =RandomF(1.0f/1.2f,1.2f);
		// Matrix3 attack_orn   =Orient(!(src->pos()-pos()), Vec(0,1,0));
		// addBodyVel((Vec(0,0,-1)*attack_orn*random_scale*2.5f)*3);
      }
	  /*else
      {
         if(src) attack(*src);
      }*/
   }
}
/******************************************************************************/
void Chr::dismount()
{
   if(!horse.empty())
   {
      if(horse.valid())
      {
         Vec offset = cskel.matrix().x;

		 offset.setLength(1.5);
         pos(pos() + offset);
         horse().rider.clear();
      }

      horse.clear();

      ctrl.actor.collision(true).kinematic(false);
   }
}
void Chr::mount(Horse &horse)
{
   if(T.horse != horse)
   {
      dismount();

      if(!horse.rider.valid())
      {
         horse.rider = T;
         T.horse = horse;

         ctrl.actor.collision(false).kinematic(true);
         pos(horse.pos());
         alignToHorse();

         actionBreak();
      }
   }
}
void Chr::alignToHorse()
{
   //Move player to horse position
   Vec pos = horse().ctrl.actor.pos();

   pos.y += horse().cskel.skeleton()->getPoint("Saddle").pos.y * horse().cskel.scale() - cskel.skeleton()->getPoint("Saddle").pos.y * cskel.scale();
   ctrl.actor.pos(pos);

   //Set angle
   angle.x = horse().angle + horse().angle_offset;
}

Flt Chr::attackRange()
{
	Flt range=0.5f; // set initial 0.5 meter range for no weapon

	Reference<Item> &weapon=inv.slot[SLOT_ARM_R];
	if(weapon.valid() && weapon().mesh) // if we have a weapon
	{
		MAX(range, weapon().mesh->box.max.y * weapon().scale); // weapons are oriented upwards in Mesh Editor so we can assume that box.max.y is the weapon length
	}

	return range*1.5;
}
/******************************************************************************/
// ANIMATIONS
/******************************************************************************/
void Chr::animate()
{
   if(horse.valid())
   {
      cskel.clear().animate(L"anim/ride.anim", anim.time);
   }
   else
   {
      __super::animate();
	  cskel.animate(attackAnim, false);
   }
}
void Chr::animateUpdateMatrix(Matrix &body_matrix)
{
   __super::animateUpdateMatrix(body_matrix);
   
   if(horse.valid())
   {
      Vec saddle_dest = horse().cskel.getPoint("Saddle").pos;
      Vec saddle_src = cskel.getPoint("Saddle").pos; 

      cskel.offset(saddle_dest - saddle_src);
   }
}
/******************************************************************************/
Bool Chr::inAttackRange(Chr &chr, Flt dist)
{
	return Dist(pos(),chr.pos()) <= ctrl.radius()+chr.ctrl.radius()+dist;
}
Bool Chr::inRange(Game::Obj &obj, Flt dist)
{
	return Dist(pos(),obj.pos()) <= ctrl.radius()+dist;
}

void Chr::animateStepOccured()
{
	SoundPlay(S+"sound/step/"+Random(1, 4)+".wav", T.pos(), 1.5);
}

Bool Chr::updateAttacks()
{
	if(attackBreak)return false;

	if(attackAnim.is())
	{

		if(attackAnim.eventOccured("sound") && !horse.valid())SoundPlay(S+"sound/swing/"+Random(0, 2)+".wav", T.pos(), 1.5);

		if(attackAnim.eventBetween("hit-from","hit-to") || attackAnim.eventOccured("hit"))
		{


			HandSwing       &hand  =handSwing;
			Vec              center=hand.center();
			Reference<Item> &weapon=inv.slot[SLOT_ARM_R];
			Memc<Obj*>       objects; Game::World.objGet(objects,Ball(3,center));

			// find nearest character
			Flt dist; Chr *nearest=NULL;
			REPA(objects)
				if(Chr *chr=CAST(Chr,objects[i]))
					if(chr!=this && chr->alive())
					{
						Flt d=Dist(center,chr->pos()); if(!nearest || d<dist){nearest=chr; dist=d;}
					}

					// check if the swing triangle interacts with character's capsule
					if(nearest)
			{
				Shape chr_shape=nearest->ctrl.actor.shape(false); DYNAMIC_ASSERT(chr_shape.type==SHAPE_CAPSULE, "Controller Shape is not a Capsule");
				Tri   triangles[3]; hand.tri().circularLerp(triangles,Elms(triangles)); // for more precision split the 'swing triangle' into few smaller triangles
				REPA( triangles)
				{
					if(Cuts(triangles[i],chr_shape.capsule) && !attackBreak) // hit the 'nearest' character
					{
						Flt     weapon_damage=(weapon.valid() ? weapon().power : 0),
							random_scale =RandomF(0.9,1.1);

						attackBreak = true;
						Flt damage = (strength+weapon_damage)*random_scale;
						AddMessage(S+damage);

						nearest->addDamage(damage,this);
							
					}
				}
			}
		}
	}

	return true;
}
Bool Chr::update()
{
	// update horse
	if(!horse.empty())
	{
		if(!horse.valid() || horse().rider!=this)
			dismount();

		if(horse.valid())
		{
			alignToHorse();

			// break any active action
			actionBreak();

			// disable input
			input.turn.x=0;
			input.turn.y=Kb.b(KB_T)-Kb.b(KB_G);
			input.move.zero();
			input.dodge=0;
			input.crouch=0;
			input.walk=0;
			input.jump=0;
		}
	}


	if(__super::update())
	{
		attackAnim.updateAuto(3, 3, 1);

		handSwing.update(cskel.getPoint("HandR"),attackRange());


		updateAttacks();

		if(health < 0) health = 0;

		return true;
	}
	return false;
}

void Chr::toggleTorchiere(Torchiere &torchiere)
{
	if(torchiere.items.elms()) // get from torchiere
	{
		itemGetFrom(torchiere.items[0],torchiere);
	}
	else // put to torchiere
	{
		if(inv.slot[SLOT_TEMP].valid()) // temp slot
		{
			if(Contains(inv.slot[SLOT_TEMP]().name,"Torch"))itemPutTo(inv.slot[SLOT_TEMP](),torchiere);
		}
		else // first torch from inventory
			REPA(inv.items)
			if(Contains(inv.items[i].name,"Torch"))
			{
				itemPutTo(inv.items[i],torchiere);
				break;
			}
	}
}

void Chr::moveTo( Vec &pos )
{
	// first try finding path directly to 'pos' location
	// if it fails try finding path to positions near 'pos'

	if(!actionMoveTo(pos))
	{
		for(Int r= 1; r<=3; r++) // radius
			for(Int x=-1; x<=1; x++) // x offset
				for(Int z=-1; z<=1; z++) // z offset
					if(x || z)
						if(actionMoveTo(pos+Vec(x,0,z)*(r*0.6)))return;
	}
}
UInt Chr::drawPrepare()
{
	return __super::drawPrepare();
}