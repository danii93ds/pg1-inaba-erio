#ifndef GAME_H
#define GAME_H

#include "myengine_api.h"
#include <string>
#include <vector>

namespace Inaba
{

class DirectInput;
class Renderer;
class Timer;
class Scene;
class Renderer;

class MYENGINE_API Game
{
	public:
		Game();
		Game(std::string);
		~Game();
		virtual bool Init(Inaba::Renderer&) = 0;
		virtual void Frame(Inaba::Renderer&, Inaba::DirectInput&, Inaba::Timer&) = 0;
		virtual void DeInit() = 0;
		Scene* currentScene();
		void setScene(std::string, Renderer*);
		void AddScene(Scene*);
	private:
		Scene *_currentScene;
		std::vector<Scene*> _scenes;

};

}
#endif