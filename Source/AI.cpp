/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Int monsterKills[AIMT_ENUM_ELEMENTS];

Str AttackAnims[]=
{
   "anim/swing/r-l.anim" ,
   "anim/swing/r-r.anim" ,
   "anim/swing/r-ld.anim",
   "anim/swing/r-rd.anim",
};
/******************************************************************************/
AI::AI()
{
   isTalking = false;

   dialoguePath = NULL;

   aiType = AI_DEFAULT;
   aiClassType = AIC_DEFAULT;

   routineWaypoint = -1;
   rW = -1;
   actRoutinePoint = 0;

   time_to_walk = 0;
   strength = 0;
   attackOnAllMap = false;

   time_to_random_move = RandomF(5, 10);
   time_to_sense = 0;
   time_to_update_action = 0;
   time_to_attack = 0;

   commanderID = -1;

   commandSet = true;
   timeToUpdateCommand = 0;
   wantToUpdateCommand = false;

   nowTrading = false;

   cantAttack = false;

}

void AI::create(Game::ObjParams &obj)
{
   __super::create(obj);

   T.ctrl.actor.group(31);

   //Detect parameters
   if(Param *p = obj.findParam("Dialogue")) dialoguePath = p->asText();
   if(Param *p = obj.findParam("name")) name = p->asText();
   if(Param *p = obj.findParam("strength")) strength = p->asFlt();
   if(Param *p = obj.findParam("aiType")) aiType = p->asEnum();
   if(Param *p = obj.findParam("routineWaypoint")) routineWaypoint = p->asInt();
   if(Param *p = obj.findParam("cantAttack")) cantAttack = p->asBool();
   if(Param *p = obj.findParam("aiObjType")) aiClassType = p->asEnum();

   if(Param *p = obj.findParam("commanderID")) commanderID = p->asFlt();

   rW = routineWaypoint;

   startPos = T.pos();
   startAngle = T.angle;

   wantToRespawn = false;

	for(Game::ObjParamsPtr cur = &obj; cur; cur = cur->base())
	   FREPA(cur->sub_objs)
		{
			Game::ObjParams &o = cur->sub_objs[i];
			switch(Game::World.objType(o.type()))
			{
				case OBJ_ITEM:
					{
						Item *subObjItem = &inv.items.New();
						subObjItem->create(o);

						//If we want to equip
						//if(T.aiType == AI_VILLAGE_GUARD_1 || T.aiType == AI_VILLAGE_GUARD_2)
						{
							if(subObjItem->type == ITEM_WEAPON)
							{
								if(subObjItem->power > (inv.slot[SLOT_ARM_R].valid() ? inv.slot[SLOT_ARM_R]().power : 0)) inv.slot[SLOT_ARM_R] = subObjItem;
							}
							else if(subObjItem->type == ITEM_ARMOR)
							{
								Int slotNumber;

								slotNumber = -1;

								switch(subObjItem->type2)
								{
									case ARMOR_SHIELD:
										slotNumber = SLOT_ARM_L;
										break;
									case ARMOR_HELMET:
										slotNumber = SLOT_HEAD;
										break;
								    case ARMOR_BODY:
										slotNumber = SLOT_BODY;
										break;
								    case ARMOR_PANTS:
										slotNumber = SLOT_PANTS;
										break;
								    case ARMOR_GAUNTLET:
										slotNumber = SLOT_GAUNTLET;
										break;
								    case ARMOR_BOOT:
										slotNumber = SLOT_BOOT;
										break;
								    case ARMOR_NECK:
										slotNumber = SLOT_NECK;
										break;
								    case ARMOR_HAND:
										slotNumber = SLOT_HAND_R;
										break;
								}

								if(slotNumber > -1)
								{
									if(subObjItem->defence > (inv.slot[slotNumber].valid() ? inv.slot[slotNumber]().defence : 0)) inv.slot[slotNumber] = subObjItem;
								}
							}
						}
					}
					break;
			}
		}

   //Temporary values
   T.move_walking = true;
}
/******************************************************************************/
// OPERATIONS
/******************************************************************************/
void AI::attack(Chr &target)
{
   T.target = target;
   time_to_update_action = 0;
}
/******************************************************************************/
void AI::hit()
{
   Vec dir;
   SinCos(dir.z, dir.x, angle.x + PI_2);
   dir.y = 0; //Get AI looking direction


	Reference<Item> &weapon=inv.slot[SLOT_ARM_R];

   if(target.valid())
   {
	   if(inAttackRange(target()))
	   {
					if(AbsAngleBetween(dir, target().pos() - pos()) <= PI_3) // 60 degrees
					{
						if(weapon.valid())
						{
							Flt str = weapon().power + strength;
							target().addDamage(RandomF(str - str * 0.1, str + str * 0.1), this);
						}
						else
							target().addDamage(RandomF(strength - strength * 0.1, strength + strength * 0.1), this);
					}
	   }
   }
}
void AI::die(Chr *src)
{
	T.target.clear();

	if(aiClassType == AIC_MONSTER)
	{
		timeToRespawn = RandomF(8, 15);
		wantToRespawn = true;
	}

	__super::die(src);

	if(src == &Players[0])
	{
		if(aiClassType <= AIMT_ENUM_ELEMENTS) monsterKills[aiClassType] += 1;
		Players[0].exp += strength * 4;
		AddMessage(S + "Doœwiadczenie +" + (Int)(strength * 4));
	}
}
/******************************************************************************/

