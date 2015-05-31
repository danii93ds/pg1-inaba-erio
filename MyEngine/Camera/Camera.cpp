#include "Camera.h"
#include "../Input/directinput.h"
using namespace Inaba;

Camera::Camera():
_right(0,0,0),
_up(0,1,0),
_look(0,0,1000),
_position(0,0,-100),
_lookAt(0,0,0),
_velocity(0,0,0)
{
	CreateProjectionMatrix(D3DXToRadian(45), 1.3f, 0.001f, 1000.0f);
	Update();
}

void Camera::CreateProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane)
{
	_fov = fov;
	_aspect = aspect;
	_nearPlane = nearPlane;
	_farPlane = farPlane;
	D3DXMatrixPerspectiveFovLH(&_projection, _fov, _aspect, _nearPlane, _farPlane);
}

void Camera::MoveForward(float vel)
{
	_velocity += _look * vel;
}

void Camera::Strafe(float vel)
{
	_velocity += _right * vel;
}

void Camera::MoveUp(float vel)
{
	_velocity.y += vel;
}

void Camera::Yaw(float radians)
{
	if (radians == 0.0f)
	{
		return;
	}
	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, &_up, radians);
	D3DXVec3TransformNormal(&_right, &_right, &rotation);
	D3DXVec3TransformNormal(&_look, &_look, &rotation);
}

void Camera::Pitch(float radians)
{
	if (radians == 0.0f)
	{
		return;
	}

	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, &_right, radians);
	D3DXVec3TransformNormal(&_up, &_up, &rotation);
	D3DXVec3TransformNormal(&_look, &_look, &rotation);
}

void Camera::Roll(float radians)
{
	if (radians == 0.0f)
	{
		return;
	}
	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, &_look, radians);
	D3DXVec3TransformNormal(&_right, &_right, &rotation);
	D3DXVec3TransformNormal(&_up, &_up, &rotation);
}

void Camera::SetPosition(D3DXVECTOR3* pPosition)
{
	_position.x = pPosition->x;
	_position.y = pPosition->y;
	_position.z = pPosition->z;
}

void Camera::SetLookAt(D3DXVECTOR3* pLookAt)
{
	_lookAt.x = pLookAt->x;
	_lookAt.y = pLookAt->y;
	_lookAt.z = pLookAt->z;
	D3DXVec3Normalize(&_look, &(_lookAt - _position));
}

void Camera::Update()
{
	// Move the camera
	_position += _velocity;
	
	// Could decelerate here. I'll just stop completely.
	_velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_lookAt = _position + _look;

	// Calculate the new view matrix
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&_view, &_position, &_lookAt, &up);

	// Set the camera axes from the view matrix
	_right.x = _view._11;
	_right.y = _view._21;
	_right.z = _view._31;
	_up.x = _view._12;
	_up.y = _view._22;
	_up.z = _view._32;
	_look.x = _view._13;
	_look.y = _view._23;
	_look.z = _view._33;
}

void Camera::CameraControl(DirectInput &directInput, float cameraVel)
{
	if (directInput.keyDown(Inaba::Input::KEY_W))
		MoveForward(cameraVel);
	if (directInput.keyDown(Inaba::Input::KEY_S))
		MoveForward(-cameraVel);
	if (directInput.keyDown(Inaba::Input::KEY_A))
		Strafe(-cameraVel);
	if (directInput.keyDown(Inaba::Input::KEY_D))
		Strafe(cameraVel);
	if (directInput.keyDown(Inaba::Input::KEY_Q))
		MoveUp(-cameraVel);
	if (directInput.keyDown(Inaba::Input::KEY_E))
		MoveUp(cameraVel);

	if (directInput.keyDown(Inaba::Input::KEY_LCONTROL))
	{
		Yaw(D3DXToRadian(directInput.mouseRelPosX()/5));
		Pitch(D3DXToRadian(directInput.mouseRelPosY()/5));
	}

}

