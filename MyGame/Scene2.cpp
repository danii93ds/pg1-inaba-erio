#include "Scene2.h"
#include "Input/directinput.h"
#include "Entity2D\Sprite.h"
#include "testGame.h"
using namespace Erio;

Scene2::Scene2(Game *game)
{
	fileName = "scene2.xml";
	_name = "scene2";
	_game = game;
}
bool Scene2::Init(Inaba::Renderer& renderer)
{
	getEntity(&_sprite1, "player1");
	getEntity(&_quad1, "floor");
	getEntity(&_quad2, "wall");
	return true;
}
bool Scene2::Frame(Inaba::Renderer& renderer, Inaba::DirectInput& directInput, Inaba::Timer& timer)
{	
	 static float fSp = 1.0f;
	 if(directInput.keyDown(Inaba::Input::KEY_UP))
	  _quad1->setPos(_quad1->posX(), _quad1->posY() + fSp);
	 else if(directInput.keyDown(Inaba::Input::KEY_DOWN))
	  _quad1->setPos(_quad1->posX(), _quad1->posY() - fSp);
	 else if(directInput.keyDown(Inaba::Input::KEY_LEFT))
	  _quad1->setPos(_quad1->posX() - fSp, _quad1->posY());
	 else if(directInput.keyDown(Inaba::Input::KEY_RIGHT))
	  _quad1->setPos(_quad1->posX() + fSp, _quad1->posY());

	 if(directInput.keyDown(Inaba::Input::KEY_P))
		_game->setScene("scene3D", &renderer);


	return true;
}