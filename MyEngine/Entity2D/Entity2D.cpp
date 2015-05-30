#define NOMINMAX

#include "Entity2D.h"
#include "../Renderer/Renderer.h"
#include <Windows.h>
#include <iostream>
#include <d3dx9.h>

using namespace Inaba;

Entity2D::Entity2D():
_posX(0.0f),
_posY(0.0f),
_posZ(0.0f),
_prevPosX(0.0f),
_prevPosY(0.0f),
_prevPosZ(0.0f),
_rotation(0.0f),
_scaleX(1.0f),
_scaleY(1.0f),
_transformationMatrix(new D3DXMATRIX())
{
}

Entity2D::~Entity2D()
{
	delete _transformationMatrix;
	_transformationMatrix = NULL;
}

void Entity2D::setPos(float posX, float posY)
{
	_prevPosX = _posX;
	_prevPosY = _posY;
	_prevPosZ = _posZ;
	_posX = posX;
	_posY = posY;

	UpdateLocalTransformation();
}

void Entity2D::setPos(float posX, float posY, float posZ)
{
	_prevPosX = _posX;
	_prevPosY = _posY;
	_prevPosZ = _posZ;
	_posX = posX;
	_posY = posY;
	_posZ = posZ;

	UpdateLocalTransformation();
}

void Entity2D::setRotation(float rotation)
{
	_rotation = rotation;

	UpdateLocalTransformation();
}

void Entity2D::setScale(float scaleX, float scaleY)
{
	_scaleX = scaleX;
	_scaleY = scaleY;

	UpdateLocalTransformation();
}

void Entity2D::UpdateLocalTransformation()
{
	D3DXMATRIX translateMatrix;
	D3DXMatrixTranslation(&translateMatrix, _posX, _posY, _posZ);

	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationZ(&rotationMatrix, _rotation);

	D3DXMATRIX scaleMatrix;
	D3DXMatrixScaling(&scaleMatrix, _scaleX, _scaleY, 1);

	D3DXMatrixIdentity(_transformationMatrix);
	D3DXMatrixMultiply(_transformationMatrix,&translateMatrix,_transformationMatrix);
	D3DXMatrixMultiply(_transformationMatrix,&rotationMatrix,_transformationMatrix);
	D3DXMatrixMultiply(_transformationMatrix,&scaleMatrix,_transformationMatrix);

}

float Entity2D::posX() const
{
	return _posX;
}

float Entity2D::posY() const
{
	return _posY;
}

float Entity2D::posZ() const
{
	return _posZ;
}

float Entity2D::prevPosX() const
{
	return _prevPosX;
}

float Entity2D::prevPosY() const
{
	return _prevPosY;
}

float Entity2D::scaleX() const
{
	return _scaleX;
}

float Entity2D::scaleY() const
{
	return _scaleY;
}



Entity2D::CollisionResult Entity2D::checkCollision(Entity2D& rkEntity2D) const
{
 float fOverlapX = std::max(0.0f, 
        std::min( posX() + fabs( scaleX() ) / 2.0f,rkEntity2D.posX() + fabs( rkEntity2D.scaleX() ) / 2.0f) -  
        std::max( posX() - fabs( scaleX() ) / 2.0f,rkEntity2D.posX() - fabs( rkEntity2D.scaleX() ) / 2.0f)
 );
 float fOverlapY = std::max(0.0f, 
        std::min( posY() + fabs( scaleY() ) / 2.0f,  rkEntity2D.posY() + fabs( rkEntity2D.scaleY() ) / 2.0f) -  
        std::max( posY() - fabs( scaleY() ) / 2.0f, rkEntity2D.posY() - fabs( rkEntity2D.scaleY() ) / 2.0f)
 );

 if(fOverlapX != 0.0f && fOverlapY != 0.0f){
  if(fOverlapX > fOverlapY){
   return CollisionVertical;
  }else{
   return CollisionHorizontal;
  }
 }

 return NoCollision;
}

void Entity2D::drawAABB(Renderer& rkRenderer) const
{
 static ColorVertex s_akAABBVertices[5];
 static bool s_bIsInitialized = false;
 if(!s_bIsInitialized){
  s_bIsInitialized = true;

  s_akAABBVertices[0].x = -0.5; s_akAABBVertices[0].y = -0.5; s_akAABBVertices[0].z = 0.0; s_akAABBVertices[0].color =Inaba_COLOR_RGB(255,50,50);
  s_akAABBVertices[1].x = -0.5; s_akAABBVertices[1].y = 0.5; s_akAABBVertices[1].z = 0.0; s_akAABBVertices[1].color = Inaba_COLOR_RGB(255,70,70);
  s_akAABBVertices[2].x = 0.5; s_akAABBVertices[2].y = 0.5; s_akAABBVertices[2].z = 0.0; s_akAABBVertices[2].color = Inaba_COLOR_RGB(255,30,30);
  s_akAABBVertices[3].x = 0.5; s_akAABBVertices[3].y = -0.5; s_akAABBVertices[3].z = 0.0; s_akAABBVertices[3].color = Inaba_COLOR_RGB(255,15,15);
  s_akAABBVertices[4].x = -0.5; s_akAABBVertices[4].y = -0.5; s_akAABBVertices[4].z = 0.0; s_akAABBVertices[4].color = Inaba_COLOR_RGB(255,95,90);
 }
 rkRenderer.setCurrentTexture(NoTexture);
 rkRenderer.setMatrix(World, _transformationMatrix );
 rkRenderer.Draw(s_akAABBVertices, Inaba::LineStrip, 5);
}

void Entity2D::returnToPos(float posX, float posY)
{
	_posX = posX;
	_posY = posY;

	UpdateLocalTransformation();
}

std::string Entity2D::name() const
{
	return _name;
}

void Entity2D::setName(std::string name)
{
	_name = name;
}