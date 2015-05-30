#include "Scene1.h"
#include "Input/directinput.h"
#include "Entity2D\Sprite.h"
#include "testGame.h"
#include <string>
#include <sstream>
using namespace Erio;

Scene1::Scene1(Game *game)
{
	fileName = "scene1.xml";
	_name = "scene1";
	_game = game;
}

Scene1::~Scene1()
{
	delete _quad1;
	_quad1 = NULL;

	delete _quad2;
	_quad2 = NULL;

	delete _sprite1;
	_sprite1 = NULL;

	delete _sprite2;
	_sprite2 = NULL;
}

bool Scene1::Init(Inaba::Renderer& renderer)
{
	getEntity(&_sprite1, "player1");
	getEntity(&_sprite2, "player2");
	getEntity(&_quad1, "floor");
	getEntity(&_quad2, "wall");
	return true;
}
bool Scene1::Frame(Inaba::Renderer& renderer, Inaba::DirectInput& directInput, Inaba::Timer& timer)
{
	static float fSp = 1.0f;
	static float vSpd = 10.0f;
	std::string number;
	std::stringstream strstream;
	strstream << directInput.mouseRelPosX();
	strstream >> number;
	renderer.getFont()->Print((char*)("mouse X: " + number).c_str(), 0, 0, D3DCOLOR_XRGB(255, 255, 255), NULL, 200, 0, Inaba::FA_LEFT);
	
	renderer.getCamera()->CameraControl(directInput);

	// ------------------------------------------------
	//				SPRITE CONTROL
	// ------------------------------------------------
	if (directInput.keyDown(Inaba::Input::KEY_SUBTRACT))
		_sprite1->setPos(_sprite1->posX(), _sprite1->posY(), _sprite1->posZ() - vSpd);
	if (directInput.keyDown(Inaba::Input::KEY_ADD))
		_sprite1->setPos(_sprite1->posX(), _sprite1->posY(), _sprite1->posZ() + vSpd);

	if (directInput.keyDown(Inaba::Input::KEY_UP))
		_sprite1->setPos(_sprite1->posX(), _sprite1->posY() + fSp);
	else if (directInput.keyDown(Inaba::Input::KEY_DOWN))
		_sprite1->setPos(_sprite1->posX(), _sprite1->posY() - fSp);
	else if (directInput.keyDown(Inaba::Input::KEY_LEFT))
		_sprite1->setPos(_sprite1->posX() - fSp, _sprite1->posY());
	else if (directInput.keyDown(Inaba::Input::KEY_RIGHT))
		_sprite1->setPos(_sprite1->posX() + fSp, _sprite1->posY());
	// ------------------------------------------------

	//COLISION
	if (_sprite1->checkCollision(*_quad1) != Inaba::Entity2D::NoCollision)
		_sprite1->returnToPos(_sprite1->prevPosX(), _sprite1->prevPosY());
	if (_sprite1->checkCollision(*_quad2) != Inaba::Entity2D::NoCollision)
		_sprite1->returnToPos(_sprite1->prevPosX(), _sprite1->prevPosY());

	if (directInput.keyDown(Inaba::Input::KEY_P))
		_game->setScene("scene3D", &renderer);

	return true;
}