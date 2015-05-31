#define NOMINMAX

#include "Entity3D.h"
#include "Node.h"
#include "../Renderer/Renderer.h"
#include <Windows.h>
#include <iostream>
#include <d3dx9.h>

using namespace Inaba;

Entity3D::Entity3D():
_posX(0.0f),
_posY(0.0f),
_posZ(0.0f),
_rotX(0.0f),
_rotY(0.0f),
_rotZ(0.0f),
_scaleX(1.0f),
_scaleY(1.0f),
_scaleZ(1.0f),
_parent(NULL),
_AABB(new AABB())
{
}

Entity3D::~Entity3D()
{

}

void Entity3D::setPos(float posX, float posY, float posZ)
{
	_posX = posX;
	_posY = posY;
	_posZ = posZ;
}

void Entity3D::setRotation(float rotX,float rotY,float rotZ)
{
	_rotX = rotX;
	_rotY = rotY;
	_rotZ = rotZ;
}

void Entity3D::setScale(float scaleX, float scaleY, float scaleZ)
{
	_scaleX = scaleX;
	_scaleY = scaleY;
	_scaleZ = scaleZ;
}


float Entity3D::posX() const
{
	return _posX;
}

float Entity3D::posY() const
{
	return _posY;
}

float Entity3D::posZ() const
{
	return _posZ;
}

float Entity3D::scaleX() const
{
	return _scaleX;
}

float Entity3D::scaleY() const
{
	return _scaleY;
}

float Entity3D::scaleZ() const
{
	return _scaleZ;
}

float Entity3D::rotX() const
{
	return _rotX;
}

float Entity3D::rotY() const
{
	return _rotY;
}

float Entity3D::rotZ() const
{
	return _rotZ;
}

std::string Entity3D::name() const
{
	return _name;
}

void Entity3D::setName(std::string name)
{
	_name = name;
}

void Entity3D::SetParent(Node* parent)
{
	_parent = parent;
	
}
Node* Entity3D::GetParent()
{
	return _parent;
}

void Entity3D::Update(Timer&)
{
	
}
