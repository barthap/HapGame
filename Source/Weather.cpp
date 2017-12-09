/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
static Vec Col(Byte r, Byte g, Byte b)
{
	return Vec(r,g,b)/255;
}

struct TimeLight
{
	Flt hour;
	Vec horizon,sky, rays;
};

TimeLight tl[]=
{
	{ 0, Col( 30, 40, 56), Col( 12, 16, 23), Col(0, 0, 0)},
	{ 2, Col( 30, 40, 56), Col( 12, 16, 23), Col(0, 0, 0)},
	{ 4, Col( 204, 82, 8), Col( 8, 99, 204), Col(0, 0, 0)},
	{ 5, Col( 249, 187, 95), Col( 8, 99, 204), Vec(1, 0.733, 0)/4},
	{ 6, Col(110,136,152),  Col( 78,107,149), Vec(1, 0.733, 0)/4},
	{ 8, Col(110,136,152), Col( 78,107,149), Vec(0.3,0.3,0.3)/2},
	{12, Col(121,142,164), Col( 70,107,161), Vec(0.3,0.3,0.3)/2},
	{15, Col(121,137,154), Col( 58, 95,144), Vec(1,0.733,0)/4},
	{17, Col(121,137,154), Col( 58, 95,144), Vec(1,0.733,0)/4},
	{18,Col(164,119,92), Col( 58, 95,144), Vec(1,0.733,0)/4},
	{19, Col(179,73,73), Col( 58, 95,144), Vec(1,0.733,0)/3},
	{20, Col(164,119,92), Col( 59, 82,126), Vec(0.1,0.1,0.1)},
	{20.5, Col( 41, 57, 89), Col( 41, 57, 89), Col(0, 0, 0)},
	{22, Col( 72, 80,101), Col( 26, 30, 41), Col(0, 0, 0)},
};
static void GetTimeLight(Flt hour, TimeLight tl[], Int tls, Vec &horizon, Vec &sky, Vec &rays)
{
	if(tls)
	{
		hour=Frac(hour,24);
		Int prev; for(prev=tls-1; prev>=0; prev--)if(tl[prev].hour<hour)break; if(prev<0)prev+=tls;
		Int next=(prev+1)%tls;

		Flt prev_hour=tl[prev].hour; if(     hour< prev_hour)     hour+=24;
		Flt next_hour=tl[next].hour; if(next_hour<=prev_hour)next_hour+=24;
		Flt step=LerpRS(prev_hour,next_hour,hour);

		horizon=Lerp(tl[prev].horizon,tl[next].horizon,step);
		sky    =Lerp(tl[prev].sky    ,tl[next].sky    ,step);
		rays	=Lerp(tl[prev].rays   ,tl[next].rays	,step);
	}else
	{
		horizon.zero();
		sky    .zero();
	}
}

