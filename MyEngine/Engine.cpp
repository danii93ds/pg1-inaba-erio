#include "Engine.h"
#include "Renderer/Renderer.h"
#include "Renderer/Window.h"
#include "Game.h"
#include "Input/directinput.h"
#include "Timer/timer.h"
#include "Scene/Import.h"
#include "Scene/Import3D.h"
#include "Scene/Scene.h"
#include "Entity3D/Mesh.h"

using namespace Inaba;
 
Engine::Engine(HINSTANCE hInstance, int width, int height):
_window(new Window(hInstance)),
_renderer(new Renderer),
_directInput(new DirectInput),
_game(NULL),
_width(width),
_height(height),
_timer(new Timer())
{
}
Engine::~Engine()
{
	delete _window;
	_window = NULL;

	delete _renderer;
	_renderer = NULL;
	
	delete _game;
	_game = NULL;
	
	delete _directInput;
	_directInput = NULL;

	delete _timer;
	_timer = NULL;
}
bool Engine::Init()
{
	if(_window->Create(_width,_height) == TRUE 
	   && _renderer->Init(_window->hWnd()) == TRUE
	   && _directInput->init(_window->gethInstance(),_window->hWnd()) == TRUE)
	{
		Import::GetInstance()->setRenderer(_renderer);
		Import3D::GetInstance()->setRenderer(_renderer);
		return true;
	}
	return false;
}
void Engine::Run()
{
	bool GameOn = true;
	MSG msg;

	
	if(!_game)
		return;
	if(!_game->Init(*_renderer))
		return;

	
	
	_timer->firstMeasure();
	while(GameOn)
	{
		
		if (_renderer != NULL || _game!= NULL)
		{
			_timer->measure();
			_directInput->reacquire();
			_renderer->BeginFrame();
			_game->Frame(*_renderer, *_directInput,*_timer);
			Mesh::DrawnMeshes = 0;
			_game->currentScene()->Draw(*_renderer,*_timer);
			_renderer->UpdateCamera();
			_renderer->EndFrame();		

		}

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)
			GameOn = false;
	}
}