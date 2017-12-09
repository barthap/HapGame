/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
void _Time::setStartTime()
{
	seconds=0;
	minutes=0;
	hours=0;
	days=0;
	months=0;
	years=0;
	speed=1;
	save_seconds=0;
	save_minutes=0;
	save_hours=6;
	save_days=0;
	save_months=0;
	save_years=0;

}
//_Time::_Time()
//{
	//stopped=false;
	//setStartTime();
//}
//_Time::~_Time()
//{
//	stopped=true;
//	setStartTime();
//}
void _Time::saveTime(File &f)
{
	//f<<minutes<<hours<<days<<months<<years<<speed;
	f.putFlt(minutes);
	f.putFlt(hours);
	f.putInt(days);
	f.putInt(months);
	f.putInt(years);
	f.putFlt(speed);
}

Bool _Time::loadTime(File &f)
{
      //f>>save_minutes>>save_hours>>save_days>>save_months>>save_years>>speed;
	save_minutes = f.getFlt();
	save_hours = f.getFlt();
	save_days = f.getInt();
	save_months = f.getInt();
	save_years = f.getInt();
	speed = f.getFlt();
    
	return true;
}
void _Time::setTime()
{
		if(Kb.bp(KB_F12))
	{
		speed+=0.1;
		Time.speed(speed);
	}
	else if(Kb.bp(KB_F11))
	{
		speed-=0.1;
		Time.speed(speed);
	}
	//else if(Kb.bp(KB_F10))
	//{
//		Time.speed(0);
//	}
	else if(Kb.bp(KB_F9))
	{
		Time.speed(speed);
	}
	else if(Kb.bp(KB_6))
	{
		save_hours+=1;
	}
	else if(Kb.bp(KB_7))
	{
		save_hours-=1;
	}

}

void _Time::updateTime(Flt time)
{
	Time.speed();
	setTime();
	//time calculations
	seconds=minutes*60+save_seconds;
	minutes=time+save_minutes;
	hours=(minutes/60)+save_hours;
	days=(hours/24)+save_days;
	months=(days/31)+save_months;
	years=months/12+save_years;
	//"time cutting"
	seconds=seconds%60;
	minutes=Frac(minutes,60);
	hours=Frac(hours,24);
	days=days%31;
	months=months%12;
}