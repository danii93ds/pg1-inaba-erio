#include "Node.h"
#include "../Renderer/Renderer.h"
#include <d3dx9.h>
using namespace Inaba;

Node::Node():
Entity3D(),
_worldTransformMatrix(new D3DXMATRIX),
_AssimpTransformMatrix(new D3DXMATRIX),
_bone(NULL),
_currentAnimation(NULL),
_vBB(new D3DXVECTOR3[8])
{
	D3DXMatrixIdentity(_worldTransformMatrix);
	D3DXMatrixIdentity(_AssimpTransformMatrix);
	iKeyFrame = 0;
	meshes.clear();
	childs.clear();
	_isPlane = false;
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

	if (_bone)
	{
		delete _bone;
		_bone = NULL;
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

D3DXMATRIX Node::getWorldTransformation()
{
	return *_worldTransformMatrix;
}

D3DXMATRIX Node::getLocalTransformation()
{
	return *_AssimpTransformMatrix;
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

	calculateBB();

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
	//D3DXMatrixIdentity(_worldTransformMatrix);
	r.setMatrix(World, _worldTransformMatrix);
	//TODO FLUSH TUM
	//UpdateTransformation(_worldTransformMatrix,&r);

	NodeDraw(&r);
}

void Node::DrawMeshes(Renderer& pRenderer)
{
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	pRenderer.setMatrix(World, &identity);

	std::list<Mesh*>::iterator iter;
	for (iter = meshes.begin(); iter != meshes.end(); iter++)
	{
		pRenderer.setMatrix(World, _worldTransformMatrix);
		(*iter)->Draw(pRenderer);
	}
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

void Node::isPlane(bool isPlane)
{
	_isPlane = isPlane;
}

bool Node::isPlane()
{
	return _isPlane;
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

void Node::GetBoundings(D3DXVECTOR3* pOutMin, D3DXVECTOR3* pOutMax){
	*pOutMax = _maxBound;
	*pOutMin = _minBound;
}

void Node::calculateBB() {

	if (!meshes.empty()){

		D3DXVECTOR3 vMeshVertices[8];
		meshes.front()->getTransformedBox(_worldTransformMatrix, vMeshVertices);

		_maxBound.x = vMeshVertices[0].x;
		_maxBound.y = vMeshVertices[0].y;
		_maxBound.z = vMeshVertices[0].z;

		_minBound.x = vMeshVertices[0].x;
		_minBound.y = vMeshVertices[0].y;
		_minBound.z = vMeshVertices[0].z;

		for (int i = 1; i<8; i++) {
			if (vMeshVertices[i].x > _maxBound.x)
				_maxBound.x = vMeshVertices[i].x;
			else if (vMeshVertices[i].x < _minBound.x)
				_minBound.x = vMeshVertices[i].x;

			if (vMeshVertices[i].y > _maxBound.y)
				_maxBound.y = vMeshVertices[i].y;
			else if (vMeshVertices[i].y < _minBound.y)
				_minBound.y = vMeshVertices[i].y;

			if (vMeshVertices[i].z > _maxBound.z)
				_maxBound.z = vMeshVertices[i].z;
			else if (vMeshVertices[i].z < _minBound.z)
				_minBound.z = vMeshVertices[i].z;
		}
	}
	else if (!childs.empty()) {
		childs.front()->GetBoundings(&_minBound, &_maxBound);
	}

	std::list<Node*>::iterator iter;
	for (iter = childs.begin(); iter != childs.end(); iter++) {
		D3DXVECTOR3 vChildMax;
		D3DXVECTOR3 vChildMin;

		(*iter)->GetBoundings(&vChildMin, &vChildMax);

		if (vChildMax.x > _maxBound.x) {
			_maxBound.x = vChildMax.x;
		}

		if (vChildMax.y > _maxBound.y) {
			_maxBound.y = vChildMax.y;
		}

		if (vChildMax.z > _maxBound.z) {
			_maxBound.z = vChildMax.z;
		}

		if (vChildMin.x < _minBound.x) {
			_minBound.x = vChildMin.x;
		}

		if (vChildMin.y < _minBound.y) {
			_minBound.y = vChildMin.y;
		}

		if (vChildMin.z < _minBound.z) {
			_minBound.z = vChildMin.z;
		}
	}

	_vBB[0].x = _maxBound.x;
	_vBB[0].y = _maxBound.y;
	_vBB[0].z = _maxBound.z;

	_vBB[1].x = _maxBound.x;
	_vBB[1].y = _minBound.y;
	_vBB[1].z = _maxBound.z;

	_vBB[2].x = _minBound.x;
	_vBB[2].y = _minBound.y;
	_vBB[2].z = _maxBound.z;

	_vBB[3].x = _minBound.x;
	_vBB[3].y = _maxBound.y;
	_vBB[3].z = _maxBound.z;

	_vBB[4].x = _maxBound.x;
	_vBB[4].y = _maxBound.y;
	_vBB[4].z = _minBound.z;

	_vBB[5].x = _maxBound.x;
	_vBB[5].y = _minBound.y;
	_vBB[5].z = _minBound.z;

	_vBB[6].x = _minBound.x;
	_vBB[6].y = _minBound.y;
	_vBB[6].z = _minBound.z;

	_vBB[7].x = _minBound.x;
	_vBB[7].y = _maxBound.y;
	_vBB[7].z = _minBound.z;
}

D3DXPLANE Node::GetPlane(){
	return meshes.front()->GetPlane(_worldTransformMatrix);
}

D3DXVECTOR3* Node::getBB(){
	return _vBB;
}