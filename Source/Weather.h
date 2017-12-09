struct _Weather
{
	Astro &Moon;
	void create();
	void deleteweather();
	void update(Flt time);
	void init();
	void sunRise();
	void sunSet();
	//void setDay();
	//void setNight();
	void setSkyColor(Vec4 colorHorizon, Vec4 colorSky, Flt skok);
	void updateSkyColor();
	_Weather() : Moon(Astros.New())
	{create();}
private:
	Vec4 newSkyColor,
		newHorizonColor,
		oldSkyColor,
		oldHorizonColor,
		skyColor,
		horizonColor;
	Bool zmiana;
	Flt _skok,_time,power,_tim;
	Int _hours,_minutes;
};