#include "Scene3D.h"
#include "Input/directinput.h"
#include "testGame.h"
#include <string>
#include <sstream>
#include <Entity3D\Mesh.h>
#include <Entity3D\Node.h>

using namespace Erio;

Inaba::TextureCoordVertex g_8Vertices[] = {
{-0.5f, 0.5f, -0.5f,0,0}, // 0 
{0.5f, 0.5f, -0.5f, 0,0}, // 1 
{ 0.5f, 0.5f, 0.5f, 0,0}, // 2 
{ -0.5f, 0.5f, 0.5f,0,0}, // 3

{ -0.5f, -0.5f, 0.5f, 0,0}, // 4
{  0.5f, -0.5f, 0.5f, 0,0}, // 5
{  0.5f, -0.5f,-0.5f, 0,0}, // 6
{ -0.5f, -0.5f,-0.5f, 0,0} // 7
};

USHORT g_indices[] = { 0, 1, 2, 0, 2, 3,
                       4, 5, 6, 4, 6, 7,
                       3, 2, 5, 3, 5, 4,
                       2, 1, 6, 2, 6, 5,
                       1, 7, 6, 1, 0, 7,
                       0, 3, 4, 0, 4, 7};


Scene3D::Scene3D(Game *game)
{
	fileName = "scene3D.xml";
	_name = "scene3D";
	_game = game;
}

Scene3D::~Scene3D()
{

}

bool Scene3D::Init(Inaba::Renderer& renderer)
{   

	_Node = new Inaba::Node();
	
	Inaba::Import3D::GetInstance()->importScene("tiny.x",*this, *_Node);

	_Node->setScale(0.2f,0.2f,0.2f);
	_Node->setPos(0,0,100);
	_Node->setRotation(-90,0,135);
	
	_Node->playAnimation("");

	return true;
}

bool Scene3D::Frame(Inaba::Renderer& renderer, Inaba::DirectInput& directInput, Inaba::Timer& timer)
{
	static float fSp = 1.0f;
	static float vSpd = 10.0f;
	std::string number;
	std::stringstream strstream;
	strstream << directInput.mouseRelPosX()/10;
	strstream >> number;
	//renderer.getFont()->Print((char*)("mouse X: " + number).c_str(), 0, 0, D3DCOLOR_XRGB(255, 255, 255), NULL, 200, 0, Inaba::FA_LEFT);
	//_entities3D[_entities3D.size() - 1]->setPos(_entities3D[_entities3D.size() - 1]->posX() + 0.5,_entities3D[_entities3D.size() - 1]->posY(),_entities3D[_entities3D.size() - 1]->posZ());
	renderer.getCamera()->CameraControl(directInput,0.1f * timer.timeBetweenFrames() );
	
	std::stringstream ss;
	ss << "Meshes: " << Inaba::Mesh::DrawnMeshes;
	std::string msg = ss.str();

	renderer.getFont()->Print( msg.c_str(), 0, 30, D3DCOLOR_XRGB(255, 255, 255), NULL, 200, 0, Inaba::FA_LEFT);

	//_mesh1->Draw(renderer);

	if(directInput.keyDown(Inaba::Input::KEY_P))
		_game->setScene("scene1", &renderer);

	//if (directInput.keyDown(Inaba::Input::KEY_1))
	//		_Node->playAnimation();

	return true;
}