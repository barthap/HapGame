//Camera.cpp
/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Camera desired_camera; // create a helper desired camera
CAMERA cam;

void CAMERA::changeView()
{
	// set next camera mode when Tab pressed
	if(Kb.bp(KB_F2))
	{
		View=(View+1)%VIEW_NUM;
	}
}
void CAMERA::updateCam()
{
	   // setup the camera
   
	// setup the camera
   if(Players.elms()) // if we have at least one player
   {
		

      // set camera depending on current view mode
      switch(View)
      {
         case VIEW_FPP:
         {
            OrientP &head=Players[0].cskel.getPoint("head"); // obtain player "head" skeleton point (this was created in Mesh Editor)
            Cam.setPosDir(head.pos,head.dir,head.perp); // set camera from 'head' position, direction and perpendicular to direction
         }break;

         case VIEW_TPP:
         {
				desired_camera.yaw = Players[0].angle.x;          // update camera yaw   angle according to mouse smooth delta x
				desired_camera.pitch =Players[0].angle.y;          // update camera pitch angle according to mouse smooth delta y
				Clamp(desired_camera.pitch,-PI_2,PI_4); // clamp to possible camera pitch angle
				desired_camera.dist=Max(1.0f,desired_camera.dist*ScaleFactor(Ms.wheel()*-0.1)); // update camera distance according to mouse wheel

				/*
				Vec offset = Vec(0);
								if(OrientP *p = Players[0].cskel.findPoint("Head"))
									offset = p->cross() * 1.f;
								//THIS!!! here you can delete Vec(0.5,1.2,0); and it should be normal
								desired_camera.at  =Players[0].pos()+Vec(offset.x,1.35,offset.z);
				*/

				// camera position
				{
					Player &plr = Players[0];

					Vec head =plr.cskel.skeleton()->getPoint("Head").pos+Vec(0,0.18f,-0.05f) // calculate behind and above head position relative to body bone
						-plr.cskel.skeleton()->getBone ("Body").pos;
					head*=plr.cskel.scale   ();

					Matrix3 look_matrix; look_matrix.setRotateXY(-plr.angle.y*(plr.horse.valid() ? 0.4f : 0.6f),-plr.angle.x); head*=look_matrix;

					Vec pos=          plr.ctrl_pos+head;
					//pos=Lerp(pos, plr.stored_pos              , 0.5f*plr.standCrouch());
					//pos=Lerp(pos, plr.stored_pos+Vec(0,0.2f,0),           CamTppCustom);

					desired_camera.at=pos;
				}

				

				desired_camera.setSpherical(); // set as spherical from current values, this will set the camera's matrix (desired_camera.matrix)
				// create a helper ball which will be used for collision detection
				Ball ball(0.1f, desired_camera.at); // we place it at starting point (where the camera is looking at)

				// now we'll move the ball in the direction where the camera should be
				Physics.move(ball, desired_camera.matrix.pos-ball.pos); // use physics movement to move the ball as far as it can go without any collisions
				Cam.setPosDir(ball.pos, desired_camera.matrix.z, desired_camera.matrix.y); // we'll use 'desired_camera.matrix' directions which were set in 'setSpherical' camera method
         }break;

         default: // VIEW_FPP
			{
				OrientP &head=Players[0].cskel.getPoint("head"); // obtain player "head" skeleton point (this was created in Mesh Editor)
            Cam.setPosDir(head.pos,head.dir,head.perp); // set camera from 'head' position, direction and perpendicular to direction
			}
         break;
      }

      // after setting camera position and angles:
      Cam.updateVelocities().set(); // update camera velocities and activate it
   }
   else // when no player on the scene
   {
      CamHandle(0.1,100,CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT)); // default camera handling actions
   }

}