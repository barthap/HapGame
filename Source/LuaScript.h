#pragma once



class LuaClass
{
   // void define_funcs();

protected:
    lua_State* L;
    
public:
	LuaClass();
	~LuaClass();

	lua_State* Pointer() {return L;}

	void RegisterFunctions();
	void RegisterCustomFunction(){} //not implemented, use lua_register() here

	void Run(Str path);

	void SelectGlobal(Str name);
	void Call(int numberOfArguments = 0, int numberOfReturns = 0);
};
