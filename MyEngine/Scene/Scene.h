#ifndef SCENE_H
#define SCENE_H

#include "../myengine_api.h"
#include <iostream>
#include <vector>
#include <string>

namespace Inaba
{
	class Renderer;
	class Entity2D;
	class Entity3D;
	class Mesh;
	class Quad;
	class Sprite;
	class Animation;
	class Timer;
	class Game;
	class DirectInput;

	class MYENGINE_API Scene
	{
		protected:
			std::vector<Entity2D*> _entities;
			std::vector<Entity3D*> _entities3D;
			std::string _name;
			Game *_game;
		
		public:
			Scene();
			virtual bool Init(Renderer&) = 0;
			virtual bool Frame(Renderer&,DirectInput&,Timer&)= 0;
			bool deInit();
			bool Draw(Renderer&,Timer&);
			bool CheckDraw(Renderer&,Entity3D&);
			bool getEntity(Sprite** ent ,std::string name);
			bool getEntity(Quad** ent ,std::string name);
			bool getEntity(Mesh** ent ,std::string name);
			void AddEntity(Entity2D*);
			void AddEntity(Entity3D*);
			void setName(std::string);
			std::string Name(){return _name;}
			std::string fileName;
	};
}




#endif