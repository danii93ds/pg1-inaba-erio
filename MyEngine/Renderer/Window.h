#ifndef WINDOW_H
#define WINDOW_H
#include <windows.h>

namespace Inaba
{

class Window
{
	private:
		WNDCLASS *_wc;
		HWND _hWnd;
		HINSTANCE _hInstance;
	public:
		Window(HINSTANCE);
		~Window();
		bool Create(unsigned int, unsigned int);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		HWND hWnd();
		HINSTANCE gethInstance();
};

}
#endif