#ifndef CAMERA_H
#define CAMERA_H

#include "../myengine_api.h"
#include <d3dx9.h>
#include "../Entity3D/Entity3D.h"
#include "../Renderer/AABB.h"

namespace Inaba
{
	class DirectInput;

	class MYENGINE_API Camera
	{
	public:
		Camera();
		void CreateProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane);
		void MoveForward(float vel);
		void Strafe(float vel);
		void MoveUp(float vel);

		void Yaw(float radians);
		void Pitch(float radians);
		void Roll(float radians);

		D3DXMATRIX* getViewMatrix()			{ return &_view; }
		D3DXMATRIX* getProjectionMatrix()	{ return &_projection; }
		void SetFOV(float fov)				{ CreateProjectionMatrix(fov, _aspect, _nearPlane, _farPlane); }
		void SetAspectRatio(float aspect)	{ CreateProjectionMatrix(_fov, aspect, _nearPlane, _farPlane); }
		void SetNearPlane(float plane)		{ CreateProjectionMatrix(_fov, _aspect, plane, _farPlane); }
		void SetFarPlane(float plane)		{ CreateProjectionMatrix(_fov, _aspect, _nearPlane, plane); }
		void SetPosition(D3DXVECTOR3* pPosition);
		D3DXVECTOR3 getPosition();
		void SetLookAt(D3DXVECTOR3* pLookAt);
		void BuildViewFrustum();
		UINT checkCollisionAABB(Entity3D* entity3D);
		void Update();
		static enum {OUTSIDE, INSIDE, INTERSECT};
		void CameraControl(DirectInput &directInput, float cameraVel = 10.0f);

	private:
		D3DXMATRIX  _view;
		D3DXMATRIX  _projection;
		D3DXVECTOR3 _right;
		D3DXVECTOR3 _up;		// the up direction
		D3DXVECTOR3 _look;		// the look-at position
		D3DXVECTOR3 _position;	// the camera position
		D3DXVECTOR3 _lookAt;
		D3DXVECTOR3 _velocity;
		D3DXPLANE FrustumPlane[6];
		float _fov;
		float _aspect;
		float _nearPlane;
		float _farPlane;
	};
}

#endif