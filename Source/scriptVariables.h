/******************************************************************************/
enum scriptVariableType
{
	SVT_INT,
	SVT_STR,
	SVT_BOOL,
};
/******************************************************************************/
struct scriptVariable
{
	Str  name;
	Byte type;

	Int  valueInt;
	Str  valueStr;
	Bool valueBool;

	void save(File &f);
	Bool load(File &f);
};

extern Memb<scriptVariable> scriptVariables;
/******************************************************************************/
struct scriptVariableManager
{
	Int find(Str name);
};

extern scriptVariableManager svManager;
/******************************************************************************/