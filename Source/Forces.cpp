/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
_Forces Forces;
/******************************************************************************/
_Forces::_Forces()
{
   FREP(F_NUMBER_OF_FORCES) enabled[i] = true;

   signedForce = 0;

   posInPrevFrame = Vec2(0, 0);
}
void _Forces::create()
{
	if(guiObjects.load("Gui/Obj/forces.gobj"))
    {
      /*Gui +=*/ guiObjects;

      forceImages[0] = &guiObjects.getImage("force1");
      forceImages[1] = &guiObjects.getImage("force2");
    }

	Forces.forceImages[0]->image = Images(S + "Gfx/Forces/0b.gfx");
}
void _Forces::del()
{
	guiObjects.del();
}
/******************************************************************************/
Bool _Forces::update()
{
	FREP(F_NUMBER_OF_FORCES)
	{
		if(enabled[i] == true && !forceImages[i]->visible())
		{
			forceImages[i]->visible(true);
		}
		else if(enabled[i] == false && forceImages[i]->visible())
		{
			forceImages[i]->visible(false);
		}
	}

	if(Ms.bp(2))
	{
		switch(signedForce)
		{
			case F_FORCE:
				{
					if(Players[0].mana >= 15)
					{
						Players[0].animations.New().set(Players[0].cskel, CastSpellNormal[Random(Elms(CastSpellNormal))]);

						Game::ObjParams op;
						op.type(true, "OBJ_FIREBALL");

						Game::World.objCreate(op, Matrix(Players[0].cskel.getPoint("HandR").pos));
						Game::Obj* fireBallObj = &Fireballs[Fireballs.elms() - 1];

						if(Fireball *fb = CAST(Fireball, fireBallObj))
						{
						   Players[0].mana -= 15;
						   fb->type = FB_F_USE_FORCE;
						   fb->vel = Players[0].cskel.getPoint("head").dir;
						   fb->caster = Players[0];
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
		}
	}

	if(Ms.b(2))
	{
		if(signedForce == F_TELEKINESIS)
		{
			if(!tObject.valid())
			{
				if(AI *chr = CAST(AI, Lit))
				{
					tObject = chr;

					distanceToObj = Dist(Players[0].pos(), chr->pos());

					//chr->ragdollEnable();
				}
			}
		}
	}

	if(Ms.br(2))
	{
		if(signedForce == F_TELEKINESIS)
		{
			if(tObject.valid())
			{
				if(AI *chr = CAST(AI, &tObject())) chr->ragdollDisable();
				tObject.clear();
				distanceToObj = -1;
			}
		}
	}

	if(tObject.valid())
	{
		if(posInPrevFrame != Ms.pos())
		{
			//tObject().actor->
		}
	}

	posInPrevFrame = Ms.pos();

	return true;
}
void _Forces::draw()
{
	
}
/******************************************************************************/
Int _Forces::nextEnabledForce()
{
	if(signedForce + 1 == F_NUMBER_OF_FORCES)
	{
		FREP(F_NUMBER_OF_FORCES)
		{
			if(enabled[i]) return i;
		}
	}
	else
	{
		Int i = signedForce + 1;

		FREPD(i, F_NUMBER_OF_FORCES - 1 - signedForce)
		{
			if(enabled[i]) return i;
		}
	}

	return -1;
}
Int _Forces::previousEnabledForce()
{
	if(signedForce - 1 == 0)
	{
		REP(F_NUMBER_OF_FORCES)
		{
			if(enabled[i]) return i;
		}
	}
	else
	{
		Int i = signedForce - 1;

		REPD(i, F_NUMBER_OF_FORCES - 1 - signedForce)
		{
			if(enabled[i]) return i;
		}
	}

	return -1;
}
Int _Forces::enabledForces()
{
	Int result;

	result = 0;

	FREP(F_NUMBER_OF_FORCES)
	{
		if(enabled[i]) result += 1;
	}

	return result;
}
/******************************************************************************/
void _Forces::save(File &f)
{
   FREP(F_NUMBER_OF_FORCES) f.putBool(enabled[i]);
}
Bool _Forces::load(File &f)
{
   FREP(F_NUMBER_OF_FORCES) enabled[i] = f.getBool();

   return true;
}
/******************************************************************************/