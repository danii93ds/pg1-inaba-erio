#include "Window.h"

using namespace Inaba;
 
Window::Window(HINSTANCE hInstance):
_wc(new WNDCLASS),
_hWnd(NULL),
_hInstance(hInstance)
{
}

Window::~Window()
{
	delete _wc;
	_wc = NULL;
}

bool Window::Create(unsigned int w, unsigned int h)
{
	ZeroMemory(_wc, sizeof(*_wc));
	_wc->lpfnWndProc=WndProc;
	_wc->hInstance=_hInstance;
	_wc->hbrBackground=(HBRUSH)(COLOR_BACKGROUND);
	_wc->lpszClassName="myengineWindow";
	
	if(!RegisterClass(_wc)){ return false;}
	
	_hWnd = CreateWindow(_wc->lpszClassName,
						"PG Engine",
						WS_OVERLAPPEDWINDOW|WS_VISIBLE,
						0,0,w+8,h+30,0,0,_hInstance,NULL);
	 if (_hWnd != NULL)
		 return true;
	 return false;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

 switch(message)
 {
   case WM_CLOSE:
     PostQuitMessage(0);
     break;
   default:
     return DefWindowProc(hWnd, message, wParam, lParam);
 }
 return 0;

}

HWND Window::hWnd()
{
	return _hWnd;
}

HINSTANCE Window::gethInstance()
{
	return _hInstance;
}