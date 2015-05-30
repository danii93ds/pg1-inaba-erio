#ifndef TESTGAME_H
#define TESTGAME_H

#include "Game.h"
#include "Renderer/RenderTypes.h"
#include "Entity2D/Quad.h"
#include "Renderer/Renderer.h"
#include "Entity2D/Sprite.h"
#include "Timer/timer.h"
#include "Entity2D/Animation.h"
#include "Scene/Import.h"
#include "Scene/Scene.h"
#include "Scene1.h"
#include "Entity3D\Mesh.h"
#include "Scene\Import3D.h"

namespace Erio
{
	class Game : public Inaba::Game
	{
	public:
		Game();
		bool Init(Inaba::Renderer&);
		void Frame(Inaba::Renderer&,Inaba::DirectInput&, Inaba::Timer&);
		void DeInit();

	private: 
		Inaba::Scene *scene1;
		Inaba::Scene *scene2;
		Inaba::Scene *scene3D;
	};
}
#endif