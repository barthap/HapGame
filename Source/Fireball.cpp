/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Fireball::Fireball()
{
   exploded = false;
	type = FB_DEFAULT;
   power = 0;
}
/******************************************************************************/
// MANAGE
/******************************************************************************/
void Fireball::create(Game::ObjParams &obj)
{
   particles.create(*ParticlesCache("particle/fireball/0.particle"));
   particles.matrix.pos = obj.matrix.pos;

   Flt scale = 0.45f;
   particles.radius       *= scale;
   particles.offset_range *= scale;
   particles.vel_random   *= scale;
   particles.vel_constant *= scale;
   particles.accel        *= scale;
   particles.shape        *= scale;
   particles.fade_in      = 0.1f;
   particles.life_total   = 20;

   particles.reset();

   ball.set(0.10f, obj.matrix.pos);
   vel.zero();
}
/******************************************************************************/
// UPDATE
/******************************************************************************/
Bool Fireball::update()
{
   if(!exploded)
   {
      struct FireballCutsCallback : PhysCutsCallback
      {
         Bool cuts;
         Obj  *caster;

         Bool hit(Int group, Ptr user, Game::Obj *obj)
         {
            if(obj != caster)
            {
               cuts = true;
               return false;
            }

            return true;
         }

         FireballCutsCallback(Game::Obj *caster)
         {
            T.cuts = false;
            T.caster = caster;
         }
      };

      struct FireballHitCallback : PhysHitCallback
      {
         Bool      cuts;
         Flt       dist;
         Plane     plane;
         Game::Obj *caster;

         Bool hit(PhysHit &phys_hit)
         {
            if(phys_hit.obj != caster)
            {
               if(!cuts || phys_hit.dist < dist)
               {
                  cuts = true;
                  dist = phys_hit.dist;
                  plane = phys_hit.plane;
               }
            }

            return true;
         }

         FireballHitCallback(Game::Obj *caster)
         {
            T.cuts = false;
            T.caster = caster;
         }
      };

      //UInt actor_groups =~ (IndexToFlag(AG_KINEMATIC_RAGDOLL));
	  UInt actor_groups =~ IndexToFlag(AG_CONTROLLER);
      Vec  move = vel * Time.d();

      //Check for collision
      FireballCutsCallback fcc(caster.valid() ? &caster() : NULL); Physics.cuts (ball, fcc, actor_groups);
      FireballHitCallback  fhc(caster.valid() ? &caster() : NULL); Physics.sweep(ball, move, fhc, actor_groups);

      if(fcc.cuts || fhc.cuts) explode(fhc.cuts ? &fhc.plane : NULL);

      if(!exploded)
      {
         ball += move;
         particles.matrix.pos = ball.pos;
      }
   }

   if(!particles.update()) return false;

   return true;
}
/******************************************************************************/
// OPERATIONS
/******************************************************************************/
void Fireball::explode(Plane *plane)
{
   if(!exploded)
   {
      exploded = true;

      //Add damage to all nearby characters
      Memc<Obj*> objects;
	  
	  Game::World.objGet(objects, Ball(2, pos()));

      REPA(objects)
	  {
		  if(Chr *chr = CAST(Chr, objects[i]))
		  {
			  switch(type)
			  {
			  case FB_FIRE_ATTACK:
					if(chr->alive() && caster != chr)
					{
						 Vec hit_dir = chr->pos() - pos();
						 Flt dist = Max(0.0f, hit_dir.normalize() - chr->ctrl.radius());
						 Flt fallof = LerpRS(1.5f, 0.5f, dist);
						 Flt random_scale = RandomF(1.0f / 1.2f, 1.2f);

						 if(fallof > 0) chr->addDamage(power * fallof, caster.valid() ? &caster() : NULL);
					}
					break;
			  case FB_FIRE_HEALTH:
				  if(chr->alive() && caster != chr)
					{
						 Vec hit_dir = chr->pos() - pos();
						 Flt dist = Max(0.0f, hit_dir.normalize() - chr->ctrl.radius());
						 Flt fallof = LerpRS(1.5f, 0.5f, dist);
						 Flt random_scale = RandomF(1.0f / 1.2f, 1.2f);

						  if(fallof > 0) chr->health+=(power * fallof);
					}
				  break;
			  }
		  }
		  else if(Game::Destructible *destruct = CAST(Game::Destructible, objects[i]))
		  {
			  switch(type)
			  {
				case FB_F_USE_FORCE:
					Vec hit_dir = destruct->pos() - pos();
					Flt dist = Max(0.0f, hit_dir.normalize());
					Flt fallof = LerpRS(1.5f, 0.5f, dist);
					Flt random_scale = RandomF(1.0f / 1.2f, 1.2f);

					//if(fallof > 0) chr->health+=(power * fallof);
					destruct->toPieces();
				  break;
			  }
		  }
	  }

          particles.reborn = false;
          particles.fade_in = 0;
          particles.fade_out = 0;
          particles.life_cur = 0.0f;
          particles.life_total = 1.5f;
          particles.accel.set(0, -12, 0);

      Vec normal = !(plane ? plane->normal : -vel);

      REPA(particles)
      {
         Particle &p = particles.p(i);
         if(plane) p.pos = Lerp(p.pos, plane->pos, 0.25f);
         p.life *= 3.2f * 0.8f;
         p.life_max *= 3.2f;
         p.radius *= 0.6f;
         p.vel = Random.dir(normal, 1.2f) * 5.0f;
      }
   }
}
/******************************************************************************/
// DRAW
/******************************************************************************/
UInt Fireball::drawPrepare()
{
   LightPoint(5 * particles.fade() * particles.opacity(), particles.matrix.pos, Vec(1, 0.9, 0.8)).add(false, this);

   if(Frustum(Ball(ball).extend(3)))
   {
      return IndexToFlag(particles.palette() ? RM_PALETTE : RM_BLEND) | IndexToFlag(RM_BLEND);
   }
   return 0;
}
void Fireball::drawPalette() { particles.draw(); }
void Fireball::drawBlend  () { particles.draw(); if(!exploded) DrawParticle(*Images("particle/fireball/star.gfx"), 0, Color(255, 255, 170, 0), 0.15f, 1.1f, 0, particles.matrix.pos, Vec(0)); }
/******************************************************************************/
// ENABLE / DISABLE
/******************************************************************************/
void Fireball::disable() { __super::disable(); }
void Fireball:: enable() { __super:: enable(); }
/******************************************************************************/
// IO
/******************************************************************************/
void Fireball::save(File &f)
{
    __super::save(f);

   caster.save(f);
   particles.save(f, true);

   f << exploded << power << ball << vel;
}
Bool Fireball::load(File &f)
{
   if(__super::load(f))
   {
      caster.load(f);

      if(!particles.load(f)) return false;

      f >> exploded >> power >> ball >> vel;

      return true;
   }

   return false;
}
/******************************************************************************/
void Fireball::linkReferences() { caster.link(); }
/******************************************************************************/
