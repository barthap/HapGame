enum CONTROLLER
{
	CONTROLLER_KEYBOARD_MOUSE,
	CONTROLLER_PS3_PAD,
	CONTROLLER_XBOX360_PAD,
	CONTROLLER_COMPUTER_PAD
};
extern CONTROLLER activeCtrl;

enum PS3PAD_AXIS
{
	AXIS_X_PLUS = 20,	//w prawo
	AXIS_X_MINUS,		//w lewo
	AXIS_Y_PLUS,		//w gore
	AXIS_Y_MINUS		//w dol
};

enum PS3PAD_BUTTONS
{
	BTN_GREEN_TRIANGLE = 0, //zielony trojkat
	BTN_RED_CIRCLE,			//czerwone kolo
	BTN_BLUE_CROSS,			//nebieski krzyzyk
	BTN_PINK_SQUARE,		//rozowy kwadrat
	BTN_L1 = 4,				//przycisk L1
	BTN_R1,					//przycisk R1
	BTN_SELECT = 8,			//przysisk Select
	BTN_START,				//przycisk Start
	BTN_L3 = 10,			//przycisk L3 (wcisniety lewy drazek)
	BTN_R3,					//przycisk R3 (wcisniety prawy drazek)
	BTN_PLAYSTATION = 12,	//przysisk PS3
};

enum POINTOFVIEW_BUTTONS
{
	POV_TOP,
	POV_BOTTOM,
	POV_LEFT,
	POV_RIGHT
};

class Ps3Controller
{
	JoypadClass &joypad;
	
	bool pov_pressed[4];
public:

	//konstruktor
	Ps3Controller();

	//lewy i prawy dr¹¿ek
	bool Axis(PS3PAD_AXIS Axis, bool LeftAxis = true, Flt deadZone = 0.1f);
	//przyciski, BtnNumber mo¿e byæ wartoœci¹ enum PS3PAD_BUTTONS
	bool Btn(int BtnNumber, bool Long = true);
	//przyciski L2 i R2
	bool LR2(bool Left, Flt deadZone = 0.2f);
	
	//G-Sensor, im bardziej wartosc rozna od 0, tym bardziej nachylony jest pad
	//ujemna X - nachylenie do tylu, dodatnia X - nachylenie w przód
	//ujemna Y - nachylenie w lewo, dodatnia X - nachylenie w prawo
	Vec2 GetControllerPosition();

	//przyciski zmiany widoku
	bool PointOfViewButtonPressed(POINTOFVIEW_BUTTONS button, bool Long);

} extern Ps3;