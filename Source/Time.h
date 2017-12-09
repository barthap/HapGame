
struct _Time
{
public:
	void setTime();
	void updateTime(Flt time);
	void setStartTime();
	Int getSeconds() const {return seconds;}
	Flt getMinutes() const {return minutes;}
	Flt getHours()   const {return hours;  }
	Int getDays()    const {return days;   }
	Int getMonths()  const {return months; }
	Flt getSpeed()   const {return speed;  }
	Int getYears()   const {return years;  }
	Flt getSaveMinutes() const {return save_minutes;}
	Flt getSaveHours()   const {return save_hours;}
	void saveTime(File &f);
	Bool loadTime(File &f);
	//_Time();
	//~_Time();
	//Bool stopped;
private:
	Flt minutes,hours,speed;
	Int seconds,days,years,months;
	Flt save_seconds,save_minutes,save_hours;
	Int save_days,save_years,save_months;
}extern time;