_Time time;
void _Weather::create()
{
	
	//set sky
	Sky.atmospheric();

	//set sun
	Sun.image=Images("gfx/sky/sun.gfx");
	Sun.rays_mode=SUN_RAYS_HIGH;
	Sun.rays_res = 1;

	{
      //Moon=Astros.New();              // create a new astronomical object in 'Astros' container
      Moon.image=Images("gfx/sky/moon.gfx"); // set image
		Moon.size*=0.5;                        // decrease default size
      Moon.blend=false;                      // disable blending, and thus set adding mode
      Moon.image_color.set(80,80,80,0);      // set image color
      Moon.light_color.zero();               // disable light casting
	}

	//set clouds
	Clouds.layered.set(3,Images("gfx/Clouds/Layers/0.gfx"));
	REPAO(Clouds.layered.layer).velocity*=2;

	//Water
	//Water.images(Images("water/0.gfx"),Images("water/0.n.gfx"),Images("water/reflection.gfx")); //Set water from textures
    //Water.draw = true; //Enable drawing
    //Water.wave_scale = 0.2;

	_time=0;
	_skok=0;
	zmiana=true;
}
void _Weather::init()
{
	time.setStartTime();

}
void _Weather::update(Flt timeUpdate)
{

	time.updateTime(timeUpdate);
	//here we cut value from getHours to integer type _hours
	_hours=time.getHours();
	_minutes=time.getMinutes();
	//sun move
	SinCos(Sun.pos.y,Sun.pos.x,(time.getHours())/24*PI2-PI_2); Sun.pos.z=0;
	Sun.pos*=Matrix3().setRotateY(PI-PI_4);

	//moon move - DON'T WORK
	//TODO: Make the moon move work
	SinCos(Moon.pos.y,Moon.pos.x,(time.getHours())/24*PI2+PI_2); Moon.pos.z=0;
	Moon.pos*=Matrix3().setRotateX(PI+PI_4);

	// tweak the sun to rise earlier, and set later
	Sun.pos.y+=0.4f;
	Sun.pos.normalize();

	// set sun color
	Sun.light_color=(1-D.ambPower())*Sat(Cbrt(Sun.pos.y)-0.2 );
	Sun.image_color=Color(Sun.light_color);

	// set sun rays and highlight
	Flt rays=Sqrt(Sun.pos.y);
	Sun.highlight_front=Lerp(0.80f,0.20f,rays);

	// set sky color
	Vec horizon,sky,SunRays; GetTimeLight(_hours,tl,Elms(tl),horizon,sky,SunRays);
	Sky.atmosphericColor(Vec4(horizon,1),Vec4(sky,1));
	Sun.rays_color=SunRays;
	//Sun.image_color = Color(SunRays);

	//sun power
	//power=Sat(Cbrt(Sun.pos.y));
	//color change
	//updateSkyColor();
	//checking hours and setting sky color
	//sunRise();
	//sunSet();

	//setting sun colors
//#pragma region sun colors
//	if (_hours>=18 && _hours<=5)
//   {
//      Sun.light_color=Vec(0,0,0);
//      Sun.light_color=Vec(0,0,0);
//   }
//   else if (_hours>=5 && _hours<=18)
//   {
//		if(_hours>12 && _hours<15)
//			Flt power=Sat(Cbrt(Sun.pos.y))-0.2;
//		else
//			Flt power=Sat(Cbrt(Sun.pos.y));
//
//   Sun.light_color=(1-D.ambPower())*power;
//
//	if(_hours>7 && _hours <17)
//		Sun.image_color=Color(Sun.light_color);
//	else
//		Sun.image_color=Color(255, 255, 128); //YELLOW_LIGHT
//   }
//	if(_hours>5 && _minutes>=25 && _hours<6 && _minutes<=35)
//		Sun.rays_color=Lerp(Sun.rays_color,(Vec(1,0.733,0)/2),Sat(_time)); //DON'T WORK
//	//if(_hours==5 && _minutes==25)
//	//	Sun.rays_color=(Vec(1,0.733,0)/2);
//	else if(_hours==18 && _minutes==50)
//		Sun.rays_color=Lerp(Sun.rays_color,Vec(0.1,0.1,0.1), Sat(_time));
//	if(_hours>8 && _hours<15)Sun.rays_color=Vec(0.3,0.3,0.3);
//	if(_hours==15)Sun.rays_color=Vec(1,0.733,0)/3;
//	if(_hours==16 && _minutes==30)Sun.rays_color=Vec(1,0.733,0)/2;
//#pragma endregion

	//update Clouds
	Clouds.layered.update();
	REP(Clouds.layered.layers())Clouds.layered.layer[i].color=ColorBrightness(horizon.max()*(255.0f/164));

    //Update water
	Water.update(Vec2(0.01, 0.01));
}
void _Weather::setSkyColor(Vec4 colorHorizon, Vec4 colorSky, Flt skok)
{
	//TEN KOD JEST PRAWID£OWY, NIE KOMENTUJ
	if(colorHorizon==Vec4(0))
		newHorizonColor=Sky.atmosphericHorizonColor();
	else
		newHorizonColor=colorHorizon;
	if(colorSky==Vec4(0))
		newSkyColor=Sky.atmosphericSkyColor();
	else
		newSkyColor=colorSky;
	oldSkyColor=Sky.atmosphericSkyColor();
	oldHorizonColor=Sky.atmosphericHorizonColor();
	_time=1;
	_skok=skok*Time.speed();
}
void _Weather::updateSkyColor()
{
	_time=_time-_skok;
	if(_time>_skok)
	{
		_tim=Sat(_time);
		Sky.atmosphericColor((Vec4)Lerp(newHorizonColor,oldHorizonColor,_tim), (Vec4)Lerp(newSkyColor,oldSkyColor,_tim));
	}
}
void _Weather::sunRise()
{
	//sun rise
	if(_hours==3)
	{
		if(zmiana)
		{
			setSkyColor(Vec4(0.01,0.21,0.43,1),Vec4(0.01,0.21,0.43,1),0.001);
			zmiana=false;
		}
	}
	else zmiana=true;
	if(_hours==4)
	{
		if(zmiana)
		{
			setSkyColor(Vec4(0.02,0.27,0.56,1),Vec4(0.02,0.27,0.56,1),0.001);
			zmiana=false;
		}
	}
	else zmiana=true;
	if(_hours==5 && _minutes==30)
	{
		if(zmiana)
		{
			setSkyColor(Vec4(0.80,0.32,0.03),Vec4(0.03,0.39,0.80),0.001);
			zmiana=false;
		}
	}
	else zmiana=true;
	if(_hours==6 && _minutes==5)
	{
		if(zmiana)
		{
			setSkyColor(Vec4(0.03,0.39,0.80),Vec4(0.03,0.39,0.80),0.001);
			zmiana=false;
		}
	}
	else zmiana=true;
}
void _Weather::sunSet()
{
	//zachód s³oñca
	if(_hours==16 && _minutes==30)
	{
		if(zmiana)
		{
			setSkyColor(Vec4(0.80,0.32,0.03,1),Vec4(0.03,0.39,0.80,1),0.001);
			zmiana=false;
		}
	}
	else zmiana=true;
	if(_hours==17 && _minutes==30)
	{
		if(zmiana)
		{
			setSkyColor(Vec4(0.02,0.27,0.56,1),Vec4(0.02,0.27,0.56,1),0.001);
			zmiana=false;
		}
	}
	else zmiana=true;
	if(_hours==18)
	{
		if(zmiana)
		{
			setSkyColor(Vec4(0.01,0.21,0.43,1),Vec4(0.01,0.21,0.43,1),0.001);
			zmiana=false;
		}
	}
	else zmiana=true;
	if(_hours==19)
	{
		if(zmiana)
		{
			setSkyColor(Vec4(0.01,0.02,0.26,1),Vec4(0.01,0.02,0.26,1),0.001);
			zmiana=false;
		}
	}
	else zmiana=true;

}
void _Weather::deleteweather()
{
	Sun.light_color=0;
	Sun.light_vol=0;
	Moon.light_color=0;
	Moon.light_vol=0;
REPAO(Clouds.layered.layer).velocity/=2;
	time.setStartTime();
}