/******************************************************************************/
void AI::animate()
{
   __super::animate();

   REPA(animations)cskel.animate(animations[i], true); // apply attack animations
}
Bool AI::update()
{
	time_to_walk -= Time.d();
	/*scriptPointer = &T;
	script.run("AI/test.lua");
	script.onUpdate();
	scriptPointer = NULL;*/
	saySound.pos(T.pos());

	if(wantToRespawn) timeToRespawn -= Time.d();

	if(__super::update())
	{
		if(isTalking == true)
		{
			//Rotate face to player
			actionBreak();
			
			Vec v1 = Players[0].pos() - pos();

			if(inAttackRange(Players[0], 0.4))
				actionBreak();
			else
				moveTo( Players[0].pos());

			Flt angle_delta = AngleDelta(angle.x + PI_2, Angle(Players[0].pos().xz() - pos().xz()));
			Flt	max_frame_delta = Time.d() * DegToRad(270);

			Clamp(angle_delta, -max_frame_delta, max_frame_delta);
			angle.x += angle_delta;

			return true;
		}

		if(!target.valid())
		{
			FREPA(Chrs)
			{
				if(Dist(pos(), Chrs[i].pos()) <= 5)
				{
					if(Chrs[i].target.valid() && Chrs[i].inAttackRange(Chrs[i].target(), 1))
					{
						//Rotate face to one of the fighters
						actionBreak();

						Flt angle_delta = AngleDelta(angle.x + PI_2, Angle(Chrs[i].pos().xz() - pos().xz()));
						Flt	max_frame_delta = Time.d() * DegToRad(270);

						Clamp(angle_delta, -max_frame_delta, max_frame_delta);
						angle.x += angle_delta;

						return true;
					}
				}
			}
		}

		if(wantToRespawn && timeToRespawn <= 0) //If it's time to respawn
		{
			Vec dir;

			SinCos(dir.z, dir.x, Players[0].angle.x + PI_2);
			dir.y = 0;

			if(!(AbsAngleBetween(dir, pos() - Players[0].pos()) <= PI_2) && (Dist(pos(), Players[0].pos()) >= 5)) //If the player can't see us
			{
				health = 100;
				ragdollDisable();
				target.clear();
				
				Flt x = RandomF(-5, 10);
				Flt z = RandomF(-5, 10);

				pos(startPos + Vec(x, Game::World.hmHeight(Vec2(x, z)) + 0.1, z));

				wantToRespawn = false;
			}
		}

		if(alive())
		{
			inv.update(T);

			REPA(animations)
			{
				Motion &motion=animations[i];
				Reference<Item> &weapon=inv.slot[SLOT_ARM_R];
				if(weapon.valid())
				{
					if(motion.eventOccured("hit-from"))hit();
				}
				else
					if(motion.eventOccured("hit"))hit();                  // if "hit" event occured (event's are set in Mesh Editor tool - Animation mode)

				if(!motion.updateAuto(3,3,1)) animations.remove(i); // if finished playing then remove
			}


			if(time_to_walk <= 0) T.move_walking = true;

			switch(aiType)
			{
				case AI_SETTLER_1:
					{
						//settler 1 jest typem ai chodzacym po waypointach, nic poza tym nie robi
						waypointMove();

					}
				break;
				
				case AI_SOLIDER_1:
					{
						/*if(Contains(command, "attack::rand"))
						{
							if(helpVar > -1)
							{
								Int number = Random(getChrByID(helpVar)->subordinates.elms() - 1);

								if(number > -1) attack(getChrByID(helpVar)->subordinates[number]());
							}

							command.clear();
						}*/

						REPA(Chrs)
						{
							if(commanderID == Chrs[i].uniqueID && !enemy.valid())
								if(Chrs[i].target.valid())attack(Chrs[i].target());
						}
					}
				break;

				case AI_COMMANDER_1:
					{
						FREPA(subordinates)
						{
							if(!subordinates[i].valid()) subordinates.remove(i, true);
							else if(!subordinates[i]().alive()) subordinates.remove(i, true);
						}

						if(wantToUpdateCommand == true)
						{
							timeToUpdateCommand -= Time.d();
							if(timeToUpdateCommand <= 0) commandSet = false;

							if(!commandSet)
							{
								if(enemy.valid())
								{
									FREPA(subordinates)
									{
										if(subordinates[i].valid() && !subordinates[i]().target.valid())
										{
											//subordinates[i]().command = "attack::rand";
											//subordinates[i]().helpVar = -1;
											//subordinates[i]().helpVar = enemy().uniqueID;

											Int number = Random(enemy().subordinates.elms() - 1);

											if(number > -1)
											{
												if(Chr *chrToAttack = CAST(Chr, &enemy().subordinates[number]())) subordinates[i]().attack(*chrToAttack);
											}
											else
											{
												if(enemy().alive())
												{
													if(Chr *chrToAttack = CAST(Chr, &enemy())) subordinates[i]().attack(*chrToAttack);
												}
											}
										}
									}
								}

								timeToUpdateCommand = 20;
								commandSet = true;
							}
						}

						//if(target.valid() && target().alive()) updateAttacks();
					}
				break;

				case AI_VILLAGE_GUARD_1:
					{
						if(!target.valid() || !target().alive())	//FOR ALL AI TYPES (but AI_MONSTER_1 not)!!!
						{
							if(Game::Waypoint *waypoint = Game::World.findWaypoint(S + "WAYPOINT_GUARD_ID" + routineWaypoint))
							{
								if(Dist(waypoint->points[actRoutinePoint].pos, pos()) - ctrl.radius() <= 0.5) actRoutinePoint = (actRoutinePoint + 1) % waypoint->points.elms();
								T.moveTo(waypoint->points[actRoutinePoint].pos); //Access waypoint position
								//T.move_walking = true;
							}

							FREPA(Chrs)
							{
								if(i < Chrs.elms())
								{
									if(Chrs[i].target.valid() && Chrs[i].target().alive() && Chrs[i].alive())
									{
										if(Chrs[i].inAttackRange(Chrs[i].target(), 5))
										{
											Flt dist = Dist(pos(), Chrs[i].pos());
											if(dist <= 15)
											{
												//if(Chrs[i].target().alive()/* && Chrs[i].target == Players[0]*/)
												//{
													//AI *chr = CAST(AI, &Chrs[i].target());
													if(Chrs[i].aiClassType == AIC_MONSTER) attack(Chrs[i]);
													else if(Chrs[i].target().isPlayer){Chrs[i].target().isWanted = true;attack(Chrs[i].target());}
													else if(Chrs[i].aiType != AI_VILLAGE_GUARD_1 && Chrs[i].aiType != AI_VILLAGE_GUARD_2 && Chrs[i].aiType !=AI_VILLAGE_GUARD_3)attack(Chrs[i]);
													else if(Chrs[i].target().aiType != AI_VILLAGE_GUARD_1 && Chrs[i].target().aiType != AI_VILLAGE_GUARD_2) attack(Chrs[i].target());
												//}
											}
										}
										else if(Chrs[i].aiClassType == AIC_MONSTER)
										{
											if(Chrs[i].inAttackRange(T, 5)) attack(Chrs[i]);
										}
									}
								}
							}
							if(Players[0].timeToAvadaKedavraEnd > 0 && Players[0].usedAvadaKedavra)
								if(inAttackRange(Players[0], 12) && Players[0].alive())
								{
									if(T.target!=Players[0])attack(Players[0]);
								}
						}
					}
				break;

				case AI_VILLAGE_GUARD_2:
					{
						

						if(pos() != startPos && !target.valid())
						{
							moveTo(startPos);
						}
						else if(angle != startAngle && !target.valid())
						{
							actionBreak();

							Flt angle_delta = AngleDelta(angle.x + PI_2, Angle(startPos.xz() - pos().xz()));
							Flt	max_frame_delta = Time.d() * DegToRad(270);

							Clamp(angle_delta, -max_frame_delta, max_frame_delta);
							angle.x += angle_delta;
						}

						if(!target.valid())
							FREPA(Chrs)
							{
								if(i < Chrs.elms())
								{
									if(Chrs[i].target.valid() && Chrs[i].target().alive() && Chrs[i].alive())
									{
										if(Chrs[i].inAttackRange(Chrs[i].target(), 5))
										{
											Flt dist = Dist(pos(), Chrs[i].pos());
											if(dist <= 15)
											{
												//if(Chrs[i].target().alive()/* && Chrs[i].target == Players[0]*/)
												//{
													//AI *chr = CAST(AI, &Chrs[i].target());
													if(Chrs[i].aiClassType == AIC_MONSTER) attack(Chrs[i]);
													else if(Chrs[i].target().isPlayer){Chrs[i].target().isWanted = true;attack(Chrs[i].target());}
													else if(Chrs[i].aiType != AI_VILLAGE_GUARD_1 && Chrs[i].aiType != AI_VILLAGE_GUARD_2 && Chrs[i].aiType !=AI_VILLAGE_GUARD_3)attack(Chrs[i]);
													else if(Chrs[i].target().aiType != AI_VILLAGE_GUARD_1 && Chrs[i].target().aiType != AI_VILLAGE_GUARD_2) attack(Chrs[i].target());
												//}
											}
										}
										else if(Chrs[i].aiClassType == AIC_MONSTER)
										{
											if(Chrs[i].inAttackRange(T, 10)) attack(Chrs[i]);
										}
									}
								}
							}
							if(Players[0].timeToAvadaKedavraEnd > 0 && Players[0].usedAvadaKedavra)
								if(inAttackRange(Players[0], 12) && Players[0].alive())
								{
									if(T.target!=Players[0])attack(Players[0]);
								}
					}
				break;

				case AI_VILLAGE_GUARD_3:
					{
						randomMove();

							FREPA(Chrs)
							{
								if(i < Chrs.elms())
								{
									if(Chrs[i].target.valid() && Chrs[i].target().alive() && Chrs[i].alive())
									{
										if(Chrs[i].inAttackRange(Chrs[i].target(), 10))
										{

											if(inAttackRange(Chrs[i], 25))
											{
												//jesli jest to straznik to mu pomaga
												if(Chrs[i].aiType == AI_VILLAGE_GUARD_1 || Chrs[i].aiType == AI_VILLAGE_GUARD_2)
													if(Chrs[i].health >= 40)
												{
													attack(Chrs[i].target());
													break;
												}
												
												if(Chrs[i].aiClassType == AIC_MONSTER)
												{
													if(callReinforcements(Chrs[i]))
														attack(Chrs[i]);
												}
												else if(Chrs[i].aiType == AI_VILLAGE_GUARD_1 || Chrs[i].aiType == AI_VILLAGE_GUARD_2)
												{
													if(callReinforcements(Chrs[i].target()))
														attack(Chrs[i].target());
												}
												else if(Chrs[i].target().aiType != AI_VILLAGE_GUARD_1 && Chrs[i].target().aiType != AI_VILLAGE_GUARD_2)
												{
													if(callReinforcements(Chrs[i].target()))
														attack(Chrs[i].target());
												}
												
											}
										}
										else if(Chrs[i].aiClassType == AIC_MONSTER)
										{
											if(Chrs[i].inAttackRange(T, 5)) attack(Chrs[i]);
										}
									}
								}
							}
							if(Players[0].timeToAvadaKedavraEnd > 0 && Players[0].usedAvadaKedavra)
								if(inAttackRange(Players[0], 12) && Players[0].alive())
								{
									callReinforcements(Players[0]);
									if(T.target!=Players[0])attack(Players[0]);
								}
						}
					break;

				case AI_MERCHANT_1:
					//Rotate face to player if trading
					if(nowTrading)
					{
						actionBreak();

						Flt angle_delta = AngleDelta(angle.x + PI_2, Angle(Players[0].pos().xz() - pos().xz()));
						Flt	max_frame_delta = Time.d() * DegToRad(270);

						Clamp(angle_delta, -max_frame_delta, max_frame_delta);
						angle.x += angle_delta;
					}
				break;

				case AI_MONSTER_1:
					{
						if(!target.valid())
						{
							T.move_walking = true;

							if((time_to_sense-=Time.d())<=0)
							{
								if(Players.elms() && Players[0].alive() && Dist(pos(),Players[0].pos())<=24)attack(Players[0]);
								time_to_sense=RandomF(1,2);
							}
						}

						//updateAttacks(); //AI_MONSTER actions

						randomMove();


					}
				break;
			}

			if(target.valid() && target().alive()) updateAiAttacks();
		}


		return true;
	} //if __super::update()
	return false;
}

