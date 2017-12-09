#include "stdafx.h"
#include "Main.h"
#include "scriptObjects.h"

class ObjManager
{
public:
	AI* createChr(const char* chrPath, int x, int y, int z)
	{

		Str path = addPath(actModCheck->modDirectoryName, chrPath);

		Game::ObjParams &obj = *Game::Objs(path);
		Game::World.objCreate(obj, Matrix(obj.scale(), Vec(x, y, z)));
		AI *newChr = &Chrs[Chrs.elms() - 1];

		return newChr;
	}


	Chr* getChr(UInt id)
	{
		REPA(Chrs)
			if(Chrs[i].uniqueID == id)return &Chrs[i];

		return NULL;
	}

	Item* createItem(const char* itemPath, int x, int y, int z)
	{
		Str path = addPath(actModCheck->modDirectoryName, itemPath);

		Game::ObjParams &obj = *Game::Objs(path);
		Game::World.objCreate(obj, Matrix(obj.scale(), Vec(x,y,z)));
		Item *newItem = &Items[Items.elms() - 1];

		return newItem;
	}

	Trigger* getTrigger(UInt id)
	{
		REPA(Triggers)
			if(Triggers[i].id== id)return &Triggers[i];

		return NULL;
	}

	//konwersja z AI na Chr (Chr na ChrBase)
	Chr* chrToBase(object const &obj)
	{
		AI* ai = object_cast<AI*>(obj);
		return CAST(Chr,ai);
	}

}objManager;

void chrPtrID(lua_State* L)
{
	Chr* chr = (Chr*)lua_touserdata(L, 1);
	lua_pushnumber(L, chr->uniqueID);
}

void SF_setObjective(int colorR, int colorG, int colorB, float posX, float posY, float posZ, bool drawDecal)
{
	objectiveMarker.Set(Vec4(Vec(colorR, colorG, colorB)/255, 1), Vec(posX, posY, posZ), drawDecal);
}
void SF_clearObjective()
{
	objectiveMarker.isSet = false;
}
//funkcja mimo tego ze jest 'void' zwraca wartosc do skryptu, za pomoca lua_push....
void SF_toPointer(luabind::object const& obj, UInt type)
{
	try
	{
		//dla oszczednosci pamieci
		//i tak tylko 1 na raz jest uzywane.
		union
		{
			Player* plr;
			Chr* chr;
			Item* item;
			Trigger* trigger;
		};

		switch(type)
		{
		case 0: //player
			plr = luabind::object_cast<Player*>(obj);
			lua_pushlightuserdata(obj.interpreter(), plr);
			break;
		case 1: //chr
			chr = luabind::object_cast<Chr*>(obj);
			lua_pushlightuserdata(obj.interpreter(), chr);
			break;
		case 2: //item
			item = luabind::object_cast<Item*>(obj);
			lua_pushlightuserdata(obj.interpreter(), item);
			break;
		case 3: //trigger
			trigger = luabind::object_cast<Trigger*>(obj);
			lua_pushlightuserdata(obj.interpreter(), trigger);
			break;
		default:
			lua_pushnil(obj.interpreter());
			break;
		}


	}
	catch(luabind::error &e)
	{
		Exit(e.what());
	}

}

void setGlobalNumber(lua_State* lua, Str8 name, Dbl value)
{
	lua_pushnumber(lua, (int)value);
	lua_setglobal(lua, name);
}

