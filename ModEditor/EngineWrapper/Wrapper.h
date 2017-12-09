#pragma once

namespace Hapex { namespace HapGame { namespace  EditorWrapper
{

public ref class EngineWrapper
{
public:
	EngineWrapper(void);
	bool Init(void* dll_module_instance,void *hwnd,int width,int height);
	void *EngineWrapper::GetHwnd();

	void Kill();
};

}}}