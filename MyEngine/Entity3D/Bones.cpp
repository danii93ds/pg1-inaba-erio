#include "Bones.h"

using namespace Inaba;

Bones::Bones(std::string name)
{
	D3DXMatrixIdentity(&_BoneTransformationMatrix);
	_name = name;
}

Bones::~Bones()
{
	
}

std::string Bones::name()
{
	return this->_name;
}

void Bones::setName(std::string nName)
{
	_name = nName;
}

D3DXMATRIX Bones::getTransformationMatrix()
{
	return _BoneTransformationMatrix;
}

void Bones::setTransformationMatrix(D3DXMATRIX tMatrix)
{
	_BoneTransformationMatrix = tMatrix;
}


