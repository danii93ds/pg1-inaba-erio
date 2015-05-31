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
_prevPosX(0.0f),
_prevPosY(0.0f),
_prevPosZ(0.0f),
_rotX(0.0f),
_rotY(0.0f),
_rotZ(0.0f),
_scaleX(1.0f),
_scaleY(1.0f),
_scaleZ(1.0f),
_AABB(new AABB()),
_transformationMatrix(new D3DXMATRIX()),
_worldTransformationMatrix(new D3DXMATRIX()),
_parent(NULL)
{
	D3DXMatrixIdentity(_transformationMatrix);
	D3DXMatrixIdentity(_worldTransformationMatrix);
	UpdateLocalTransformation();
	
}

Entity3D::~Entity3D()
{
	delete _transformationMatrix;
	_transformationMatrix = NULL;
}

void Entity3D::setPos(float posX, float posY, float posZ)
{
	_prevPosX = _posX;
	_prevPosY = _posY;
	_prevPosZ = _posZ;
	_posX = posX;
	_posY = posY;
	_posZ = posZ;

	

	//UpdateLocalTransformation();
}

void Entity3D::setRotation(float rotX,float rotY,float rotZ)
{
	_rotX = rotX;
	_rotY = rotY;
	_rotZ = rotZ;

	

	//UpdateLocalTransformation();
}

void Entity3D::setScale(float scaleX, float scaleY, float scaleZ)
{
	_scaleX = scaleX;
	_scaleY = scaleY;
	_scaleZ = scaleZ;

	//UpdateLocalTransformation();
}

