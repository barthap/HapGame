#include <windows.h>




void TransFormToChild(void     *Ptr,void *Parent)
{
	 
	HWND hWnd = (HWND) Ptr;
	HWND hWndParent = (HWND) Parent;
	SetWindowLong(hWnd,GWL_EXSTYLE,WS_EX_LEFT|WS_EX_NOPARENTNOTIFY);
	SetWindowLong(hWnd,GWL_STYLE,WS_CHILD|WS_VISIBLE);
	SetParent(hWnd,hWndParent);
	//SetWindowPos(hWnd,HWND_TOP,0,0,640,480,SWP_SHOWWINDOW);
}


