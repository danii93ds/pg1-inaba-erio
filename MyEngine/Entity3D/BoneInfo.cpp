#include "BoneInfo.h"

using namespace Inaba;

BoneInfo::BoneInfo()
{

}

BoneInfo::~BoneInfo()
{
	
}

UINT BoneInfo::numWeights()
{
	return _nWeights;
}

void BoneInfo::setWeightCount(UINT numWeights)
{
	_nWeights = numWeights;
}

std::vector<BoneInfo::Weight*> BoneInfo::getWeights()
{
	return _Weights;
}

void BoneInfo::insertWeight(Weight* boneWeight)
{
	this->_Weights.push_back(boneWeight);
}

void BoneInfo::setOffsetMatrix(D3DXMATRIX matrix)
{
	this->_offsetMatrix = matrix;
}

D3DXMATRIX BoneInfo::getOffsetMatrix()
{
	return _offsetMatrix;
}

Bones* BoneInfo::getBone()
{
	return _bone;
}

void BoneInfo::setNodeBone(Bones* tBone)
{
	_bone = tBone;
}


