#include "stdafx.h"
#include "Main.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#undef T
#undef C
#undef T2
#undef T1
#include "Luabind/luabind.hpp"
using namespace luabind;
#define T1(a    )   template<typename a                        > // 1 type  template
#define T2(a,b  )   template<typename a, typename b            > // 2 types template
#define T        (*this)
#define C        const




LuaClass::LuaClass()
{
	this->L = lua_open();
	luaL_openlibs(L);
}

LuaClass::~LuaClass()
{
	lua_close(L);
}

void LuaClass::RegisterFunctions()
{
	SF_registerAllFunctions(L);
}


void LuaClass::Run( Str path )
{
	char script[128];
	
	if(path.length() > 128)Exit(S+"Œcie¿ka jest za d³uga, maxymalna wartoœæ 128 znaków\n"+path);

	Set(script,path);
	luaL_dofile(L, script);
	delete [] script;
}

void LuaClass::SelectGlobal( Str name )
{
	char global[64];
	Set(global, name);
	lua_getglobal(L, global);
}

void LuaClass::Call( int numberOfArguments /*= 0*/, int numberOfReturns /*= 0*/ )
{
	lua_call(L, numberOfArguments, numberOfReturns);
}