void RegisterKeyboardGlobals(lua_State* L)
{
	//add key defines to script
	setGlobalNumber(L, "KB_ESC", 1);

	setGlobalNumber(L, "KB_1", 2 );
	setGlobalNumber(L, "KB_2", 3 );
	setGlobalNumber(L, "KB_3", 4 );
	setGlobalNumber(L, "KB_4", 5 );
	setGlobalNumber(L, "KB_5", 6 );
	setGlobalNumber(L, "KB_6", 7 );
	setGlobalNumber(L, "KB_7", 8 );
	setGlobalNumber(L, "KB_8", 9 );
	setGlobalNumber(L, "KB_9", 10 );
	setGlobalNumber(L, "KB_0", 11 );
	setGlobalNumber(L, "KB_MINUS", 12 );
	setGlobalNumber(L, "KB_EQUAL", 13 );
	setGlobalNumber(L, "KB_BACKSPACE", 14 );
	setGlobalNumber(L, "KB_TAB", 15 );
	setGlobalNumber(L, "KB_Q", 16 );
	setGlobalNumber(L, "KB_W", 17 );
	setGlobalNumber(L, "KB_E", 18 );
	setGlobalNumber(L, "KB_R", 19 );
	setGlobalNumber(L, "KB_T", 20 );
	setGlobalNumber(L, "KB_Y", 21 );
	setGlobalNumber(L, "KB_U", 22 );
	setGlobalNumber(L, "KB_I", 23 );
	setGlobalNumber(L, "KB_O", 24 );
	setGlobalNumber(L, "KB_P", 25 );
	setGlobalNumber(L, "KB_LBRACE", 26 );
	setGlobalNumber(L, "KB_RBRACE", 27 );
	setGlobalNumber(L, "KB_ENTER", 28 );
	setGlobalNumber(L, "KB_LCTRL", 29 );
	setGlobalNumber(L, "KB_A", 30 );
	setGlobalNumber(L, "KB_S", 31 );
	setGlobalNumber(L, "KB_D", 32 );
	setGlobalNumber(L, "KB_F", 33 );
	setGlobalNumber(L, "KB_G", 34 );
	setGlobalNumber(L, "KB_H", 35 );
	setGlobalNumber(L, "KB_J", 36 );
	setGlobalNumber(L, "KB_K", 37 );
	setGlobalNumber(L, "KB_L", 38 );
	setGlobalNumber(L, "KB_COLON", 39 );
	setGlobalNumber(L, "KB_QUOTE", 40 );
	setGlobalNumber(L, "KB_LSHIFT", 42 );
	setGlobalNumber(L, "KB_BSLASH", 43 );
	setGlobalNumber(L, "KB_Z", 44 );
	setGlobalNumber(L, "KB_X", 45 );
	setGlobalNumber(L, "KB_C", 46 );
	setGlobalNumber(L, "KB_V", 47 );
	setGlobalNumber(L, "KB_B", 48 );
	setGlobalNumber(L, "KB_N", 49 );
	setGlobalNumber(L, "KB_M", 50 );
	setGlobalNumber(L, "KB_COMMA", 51 );
	setGlobalNumber(L, "KB_PERIOD", 52 );
	setGlobalNumber(L, "KB_FSLASH", 53 );
	setGlobalNumber(L, "KB_RSHIFT", 54 );
	setGlobalNumber(L, "KB_NUMMULT", 55 );
	setGlobalNumber(L, "KB_LALT", 56 );
	setGlobalNumber(L, "KB_SPACE", 57 );
	setGlobalNumber(L, "KB_CAPSLOCK", 58 );
	setGlobalNumber(L, "KB_F1", 59 );
	setGlobalNumber(L, "KB_F2", 60 );
	setGlobalNumber(L, "KB_F3", 61 );
	setGlobalNumber(L, "KB_F4", 62 );
	setGlobalNumber(L, "KB_F5", 63 );
	setGlobalNumber(L, "KB_F6", 64 );
	setGlobalNumber(L, "KB_F7", 65 );
	setGlobalNumber(L, "KB_F8", 66 );
	setGlobalNumber(L, "KB_F9", 67 );
	setGlobalNumber(L, "KB_F10", 68 );
	setGlobalNumber(L, "KB_F11", 87 );
	setGlobalNumber(L, "KB_F12", 88 );
	setGlobalNumber(L, "KB_NUMLOCK", 69 );
	setGlobalNumber(L, "KB_SCROLL", 70 );
	setGlobalNumber(L, "KB_NUM7", 71 );
	setGlobalNumber(L, "KB_NUM8", 72 );
	setGlobalNumber(L, "KB_NUM9", 73 );
	setGlobalNumber(L, "KB_NUMMINUS", 74 );
	setGlobalNumber(L, "KB_NUM4", 75 );
	setGlobalNumber(L, "KB_NUM5", 76 );
	setGlobalNumber(L, "KB_NUM6", 77 );
	setGlobalNumber(L, "KB_NUMPLUS", 78 );
	setGlobalNumber(L, "KB_NUM1", 79 );
	setGlobalNumber(L, "KB_NUM2", 80 );
	setGlobalNumber(L, "KB_NUM3", 81 );
	setGlobalNumber(L, "KB_NUM0", 82 );
	setGlobalNumber(L, "KB_NUMPERIOD", 83 );
	setGlobalNumber(L, "KB_NUMENTER", 156 );
	setGlobalNumber(L, "KB_RCTRL", 157 );
	setGlobalNumber(L, "KB_NUMDIV", 181 );
	setGlobalNumber(L, "KB_PRINT", 183 );
	setGlobalNumber(L, "KB_RALT", 184 );
	setGlobalNumber(L, "KB_BREAK", 197 );
	setGlobalNumber(L, "KB_HOME", 199 );
	setGlobalNumber(L, "KB_UP", 200 );
	setGlobalNumber(L, "KB_PGUP", 201 );
	setGlobalNumber(L, "KB_LEFT", 203 );
	setGlobalNumber(L, "KB_RIGHT", 205 );
	setGlobalNumber(L, "KB_END", 207 );
	setGlobalNumber(L, "KB_DOWN", 208 );
	setGlobalNumber(L, "KB_PGDN", 209 );
	setGlobalNumber(L, "KB_INSERT", 210 );
	setGlobalNumber(L, "KB_DELETE", 211 );
	setGlobalNumber(L, "KB_LWINDOWS", 219 );
	setGlobalNumber(L, "KB_RWINDOWS", 220 );
	setGlobalNumber(L, "KB_CONTEXT", 221 );
}