void AI::updateAiAttacks()
{


	if(target.valid())
	{
		if(!target().alive())
		{
			target.clear();
			actionBreak();
		}else
		{
			T.move_walking = false;

			Bool in_range=inAttackRange(target()); // detect if target is in attack range

			// update moving path in case the target changes location
			if((time_to_update_action-=Time.d())<=0 && !in_range)
			{
				moveTo(target().pos());
				time_to_update_action=RandomF(0.05,0.1);
			}

			if(target().isWanted && target().timeToGiveUp > 3)
			{
				target().timeToEndWanted -=Time.d();
				if(target().timeToEndWanted <= 0)
				{
					target().isWanted = false;
					target().timeToEndWanted = 10;
				}
				return;
			}

			// if in attack range then rotate to face the target
			if(in_range)
			{
				actionBreak(); // stop moving any further

				Flt angle_delta    =AngleDelta(angle.x+PI_2, Angle(target().pos().xz() - pos().xz())), // calculate angle delta between current look angle and target angle
					max_frame_delta=Time.d()*DegToRad(270); // 270 degrees per second, this is the maximum angle delta limit to change per frame

				Clamp(angle_delta, -max_frame_delta, max_frame_delta);
				angle.x+=angle_delta;
			}

			// if in attack range then attack
			if((time_to_attack-=Time.d())<=0 && in_range)
			{
				attackBreak = false;

				Reference<Item> &weapon=inv.slot[SLOT_ARM_R];
				if(weapon.valid())
				{
					attackAnim.set(cskel,AttackAnims[Random(Elms(AttackAnims))]);
				}
				else
					attackAnim.set(cskel,"anim/ai/attack.anim");

				time_to_attack=RandomF(1,1.5);
			}
		}
	}
	
}
/******************************************************************************/
// DRAW
/******************************************************************************/
UInt AI::drawPrepare()
{
   inv.drawPrepare(T);

   return __super::drawPrepare();
}
void AI::drawShadow()
{
   inv.drawShadow(T);

   return __super::drawShadow();
}
/******************************************************************************/
void AI::save(File &f)
{
   __super::save(f);

   inv.save(f);

   //f.put(&target, SIZE(Reference<ChrEC>));
   target.save(f);

   f.putByte(aiType);
   f.putByte(aiClassType);
   f.putStr(dialoguePath);
   f.putInt(actRoutinePoint);
   f.putInt(routineWaypoint);
   f.putFlt(health);
   f.putFlt(mana);

   f.putStr(name);
   f.putBool(isTalking);

   f.putStr(textToSay);
   f.putFlt(timeToSay);
   f.putInt(rW);
   f.putBool(attackOnAllMap);
   f.putBool(cantAttack);

   f.putFlt(time_to_random_move);
   f.putFlt(time_to_sense);
   f.putFlt(time_to_update_action);
   f.putFlt(time_to_attack);

   f.putFlt(strength);
   f.putFlt(time_to_walk);

	f.putFlt(defense);

   f.put(&startPos, SIZE(Vec));
   f.put(&startAngle, SIZE(Vec));

   animations.save(f);
}
Bool AI::load(File &f)
{
   if(__super::load(f))
   {
      if(!inv.load(f)) return false;

	  //f >> health >> aiType >> routineWaypoint >> name >> actRoutinePoint >> isTalking >> dialoguePath >> aiObjType;

	  //f.get(&target, SIZE(Reference<ChrEC>));
	  target.load(f);

      aiType = f.getByte();
      aiClassType = f.getByte();
      dialoguePath = f.getStr();
      actRoutinePoint = f.getInt();
      routineWaypoint = f.getInt();
      health = f.getFlt();
	  mana = f.getFlt();

	  name = f.getStr();
	  isTalking = f.getBool();

	  textToSay = f.getStr();
	  timeToSay = f.getFlt();
	  rW = f.getInt();
	  attackOnAllMap = f.getBool();
	  cantAttack = f.getBool();

	  time_to_random_move = f.getFlt();
	  time_to_sense = f.getFlt();
	  time_to_update_action = f.getFlt();
	  time_to_attack = f.getFlt();

	  strength = f.getFlt();
	  time_to_walk = f.getFlt();

	  defense = f.getFlt();

	  f.get(&startPos, SIZE(Vec));
	  f.get(&startAngle, SIZE(Vec));

	  REPA(animations)
		animations[i].load(f, cskel);

	  T.ctrl.actor.group(31);

	  return true;
   }

   return false;
}
/******************************************************************************/
void AI::linkReferences()
{
   target.link();

   //Set the subordinates container
   if(aiType == AI_COMMANDER_1)
   {
	   FREPA(Chrs)
	   {
			if(Chrs[i].commanderID == uniqueID)
			{
				Reference<AI> *newSO = &subordinates.New();
				//Reference<AI> newSOTS = &Chrs[i]/*CAST(ChrEC, &Chrs[i])*/;

				*newSO = &Chrs[i]/*&newSOTS*/;
			}
	   }
   }
}
void AI::addDamage(Flt damage, Chr *src)
{
	__super::addDamage(damage, src);

	if(target.valid() && target().isPlayer && src == &Players[0]) return;

	routineWaypoint = -1;
	if(cantAttack == false || aiClassType == AIC_MONSTER)
	{
		attack(*src);
	}
	else
	{
		T.move_walking = false;
		actionMoveTo(pos()+Random.vec(-3,10));
		routineWaypoint = rW;
		time_to_walk = 3;
		callReinforcements(*src);
	}
		
}
/*void AI::l_update(Game::Chr &owner)
{
   if(loot_gui)
   {
	   loot_gui->update(owner);
   }
}*/
/******************************************************************************/
void AI::randomMove()
{
	if(!target.valid())
	{
		if((time_to_random_move-=Time.d())<=0)
		{
			if(!action)
			{
				actionMoveTo(pos()+Random.vec(-3,3));
				time_to_random_move=RandomF(3,10);
			}
		}
	}
}