void Entity3D::UpdateLocalTransformation()
{
 D3DXMATRIX translateMatrix;
 D3DXMatrixTranslation(&translateMatrix,_posX, _posY,_posZ);

 D3DXMATRIX rotationMatrixX;
 D3DXMATRIX rotationMatrixY;
 D3DXMATRIX rotationMatrixZ;
  
 D3DXMatrixRotationX(&rotationMatrixX,_rotX);
 D3DXMatrixRotationY(&rotationMatrixY,_rotY);
 D3DXMatrixRotationZ(&rotationMatrixZ, _rotZ);

 D3DXMATRIX scaleMatrix;
 D3DXMatrixScaling(&scaleMatrix, _scaleX, _scaleY, _scaleZ);

 D3DXMatrixIdentity(_transformationMatrix);
 D3DXMatrixMultiply(_transformationMatrix,&translateMatrix,_transformationMatrix);
 D3DXMatrixMultiply(_transformationMatrix,&rotationMatrixX,_transformationMatrix);
 D3DXMatrixMultiply(_transformationMatrix,&rotationMatrixY,_transformationMatrix);
 D3DXMatrixMultiply(_transformationMatrix,&rotationMatrixZ,_transformationMatrix);
 D3DXMatrixMultiply(_transformationMatrix,&scaleMatrix,_transformationMatrix);

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

float Entity3D::prevPosX() const
{
	return _prevPosX;
}

float Entity3D::prevPosY() const
{
	return _prevPosY;
}

float Entity3D::prevPosZ() const
{
	return _prevPosZ;
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


Entity3D::CollisionResult Entity3D::checkCollision(Entity3D& rkEntity3D) const
{
 float fOverlapX = std::max(0.0f, 
        std::min( posX() + fabs( scaleX() ) / 2.0f,rkEntity3D.posX() + fabs( rkEntity3D.scaleX() ) / 2.0f) -  
        std::max( posX() - fabs( scaleX() ) / 2.0f,rkEntity3D.posX() - fabs( rkEntity3D.scaleX() ) / 2.0f)
 );
 float fOverlapY = std::max(0.0f, 
        std::min( posY() + fabs( scaleY() ) / 2.0f,  rkEntity3D.posY() + fabs( rkEntity3D.scaleY() ) / 2.0f) -  
        std::max( posY() - fabs( scaleY() ) / 2.0f, rkEntity3D.posY() - fabs( rkEntity3D.scaleY() ) / 2.0f)
 );

 // COLLISION Z LO HACEMOS OTRO DIA


 if(fOverlapX != 0.0f && fOverlapY != 0.0f){
  if(fOverlapX > fOverlapY){
   return CollisionVertical;
  }else{
   return CollisionHorizontal;
  }
 }

 return NoCollision;
}

void Entity3D::drawAABB(Renderer& pkRenderer) const{
      /*static Mesh* s_AKAABBMesh;
        static bool s_bIsInitialized = false;
        if(!s_bIsInitialized){
                s_bIsInitialized = true;
                s_AKAABBMesh = new Mesh(pkRenderer);
                Inaba::TextureCoordVertex * pakVertices = new Inaba::TextureCoordVertex[8];
                unsigned short* pausIndices = new unsigned short[24];

                pakVertices[0].x = +0.5f;       pakVertices[0].y = +0.5f;       pakVertices[0].z = +0.5f;
		pakVertices[1].x = +0.5f;       pakVertices[1].y = +0.5f;       pakVertices[1].z = -0.5f;
        pakVertices[2].x = +0.5f;       pakVertices[2].y = -0.5f;       pakVertices[2].z = +0.5f;
        pakVertices[3].x = +0.5f;       pakVertices[3].y = -0.5f;       pakVertices[3].z = -0.5f;
        pakVertices[4].x = -0.5f;       pakVertices[4].y = +0.5f;       pakVertices[4].z = +0.5f;
        pakVertices[5].x = -0.5f;       pakVertices[5].y = +0.5f;       pakVertices[5].z = -0.5f;
        pakVertices[6].x = -0.5f;       pakVertices[6].y = -0.5f;       pakVertices[6].z = +0.5f;
        pakVertices[7].x = -0.5f;       pakVertices[7].y = -0.5f;       pakVertices[7].z = -0.5f;

        pausIndices[0] =        0;                      pausIndices[1] =        1;                      pausIndices[2] =        1;              pausIndices[3] = 3;
        pausIndices[4] =        3;                      pausIndices[5] =        2;                      pausIndices[6] =        2;              pausIndices[7] = 0;
        pausIndices[8] =        4;                      pausIndices[9] =        5;                      pausIndices[10] =       5;              pausIndices[11] = 7;
        pausIndices[12] =       7;                      pausIndices[13] =       6;                      pausIndices[14] =       6;              pausIndices[15] = 4;
        pausIndices[16] =       0;                      pausIndices[17] =       4;                      pausIndices[18] =       1;              pausIndices[19] = 5;
        pausIndices[20] =       3;                      pausIndices[21] =       7;                      pausIndices[22] =       2;              pausIndices[23] = 6;

                s_AKAABBMesh->setData(pakVertices,8,Inaba::LineStrip,pausIndices,24);

                delete[] pakVertices;
                delete[] pausIndices;
        }
                                                  
		s_AKAABBMesh->setPos(_AABB->getCenter().x + _worldTransformationMatrix->_41, _AABB->getCenter().y + _worldTransformationMatrix->_42 ,_AABB->getCenter().z+ _worldTransformationMatrix->_43);
        //s_AKAABBMesh->setPos(0,0,0);
        
        //s_AKAABBMesh->setPos(aabb().offset()->x + posX() , aabb().offset()->y + posY() , aabb().offset()->z + posZ() );
        
		s_AKAABBMesh->setScale(_AABB->getWidth()  ,_AABB->getHeight() ,_AABB->getDepth() );
		s_AKAABBMesh->UpdateTransformation();
        s_AKAABBMesh->Draw(pkRenderer);
		*/
}

void Entity3D::returnToPos(float posX, float posY, float posZ)
{
	_posX = posX;
	_posY = posY;
	_posZ = posZ;

	//UpdateLocalTransformation();
}

std::string Entity3D::name() const
{
	return _name;
}

void Entity3D::setName(std::string name)
{
	_name = name;
}

void Entity3D::UpdateTransformation()
{
	UpdateLocalTransformation();

	if(!_parent)
	{
		_worldTransformationMatrix = _transformationMatrix;
	}else
	{
		D3DXMatrixMultiply(_worldTransformationMatrix, (_parent->GetTranformationMatrix()), _transformationMatrix);
	}
	
}

/*void Entity3D::drawAABB(Renderer& renderer) const{
        static Mesh* s_AKAABBMesh;
        static bool s_bIsInitialized = false;
        if(!s_bIsInitialized){
                s_bIsInitialized = true;
                s_AKAABBMesh = new Mesh(renderer);
                Inaba::TextureCoordVertex * pakVertices = new Inaba::TextureCoordVertex[8];
                unsigned short* pausIndices = new unsigned short[24];

                pakVertices[0].x = +0.5f;       pakVertices[0].y = +0.5f;       pakVertices[0].z = +0.5f;
        pakVertices[1].x = +0.5f;       pakVertices[1].y = +0.5f;       pakVertices[1].z = -0.5f;
        pakVertices[2].x = +0.5f;       pakVertices[2].y = -0.5f;       pakVertices[2].z = +0.5f;
        pakVertices[3].x = +0.5f;       pakVertices[3].y = -0.5f;       pakVertices[3].z = -0.5f;
        pakVertices[4].x = -0.5f;       pakVertices[4].y = +0.5f;       pakVertices[4].z = +0.5f;
        pakVertices[5].x = -0.5f;       pakVertices[5].y = +0.5f;       pakVertices[5].z = -0.5f;
        pakVertices[6].x = -0.5f;       pakVertices[6].y = -0.5f;       pakVertices[6].z = +0.5f;
        pakVertices[7].x = -0.5f;       pakVertices[7].y = -0.5f;       pakVertices[7].z = -0.5f;

        pausIndices[0] =        0;                      pausIndices[1] =        1;                      pausIndices[2] =        1;              pausIndices[3] = 3;
        pausIndices[4] =        3;                      pausIndices[5] =        2;                      pausIndices[6] =        2;              pausIndices[7] = 0;
        pausIndices[8] =        4;                      pausIndices[9] =        5;                      pausIndices[10] =       5;              pausIndices[11] = 7;
        pausIndices[12] =       7;                      pausIndices[13] =       6;                      pausIndices[14] =       6;              pausIndices[15] = 4;
        pausIndices[16] =       0;                      pausIndices[17] =       4;                      pausIndices[18] =       1;              pausIndices[19] = 5;
        pausIndices[20] =       3;                      pausIndices[21] =       7;                      pausIndices[22] =       2;              pausIndices[23] = 6;

                s_AKAABBMesh->setData(pakVertices,8,Inaba::LineStrip,pausIndices,24);

                delete[] pakVertices;
                delete[] pausIndices;
        }
        /*const Entity3D* t = this;
        const Node* pk = dynamic_cast<const Node*>(t) ;
        if(pk){
                s_AKAABBMesh->setParent((Node*) this);
        }*/
        
        
        //s_AKAABBMesh->setParent((Node*)this);
		//s_AKAABBMesh->setPos( (_worldTransformationMatrix->_41 + _transformationMatrix->_41) / _AABB->getWidth() + _AABB->getCenter().x / _AABB->getWidth()  ,
                                 //                 (_worldTransformationMatrix->_42 + _transformationMatrix->_42) / _AABB->getWidth() + _AABB->getCenter().y / _AABB->getHeight() , 
								//				  (_worldTransformationMatrix->_43 + _transformationMatrix->_43) / _AABB->getWidth() + _AABB->getCenter().z /  _AABB->getDepth() );
                                                  
        //s_AKAABBMesh->setPos(aabb().offset()->x - _TrMatrix->_41, aabb().offset()->y - _TrMatrix->_42 ,aabb().offset()->z - _TrMatrix->_43);
        //s_AKAABBMesh->setPos(0,0,0);
        
        //s_AKAABBMesh->setPos(aabb().offset()->x + posX() , aabb().offset()->y + posY() , aabb().offset()->z + posZ() );
        
      //  s_AKAABBMesh->setScale(_AABB->getWidth()  ,_AABB->getHeight() ,_AABB->getDepth() );
	//	s_AKAABBMesh->UpdateLocalTransformation();
      //  s_AKAABBMesh->Draw(renderer);

//}


AABB* Entity3D::getAABB()
{
	return _AABB;
}

void Entity3D::SetParent(Node* parent)
{
	_parent = parent;
	
}
Node* Entity3D::GetParent()
{
	return _parent;
}

Matrix Entity3D::GetTranformationMatrix()
{
	return _worldTransformationMatrix;
}

void Entity3D::Update(Timer&)
{
	
}
