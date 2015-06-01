#ifndef SCENE3D_H
#define SCENE3D_H

#include "Game.h"
#include "Renderer/RenderTypes.h"
#include "Entity3D/Mesh.h"
#include "Renderer/Renderer.h"
#include "Timer/timer.h"
#include "Scene/Import.h"
#include "Scene/Scene.h"

namespace Erio
{
	class Game;

	class Scene3D : public Inaba::Scene
	{
		public:
			Scene3D(Game*);
			~Scene3D();
			bool Init(Inaba::Renderer&);
			bool Frame(Inaba::Renderer&,Inaba::DirectInput&, Inaba::Timer&);
		private:		
			Inaba::Node *_NodeBones;
	};

}
#endif