void AI::waypointMove()
{
	if(!target.valid() || !target().alive())
	{
		routineWaypoint = rW;
	}
	//Do your daily routine
	if(Game::Waypoint *waypoint = Game::World.findWaypoint(S + "WAYPOINT_ID" + routineWaypoint))
	{
		if(Dist(waypoint->points[actRoutinePoint].pos, pos()) - ctrl.radius() <= 0.5) actRoutinePoint = (actRoutinePoint + 1) % waypoint->points.elms();
		T.moveTo(waypoint->points[actRoutinePoint].pos); //Access waypoint position
		//T.move_walking = true;
	}
}

bool AI::callReinforcements( Chr &target, int needed/*=2*/ )
{
	int reinforcementsNr = 0;

	REPA(Chrs)
	{
		if(inAttackRange(Chrs[i], 100) && (Chrs[i].aiType == AI_VILLAGE_GUARD_1 || Chrs[i].aiType == AI_VILLAGE_GUARD_2))
		{
			Chrs[i].attack(target);
			reinforcementsNr++;
		}

		if(reinforcementsNr >= needed) return true;

		if(inAttackRange(Chrs[i], 200) && (Chrs[i].aiType == AI_VILLAGE_GUARD_1 || Chrs[i].aiType == AI_VILLAGE_GUARD_2))
		{
			Chrs[i].attack(target);
			reinforcementsNr++;
		}

		if(reinforcementsNr >= needed) return true;

		Game::ObjParamsPtr playerCharacter; playerCharacter.require("Obj/Chr/Human/0.obj");

		Game::ObjParams obj;
		obj.base(playerCharacter);
		obj.type(true,"OBJ_CHR");
		Game::World.objCreate(obj,Matrix().setScalePos(obj.scale(),T.pos() - RandomF(25, 50)));
		Chrs[Chrs.elms()-1].aiType = AI_VILLAGE_GUARD_3;
	}

	return false;
}

void AI::SF_Attack( luabind::object target )
{
	Chr *chr = object_cast<Chr*>(target);
	attack(*chr);
}
