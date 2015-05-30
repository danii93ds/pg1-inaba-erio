#include "RigidBody.h"
#include "../Renderer/xMath.h"
#include "Collider.h"
#include <d3dx9.h>
#include <Common/Base/hkBase.h>
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>


using namespace Inaba;

hkpMotion::MotionType _HavokMotionType[RigidBody::HavokMotionCount] =
{
	hkpMotion::MOTION_FIXED,
	hkpMotion::MOTION_DYNAMIC
};

RigidBody::RigidBody()
	: _rigidbody(NULL),
	_HMotion(Dynamic),
	_collider(NULL)
{
	hkpBoxShape* box = new hkpBoxShape(hkVector4(1.0f, 1.0f, 1.0f));

	hkpRigidBodyCinfo RigidBodyInfo;
	RigidBodyInfo.m_shape = box;
	RigidBodyInfo.m_position = hkVector4(0, 0, 0);
	RigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;

	_rigidbody = new hkpRigidBody(RigidBodyInfo);

	box->removeReference();
}

RigidBody::~RigidBody()
{
	_rigidbody->removeReference();
	_rigidbody = NULL;

	if (_collider != NULL)
	{
		delete _collider;
		_collider = NULL;
	}
}

void RigidBody::SetPosition(float x, float y, float z)
{
	_rigidbody->markForWrite();
	_rigidbody->setPosition(hkVector4(-x, y, z));
	_rigidbody->unmarkForWrite();
}

float RigidBody::posX() const
{
	_rigidbody->markForRead();
	float posX = -_rigidbody->getPosition().getComponent(0);
	_rigidbody->unmarkForRead();
	return posX;
}
float RigidBody::posY() const{
	_rigidbody->markForRead();
	float posY = _rigidbody->getPosition().getComponent(1);
	_rigidbody->unmarkForRead();
	return posY;
}

float RigidBody::posZ() const{
	_rigidbody->markForRead();
	float posZ = _rigidbody->getPosition().getComponent(2);
	_rigidbody->unmarkForRead();
	return posZ;
}

void RigidBody::setRotation(float px, float py, float pz){
	_rigidbody->markForWrite();

	float x, y, z, w;
	MATHX::eulerAnglesToQuaternion(px, py, pz, x, y, z, w);
	hkQuaternion kRotation(x, y, z, w);
	_rigidbody->setRotation(kRotation);

	_rigidbody->unmarkForWrite();
}

float RigidBody::rotationX() const
{
	_rigidbody->markForRead();

	float rotX, rotY, rotZ;
	MATHX::quaternionToEulerAngles(_rigidbody->getRotation()(0), _rigidbody->getRotation()(1), _rigidbody->getRotation()(2), _rigidbody->getRotation()(3), rotX, rotY, rotZ);

	_rigidbody->unmarkForRead();

	return rotX;
}
float RigidBody::rotationY() const{
	_rigidbody->markForRead();

	float rotX, rotY, rotZ;
	MATHX::quaternionToEulerAngles(_rigidbody->getRotation()(0), _rigidbody->getRotation()(1), _rigidbody->getRotation()(2), _rigidbody->getRotation()(3), rotX, rotY, rotZ);

	_rigidbody->unmarkForRead();

	return rotY;
}
float RigidBody::rotationZ() const{
	_rigidbody->markForRead();

	float rotX, rotY, rotZ;
	MATHX::quaternionToEulerAngles(_rigidbody->getRotation()(0), _rigidbody->getRotation()(1), _rigidbody->getRotation()(2), _rigidbody->getRotation()(3), rotX, rotY, rotZ);

	_rigidbody->unmarkForRead();

	return rotZ;
}

void RigidBody::setCollider(Collider* pkCollider)
{

	if (_collider != NULL){
		_rigidbody->markForWrite();

		delete _collider;
		_collider = pkCollider;

	}
	else{
		_collider = pkCollider;
		_rigidbody->markForWrite();
	}
	_rigidbody->setShape(_collider->shape());
	_rigidbody->unmarkForWrite();
}

void RigidBody::setHavokMotion(RigidBody::HavokMotion type){
	_HMotion = type;
	_rigidbody->markForWrite();
	_rigidbody->setMotionType(_HavokMotionType[type]);
	_rigidbody->unmarkForWrite();
}

const Matrix& RigidBody::transform() const{

	hkTransform rbTransformation = _rigidbody->getTransform();

	D3DXMATRIX rigidbodyMaterial(rbTransformation(0, 0), rbTransformation(1, 0), rbTransformation(2, 0), rbTransformation(3, 0),
		rbTransformation(0, 1), rbTransformation(1, 1), rbTransformation(2, 1), rbTransformation(3, 1),
		rbTransformation(0, 2), rbTransformation(1, 2), rbTransformation(2, 2), rbTransformation(3, 2),
		rbTransformation(0, 3), rbTransformation(1, 3), rbTransformation(2, 3), rbTransformation(3, 3));

	return &rigidbodyMaterial;
}



