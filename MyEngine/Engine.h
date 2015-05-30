#ifndef ENGINE_H
#define ENGINE_H

#include <windows.h>
#include "myengine_api.h"

namespace Inaba
{
	class DirectInput;
	class Renderer;
	class Window;
	class Game;
	class Timer;
	class MYENGINE_API Engine
	{
	private:
		Window *_window;
		Renderer *_renderer;
		DirectInput *_directInput;
		//Game *_game;
		int _width;
		int _height;
	public:
		Engine(HINSTANCE, int, int);
		~Engine();
		bool Init();
		Renderer* getRenderer() { return _renderer;}
		void Run();
		Game *_game;
		Timer * _timer;
	};

}
#endif