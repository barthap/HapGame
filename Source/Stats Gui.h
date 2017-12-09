/******************************************************************************/
struct _StatsGui
{
	Window window;

	Text mainStatsT[2];
	Text levelInfo[3];
	Button addMainStat[4];

	void create();

	void toggle();
	void update();

} extern StatsGui;

extern TextDS statsGuiTDS;
/******************************************************************************/