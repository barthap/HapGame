#include "Wrapper.h"

#include <EsenthelEngine/EsenthelEngine.h>

extern Ptr	mHwndMain;

extern bool toKill;

namespace Hapex { namespace HapGame { namespace  EditorWrapper
{
	

EngineWrapper::EngineWrapper(void)
{
}


bool EngineWrapper::Init(void* dll_module_instance,void *hwnd,int width,int height)
{
		mHwndMain=(Ptr)hwnd;
		 
	   StartEEManually((Ptr)dll_module_instance);


	   return(true);
}


void *EngineWrapper::GetHwnd()
{
	return(App.hwnd());
}

void EngineWrapper::Kill()
{
	toKill = true;
}

}}}//namespace