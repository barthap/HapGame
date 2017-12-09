/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Horse::Horse()
{
   angle=0;
   angle_offset=0;
   stand_run=0;
   turn=0;
   dir.zero();
}
/******************************************************************************/
// MANAGE
/******************************************************************************/
void Horse::createKinematicRagdoll()
{
   kinematic_ragdoll.create  (cskel,1,true          )  // create the helper ragdoll as kinematic
                    .group   (AG_CONTROLLER )// set desired actor group
                   // .ray     (false                 )  // disable ray-testing
                    .obj     (this                  )  // set game object owner
                    .fromSkel(cskel,cskel.vel(),true); // use 'true' for immediate pose setting at creation time, to ensure that the pose will be set immediately, please read comments on 'fromSkel' method for more information about it
}
void Horse::create(Game::ObjParams &obj)
{
               mesh=obj.mesh    (); if(!mesh)Exit("Horse doesn't have mesh");
   Skeleton   *skel=obj.skeleton(); if(!skel)Exit("Horse doesn't have skel");
   PhysBodyPtr phys=obj.phys    ();

   cskel.create(skel,obj.scale(),obj.matrixFinal());

   createKinematicRagdoll();

   Flt r=1, h=2;
   Vec pos=obj.matrixFinal().pos;

   if(phys && phys->parts.elms()>=1 && phys->parts[0].type()==PHYS_SHAPE)
   {
      r=phys->parts[0].shape.capsule.r;
      h=phys->parts[0].shape.capsule.h;
   }

   ctrl.createCapsule(r,h,pos).actor.obj(this).damping(0.1f).ccd(true).group(31).ray(true);

   angle=Angle(obj.matrixFinal().z.xz())-PI_2;

}
/******************************************************************************/
// GET / SET
/******************************************************************************/
Vec    Horse::pos   (          ) {return cskel.pos   ();}
Matrix Horse::matrix(          ) {return cskel.matrix();}
void   Horse::pos   (C Vec &pos) {Vec delta=pos-T.pos(); cskel.move(delta); ctrl.actor.pos(ctrl.actor.pos()+delta);}
/******************************************************************************/
void Horse::emitSound(Str name, Flt range, Flt volume)
{
   SoundPlay(name,pos(),range*2.5f,volume);
}
/******************************************************************************/
// CALLBACKS
/******************************************************************************/
void Horse::memoryAddressChanged() {ctrl.actor.obj(this); kinematic_ragdoll.obj(this);}
void Horse::linkReferences      () {rider.link();}
/******************************************************************************/
// UPDATE
/******************************************************************************/
Bool Horse::update()
{
   Vec2 move;
   Bool want_move;

   // update rider
   if(!rider.empty())
      if(!rider.valid() || rider().horse!=this)
         rider.clear();

   // update input
   if(rider.valid())
   {
      // move
      VecI2 diri(Kb.b(KB_D)-Kb.b(KB_A),
                 Kb.b(KB_W)-Kb.b(KB_S));

      if(!diri.y)diri.x=0; // don't allow strafing

      // rotate
      AdjustValTime(turn ,Kb.b(KB_Q)-Kb.b(KB_E),0.0001f);
      AdjustValTime(dir.x,diri.x,0.01f);
      AdjustValTime(dir.y,diri.y,0.01f);
      AdjustValTime(angle_offset,diri.x*diri.y*-PI_4,0.01f);
      angle+=turn*2*Time.d();

      if(Ms.hidden())angle+=Ms.d().x*-0.8f;

      want_move=diri.any();
      move=dir*12; move.clampLength(0,12);
      move.rotate(angle);
   }else
   {
      angle_offset=0;
      turn=0;
      dir =0;
      move=0;
      want_move=false;
   }
   AdjustValBool(stand_run,want_move,Time.d()*4);
   ctrl.update(move.x0y(),false,0);

   // animate
   Matrix m; m.setRotateY(-angle-angle_offset).move(ctrl.actor.pos());

   cskel.clear();
   cskel.animate(L"anim/horse/stand.anim",                         Time.time(),1-stand_run);
   cskel.animate(L"anim/horse/run.anim"  ,dir.y<0 ? -Time.time() : Time.time(),  stand_run);
   cskel.updateMatrix(m).updateVelocities();

   // step sound
   if(ctrl.onGround() && stand_run>0.25f && Animations("anim/horse/run.anim")->eventOccured("step",Time.time(),Time.d()))
      SoundPlay(S+"sound/step/horse/"+Random(1, 4)+".wav", T.pos(), 1.5);

  // kinematic_ragdoll.fromSkel(cskel,cskel.vel(),true);

   return true;
}
/******************************************************************************/
// ENABLE / DISABLE
/******************************************************************************/
void Horse::disable() {super::disable(); ctrl.actor.kinematic(true );}
void Horse:: enable() {super:: enable(); ctrl.actor.kinematic(false);}
/******************************************************************************/
// DRAW
/******************************************************************************/
UInt Horse::drawPrepare()
{
   if(mesh)if(Frustum(Ball().setAnimated(mesh->box,cskel)))
   {
      SetHighlight((Lit==this && !rider.valid()) ? Color(34,85,85) : Color(0,0,0)); mesh->draw(cskel);
      SetHighlight(                                                     Color(0,0,0));
   }
   return 0;
}
void Horse::drawShadow()
{
   if(mesh)if(Frustum(Ball().setAnimated(mesh->box,cskel)))mesh->drawShadow(cskel);
}
/******************************************************************************/
// IO
/******************************************************************************/
void Horse::save(File &f)
{
   super::save(f);

   f.putStr(mesh.name());
   cskel.save(f);
   ctrl .save(f);
   rider.save(f);
   f<<angle<<angle_offset<<stand_run<<turn<<dir;
   kinematic_ragdoll.saveState(f,true);
}
Bool Horse::load(File &f)
{
   if(super::load(f))
   {
      mesh .require(f.getStr());
      cskel.load   (f);
      ctrl .load   (f); ctrl.actor.obj(this);
      rider.load   (f);
      f>>angle>>angle_offset>>stand_run>>turn>>dir;
      createKinematicRagdoll(); if(!kinematic_ragdoll.loadState(f))return false;
      return true;
   }
   return false;
}
/******************************************************************************/
