#include "Game.h"
#include "Scene/Scene.h"
#include "Scene/Import.h"
#include "Scene\Import3D.h"
#include "Renderer\Renderer.h"
using namespace Inaba;

Game::Game()
:_currentScene(NULL)
{
}

Game::Game(std::string sceneName)
:_currentScene(NULL)
{
	// otras cosas
	//setScene(sceneName,);
}

Game::~Game()
 {
	delete _currentScene;
	_currentScene = NULL;

	/*for(int i=0; i < _scenes.size(); i ++)
	{
		delete _scenes[i];
		_scenes[i] = NULL;
	}*/	
	_scenes.clear();
}

Scene* Game::currentScene()
{
	return _currentScene;
}

void Game::setScene(std::string sceneName, Renderer* renderer)
{
	if(_currentScene)
	{
		_currentScene->deInit(); //LIMPIO LISTA DE ENTIDADES
		renderer->deInit(); //BORRO TEXTURAS
	}

	if(_scenes.empty())
		return;
		
	for(int i = 0; i < _scenes.size(); i++)
	{
		if(_scenes[i]->Name() == sceneName)
		{
			_currentScene = _scenes[i];
			break;
		}
	}

	if(!_currentScene) // SI NO LO ENCONTRO POR NOMBRE
		return;

	Import::GetInstance()->importScene(*_currentScene);
	_currentScene->Init(*renderer);
}

void Game::AddScene(Scene* scene)
{
	_scenes.push_back(scene);
}