void Camera::BuildViewFrustum()
{
	D3DXMATRIX _viewProj;
	D3DXMatrixMultiply(&_viewProj,&_view,&_projection);
	

	// Left Frustum Plane
    // Add first column of the matrix to the fourth column
	FrustumPlane[0].a = _viewProj._14 + _viewProj._11; 
	FrustumPlane[0].b = _viewProj._24 + _viewProj._21;
	FrustumPlane[0].c = _viewProj._34 + _viewProj._31;
	FrustumPlane[0].d = _viewProj._44 + _viewProj._41;

	// Right Frustum Plane
    // Subtract first column of matrix from the fourth column
	FrustumPlane[1].a = _viewProj._14 - _viewProj._11; 
	FrustumPlane[1].b = _viewProj._24 - _viewProj._21;
	FrustumPlane[1].c = _viewProj._34 - _viewProj._31;
	FrustumPlane[1].d = _viewProj._44 - _viewProj._41;

	// Top Frustum Plane
    // Subtract second column of matrix from the fourth column
	FrustumPlane[2].a = _viewProj._14 - _viewProj._12; 
	FrustumPlane[2].b = _viewProj._24 - _viewProj._22;
	FrustumPlane[2].c = _viewProj._34 - _viewProj._32;
	FrustumPlane[2].d = _viewProj._44 - _viewProj._42;

	// Bottom Frustum Plane
    // Add second column of the matrix to the fourth column
	FrustumPlane[3].a = _viewProj._14 + _viewProj._12;
	FrustumPlane[3].b = _viewProj._24 + _viewProj._22;
	FrustumPlane[3].c = _viewProj._34 + _viewProj._32;
	FrustumPlane[3].d = _viewProj._44 + _viewProj._42;

	// Near Frustum Plane
    // We could add the third column to the fourth column to get the near plane,
    // but we don't have to do this because the third column IS the near plane
	FrustumPlane[4].a = _viewProj._13;
	FrustumPlane[4].b = _viewProj._23;
	FrustumPlane[4].c = _viewProj._33;
	FrustumPlane[4].d = _viewProj._43;

	// Far Frustum Plane
    // Subtract third column of matrix from the fourth column
	FrustumPlane[5].a = _viewProj._14 - _viewProj._13; 
	FrustumPlane[5].b = _viewProj._24 - _viewProj._23;
	FrustumPlane[5].c = _viewProj._34 - _viewProj._33;
	FrustumPlane[5].d = _viewProj._44 - _viewProj._43;

	for(int i = 0; i < 6; i ++)
	{
		D3DXPlaneNormalize( &(FrustumPlane[i]), &(FrustumPlane[i]) );
	}
}

UINT Camera::checkCollisionAABB(Entity3D* entity3D)
{
	//AABB *_aabb = entity3D->getAABB();

/*	
	D3DXVECTOR3 aabbSize = D3DXVECTOR3(_aabb->getWidth(),_aabb->getHeight(),_aabb->getDepth());
	
	D3DXVECTOR3 aabbCenter = _aabb->getCenter();
	//D3DXVECTOR3 aabbCenter = D3DXVECTOR3( _aabb->getCenter().x + entity3D->GetTranformationMatrix()->_41 , _aabb->getCenter().y + entity3D->GetTranformationMatrix()->_42 , _aabb->getCenter().z + entity3D->GetTranformationMatrix()->_43);
	unsigned int result = INSIDE;
	
	for(unsigned int i = 0;i < 6;++i)
	{	
		const D3DXPLANE fPlane = FrustumPlane[i];
		
		float d = aabbCenter.x * fPlane.a + 
				  aabbCenter.y * fPlane.b + 
				  aabbCenter.z * fPlane.c;

		float r = aabbSize.x * std::abs(fPlane.a) + 
				  aabbSize.y * std::abs(fPlane.b )+ 
				  aabbSize.z * std::abs(fPlane.c);

			float d_p_r = d + r;
			float d_m_r = d - r;

			if(d_p_r < -fPlane.d)
			{
				result = OUTSIDE;
				break;
			}
			else if(d_m_r < -fPlane.d)
				result = INTERSECT;
	}

	return result;*/
	return 1;
}