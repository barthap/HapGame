/******************************************************************************/
#include "stdafx.h"
namespace EE{
namespace Game{
/******************************************************************************/
#define DEFAULT_MODE BREAKABLE // here you can specify the default mode for Destructible objects - STATIC or BREAKABLE
/******************************************************************************/
static void CreateJoint(Joint &joint,Actor &a,Actor *b)
{
   joint.create(a,b).breakable(0.13f,0.01f);
}
static void CreateJoint(Joint &joint,Destructible &a,Destructible *b)
{
   if(a.actors.elms())
   {
      if(b) // joint attaches actor with other actor
      {
         if(b->actors.elms())CreateJoint(joint,a.actors.first(),&b->actors.first());
      }
      else // joint attaches actor with world
      {
         CreateJoint(joint,a.actors.first(),NULL);
      }
   }
}
/******************************************************************************/
Destructible::~Destructible()
{
}
Destructible::Destructible()
{
   mode         =STATIC;
   piece_index  =-1;
   scale        = 0;
   mesh         =NULL;
   material     =NULL;
   phys         =NULL;
   destruct_mesh=NULL;
}
/******************************************************************************/
// MANAGE
/******************************************************************************/
void Destructible::create(ObjParams &obj)
{
   mode       =DEFAULT_MODE;
   piece_index=-1;
   scale      =obj.scale   ();
   mesh       =obj.mesh    ();
   material   =obj.material();
   phys       =obj.phys    ();

   Matrix matrix=obj.matrixFinal().normalize();

   if(Param *destruct=obj.findParam("destruct"))destruct_mesh=DestructMeshs.get(destruct->asStr());

   if(mode==BREAKABLE) // create as BREAKABLE
   {
      if(destruct_mesh)
      {
         // create actors
         FREP(destruct_mesh->parts())actors.New().create(destruct_mesh->part(i).phys,1,scale)
                                                 .matrix(matrix)
                                                 .obj   (this);
         // put actors to sleep
         REPAO(actors).vel(Vec(0)).angVel(Vec(0)).sleep(true);
      }
   }else
   if(mode==STATIC) // create as STATIC
   {
      if(phys)actors.New().create(*phys,0,scale) // create main actor
                          .matrix(matrix)
                          .obj   (this);
   }else
   {
      Exit("Unrecognized Destructible Mode");
   }
}
/******************************************************************************/
// GET / SET
/******************************************************************************/
Vec    Destructible::pos         (              ){return actors.elms() ? actors.first().pos   (      )                 :              0;}
Matrix Destructible::matrix      (              ){return actors.elms() ? actors.first().matrix(      )                 : MatrixIdentity;}
Matrix Destructible::matrixScaled(              ){return actors.elms() ? actors.first().matrix(      ).scaleOrn(scale) : MatrixIdentity;}
void   Destructible::pos         (Vec    &pos   ){                 REPAO(actors)       .pos   (pos   );                                 }
void   Destructible::matrix      (Matrix &matrix){                 REPAO(actors)       .matrix(matrix); }
//void   Destructible::callBreak(Bool create_joints) { Break(create_joints); }
/******************************************************************************/
// OPERATIONS
/******************************************************************************/
void Destructible::Break(Bool create_joints)
{
   if((mode==STATIC || mode==BREAKABLE) // not a piece
   && destruct_mesh                     // has information about destructible mesh
   && destruct_mesh->parts()            // destructible mesh has parts
   && type()>=0)                        // has information about OBJ_TYPE
   {
      struct ActorInfo
      {
         Vec    vel,ang_vel;
         Matrix matrix;

         void set(Matrix &matrix,Vec &vel,Vec &ang_vel){T.matrix=matrix; T.vel=vel; T.ang_vel=ang_vel;}
      };
      Memc<Destructible*> pieces    ; // i-th element will point to i-th DestructMesh part
      Memc<ActorInfo    > actor_info; // i-th element will point to i-th DestructMesh part

      // create objects (pieces)
      ObjParams op;
      op.type    (true,World.objTypeName(type()));
      op.material(true,material);
      FREP(destruct_mesh->parts()) // order is important
      {
         Destructible *destr=NULL;
         if(!i)destr=this;                                                      // 0-th    piece  is  created from this object
         else  destr=CAST(Destructible,World.objCreateNear(op,matrixScaled())); // rest of pieces are dynamically created from new objects
         pieces.add(destr);                                                     // add object to the lookup array
      }
      if(mode==BREAKABLE && actors.elms()==pieces.elms())FREPA(actors)actor_info.New().set(actors[i].matrix(),actors[i].vel(),actors[i].angVel()); // remember actor info from current object, to recreate later
      else                                                            actor_info.New().set(        T.matrix(),         Vec(0),            Vec(0));

      // setup new objects
      joints.del(); // delete all joints of current object
      actors.del(); // delete all actors of current object
      FREPA(pieces)if(Destructible *destr=pieces[i]) // create each piece
      {
         destr->mode                     = PIECE;
         destr->piece_index              = i;
         destr->phys                     = NULL;
         destr->destruct_mesh            = destruct_mesh;
         destr->mesh                     =&destruct_mesh->part(i).mesh;
         destr->actors.del().New().create( destruct_mesh->part(i).phys,1,scale).obj(destr).sleepEnergy(0.35f); // set big sleeping energy threshold to more often put actors to sleep
      }

      // create joints
      if(create_joints)FREP(destruct_mesh->joints())
      {
         DestructMesh::Joint joint=destruct_mesh->joint(i);
         if(!InRange(joint.a,pieces) || !pieces[joint.a])Swap(joint.a,joint.b); // if 'a' is not valid then swap 'a' with 'b'
         if( InRange(joint.a,pieces) &&  pieces[joint.a])                       // if 'a' is     valid
         {
            Destructible *destr_a=pieces[joint.a];
            if(InRange(joint.b,pieces)) // b index is valid and points to other piece
            {
               if(Destructible *destr_b=pieces[joint.b])
               {
                  Joint &joint=destr_a->joints.New();
                              joint.destr=         destr_b;
                  CreateJoint(joint.joint,*destr_a,destr_b);
               }
            }
            else // b index is not valid, so attach to world
            {
               Joint &joint=destr_a->joints.New();
               CreateJoint(joint.joint,*destr_a,NULL);
            }
         }
      }

      // adjust object actors
      FREPA(pieces)if(Destructible *destr=pieces[i])
      {
         ActorInfo &ai=actor_info[Min(i,actor_info.elms()-1)];
         destr->actors.first().matrix(ai.matrix).vel(ai.vel).angVel(ai.ang_vel);
      }
   }
}
/******************************************************************************/
void Destructible::toStatic()
{
   if(mode==BREAKABLE)
   {
      mode=STATIC;

      Matrix matrix=T.matrix();

      joints.del();
      actors.del();
      if(phys)actors.New().create(*phys,0,scale) // create main actor
                          .matrix(matrix)
                          .obj   (this);
   }
}
/******************************************************************************/
void Destructible::toBreakable()
{
   if(mode==STATIC)
   {
      mode=BREAKABLE;

      Matrix matrix=T.matrix();

      joints.del();
      actors.del();
      if(destruct_mesh)
      {
         // create actors
         FREP(destruct_mesh->parts())actors.New().create(destruct_mesh->part(i).phys,1,scale)
                                                 .matrix(matrix)
                                                 .obj   (this);
         // put actors to sleep
         REPAO(actors).vel(Vec(0)).angVel(Vec(0)).sleep(true);
      }
   }
}
/******************************************************************************/
void Destructible::toPieces()
{
   if(mode==STATIC || mode==BREAKABLE)
   {
      Break(false);
   }
}
/******************************************************************************/
// UPDATE
/******************************************************************************/
Bool Destructible::update()
{
   switch(mode)
   {
      case BREAKABLE:
      {
         /*REPA(actors)if(!actors[i].sleep() && actors[i].energy()>=EPSL) // if at least one actor has been moved
         {
            Break(true);
            break;
         }*/
      }break;

      case PIECE:
      {
         if(!destruct_mesh || !InRange(piece_index,destruct_mesh->parts()))return false;
      }break;
   }
   return true;
}
/******************************************************************************/
// DRAW
/******************************************************************************/
UInt Destructible::drawPrepare()
{
   if(mesh)
   {
      Matrix matrix=matrixScaled();
      if(Frustum(mesh->box,matrix))
      {
         MaterialLock=material; mesh->draw(matrix, actors.elms() ? actors.first().vel() : Vec(0), actors.elms() ? actors.first().angVel() : Vec(0));
         MaterialLock=NULL    ;
      }
   }
   return 0; // no additional render modes required
}
/******************************************************************************/
void Destructible::drawShadow()
{
   if(mesh)
   {
      Matrix matrix=matrixScaled();
      if(Frustum(mesh->box,matrix))
      {
         MaterialLock=material; mesh->drawShadow(matrix);
         MaterialLock=NULL    ;
      }
   }
}
/******************************************************************************/
// IO
/******************************************************************************/
void Destructible::Joint::save(File &f){destr.save(f);}
Bool Destructible::Joint::load(File &f){destr.load(f); return true;}
/******************************************************************************/
Bool Destructible::canBeSaved(){return __super::canBeSaved() && actors.elms()>=1;} // save only if have at least 1 actor
/******************************************************************************/
void Destructible::save(File &f)
{
   DYNAMIC_ASSERT(actors.elms()>=1,"Destructible object doesn't have any actors");

   __super::save(f);

   f<<piece_index<<scale;
   f.putByte(              mode          );
   f.putStr (Materials    (material     ));
   f.putStr (DestructMeshs(destruct_mesh));

   switch(mode)
   {
      case STATIC:
      case BREAKABLE:
      {
         f.putStr(Meshs(mesh));
         f.putStr(Physs(phys));
         f<<actors.first().matrix();
      }break;

      case PIECE:
      {
         REPA(joints)if(joints[i].joint.broken())joints.remove(i); // remove unused joints
         joints.save (f);
         actors.first().saveState(f);
      }break;
   }
}
/******************************************************************************/
Bool Destructible::load(File &f)
{
   if(__super::load(f))
   {
      f>>piece_index>>scale;
      mode         =MODE         (f.getByte());
      material     =Materials    (f.getStr ());
      destruct_mesh=DestructMeshs(f.getStr ());

      switch(mode)
      {
         case STATIC   :
         case BREAKABLE:
         {
            mesh=Meshs(f.getStr());
            phys=Physs(f.getStr());
            Matrix matrix; f>>matrix;
            if(mode==STATIC)
            {
               if(phys)actors.New().create(*phys,0,scale).matrix(matrix).obj(this);
            }else
            {
               if(destruct_mesh)FREP(destruct_mesh->parts())actors.New().create(destruct_mesh->part(i).phys,1,scale).matrix(matrix).obj(this).sleep(true);
            }
         }return true;

         case PIECE:
         {
            joints.load(f);
            if(destruct_mesh && InRange(piece_index,destruct_mesh->parts()))
            {
                              mesh=&destruct_mesh->part(piece_index).mesh;
               actors.New().create( destruct_mesh->part(piece_index).phys,1,scale);
               if(!actors.first().loadState(f))return false;
            }else
            {
               mesh=NULL;
               if(!Actor().loadState(f))return false;
            }
            REPAO(actors).obj(this);

            // create joints that attach actor with world (joints which attach actor with other objects must be performed later in 'linkReferences')
            REPA(joints)
            {
               Joint &joint=joints[i];
               if(joint.destr.empty())CreateJoint(joint.joint,T,NULL); // if joint doesn't want to point to any other object, it means that it's a world joint
            }
         }return true;
      }
   }
   return false;
}
/******************************************************************************/
void Destructible::linkReferences()
{
   // create joints that attach actor with other objects (and their actors)
   REPA(joints)
   {
      Joint &joint=joints[i];
      Bool was_valid=   joint.destr.valid(); joint.destr.link();
      if( !was_valid && joint.destr.valid()) CreateJoint(joint.joint,T,&joint.destr()); // create the joint only if reference was successfully linked at this moment (was not valid, but after linking it is valid)
   }
}
/******************************************************************************/
}}
/******************************************************************************/