void RegisterLuaBind(lua_State *L)
{
	luabind::open(L);

	luabind::module(L) [

		//enum okreslajacy typ obiektu, uzywany w funkcji toPointer()

		luabind::class_<detail::unspecified>("ObjType")
			.enum_("_objType")
			[
				value("player", 0),
				value("chr", 1), //dziala dla ChrBase i Chr
				value("item", 2),
				value("trigger", 3)
			],


		
		
					luabind::class_<int>("ValueType")
							.enum_("_valueType")
							[
								value("Int", 0),
								value("String", 1),
								value("Bool", 2)
							],
				
		


			def("toPointer", &SF_toPointer),	//funkcja konwertujaca obiekt na wskaznik
			def("setObjective", &SF_setObjective),
			def("clearObjective", &SF_clearObjective),
			def("getChrPtrID", &chrPtrID),

			luabind::class_<ObjManager>("_ObjManager")
			.def("createChr", &ObjManager::createChr)
			.def("createItem", &ObjManager::createItem)
			.def("getChr", &ObjManager::getChr)
			.def("getTrigger", &ObjManager::getTrigger)
			.def("chrToBase", &ObjManager::chrToBase),

			//ustawienie klasy ChrBase, odpowiednik Chr
			luabind::class_<Chr>("ChrBase")
			.def("actionBreak", &Chr::actionBreak)
			.def_readwrite("health", &Chr::health)
			.def_readwrite("mana", &Chr::mana),

			//ustawienie klasy Chr, odpowiednika AI
			luabind::class_<AI, Chr>("Chr")
			.def("attack", &AI::SF_Attack),	//TODO:PRZETESTOWAC!!!!!!!!!!!!!!!!!!!!!


			
			luabind::class_<Item>("Item")
						.def_readwrite("type", &Item::type)
						.def_readwrite("power", &Item::power),
			


			luabind::class_<Trigger>("Trigger")
			.def_readwrite("enabled", &Trigger::enabled),

			//ustawienie klasy player, definiowanie wlasciwosci
			luabind::class_<Player, Chr>("_Player")
			//funkcje, metody

			//wszystkie properties
			.def_readwrite("gold", &Player::gold)
			.def_readwrite("exp", &Player::exp)
			.def_readonly("level", &Player::lvl) //level mozna zmienic tylko zmieniajac exp
			.def_readwrite("wood", &Player::wood)
			.def_readwrite("stone", &Player::stone)
			.def_readwrite("iron", &Player::iron)
	];

	luabind::globals(L)["ObjManager"] = &objManager; //ustawienie managera obiektow
	luabind::globals(L)["PI"] = PI;

}
void RegisterWorldGlobals(lua_State *L)
{
	if(Players.elms())
		luabind::globals(L)["Player"] = &Players[0]; //ustawienie obiektu globalnego Player
}