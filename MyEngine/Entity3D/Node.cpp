#include "Node.h"
#include "../Renderer/Renderer.h"
#include <d3dx9.h>
using namespace Inaba;

Node::Node():
Entity3D(),
_worldTransformMatrix(new D3DXMATRIX),
_AssimpTransformMatrix(new D3DXMATRIX),
_bone(NULL),
_currentAnimation(NULL)
{
	D3DXMatrixIdentity(_worldTransformMatrix);
	D3DXMatrixIdentity(_AssimpTransformMatrix);
	iKeyFrame = 0;
	meshes.clear();
	childs.clear();
}

Node::~Node()
{
	while(!childs.empty())
	{
		Entity3D* child = childs.back();
		childs.pop_back();

		delete child;
		child = NULL;
	}

	while(!meshes.empty())
	{
		Mesh* mesh = meshes.back();
		meshes.pop_back();

		delete mesh;
		mesh = NULL;
	}

	while(!animations.empty())
	{
		Animation3D* anim = animations.back();
		animations.pop_back();

		delete anim;
		anim = NULL;
	}

	_currentAnimation = NULL;

	delete _worldTransformMatrix;
	_worldTransformMatrix = NULL;

	delete _AssimpTransformMatrix;
	_AssimpTransformMatrix = NULL;
}

void Node::AddChild(Node* child)
{
	childs.push_back(child);
}

void Node::RemoveChild(Node* child)
{
	childs.remove(child);
}

void Node::AddMesh(Mesh* child)
{
	meshes.push_back(child);
}

void Node::RemoveMesh(Mesh* child)
{
	meshes.remove(child);
}

void Node::AddAnimation(Animation3D* animation)
{
	animations.push_back(animation);
}

void Node::RemoveAnimation(Animation3D* animation)
{
	animations.remove(animation);
}

std::list<Node*> Node::getChilds(){
	return childs;
}

std::list<Mesh*> Node::getMeshes(){
	return meshes;
}

void Node::UpdateTransformation(Matrix transformation, Renderer *renderer)
{
	D3DXMATRIX translation;
	D3DXMATRIX scale;
	D3DXMATRIX rotationX;
	D3DXMATRIX rotationY;
	D3DXMATRIX rotationZ;

	D3DXMatrixTranslation(&translation, _posX,_posY,_posZ);
	D3DXMatrixScaling(&scale,_scaleX,_scaleY,_scaleZ);	

	//TODO RADIANES
	D3DXMatrixRotationX(&rotationX,_rotX);
	D3DXMatrixRotationY(&rotationY,_rotY);
	D3DXMatrixRotationZ(&rotationZ,_rotZ);

	D3DXMATRIX _localtransformation;

	D3DXMatrixIdentity(&_localtransformation);
	D3DXMatrixMultiply(&_localtransformation,&translation,&_localtransformation);
	D3DXMatrixMultiply(&_localtransformation,&rotationX,&_localtransformation);
	D3DXMatrixMultiply(&_localtransformation,&rotationY,&_localtransformation);
	D3DXMatrixMultiply(&_localtransformation,&rotationZ,&_localtransformation);
	D3DXMatrixMultiply(&_localtransformation,&scale,&_localtransformation);

	if (_currentAnimation != NULL && _currentAnimation->getState() == Animation3D::PLAY && iKeyFrame != -1)
	{
		D3DXMATRIX animTransform = _currentAnimation->getTransformationMatrix(iKeyFrame);

		D3DXMatrixMultiply(&_localtransformation, &animTransform, &_localtransformation);

	}
	else
	{
		D3DXMatrixMultiply(&_localtransformation,&_localtransformation,_AssimpTransformMatrix);
	}

	D3DXMatrixMultiply(_worldTransformMatrix,&_localtransformation,transformation);

	if(_bone != NULL){
		_bone->setTransformationMatrix(*_worldTransformMatrix);
	}

	if(childs.size())
	{
		std::list<Node*>::iterator iter;
		for(iter = childs.begin(); iter != childs.end(); iter++)
		{
			(*iter)->UpdateTransformation(_worldTransformMatrix,renderer);
		}
	}

}

void Node::SetFirstTransform(float a1,float a2,float a3, float a4, 
							   float b1, float b2,float b3, float b4,
							   float c1,float c2,float c3,float c4,
							   float d1,float d2, float d3, float d4)
{
 _AssimpTransformMatrix->_11 = a1;
 _AssimpTransformMatrix->_12 = a2;
 _AssimpTransformMatrix->_13 = a3;
 _AssimpTransformMatrix->_14 = a4;
					   
 _AssimpTransformMatrix->_21 = b1;
 _AssimpTransformMatrix->_22 = b2;
 _AssimpTransformMatrix->_23 = b3;
 _AssimpTransformMatrix->_24 = b4;
					   
 _AssimpTransformMatrix->_31 = c1;
 _AssimpTransformMatrix->_32 = c2;
 _AssimpTransformMatrix->_33 = c3;
 _AssimpTransformMatrix->_34 = c4;
					   
 _AssimpTransformMatrix->_41 = d1;
 _AssimpTransformMatrix->_42 = d2;
 _AssimpTransformMatrix->_43 = d3;
 _AssimpTransformMatrix->_44 = d4;

}

void Node::Update(Timer& rkTimer)
{
	if (_currentAnimation != NULL)
		_currentAnimation->Update(rkTimer);
}

void Node::Draw(Renderer& r)
{
	D3DXMatrixIdentity(_worldTransformMatrix);
	r.setMatrix(World, _worldTransformMatrix);
	//TODO FLUSH TUM
	UpdateTransformation(_worldTransformMatrix,&r);

	NodeDraw(&r);
}

void Node::NodeDraw(Renderer* renderer)
{
	if (_bone == NULL) 
	{
		std::list<Mesh*>::iterator iter;
		for(iter = meshes.begin(); iter != meshes.end(); iter++)
		{
			if ((*iter)->getBones().empty())
			{
				renderer->setMatrix(World, _worldTransformMatrix);
				(*iter)->Draw(*renderer);
			}
			else
			{
				(*iter)->drawAnimation(*renderer);
			}
		}

		std::list<Node*>::iterator iter2;
		for(iter2 = childs.begin(); iter2 != childs.end(); iter2++)
		{
			(*iter2)->NodeDraw(renderer);
		}
	}
	
}

void Node::setBone(Bones* tBone)
{
	_bone = tBone;
}

Bones* Node::nodeBone()
{
	return _bone;
}

void Node::setAnimation(Animation3D* animation)
{
	this->_currentAnimation = animation;

	iKeyFrame = _currentAnimation->getIndexFrameKey(_name);

	std::list<Node*>::iterator iter;
	for(iter = childs.begin(); iter != childs.end(); iter++)
	{
		(*iter)->setAnimation(animation);
	}
}

bool Node::playAnimation(std::string animName)
{
	bool found = false;

	std::list<Animation3D*>::iterator iter;

	if( _currentAnimation == NULL || animName != _currentAnimation->name()) 
	{
		for(iter = animations.begin(); iter != animations.end(); iter++)
		{
	
			if ((*iter)->name() == animName)
			{
				if (_currentAnimation != NULL)
					_currentAnimation->Stop();

				setAnimation((*iter));
				found = true;
				break;
			}	
		}

		if (found)
			_currentAnimation->Play();
	}

	return found;
}
