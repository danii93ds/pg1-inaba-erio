#include "Animation3D.h"
#include "../Renderer/Renderer.h"
#include "../Timer/timer.h"
#include "../Renderer/xMath.h"
#include <d3dx9.h>

using namespace Inaba;

Animation3D::Animation3D() :
	_name(""),
	_duration(0),
	_ticksPerSecond(0),
	_cFrameTime(0),
	_state(STOP)
{

}

Animation3D::~Animation3D()
{
	while(!keyFrames.empty())
	{
		keyFrame* kF = keyFrames.back();
		keyFrames.pop_back();

		delete kF;
		kF = NULL;
	}
}

void Animation3D::Play()
{
	this->_state = PLAY;
}

void Animation3D::Stop()
{
	this->_state = STOP;
	this->_cFrameTime  = 0;
}

std::string Animation3D::name() 
{ 
	return this->_name; 
}

void Animation3D::setName(std::string name){
	this->_name = name;
}

double Animation3D::duration()
{
	return this->_duration;
}

void Animation3D::setDuration(double value){
	this->_duration = value;
}

double Animation3D::ticksPerSecond()
{
	return this->_ticksPerSecond;
}

void Animation3D::setTicksPerSecond(double value){
	this->_ticksPerSecond = value;
}

void Animation3D::addKeyFrame(keyFrame* kFrame)
{
	keyFrames.push_back(kFrame);
}

Animation3D::State Animation3D::getState()
{
	return this->_state;
}

D3DXMATRIX Animation3D::getTransformationMatrix(unsigned int index)
{
	D3DXMATRIX transformationMatrix;
	D3DXMatrixIdentity(&transformationMatrix);
	
	D3DXVECTOR3 interpolatedPos = getInterpolatingVec(keyFrames[index]->positionKeys, keyFrames[index]->nPositionKeys, _cFrameTime);
	D3DXVECTOR3 interpolatedScale = getInterpolatingVec(keyFrames[index]->scalingKeys, keyFrames[index]->nScalingKeys, _cFrameTime);
	D3DXQUATERNION interpolatedRot = getInterpolatingQuat(keyFrames[index]->rotationKeys, keyFrames[index]->nRotationKeys, _cFrameTime);
	
	D3DXMATRIX translation, scalation, rotation;
	D3DXMatrixTranslation(&translation, interpolatedPos.x, interpolatedPos.y, interpolatedPos.z);
	D3DXMatrixScaling(&scalation, interpolatedScale.x, interpolatedScale.y, interpolatedScale.z);
	D3DXMatrixRotationQuaternion(&rotation, &interpolatedRot);
	
	D3DXMatrixMultiply(&transformationMatrix, &translation, &transformationMatrix);
	D3DXMatrixMultiply(&transformationMatrix, &scalation,&transformationMatrix);
	D3DXMatrixMultiply(&transformationMatrix, &rotation, &transformationMatrix);

	return transformationMatrix;
}

D3DXVECTOR3 Animation3D::getInterpolatingVec(aiVectorKey* vecKey, int nKeys, double fTime)
{
	D3DXVECTOR3 finalVector;

	int vecKeyIndex = -1;

	for (int i = 0; i < nKeys; i++)
	{
		if(vecKey[i].mTime == fTime){
			finalVector = D3DXVECTOR3(vecKey[i].mValue.x, vecKey[i].mValue.y, vecKey[i].mValue.z);
			return finalVector;
		}

		if(fTime < vecKey[i].mTime){
			vecKeyIndex = i;
			break;
		}
	}

	float dAux = (fTime -  vecKey[vecKeyIndex - 1].mTime) / ( vecKey[vecKeyIndex].mTime - vecKey[vecKeyIndex - 1].mTime );
	assert(dAux >= 0.0f && dAux <= 1.0f);
	D3DXVECTOR3 first(vecKey[vecKeyIndex - 1].mValue.x, vecKey[vecKeyIndex - 1].mValue.y, vecKey[vecKeyIndex -1].mValue.z);
	D3DXVECTOR3 last(vecKey[vecKeyIndex].mValue.x, vecKey[vecKeyIndex].mValue.y, vecKey[vecKeyIndex].mValue.z);

	finalVector = last * dAux + first * (1.0f - dAux);
	return finalVector;
}

D3DXQUATERNION Animation3D::getInterpolatingQuat(aiQuatKey* quatKey, int nKeys, double fTime)
{
	D3DXQUATERNION finalQuaternion;

	int quatKeyIndex = -1;

	for (int i = 0; i < nKeys; i++)
	{
		if(quatKey[i].mTime == fTime){
			finalQuaternion = D3DXQUATERNION(quatKey[i].mValue.x, quatKey[i].mValue.y, quatKey[i].mValue.z, quatKey[i].mValue.w);
			return finalQuaternion;
		}

		if(fTime < quatKey[i].mTime){
			quatKeyIndex = i;
			break;
		}
	}

	float dAux = (fTime -  quatKey[quatKeyIndex - 1].mTime) / ( quatKey[quatKeyIndex].mTime - quatKey[quatKeyIndex - 1].mTime );
	assert(dAux >= 0.0f && dAux <= 1.0f);
	const aiQuaternion& first = quatKey[quatKeyIndex - 1].mValue;
	const aiQuaternion& last = quatKey[quatKeyIndex].mValue;
	aiQuaternion rotation;
	aiQuaternion::Interpolate(rotation, first, last, dAux);

	finalQuaternion	= D3DXQUATERNION(rotation.x,rotation.y,rotation.z,rotation.w);
	return finalQuaternion;

}

int Animation3D::getIndexFrameKey(std::string nodeName)
{
	for(int keyIndex=0; keyIndex < keyFrames.size(); keyIndex++)
	{
		if(keyFrames[keyIndex]->nodeName.C_Str() == nodeName)
			return keyIndex;
	}

	return -1;
}

void Animation3D::Update(Timer& rkTimer)
{
	if (_state == PLAY)
	{
		_cFrameTime += (rkTimer.timeBetweenFrames() /1000) * _ticksPerSecond;
		while(_cFrameTime >= _duration)
			_cFrameTime -= _duration;
	}
}
