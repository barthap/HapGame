/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Memb<scriptVariable> scriptVariables;
scriptVariableManager svManager;
/******************************************************************************/
void scriptVariable::save(File &f)
{
	//f << name << type << valueInt << valueStr << valueBool;
	f.putStr(name);
	f.putByte(type);
	f.putInt(valueInt);
	f.putStr(valueStr);
	f.putBool(valueBool);
}
Bool scriptVariable::load(File &f)
{
	//f >> name >> type >> valueInt >> valueStr >> valueBool;
	name = f.getStr();
	type = f.getByte();
	valueInt = f.getInt();
	valueStr = f.getStr();
	valueBool = f.getBool();

	return true;
}
/******************************************************************************/
Int scriptVariableManager::find(Str name)
{
	FREPA(scriptVariables)
	{
		if(Contains(scriptVariables[i].name, name))
		{
			return i;
		}
	}

	return -1;
}
/******************************************************************************/