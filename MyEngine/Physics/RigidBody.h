#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../myengine_api.h"
#include "../Renderer\xMath.h"

class hkpRigidBody;
namespace Inaba
{
	class Collider;	

	class MYENGINE_API RigidBody
	{
		public:
			RigidBody();
			virtual ~RigidBody();

			void SetPosition(float, float, float);
			float posX() const;
			float posY() const;
			float posZ() const;

			void setRotation(float, float, float);
			float rotationX() const;
			float rotationY() const;
			float rotationZ() const;

			const Matrix& transform() const;

			void setCollider(Collider*);
			const Collider* collider() const  { return _collider; }

			enum HavokMotion{
				Static,
				Dynamic,
				HavokMotionCount
			};

			void setHavokMotion(HavokMotion type);
			HavokMotion havokMotion() const { return _HMotion; }

		private:
			Collider *_collider;
			HavokMotion _HMotion;

		public:
			hkpRigidBody *rigidbody() { return _rigidbody; }
			hkpRigidBody *_rigidbody;
	};
}

#endif